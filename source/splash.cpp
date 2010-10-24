/****************************************
 * 		NDS NeHe Lesson 11    			*
 * 		Author: Dovoto					*
 ****************************************/

// include your ndslib
#include <nds.h>
#include <malloc.h>
#include <stdio.h>
#include <PA9.h>

//needed to load pcx files
#include <nds/arm9/image.h>
#include <nds/arm9/trig_lut.h>
#include "splash.h"

#include "gekinzuku.h"


v16 points[64][32][3];    // The Array For The Points On The Grid Of Our "Wave"
int wiggle_count = 0;		// Counter Used To Control How Fast Flag Waves

float	xrot;				// X Rotation ( NEW )
float	yrot;				// Y Rotation ( NEW )
float	zrot;				// Z Rotation ( NEW )
v16 hold;				// Temporarily Holds A Floating Point Value

int	texture[1];			// Storage For 3 Textures (only going to use 1 on the DS for this demo)



float sin(float angle)
{
	int32 s = SIN[(int)((angle * LUT_SIZE) / 360.0) & LUT_MASK];

	return f32tofloat(s);
}

float cos(float angle)
{
	int32 c = COS[(int)((angle * LUT_SIZE) / 360.0) & LUT_MASK];

	return f32tofloat(c);
}
 
 
int LoadGLTextures()									// Load PCX files And Convert To Textures
{
	sImage pcx;                //////////////(NEW) and different from nehe.

	//load our texture
	loadPCX((u8*)gekinzuku, &pcx);
	
	image8to16(&pcx);

	glGenTextures(1, &texture[0]);
	glBindTexture(0, texture[0]);
	glTexImage2D(0, 0, GL_RGB, TEXTURE_SIZE_256 , TEXTURE_SIZE_256, 0, TEXGEN_TEXCOORD, pcx.image.data8);

	return TRUE;
}
void InitGL(void)	
{
		// Turn on everything
//	powerON(POWER_ALL);
	
	// Setup the Main screen for 3D 
	videoSetMode(MODE_0_3D);
	vramSetBankA(VRAM_A_TEXTURE);                        //NEW  must set up some memory for textures
	
//		videoSetModeSub(MODE_0_2D | DISPLAY_BG0_ACTIVE);	//sub bg 0 will be used to print text/2D
//	vramSetBankC(VRAM_C_SUB_BG);
//	SUB_BG0_CR = BG_MAP_BASE(31);
	
//	BG_PALETTE_SUB[255] = RGB15(31,31,31);	//by default font will be rendered with color 255

	// IRQ basic setup
//	irqInit();
//	irqSet(IRQ_VBLANK, 0);
	
	// initialize the geometry engine
	glInit();
	
	// enable textures
	glEnable(GL_TEXTURE_2D);
	
	// Set our viewport to be the same size as the screen
	glViewport(0,0,255,191);
	
	// enable antialiasing
	glEnable(GL_ANTIALIAS);
	
	// setup the rear plane
	glClearColor(0,0,0,31); // BG must be opaque for AA to work
	glClearPolyID(63); // BG must have a unique polygon ID for AA to work
	glClearDepth(0x7FFF);
	
	LoadGLTextures();
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, 256.0 / 192.0, 0.1, 100);
	
	//need to set up some material properties since DS does not have them set by default
	glMaterialf(GL_AMBIENT, RGB15(31,31,31));
	glMaterialf(GL_DIFFUSE, RGB15(31,31,31));
	glMaterialf(GL_SPECULAR, BIT(15) | RGB15(16,16,16));
	glMaterialf(GL_EMISSION, RGB15(31,31,31));
	
	//ds uses a table for shinyness..this generates a half-ass one
	glMaterialShinyness();
	
	//ds specific, several attributes can be set here	
	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE );
	
	for(int x=0; x<32; x++)
	{
		for(int y=0; y<32; y++)
		{
			points[x][y][0]= (inttov16(x)/4);
			points[x][y][1]= (inttov16(y)/4);
			points[x][y][2]= SIN[ (x * 16) & LUT_MASK];
		}
	}
	
	return;
}

float i = -32;

void DrawSplash()
{	
	InitGL();
	
	glMatrixMode(GL_MODELVIEW);
	
	int iLOL = 0;
	
//	int iHateYou = 0;
	
	float God = 0;
	
	while (1) 
	{
		DrawGLScene();
		
		// flush to screen	
		glFlush(0);
		
		iLOL++;
		
		if (iLOL > 185)
		{
				God -= 0.25;
				SetBrightness(0, int(God)); 
				SetBrightness(1, int(God)); 
				
				if (God == -32)
				{
					return;	   
				}			
		}		
			
		// Wait for the screen to refresh
		swiWaitForVBlank();
	}
	
	return;
}

void SplashScreen()//used the splash screen code from palib
{
	lcdMainOnTop();	
	DrawSplash();
	
	PA_Init2D();

	PA_ResetBgSys();
	PA_SetBrightness(0, 0); //normal
	PA_SetBrightness(1, 0); //normal
}

void SetBrightness(u8 screen, s8 bright) {
u16 mode = 1 << 14;

	if (bright < 0) {
		mode = 2 << 14;
		bright = -bright;
	}
	if (bright > 31) bright = 31;
	_REG16(REG_BRIGHT + (0x1000 * screen)) = bright + mode;
}


int DrawGLScene()											// Here's Where We Do All The Drawing
{
   
   i += 0.25;
   
   if (i > 0)
   i = 0;
   
	SetBrightness(1, int(i)); 
	SetBrightness(0, int(i)); 
   
   
	int x, y;
	t16 float_x, float_y, float_xb, float_yb;
	
	glColor3b(255,255,255);    // set the vertex color
	
	glLoadIdentity();									// Reset The View

	glTranslatef(-4.0f,4.0f,-5.0f);
	  
//	glRotatef(xrot,1.0f,0.0f,0.0f);
	glRotatef(180,0.0f,1.0f,0.0f);  
	glRotatef(180,0.0f,0.0f,1.0f);

	glBindTexture(GL_TEXTURE_2D, texture[0]);

	glBegin(GL_QUADS);
	for( x = 0; x < 31; x++ )
	{
		for( y = 0; y < 31; y++ )
		{
			float_x = inttot16(x) << 3;
			float_y = inttot16(y) << 3;
			float_xb = inttot16(x+1) << 3;
			float_yb = inttot16(y+1) << 3;

			glTexCoord2t16( float_x, float_y);
			glVertex3v16( points[x][y][0], points[x][y][1], points[x][y][2] );

			glTexCoord2t16( float_x, float_yb );
			glVertex3v16( points[x][y+1][0], points[x][y+1][1], points[x][y+1][2] );

			glTexCoord2t16( float_xb, float_yb );
			glVertex3v16( points[x+1][y+1][0], points[x+1][y+1][1], points[x+1][y+1][2] );

			glTexCoord2t16( float_xb, float_y );
			glVertex3v16( points[x+1][y][0], points[x+1][y][1], points[x+1][y][2] );
		}
	}
	glEnd();

	if( wiggle_count == 3 )
	{
		for( y = 0; y < 32; y++ )
		{
			hold=points[0][y][2];
			for( x = 0; x < 32; x++)
			{
				points[x][y][2] = points[x+1][y][2];
			}
			points[31][y][2]=hold;
		}
		wiggle_count = 0;
	}

	wiggle_count++;

	xrot+=0.3f;
	yrot+=0.2f;
	zrot+=0.4f;

	return TRUE;											// Everything Went OK

}
