#include "Board.h"

/// The storage for matrices
extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];

/// The normal matrix
extern float mNormal3x3[9];

//constructor
Board::Board(){
    //to do
}

//destructor
Board::~Board(){
    //to do
}

//desenha mesa
void Board::create(struct MyMesh* mesh){

	float amb[]= {0.2f, 0.15f, 0.1f, 1.0f};
	float diff[] = {0.8f, 0.6f, 0.4f, 1.0f};
	float spec[] = {0.8f, 0.8f, 0.8f, 1.0f};

	float emissive[] = {0.0f, 0.0f, 0.0f, 1.0f};
	float shininess= 100.0f;
	int texcount = 0;

	int objId=1;
	memcpy(mesh[objId].mat.ambient, amb,4*sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff,4*sizeof(float));
	memcpy(mesh[objId].mat.specular, spec,4*sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive,4*sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	createCube();
}

void Board::draw(struct MyMesh* mesh, VSShaderLib& shader, GLint& pvm_uniformId, GLint& vm_uniformId, GLint& normal_uniformId, GLint& texMode_uniformId){
	int objId=1;
	GLint loc;
	// send the material
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
	glUniform4fv(loc, 1, mesh[objId].mat.ambient);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
	glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
	glUniform4fv(loc, 1, mesh[objId].mat.specular);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
	glUniform1f(loc,mesh[objId].mat.shininess);
	pushMatrix(MODEL);
	translate(MODEL, -15.5f, -3.0f, -15.5f);
	scale(MODEL, 31.0f, 3.0f, 31.0f);

	// send matrices to OGL
	computeDerivedMatrix(PROJ_VIEW_MODEL);
	glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
	glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
	computeNormalMatrix3x3();
	glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

	// Render mesh
	//glUniform1i(texMode_uniformId, 0); // modulate Phong color with texel color
	//glUniform1i(texMode_uniformId, 1); // s� componente especular
	glUniform1i(texMode_uniformId, 2); // multitexturing

	glBindVertexArray(mesh[objId].vao);
	glDrawElements(mesh[objId].type,mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	popMatrix(MODEL);
}