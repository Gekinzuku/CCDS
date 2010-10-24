#include "rooms.h"
#include <stdio.h>
#include <stdlib.h>
#include <PA9.h>
#include <fat.h>
#include <sys/dir.h>
#include "gfx/all_gfx.h"

const char GAME_VERSION[9] = "1.1";

char cRoom[99][99];
short int iMaxX, iMaxY; //used to know the max size of the room
short int iLocX, iLocY; //used to keep the current location of the screen in the room
short int iChips;
short int iTime; // used to know how long the player has to beat the level
short int iLevel; //used to know what number level it is
short int iMoves;
char * cHint;

void LevelSelect()
{
	iLevel = 0; //hardcoded
  	char * buffer;
	unsigned int i;
  	unsigned long lSize;
 	int i2;
 	
 	iTime = 999; //hardcoded

cHint = "Suck my dick and I'll give you a penny.";

//while (1) { PA_WaitForVBL(); }
/********************************************************************************
** Emulator level mode
*********************************************************************************/

TempFunction(); return;

/********************************************************************************
** Proper level creation!
*********************************************************************************/

/***********************************************************************************************/

//handle loading the select level from PickLevel();
	int iFile;
	int linenumber = 0;	
	char Fullfilename[99];
			
	iFile = PickLevel();
	
	struct stat st;
	char filename[256]; // to hold a full filename and string terminator
	
	DIR_ITER* dir = diropen("/Gekinzuku/ChipsChallengeDS/levels/");
	
	while (dirnext(dir, filename, &st) == 0) 
	{
	   
	   PA_OutputText(1,linenumber*3,24,"%d ", iFile);
		// st.st_mode & S_IFDIR indicates a directory
		if (linenumber == iFile+2)
		{
		   PA_ClearTextBg(1);
			PA_OutputText(1,0,20, "Loading %s...\n", filename);
			
			char cTemp2[36] = { '/','G','e','k','i','n','z','u','k','u','/','C','h','i','p','s','C','h','a','l','l','e','n','g','e','D','S','/','l','e','v','e','l','s','/'};
			strcpy(Fullfilename, cTemp2);
			strcat(Fullfilename, filename);
			break;
		}		
		linenumber++;//next line
	}
	
	/***********************************************************************************************/
	
	//loads all the level info and puts them in their proper varibles
	
	FILE * pFile;
  	size_t result;
  	pFile = fopen (Fullfilename, "rb" );
  	if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

  // obtain file size:
  fseek (pFile , 0 , SEEK_END);
  lSize = ftell (pFile);
  rewind (pFile);

  // allocate memory to contain the whole file:
  buffer = (char*) malloc (sizeof(char)*lSize);
  if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

  // copy the file into the buffer:
  result = fread (buffer,1,lSize,pFile);
  if (result != lSize) {fputs ("Reading error",stderr); exit (3);}


  // terminate
  fclose (pFile);
  
  buffer = Decrypt(buffer, lSize);

  //gets version support
  char cTemp[20];
  char version[9];
  
  for(i = 0; i < lSize; i++)
  {
        version[i] = buffer[i];
        if((buffer[i+1] == '|') && (buffer[i+2] == '|'))
          break;
  }
  
  version[i+1] = '\0';
  
  /****************************************************************************/
  
  //gets level name
  int iTemp = i;
  char LevelName[30];
  
  for(i = i+2; i < lSize; i++)
  {
        LevelName[i - iTemp -3] = buffer[i];
        if((buffer[i+1] == '|') && (buffer[i+2] == '|'))
          break;
  }
  LevelName[i-iTemp - 2] = '\0';
  
 // PA_OutputText(1,1,1,"Level: %s", LevelName);
  
  /****************************************************************************/
  
  //gets difficulty
  iTemp = i;
  char Difficulty[20];
  
  for(i = i+2; i < lSize; i++)
  {
        Difficulty[i - iTemp -3] = buffer[i];
        if((buffer[i+1] == '|') && (buffer[i+2] == '|'))
          break;
  }
  
  Difficulty[i-iTemp - 2] = '\0';
  
//    PA_OutputText(1,1,2,"Difficulty: %s", Difficulty);
  
  /****************************************************************************/
 
   //gets time
  iTemp = i;
  
  for(i = i+2; i < lSize; i++)
  {
        cTemp[i - iTemp -3] = buffer[i];
        if((buffer[i+1] == '|') && (buffer[i+2] == '|'))
          break;
  }

  
  cTemp[i-iTemp - 2] = '\0';
  
  int Time = atoi(cTemp);
  
  iTime = Time;
  
//    PA_OutputText(1,1,3,"Time: %d", Time);
  
  /***************************************************************************/
  
  //gets chips
  iTemp = i;
  
  for(i = i+2; i < lSize; i++)
  {
        cTemp[i - iTemp -3] = buffer[i];
        if((buffer[i+1] == '|') && (buffer[i+2] == '|'))
          break;
  }
  
  cTemp[i-iTemp - 2] = '\0';
  
  int iChipstemp = atoi(cTemp);
  
  iChips = iChipstemp;
  
 //   PA_OutputText(1,1,4,"Chips: %d  ", iChips);
  
  /***************************************************************************/
  
  //gets level size
  iTemp = i;
  
  for(i = i+2; i < lSize; i++)
  {
        cTemp[i - iTemp -3] = buffer[i];
        if((buffer[i+1] == '|') && (buffer[i+2] == '|'))
          break;
  }
  
  cTemp[i-iTemp - 2] = '\0';
  
  int iMaxXtemp = atoi(cTemp);
  
  iMaxX = iMaxXtemp;
  
  iTemp = i;
  for(i = i+2; i < lSize; i++)
  {
        cTemp[i - iTemp -3] = buffer[i];
        if((buffer[i+1] == '|') && (buffer[i+2] == '|'))
          break;
  }
  
  cTemp[i-iTemp - 2] = '\0';
  
  int iMaxYtemp = atoi(cTemp);
  
  iMaxY = iMaxYtemp;
  
//    PA_OutputText(1,1,5,"Size: %d, %d", iMaxX, iMaxY);
  
  /***************************************************************************/
  
  //gets starting player location
  iTemp = i;
  
  for(i = i+2; i < lSize; i++)
  {
        cTemp[i - iTemp -3] = buffer[i];
        if((buffer[i+1] == '|') && (buffer[i+2] == '|'))
          break;
  }
  
  cTemp[i-iTemp - 2] = '\0';
  
  int iLocXtemp = atoi(cTemp);
  
    iLocX = iLocXtemp;
  
  iTemp = i;
  for(i = i+2; i < lSize; i++)
  {
        cTemp[i - iTemp -3] = buffer[i];
        if((buffer[i+1] == '|') && (buffer[i+2] == '|'))
          break;
  }
  
  cTemp[i-iTemp - 2] = '\0';
  
  int iLocYtemp = atoi(cTemp);
  
  iLocY = iLocYtemp;

//    PA_OutputText(1,1,6,"Location: %d, %d", iLocX, iLocY);  
  
  /****************************************************************************/
  
  // int i2, i3;
   char cTest[99];
   int i3;
  for (i2 = 0; i2 < iMaxY; i2++)
  {
   iTemp = i;
   for(i = i+2; i < lSize; i++)
   {
        cTest[i - iTemp -3] = buffer[i];
        if((buffer[i+1] == '|') && (buffer[i+2] == '|'))
          break;
   }  
       for (i3=0;i3 < iMaxX; i3++)
       {
          cRoom[i3][i2] = cTest[i3];
       } 
  }
  
  	//Transition to black
	for(int iBright = 0; iBright > -32; iBright--){
		PA_SetBrightness(0, iBright); 
		PA_SetBrightness(1, iBright); 
		PA_WaitForVBL();		   
	}  	
	
   PA_SetSpriteXY(0,121,-32,-32);
	PA_SetSpriteXY(0,122,-32,-32);
	PA_SetSpriteXY(0,123,-64,-32);
	PA_SetSpriteXY(1,124,-16,-16);

	PA_ResetBgSys();
	
	//Transition to normal visible background
	for(int iBright = -31; iBright <= 0; iBright++){
		PA_SetBrightness(0, iBright); 
		PA_SetBrightness(1, iBright); 
		PA_WaitForVBL();		   
	}  
}

/**************************************************************************************************/

int PickLevel()
{ 		
   PA_ClearTextBg(1);
   
   int i;
      
   for(i=0;i<118;++i)
   {
     		PA_SetSpriteXY(0, i, -32, -32);
   } 

/*   
	//checks for FAT to be loaded. 
   if (!fatInitDefault())
   {
   	   PA_OutputText(1,0,3,"Cannot run game on emulator!");
   	   while(1) { PA_WaitForVBL(); }
	}   */

	fatInitDefault();
   
  	PA_ResetBgSys();
  	//displays the BGs
  	PA_SetBrightness(0, -31); // all black
	PA_SetBrightness(1, -31); // all black	
	PA_Init16bitBg(0, 1);
	PA_Load16bitBitmap(0, levelselect0_Bitmap);	
	PA_Init16bitBg(1, 1);
	PA_Load16bitBitmap(1, levelselect0_Bitmap);
	
	//loads the text system
	PA_InitText(0,0); 
	PA_InitText(1,0); 
   
	//sets text color to blue
	PA_SetTextCol(0, 0, 0, 20);
	PA_SetTextCol(1, 0, 0, 20);
	
	//Transition to normal visible background
	for(i = -31; i <= 0; i++){
		PA_SetBrightness(0, i); 
		PA_SetBrightness(1, i); 
		PA_WaitForVBL();		   
	}	
	
	int linenumber = 0; 
	
	PA_SetSpriteHflip(0, 121, 1);
	PA_SetSpriteXY(0,121,3,70);
	PA_SetSpriteXY(0,122,220,70);
	
	PA_StartSpriteAnim(0, 121, 0, 5, 8); 
	PA_StartSpriteAnim(0, 122, 0, 5, 8); 
	
	//start button
	PA_SetSpriteXY(0,123,96,155);

	struct stat st;
	char filename[256]; // to hold a full filename and string terminator
	
	DIR_ITER* dir = diropen("/Gekinzuku/ChipsChallengeDS/levels/");

	if (dir == NULL) 
	{
		PA_OutputText(1,2,2,"Unable to open the directory.");
	} 
	else 
	while (dirnext(dir, filename, &st) == 0) 
	{
		if ((linenumber > 1) && (linenumber < 9))
		{
			for (unsigned iNum = 0; iNum < sizeof(filename); ++iNum)
			{
				if ((filename[iNum] == '.') && (filename[iNum+1] == 'l') && (filename[iNum+2] == 'v') && (filename[iNum+3] == 'l'))
				{
					filename[iNum] = ' ';  
					filename[iNum+1] = ' ';
					filename[iNum+2] = ' ';
					filename[iNum+3] = ' ';
				}			
			}			
			PA_OutputText(0,9,(linenumber-1)*2 + 2, "%s\n", filename);
		}		
		linenumber++;//next line
	}

	int iTotal = linenumber;

	int iFile = 0;
	int iPage = 0;
	
	GetLevelDetails(iFile);	
	
	while (1)
	{ 	
	   int iFileTempo = iFile;
	   	
		if(Pad.Newpress.Down) iFile ++;
		else if (Pad.Newpress.Up) iFile --;
		
		if ((Stylus.Newpress) && (Stylus.Y > 30) && (Stylus.Y < 138)) 
		{
		   if (((Stylus.Y - 16)/16 - 1) + iPage * 7 <= iTotal - 3)
				iFile = ((Stylus.Y - 16)/16 - 1) + iPage * 7;
		}		
		
		if (iPage == 0) PA_SetSpriteXY(0,121,-32,-32);
		else PA_SetSpriteXY(0,121,3,70);
		
		if(iPage*7+7 >= iTotal - 2) PA_SetSpriteXY(0,122,-32,-32);
		else PA_SetSpriteXY(0,122,220,70);
		
		if (((PA_SpriteTouched(121)) && (Stylus.Newpress) && (iPage != 0)) || ((Pad.Newpress.L) && (iPage != 0)))
		{
		   	--iPage;
		   	iFile = 0;
		   	
				PA_ClearTextBg(0);
				linenumber = 0;
				
				dir = diropen("/Gekinzuku/ChipsChallengeDS/levels/");

				if (dir == NULL) 
				{
					PA_OutputText(1,2,2,"Unable to open the directory.");
				} 
				else 
				while (dirnext(dir, filename, &st) == 0) 
				{
					if ((linenumber > 1 + (iPage * 7)) && (linenumber < 9 + (iPage * 7)))
					{
						for (unsigned iNum = 0; iNum < sizeof(filename); ++iNum)
						{
							if ((filename[iNum] == '.') && (filename[iNum+1] == 'l') && (filename[iNum+2] == 'v') && (filename[iNum+3] == 'l'))
							{
								filename[iNum] = ' ';  
								filename[iNum+1] = ' ';
								filename[iNum+2] = ' ';
								filename[iNum+3] = ' ';
							}			
						}
						PA_OutputText(0,9,(linenumber-1 - (iPage * 7))*2 + 2, "%s\n", filename);
					}		
					linenumber++;//next line
				}  
		}		
		else if (((PA_SpriteTouched(122)) && (Stylus.Newpress) && (iPage*7+7 < iTotal - 2)) || ((Pad.Newpress.R) && (iPage*7+7 < iTotal - 2)))
		{
		   	++iPage;
		   	
				PA_ClearTextBg(0);
				linenumber = 0;
				
				dir = diropen("/Gekinzuku/ChipsChallengeDS/levels/");

				if (dir == NULL) 
				{
					PA_OutputText(1,2,2,"Unable to open the directory.");
				} 
				else 
				while (dirnext(dir, filename, &st) == 0) 
				{
					if ((linenumber > 1 + (iPage * 7)) && (linenumber < 9 + (iPage * 7)))
					{
					 
						for (unsigned iNum = 0; iNum < sizeof(filename); ++iNum)
						{
							if ((filename[iNum] == '.') && (filename[iNum+1] == 'l') && (filename[iNum+2] == 'v') && (filename[iNum+3] == 'l'))
							{
								filename[iNum] = ' ';  
								filename[iNum+1] = ' ';
								filename[iNum+2] = ' ';
								filename[iNum+3] = ' ';
							}			
						}
					   
						PA_OutputText(0,9,(linenumber-1 - (iPage * 7))*2 + 2, "%s\n", filename);
					}		
					linenumber++;//next line
				}
		}		
	
		//keeps iFile from going out of the bounds of the page
		if (iFile < 0 + (iPage*7)) iFile = 0 + (iPage*7);
		else if (iFile > 6 + (iPage*7)) iFile = 6 + (iPage*7);
		else if (iFile > iTotal - 3) iFile = iTotal - 3;
		
		if (iFile != iFileTempo)
		{
		   PA_ClearTextBg(1);
			GetLevelDetails(iFile);	   
		}
		
		if ((Pad.Newpress.A) || (Pad.Newpress.Start) || (PA_SpriteTouched(123))) 
		{
		   return iFile;
		}		
		
		PA_WaitForVBL();		
	}
}

/**************************************************************************************************/

void GetLevelDetails(int iFileTemp)
{
   char * buffer;
	unsigned int i;
  	unsigned long lSize;
 	int linenumber = 0;	
	char Fullfilename[99];
			
	
	struct stat st;
	char filename[256]; // to hold a full filename and string terminator
	
	DIR_ITER* dir = diropen("/Gekinzuku/ChipsChallengeDS/levels/");
	
	while (dirnext(dir, filename, &st) == 0) 
	{
		// st.st_mode & S_IFDIR indicates a directory
		if (linenumber == iFileTemp+2)
		{
		   //PA_ClearTextBg(1);
			
			char cTemp2[36] = { '/','G','e','k','i','n','z','u','k','u','/','C','h','i','p','s','C','h','a','l','l','e','n','g','e','D','S','/','l','e','v','e','l','s','/'};
			strcpy(Fullfilename, cTemp2);
			strcat(Fullfilename, filename);
			break;
		}		
		linenumber++;//next line
	}
	
	/***********************************************************************************************/
	
	//loads all the level info and puts them in their proper varibles
	
	FILE * pFile;
  	size_t result;
  	pFile = fopen (Fullfilename, "rb" );
  	if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

  // obtain file size:
  fseek (pFile , 0 , SEEK_END);
  lSize = ftell (pFile);
  rewind (pFile);

  // allocate memory to contain the whole file:
  buffer = (char*) malloc (sizeof(char)*lSize);
  if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

  // copy the file into the buffer:
  result = fread (buffer,1,lSize,pFile);
  if (result != lSize) {fputs ("Reading error",stderr); exit (3);}


  // terminate
  fclose (pFile);
  
  //decrypts level
  buffer = Decrypt(buffer, lSize);

  //gets version support
  char cTemp[20];
  char version[9];
  
  for(i = 0; i < lSize; i++)
  {
        version[i] = buffer[i];
        if((buffer[i+1] == '|') && (buffer[i+2] == '|'))
          break;
  }
  
  //version[i+1] = '\0';

   PA_SetSpriteXY(1,124,80,28);
	//displays an X if the game isn't the same version as the level was made for

	VerifyVersion(version);	

/*	if ((version[0] == '1') && (version[1] == '.') && (version[2] == '1'))
			PA_SetSpriteAnim(1,124,0);
	else
		PA_SetSpriteAnim(1,124,1); */
  
	PA_OutputText(1,12,4,"Version %s   ", version);
  	
	PA_SetSpriteXY(0,124,30,70);
  
  /****************************************************************************/
  
  //gets level name
  int iTemp = i;
  char LevelName[30];
  
  for(i = i+2; i < lSize; i++)
  {
        LevelName[i - iTemp -3] = buffer[i];
        if((buffer[i+1] == '|') && (buffer[i+2] == '|'))
          break;
  }
  LevelName[i-iTemp - 2] = '\0';
  
  	PA_OutputText(1,7,6,"Level:\n %s", LevelName);
  
  /****************************************************************************/
  
  //gets difficulty
  iTemp = i;
  char Difficulty[20];
  
  for(i = i+2; i < lSize; i++)
  {
        Difficulty[i - iTemp -3] = buffer[i];
        if((buffer[i+1] == '|') && (buffer[i+2] == '|'))
          break;
  }
  
  Difficulty[i-iTemp - 2] = '\0';
  
	PA_OutputText(1,7,9,"Rating: %s", Difficulty);
  
  /****************************************************************************/

   //gets time
  iTemp = i;
  
  for(i = i+2; i < lSize; i++)
  {
        cTemp[i - iTemp -3] = buffer[i];
        if((buffer[i+1] == '|') && (buffer[i+2] == '|'))
          break;
  }

  
  cTemp[i-iTemp - 2] = '\0';
    
	PA_OutputText(1,7,11,"Time: %s seconds.", cTemp);  
  
  /***************************************************************************/
  
  //gets chips
  iTemp = i;
  
  for(i = i+2; i < lSize; i++)
  {
        cTemp[i - iTemp -3] = buffer[i];
        if((buffer[i+1] == '|') && (buffer[i+2] == '|'))
          break;
  }
  
  cTemp[i-iTemp - 2] = '\0';
   
	PA_OutputText(1,7,13,"Chips: %s", cTemp); 
}

/**************************************************************************************************/

char * Decrypt(char * buffer, long lSize)
{
   int i;
	//unshuffels
   for(i = 0; i < lSize; i+=3)
   {
         char cTemp = buffer[i+1];
         char cTemp2 = buffer[i+2];
         char cTemp3 = buffer[i];
         
         buffer[i] = cTemp;
         buffer[i+1] = cTemp2;
         buffer[i+2] = cTemp3;
   }   

	//incriments it back
   for(i = 0; i < lSize; ++i)
   {
         buffer[i] += i;
   }      
   return buffer;
}

// Verifys the version of the game
void VerifyVersion(char version[9])
{
   int i;
   bool bVerify = true;
   for (i = 0; i < 9; ++i)
   {    
      if ((GAME_VERSION[i] == '\0') || (version[i] == '\0'))
      {
   	   break;      	   
   	}   
   	
   	if(version[i] != GAME_VERSION[i])
   	{
   	   bVerify = false;
   	   break;
 		}  	
	}   
	
	if(bVerify)
		PA_SetSpriteAnim(1,124,0);
	else
		PA_SetSpriteAnim(1,124,1);
}

void TempFunction()
{
   iMaxX = 20;
   iMaxY = 17;
   iLocX = 12;
   iLocY = 6;
   iChips = 10;
   
   cRoom[0][0] = 'w';
   cRoom[1][0] = 'w';
   cRoom[2][0] = 'w';
   cRoom[3][0] = 'w';
   cRoom[4][0] = 'w';
   cRoom[5][0] = 'w';
   cRoom[6][0] = 'w';
   cRoom[7][0] = 'w';
   cRoom[8][0] = 'w';
   cRoom[9][0] = 'w';
   cRoom[10][0] = 'w';
   cRoom[11][0] = 'w';
   cRoom[12][0] = 'w';
   cRoom[13][0] = 'w';
   cRoom[14][0] = 'w';
   cRoom[15][0] = 'w';
   cRoom[16][0] = 'w';
   cRoom[17][0] = 'w';
   cRoom[18][0] = 'w';
   cRoom[19][0] = 'w';

   cRoom[0][1] = 'w';
   cRoom[1][1] = 'i';
   cRoom[2][1] = '5';
   cRoom[3][1] = 'i';
   cRoom[4][1] = 'i';
   cRoom[5][1] = 'i';
   cRoom[6][1] = 'i';
   cRoom[7][1] = 'i';
   cRoom[8][1] = 'i';
   cRoom[9][1] = '5';
   cRoom[10][1] = 'i';
   cRoom[11][1] = 'i';
   cRoom[12][1] = 'i';
   cRoom[13][1] = 'i';
   cRoom[14][1] = 'i';
   cRoom[15][1] = 'i';
   cRoom[16][1] = 'i';
   cRoom[17][1] = 'c';
   cRoom[18][1] = '5';
   cRoom[19][1] = 'w';

   cRoom[0][2] = 'w';
   cRoom[1][2] = 'i';
   cRoom[2][2] = 'a';
   cRoom[3][2] = '5';
   cRoom[4][2] = 'a';
   cRoom[5][2] = 'a';
   cRoom[6][2] = 'a';
   cRoom[7][2] = 'i';
   cRoom[8][2] = 'a';
   cRoom[9][2] = 'i';
   cRoom[10][2] = 'a';
   cRoom[11][2] = 'a';
   cRoom[12][2] = 'a';
   cRoom[13][2] = 'i';
   cRoom[14][2] = 'c';
   cRoom[15][2] = 'a';
   cRoom[16][2] = 'a';
   cRoom[17][2] = 'i';
   cRoom[18][2] = 'i';
   cRoom[19][2] = 'w';

   cRoom[0][3] = 'w';
   cRoom[1][3] = '5';
   cRoom[2][3] = 'a';
   cRoom[3][3] = 'a';
   cRoom[4][3] = 'a';
   cRoom[5][3] = 'a';
   cRoom[6][3] = 'a';
   cRoom[7][3] = 'i';
   cRoom[8][3] = 'a';
   cRoom[9][3] = 'i';
   cRoom[10][3] = 'a';
   cRoom[11][3] = 'a';
   cRoom[12][3] = 'a';
   cRoom[13][3] = 'i';
   cRoom[14][3] = 'i';
   cRoom[15][3] = 'a';
   cRoom[16][3] = 'a';
   cRoom[17][3] = 'i';
   cRoom[18][3] = 'i';
   cRoom[19][3] = 'w';

   cRoom[0][4] = 'w';
   cRoom[1][4] = 'i';
   cRoom[2][4] = 'c';
   cRoom[3][4] = 'i';
   cRoom[4][4] = 'i';
   cRoom[5][4] = 'i';
   cRoom[6][4] = 'i';
   cRoom[7][4] = 'i';
   cRoom[8][4] = 'i';
   cRoom[9][4] = 'i';
   cRoom[10][4] = 'i';
   cRoom[11][4] = 'i';
   cRoom[12][4] = 'i';
   cRoom[13][4] = '5';
   cRoom[14][4] = 'i';
   cRoom[15][4] = 't';
   cRoom[16][4] = 't';
   cRoom[17][4] = 'i';
   cRoom[18][4] = 'i';
   cRoom[19][4] = 'w';

   cRoom[0][5] = 'w';
   cRoom[1][5] = 'i';
   cRoom[2][5] = '5';
   cRoom[3][5] = 'i';
   cRoom[4][5] = 'i';
   cRoom[5][5] = 'i';
   cRoom[6][5] = 'i';
   cRoom[7][5] = '5';
   cRoom[8][5] = 'i';
   cRoom[9][5] = 'i';
   cRoom[10][5] = 'i';
   cRoom[11][5] = 'i';
   cRoom[12][5] = 'i';
   cRoom[13][5] = 'i';
   cRoom[14][5] = 'i';
   cRoom[15][5] = 'a';
   cRoom[16][5] = 'a';
   cRoom[17][5] = 'a';
   cRoom[18][5] = 'c';
   cRoom[19][5] = 'w';

   cRoom[0][6] = 'w';
   cRoom[1][6] = '5';
   cRoom[2][6] = 'i';
   cRoom[3][6] = 'i';
   cRoom[4][6] = '5';
   cRoom[5][6] = 'i';
   cRoom[6][6] = 'i';
   cRoom[7][6] = 'i';
   cRoom[8][6] = 'i';
   cRoom[9][6] = 'i';
   cRoom[10][6] = 'i';
   cRoom[11][6] = 'i';
   cRoom[12][6] = 'i';
   cRoom[13][6] = 'i';
   cRoom[14][6] = 'i';
   cRoom[15][6] = 'a';
   cRoom[16][6] = 'c';
   cRoom[17][6] = 'c';
   cRoom[18][6] = 'c';
   cRoom[19][6] = 'w';

   cRoom[0][7] = 'w';
   cRoom[1][7] = 'w';
   cRoom[2][7] = 'w';
   cRoom[3][7] = 'a';
   cRoom[4][7] = 'i';
   cRoom[5][7] = 'a';
   cRoom[6][7] = 'a';
   cRoom[7][7] = 'i';
   cRoom[8][7] = 'a';
   cRoom[9][7] = 'i';
   cRoom[10][7] = 'a';
   cRoom[11][7] = 'a';
   cRoom[12][7] = 'a';
   cRoom[13][7] = 'i';
   cRoom[14][7] = 'i';
   cRoom[15][7] = 'a';
   cRoom[16][7] = 'c';
   cRoom[17][7] = 'w';
   cRoom[18][7] = 'w';
   cRoom[19][7] = 'w';

   cRoom[0][8] = 'w';
   cRoom[1][8] = 'I';
   cRoom[2][8] = 'w';
   cRoom[3][8] = 'a';
   cRoom[4][8] = 'c';
   cRoom[5][8] = 'i';
   cRoom[6][8] = 'i';
   cRoom[7][8] = 'i';
   cRoom[8][8] = 'i';
   cRoom[9][8] = '5';
   cRoom[10][8] = 'a';
   cRoom[11][8] = 'a';
   cRoom[12][8] = 'a';
   cRoom[13][8] = 'i';
   cRoom[14][8] = 'i';
   cRoom[15][8] = 'i';
   cRoom[16][8] = 'f';
   cRoom[17][8] = 'G';
   cRoom[18][8] = 'p';
   cRoom[19][8] = 'w';

   cRoom[0][9] = 'w';
   cRoom[1][9] = 's';
   cRoom[2][9] = 'w';
   cRoom[3][9] = 'a';
   cRoom[4][9] = 'i';
   cRoom[5][9] = 'a';
   cRoom[6][9] = 'a';
   cRoom[7][9] = 'i';
   cRoom[8][9] = 'a';
   cRoom[9][9] = 'i';
   cRoom[10][9] = 'a';
   cRoom[11][9] = 'a';
   cRoom[12][9] = 'a';
   cRoom[13][9] = 'i';
   cRoom[14][9] = 'i';
   cRoom[15][9] = 'i';
   cRoom[16][9] = 'i';
   cRoom[17][9] = 'w';
   cRoom[18][9] = 'g';
   cRoom[19][9] = 'w';

   cRoom[0][10] = 'w';
   cRoom[1][10] = 't';
   cRoom[2][10] = 'w';
   cRoom[3][10] = 'c';
   cRoom[4][10] = 'i';
   cRoom[5][10] = 'i';
   cRoom[6][10] = 'i';
   cRoom[7][10] = 'i';
   cRoom[8][10] = 'i';
   cRoom[9][10] = 'i';
   cRoom[10][10] = 'i';
   cRoom[11][10] = 'i';
   cRoom[12][10] = 'i';
   cRoom[13][10] = 'i';
   cRoom[14][10] = '5';
   cRoom[15][10] = 'i';
   cRoom[16][10] = '5';
   cRoom[17][10] = 'w';
   cRoom[18][10] = 't';
   cRoom[19][10] = 'w';

   cRoom[0][11] = 'w';
   cRoom[1][11] = 't';
   cRoom[2][11] = 'w';
   cRoom[3][11] = 'h';
   cRoom[4][11] = 'i';
   cRoom[5][11] = 'i';
   cRoom[6][11] = 'i';
   cRoom[7][11] = '5';
   cRoom[8][11] = 'i';
   cRoom[9][11] = 'f';
   cRoom[10][11] = 'a';
   cRoom[11][11] = 'a';
   cRoom[12][11] = 'a';
   cRoom[13][11] = 'i';
   cRoom[14][11] = 'a';
   cRoom[15][11] = 'a';
   cRoom[16][11] = 'a';
   cRoom[17][11] = 'w';
   cRoom[18][11] = 'u';
   cRoom[19][11] = 'w';

   cRoom[0][12] = 'w';
   cRoom[1][12] = 't';
   cRoom[2][12] = 'w';
   cRoom[3][12] = 'w';
   cRoom[4][12] = 't';
   cRoom[5][12] = 'w';
   cRoom[6][12] = 'w';
   cRoom[7][12] = 'w';
   cRoom[8][12] = 'w';
   cRoom[9][12] = 'w';
   cRoom[10][12] = 'w';
   cRoom[11][12] = 'w';
   cRoom[12][12] = 'w';
   cRoom[13][12] = 't';
   cRoom[14][12] = 'w';
   cRoom[15][12] = 'w';
   cRoom[16][12] = 'w';
   cRoom[17][12] = 'w';
   cRoom[18][12] = 't';
   cRoom[19][12] = 'w';

   cRoom[0][13] = 'w';
   cRoom[1][13] = 't';
   cRoom[2][13] = 't';
   cRoom[3][13] = 't';
   cRoom[4][13] = 't';
   cRoom[5][13] = '5';
   cRoom[6][13] = 't';
   cRoom[7][13] = 'a';
   cRoom[8][13] = 'a';
   cRoom[9][13] = 'a';
   cRoom[10][13] = 't';
   cRoom[11][13] = 'A';
   cRoom[12][13] = 't';
   cRoom[13][13] = 't';
   cRoom[14][13] = 'h';
   cRoom[15][13] = 't';
   cRoom[16][13] = 't';
   cRoom[17][13] = 't';
   cRoom[18][13] = 't';
   cRoom[19][13] = 'w';

   cRoom[0][14] = 'w';
   cRoom[1][14] = '%';
   cRoom[2][14] = 'w';
   cRoom[3][14] = 'w';
   cRoom[4][14] = 'w';
   cRoom[5][14] = 'w';
   cRoom[6][14] = 'w';
   cRoom[7][14] = 'w';
   cRoom[8][14] = 'w';
   cRoom[9][14] = 'w';
   cRoom[10][14] = 'w';
   cRoom[11][14] = 'w';
   cRoom[12][14] = 'w';
   cRoom[13][14] = 'w';
   cRoom[14][14] = 'w';
   cRoom[15][14] = 'w';
   cRoom[16][14] = 'w';
   cRoom[17][14] = 'w';
   cRoom[18][14] = 'w';
   cRoom[19][14] = 'w';

   cRoom[0][15] = 'w';
   cRoom[1][15] = '%';
   cRoom[2][15] = '%';
   cRoom[3][15] = '%';
   cRoom[4][15] = '%';
   cRoom[5][15] = '%';
   cRoom[6][15] = '%';
   cRoom[7][15] = '%';
   cRoom[8][15] = '%';
   cRoom[9][15] = '%';
   cRoom[10][15] = '%';
   cRoom[11][15] = '%';
   cRoom[12][15] = '%';
   cRoom[13][15] = '%';
   cRoom[14][15] = '%';
   cRoom[15][15] = 's';
   cRoom[16][15] = 's';
   cRoom[17][15] = 's';
   cRoom[18][15] = 'e';
   cRoom[19][15] = 'w';

   cRoom[0][16] = 'w';
   cRoom[1][16] = 'w';
   cRoom[2][16] = 'w';
   cRoom[3][16] = 'w';
   cRoom[4][16] = 'w';
   cRoom[5][16] = 'w';
   cRoom[6][16] = 'w';
   cRoom[7][16] = 'w';
   cRoom[8][16] = 'w';
   cRoom[9][16] = 'w';
   cRoom[10][16] = 'w';
   cRoom[11][16] = 'w';
   cRoom[12][16] = 'w';
   cRoom[13][16] = 'w';
   cRoom[14][16] = 'w';
   cRoom[15][16] = 'w';
   cRoom[16][16] = 'w';
   cRoom[17][16] = 'w';
   cRoom[18][16] = 'w';
   cRoom[19][16] = 'w';
}   
