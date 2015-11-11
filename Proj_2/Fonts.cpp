#include "Fonts.h"

void Fonts::renderBitmapString(float x, float y, void *font,char *string) {
  
  char *c;
  // set position to start drawing fonts
  glRasterPos2f(x, y);
  // loop all the characters in the string
  for (c=string; *c != '\0'; c++) {
    glutBitmapCharacter(font, *c);
  }

}


void Fonts::renderSpacedBitmapString(float x, float y,int spacing, void *font,char *string) {

  char *c;
  int x1=x;
  for (c=string; *c != '\0'; c++) {
	glRasterPos2f(x1,y);
    glutBitmapCharacter(font, *c);
	x1 = x1 + glutBitmapWidth(font,*c) + spacing;
  }

}


void Fonts::renderVerticalBitmapString(float x, float y, int bitmapHeight, void *font,char *string){
  
  char *c;
  int i;
  for (c=string,i=0; *c != '\0'; i++,c++) {
	glRasterPos2f(x, y+bitmapHeight*i);
    glutBitmapCharacter(font, *c);
  }

}

void Fonts::renderStrokeString(float x, float y, float z, void *font,char *string){
  
  char *c;
  pushMatrix(MODEL);
  translate(MODEL, x, y, z);
  for (c=string; *c != '\0'; c++) {
    glutStrokeCharacter(font, *c);
  }
  popMatrix(MODEL);

}

void Fonts::renderBitmapString(float x, float y, float z, void *font, char *string) {  

  char *c;
  glRasterPos3f(x, y,z);
  for (c=string; *c != '\0'; c++) {
    glutBitmapCharacter(font, *c);
  }

}
