/**
 *BMPtoAQFConverter.cpp
 *
 *This is a program created to help me in the creation of fonts in the "AntQuest" game. Also now usable in the decims game.
 *
 *More about the "AQF" format is detailed below, it is just essentially a 2bit bitmap with less headers and font widths appended.
 *
 *Possible Improvements:
 *
 *-Better error checking in this program,
 *-The AQF format could include rectangles for each font, that way they could be of varying size and height, and in a non linear order, however
 *this is currently not needed for the AntQuest game and therefore is a waste of time and space including this.
 *-Checking to see if padding works correctly and that all fonts are divisible correctly.
 *-More options specifying the colors that relate to each bit could be included.
 *-Support for all types of bitmap, and compression used.
*/


/**
 *AQF is designed for the "AntQuest" engine as a font file format. This enables the user to convert a bmp file to a useable font in the game engine.
 *
 *The output of this program creates a file with:
 *
 *A header of:
 *
 *A signature denoting the type,
 *An image pixel width and height,
 *The height of the characters (which has to be the same - the width of characters is allowed to be variable and will be loaded from a seperate text based file).
 *
 *Data of:
 *
 *The image map, consisting of two bits.
 *11 will be white
 *01 will be black
 *00 will be transparent
 *10 is unused
 *(The first bit is the color and the second bit is the alpha value/transparency)
 *
 *256 Character widths for each char value
 *
 *The whole point of the AQF format is to take in a bmp of black, white and other color values and to map the black to the black 2bits, the white to the white 2bits
 *and any other color becomes transparent. This can be then loaded into the game engine and used as a way to create colored fonts. Simply converting the map back into
 *a format usable to the game engine. The intended format is an SDL 32bit pixel map. When importing a specificed color can be assigned to the 11 and 01 values to create differnt
 *colored fonts from the same file, this would not be universally possible by importing a standard BMP. It also stores the width of each character information inside the file removing
 *the need for a separate file with this data in it, and allowing character of varying width.
*/

#include <iostream>
#include <fstream>

typedef unsigned short WORD;
typedef unsigned int DWORD;

// Standard struct for the windows 24bit BMP Info Header format
struct BMPInfo
{
	DWORD bmpHeadSize;  //size of info header (must be 40)
	DWORD bmpWidth;  //image width in pixels
	DWORD bmpHeight;  //image height in pixels
	WORD bmpPlanes; //number of color planes (must be 1)
	WORD bmpBPP; //number of bits per pixel
	DWORD bmpCompression;//compression used
	DWORD bmpSize;  //the size in bytes of the BMP
	DWORD bmpHozPPM;  //horizontal resolution in pixels per meter
	DWORD bmpVerPPM;  //vertical resolution in pixels per meter
	DWORD bmpColors;  //number of colors
	DWORD bmpImpColors;  //number of important colors
};

// Standard struct for the windows 24bit BMP File Header format
struct BMPFile
{
	WORD bmpSig;  //signature (should be equal to 4D42 hex)
	DWORD bmpSize;  //the size in bytes of the BMP
	WORD bmpR0;  //reserved (must be 0)
	WORD bmpR1;  //reserved (must be 0)
	DWORD bmpOffset;  //offset to start of image in bytes
};

// Standard struct for the windows 24bit BMP format
struct BMP
{
	BMPFile bmpFHeader;
	BMPInfo bmpIHeader;
	unsigned char *bmp; //bmp data (this is in the format of 3 bytes per pixel, padding is used to make each line divisible by 4)
};

// Struct for the AQF header, essentially a simplified bmp header with a different signature to reflect and data about the height of characters.
struct AQFHeader
{
  WORD aqfSig; //signature of the AQF file (this will be equal to AABF hex)
  DWORD aqffontFileWidth; //the width in pixels of the image
  DWORD aqffontFileHeight; //the height in pixels of the image
  DWORD aqfcharHeight; //the height of a character, currently must be the same
};

// Struct for the AQF format
struct AQF
{
	AQFHeader aqfHeader; //the header details.
	unsigned char *data; //data of 2 bits per pixel
	unsigned char *charWidths; //256 widths for each character
};

// The forward declarations
AQF convertToAQF(BMP bmp, unsigned char *cWidths, int height);
BMP readBMP(const char *file);
void writeAQF(AQF aqf, const char *file);
unsigned char *readWidths(const char *widthsFile);
void argumentError();
void writeWidths(int size, const char *file);

int main(int argc, const char* argv[])
{
	if (argc==5) { // Have the arguments been inputted correctly?
		try {
			//writeWidths(8,"font.wid");
			const char *fontFile=argv[1];
			const char *widthsFile=argv[2];
			int height;
			sscanf(argv[3], "%d", &height);
			if (height==0) { throw "You have inputted an invalid height\n\n"; } // Invalid height cannot continue, output information about the required arguments and end
			BMP inBMP = readBMP(fontFile);						// Read bmp and store in memory
			if (inBMP.bmp==NULL) { throw "An unknown error has occured (1)"; }
			unsigned char *cWidths = readWidths(widthsFile);	// Read widths
			if (cWidths==NULL) { throw "An unknown error has occured (2)"; }
			AQF outAQF = convertToAQF(inBMP,cWidths,height);	// Convert bmp to aqf
			if (outAQF.data==NULL) { throw "An unknown error has occured (3)"; }
			if (outAQF.charWidths==NULL) { throw "An unknown error has occured (4)"; }
			writeAQF(outAQF, argv[4]);						// Output aqf to file
			delete inBMP.bmp;;
			delete outAQF.charWidths;
			delete outAQF.data;
		} catch( char *e ) {
			std::cout << "Error: " << e << '\n';
		}
	} else { // Output information about the required arguments
		argumentError();
		return -1;
	}
	return 0;
}

/**
 * Output information about the required arguments
*/
void argumentError() {
		std::cout<<"Converts a bmp and file containing width data to a format\nusable by the AntQuest engine.\n\n";
		std::cout<<"BMPtoAQFConverter [File1] [File2] [Height] [File4]\n\n";
		std::cout<<"[File1] Font BMP File (must be in a windows 24bit format, uncompressed)\n";
		std::cout<<"[File2] The file of widths (maximum 256, one per line)\n";
		std::cout<<"[File3] Height of the characters\n";
		std::cout<<"[File4] Output file\n";
}

/**
 *I have currently not tested this method with bmps of widths/heights that are not divisible by a byte, however the code is there and the plan is to add padding at
 *the end of each pixel row just like with the BMP format.
*/
AQF convertToAQF(BMP bmp, unsigned char *cWidths, int height) {
	AQF aqfComplete;
	aqfComplete.charWidths=cWidths;
	aqfComplete.aqfHeader.aqfcharHeight=height;
	aqfComplete.aqfHeader.aqfSig=0xAABF;
	aqfComplete.aqfHeader.aqffontFileHeight=bmp.bmpIHeader.bmpHeight;
	aqfComplete.aqfHeader.aqffontFileWidth=bmp.bmpIHeader.bmpWidth;

	/* Just like bmps padding will currently be used to keep the file in multiples of 8bits (a byte)  */
	size_t padding=0;
	if (bmp.bmpIHeader.bmpWidth%4!=0) { padding=4-(bmp.bmpIHeader.bmpWidth%4); }
	size_t size =(bmp.bmpIHeader.bmpHeight*(bmp.bmpIHeader.bmpWidth+padding))/4;

	aqfComplete.data=new unsigned char[size];
	unsigned int bit=0;
	unsigned int i=0;
	for (unsigned int h=0; h<bmp.bmpIHeader.bmpHeight; h++) {
		for (unsigned int w=0; w<bmp.bmpIHeader.bmpWidth; w++) {

			if (bit==0) { aqfComplete.data[i]=0; }
			/* The following is used to make sure we encode 8 bits (4 pixels), instead of having
			one byte per pixel we compact 4 of these into one byte. */

			unsigned char r=bmp.bmp[(h*padding*3)+(3*bmp.bmpIHeader.bmpWidth*h)+(3*w)];		//read BMP
			unsigned char g=bmp.bmp[(h*padding*3)+(3*bmp.bmpIHeader.bmpWidth*h)+(3*w)+1];
			unsigned char b=bmp.bmp[(h*padding*3)+(3*bmp.bmpIHeader.bmpWidth*h)+(3*w)+2];
			bool bitOne;
			bool bitTwo;
			if (r==255 && g==255 && b==255) {	// BMP pixel is white
				bitOne=true;					// assign the bit signature 11 (white or color 1)
				bitTwo=true;
			} else if (r==0 && g==0 && b==0) {	// BMP pixel is black
				bitOne=false;					// assign the bit signature 01 (black or color 2)
				bitTwo=true;
			} else {							// BMP pixel is neither black nor white
				bitOne=false;					// assign the bit signature 00 (transparent)
				bitTwo=false;
			}
			aqfComplete.data[i]+=bitOne;		// Set bit1
			aqfComplete.data[i]<<=1;			// Shift bit1
			aqfComplete.data[i]+=bitTwo;		// Set bit2
			if (bit<6) {
				aqfComplete.data[i]<<=1;		// Shift bit2 (only if we are not at the last bit of the 8 bits
			}
			bit+=2;
			if (bit>=8) {						// If we have finished our 8bit encode then move on to the next 8 bits
				i++;
				bit=0;
			}
		}

		/* This padding could go either at the end of the whole encode or after each line, it is currently being placed at the end of each line
		the following code should do this but has not been checked or tested properly.
		*/
		for (unsigned int p=0; p<padding*2; p++ ){
			aqfComplete.data[i]+=0;
			if (bit<7) {
				aqfComplete.data[i]<<=1;
			}
			bit++;
			if (bit>=8) {
				i++;
				bit=0;
			}
		}
	}
	return aqfComplete;
}

/**
 *Simple method to read in the BMP file and assign it to the BMP struct
 *This method uses and ifstream to read the file.
*/
BMP readBMP(const char *file) {
		unsigned char *bmp;
		BMPFile bmpFHeader;
		BMPInfo bmpIHeader;
		BMP bmpComplete;
		std::ifstream f(file);

		if (f.is_open())
		{
			// BMP File Header
			f.read((char *)&bmpFHeader.bmpSig,2);
			f.read((char *)&bmpFHeader.bmpSize,4);
			f.read((char *)&bmpFHeader.bmpR0,2);
			f.read((char *)&bmpFHeader.bmpR1,2);
			f.read((char *)&bmpFHeader.bmpOffset,4);
			if (bmpFHeader.bmpSig !=0x4D42) { f.close(); throw "File is not a valid bmp."; } //BMP signature incorrect therefore not a valid BMP or file reading error

			// BMP Info Header
			f.read((char *)&bmpIHeader.bmpHeadSize,4);
			f.read((char *)&bmpIHeader.bmpWidth,4);
			f.read((char *)&bmpIHeader.bmpHeight,4);
			f.read((char *)&bmpIHeader.bmpPlanes,2);
			f.read((char *)&bmpIHeader.bmpBPP,2);
			f.read((char *)&bmpIHeader.bmpCompression,4);
			f.read((char *)&bmpIHeader.bmpSize,4);
			f.read((char *)&bmpIHeader.bmpHozPPM,4);
			f.read((char *)&bmpIHeader.bmpVerPPM,4);
			f.read((char *)&bmpIHeader.bmpColors,4);
			f.read((char *)&bmpIHeader.bmpImpColors,4);

			// Set file pointer to the bmp offset (start of bmp data)
			f.seekg(bmpFHeader.bmpOffset);

			// Allocate space for the bmp data
			bmp = new unsigned char[bmpIHeader.bmpSize];

			// Read in the bmp data
			f.read((char *)bmp,bmpIHeader.bmpSize);

			f.close();
		} else {
			throw "Error reading BMP file";
		}
		bmpComplete.bmp=bmp;
		bmpComplete.bmpFHeader=bmpFHeader;
		bmpComplete.bmpIHeader=bmpIHeader;
		return bmpComplete;
}

/**
 * This method simply reads the widths from a file, the file should contain 256 widths one for each ascii character/char value
 *that are in the bmp image file. It would be possible to create a GUI to do this, after taking in a BMP file and then allowing the
 *user to crop around each character, but currently it has to be done manually.
 *There is currently no checking for incorrectness in the file, or the values creating a divisible width or not.
*/
unsigned char *readWidths(const char *file) {
		unsigned char *widths;
		std::ifstream f(file);
		if (f.is_open())
		{
			widths= new unsigned char[256];
			int i=0;
			char line[256];
			for (int j=0; j<256; j++) {
				widths[j]=0;
			}
			while (f.getline(line,256)) {
				unsigned char number[3];
				unsigned char width[3];
				bool widthPart=false;
				int j=0;
				int l=0;
				bool endOfLine=false;
				while(j<256 && !endOfLine) {
					if (line[j]=='=' && widthPart==false) {
						widthPart=true;
						l=0;
					} else if ((line[j]=='0' || line[j]=='1' || line[j]=='2' || line[j]=='3' || line[j]=='4' || line[j]=='5' || line[j]=='6' || line[j]=='7' || line[j]=='8' || line[j]=='9')) {
						if (widthPart) {
							if (l>=3) throw "Error reading width file - width too large";
							width[l]=line[j];
							l++;
						} else {
							if (l>=3) throw "Error reading width file - character number too large";
							number[l]=line[j];
							l++;
						}
					} else if (widthPart && line[j]==0) {
						endOfLine=true;
					}
					j++;
				}
				if (!endOfLine) throw "Error reading width file - no character width";
				unsigned int index;
				sscanf((const char *)&number, "%d", &index);
				unsigned int value;
				sscanf((const char *)&width, "%d", &value);
				if (index >= 256 || index <0) throw "Error reading width file - character number too large";
				if (value >= 256 || value <0) throw "Error reading width file - character width too large";
				widths[index]=value;
				i++;
			}
			f.close();
		} else {
			throw "Error reading width file";
		}
		return widths;
}

/**
 * Simple method to write the AQF data to the specified file
 *This method uses and ofstream to write the file.
*/
void writeAQF(AQF outAQF, const char *file) {
		std::ofstream f(file);
		if (f.is_open())
		{
			// Write AQF header information
			f.write((char *)&outAQF.aqfHeader.aqfSig,2);
			f.write((char *)&outAQF.aqfHeader.aqffontFileWidth,4);
			f.write((char *)&outAQF.aqfHeader.aqffontFileHeight,4);
			f.write((char *)&outAQF.aqfHeader.aqfcharHeight,4);

			// Padding may be required if the width does not create an integer divisible by a byte
			size_t padding=0;
			if (outAQF.aqfHeader.aqffontFileWidth%4!=0) { padding=4-(outAQF.aqfHeader.aqffontFileWidth%4); }
			size_t size =(outAQF.aqfHeader.aqffontFileHeight*(outAQF.aqfHeader.aqffontFileWidth+padding))/4;

			// Write 2bit image data
			f.write((char *)outAQF.data,size);

			// Write font widths
			f.write((char *)outAQF.charWidths,256);

			f.close();
		} else {
			throw "Error writing to file";
		}
}


int strlen(char* test)
{
	int i=0;
	while (i<sizeof(test))
	{
		if (test[i]== '\0') return i;
		i++;
	}
}


void writeWidths(int size, const char *file) {
		std::ofstream f(file);
		if (f.is_open())
		{
			for (int i=0; i<256; i++)
			{
				char* buf = new char[50];
				sprintf(buf,"%d=%d\n",i,size);
				f.write(buf,strlen(buf));
				delete buf;
			}
			f.close();
		} else {
			throw "Error writing to file";
		}
}



