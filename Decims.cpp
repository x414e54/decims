#include "Decims.h"

int main(int arcv, char* argv[])
{
	const char *decimimagestrings[31];		// Images for the game gui
	decimimagestrings[0]="media/decim1.bmp";		// - Decim with value 1
	decimimagestrings[1]="media/decim2.bmp";		// - Decim with value 2
	decimimagestrings[2]="media/decim3.bmp";		// - Decim with value 3
	decimimagestrings[3]="media/decim4.bmp";		// - Decim with value 4
	decimimagestrings[4]="media/decim5.bmp";		// - Decim with value 5
	decimimagestrings[5]="media/decim6.bmp";		// - Decim with value 6
	decimimagestrings[6]="media/decim7.bmp";		// - Decim with value 7
	decimimagestrings[7]="media/decim8.bmp";		// - Decim with value 8
	decimimagestrings[8]="media/decim9.bmp";		// - Decim with value 9
	decimimagestrings[9]="media/decim10.bmp";		// - Decim with value 10
	decimimagestrings[10]="media/decimn.bmp";		// - Null/empty decim slot
	decimimagestrings[11]="media/div.bmp";		// - Divide operator
	decimimagestrings[12]="media/mul.bmp";		// - Multiply operator
	decimimagestrings[13]="media/sub.bmp";		// - Subtract operator
	decimimagestrings[14]="media/add.bmp";		// - Addition operator
	decimimagestrings[15]="media/eq.bmp";			// - Equals (=) image
	decimimagestrings[16]="media/msgbg.bmp";		// - Background for a messagebox
	decimimagestrings[17]="media/bg.bmp";			// - Main background
	decimimagestrings[18]="media/buttonon.bmp";	// - Button pressed down image
	decimimagestrings[19]="media/buttonoff.bmp";	// - Button up image
	decimimagestrings[20]="media/set1.bmp";		// - Set or small decim image with value 1
	decimimagestrings[21]="media/set2.bmp";		// - Set or small decim image with value 2
	decimimagestrings[22]="media/set3.bmp";		// - Set or small decim image with value 3
	decimimagestrings[23]="media/set4.bmp";		// - Set or small decim image with value 4
	decimimagestrings[24]="media/set5.bmp";		// - Set or small decim image with value 5
	decimimagestrings[25]="media/set6.bmp";		// - Set or small decim image with value 6
	decimimagestrings[26]="media/set7.bmp";		// - Set or small decim image with value 7
	decimimagestrings[27]="media/set8.bmp";		// - Set or small decim image with value 8
	decimimagestrings[28]="media/set9.bmp";		// - Set or small decim image with value 9
	decimimagestrings[29]="media/set10.bmp";		// - Set or small decim image with value 10
	decimimagestrings[30]="media/setn.bmp";		// - Null/empty set or small decim slot

	srand ( (unsigned int)time(0) );		// Seed random for the random generation of the decim/cards

	DecimEngine *engine;

	int hostOrJoin;
	char hostname[256];

	printf("Would you like to host or join?:\n");
	printf("1. Host\n");
	printf("2. Join\n");
	scanf("%d",&hostOrJoin);

	if (hostOrJoin==1)			// Hosting game
	{
		printf("Waiting for client...\n");
		engine = new DecimEngine(decimimagestrings);
	} else if (hostOrJoin==2)	// Joining game
	{
		printf("Please enter the host name or ip of the server:\n");
		scanf("%s",&hostname);
		printf("Connecting please wait...\n");
		engine = new DecimEngine(hostname, decimimagestrings);
	} else
	{
		printf("INVALID CHOICE\n");
		return -1;
	}

	return 0;
}
