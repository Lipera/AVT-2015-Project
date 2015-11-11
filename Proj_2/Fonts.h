#ifndef FONTS_H
#define FONTS_H

#include <gl/glut.h>
#include <gl/gl.h>
#include "AVTmathLib.h"

class Fonts {


public:
	Fonts();
	~Fonts();
	void renderBitmapString(float x, float y, void *font,char *string);
	void renderSpacedBitmapString(float x, float y,int spacing, void *font,char *string);
	void renderVerticalBitmapString(float x, float y, int bitmapHeight, void *font,char *string);
	void renderStrokeString(float x, float y, float z, void *font,char *string);
	void renderBitmapString(float x,float y,float z,void *font,char *string);

};

#endif