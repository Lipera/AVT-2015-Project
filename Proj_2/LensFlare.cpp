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
	createCube();

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

void LensFlare::draw(struct MyMesh* mesh, VSShaderLib& shader, GLint& pvm_uniformId, GLint& vm_uniformId, GLint& normal_uniformId, GLint& texMode_uniformId, int *objId){

	*objId=21;
	GLint loc;
	int aux;
			// send the material
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
			glUniform4fv(loc, 1, mesh[*objId].mat.ambient);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
			glUniform4fv(loc, 1, mesh[*objId].mat.diffuse);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
			glUniform4fv(loc, 1, mesh[*objId].mat.specular);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
			glUniform1f(loc,mesh[*objId].mat.shininess);
			pushMatrix(MODEL);
			translate(MODEL, getPosition().getX(), getPosition().getY(), getPosition().getZ());
			scale(MODEL, 4.0f, 1.0f, 4.0f);

			// send matrices to OGL
			computeDerivedMatrix(PROJ_VIEW_MODEL);
			glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
			glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
			computeNormalMatrix3x3();
			glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);
				
			if(getTexture() == 0){
				glUniform1i(texMode_uniformId, 16); //Flare1.tga Bola
			}else if(getTexture() == 1){
				glUniform1i(texMode_uniformId, 17); //Flare2.tga Foco
			}else if(getTexture() == 2){
				glUniform1i(texMode_uniformId, 18); //Flare3.tga Hexagono
			}else if(getTexture() == 3){
				glUniform1i(texMode_uniformId, 19); //Flare4.tga Circulo
			}

			glBindVertexArray(mesh[*objId].vao);
			glDrawElements(mesh[*objId].type,mesh[*objId].numIndexes, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			popMatrix(MODEL);

}