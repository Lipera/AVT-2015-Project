#include "LensFlare.h"

/// The storage for matrices
extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];

/// The normal matrix
extern float mNormal3x3[9];

//constructor
LensFlare::LensFlare(int value){
    setTexture(value);
}

//destructor
LensFlare::~LensFlare(){
    //to do
}

void LensFlare::setTexture(int value){
	_texture=value;
}
int LensFlare::getTexture(){
	return _texture;
}


//desenha mesa
void LensFlare::create(struct MyMesh* mesh, int *objId){

	float amb[]={0.18f,0.18f,0.18f,1.0f};
	float diff[]={1.0f,1.0f,1.0f,1.0f};
	float spec[]={1.0f,1.0f,1.0f,1.0f};

	float emissive[] = {0.0f, 0.0f, 0.0f, 1.0f};
	float shininess= 89.0f;
	int texcount = 0;

	*objId=21;
	memcpy(mesh[*objId].mat.ambient, amb,4*sizeof(float));
	memcpy(mesh[*objId].mat.diffuse, diff,4*sizeof(float));
	memcpy(mesh[*objId].mat.specular, spec,4*sizeof(float));
	memcpy(mesh[*objId].mat.emissive, emissive,4*sizeof(float));
	mesh[*objId].mat.shininess = shininess;
	mesh[*objId].mat.texCount = texcount;
	createSphere(3.0f, 20);
	//createQuad(5.0f, 5.0f);

}

void LensFlare::setPosition(float x, float y, float z){
	_position.set(x, y, z);
}

Vector3 LensFlare::getPosition(){
	return _position;
}

void LensFlare::setColor(float r, float g, float b){
	_color.set(r, g, b);
}

Vector3 LensFlare::getColor(){
	return _color;
}

