void LevelSelect();
void TempFunction();
void GetLevelDetails(int);
int PickLevel();
char * Decrypt(char *, long);
void VerifyVersion(char[9]);	

extern char cRoom[99][99];
extern short int iMaxX, iMaxY; //used to know the max size of the room
extern short int iLocX, iLocY; //used to keep the current location of the screen in the room
extern short int iChips;
extern short int iTime; // used to know how long the player has to beat the level
extern short int iLevel; //used to know what number level it is
extern short int iMoves;
extern char * cHint;

