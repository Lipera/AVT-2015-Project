#include "Cheerios.h"

/// The storage for matrices
extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];

/// The normal matrix
extern float mNormal3x3[9];


//constructor
Cheerios::Cheerios(float x, float y, float z){
    _position = new Vector3(x, y, z);
    _speed = new Vector3();
	setAngle(0.0f);
	_bottomLeft = new Vector3(-0.5f, 0.0f, -0.5f);
	_topRight = new Vector3(0.5f, 0.0f, 0.5f);
}

//destructor
Cheerios::~Cheerios(){
    //to do
}

void Cheerios::create(struct MyMesh* mesh, int *objId){

	//cereals
	float amb[]= {0.02f, 0.02f, 0.02f, 1.0f};
	float diff[] = {0.95f, 0.84f, 0.65f, 1.0f};
	float spec[] = {0.71f, 0.66f, 0.73f, 1.0f};
	

	float emissive[] = {0.0f, 0.0f, 0.0f, 1.0f};
	float shininess= 1000.0f;
	int texcount = 0;

	*objId=12;
	memcpy(mesh[*objId].mat.ambient, amb,4*sizeof(float));
	memcpy(mesh[*objId].mat.diffuse, diff,4*sizeof(float));
	memcpy(mesh[*objId].mat.specular, spec,4*sizeof(float));
	memcpy(mesh[*objId].mat.emissive, emissive,4*sizeof(float));
	mesh[*objId].mat.shininess = shininess;
	mesh[*objId].mat.texCount = texcount;
	createTorus(0.1f, 0.5f, 20, 20);
}

void Cheerios::draw(struct MyMesh* mesh, VSShaderLib& shader, GLint& pvm_uniformId, GLint& vm_uniformId, GLint& normal_uniformId, GLint& texMode_uniformId, int *objId){
	*objId=12;
	GLint loc;
	//int track;
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
		translate(MODEL, _position->getX(), _position->getY(), _position->getZ());
		// send matrices to OGL
		computeDerivedMatrix(PROJ_VIEW_MODEL);
		glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
		glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
		computeNormalMatrix3x3();
		glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

		// Render mesh
		glUniform1i(texMode_uniformId, 7); // só o texel

		glBindVertexArray(mesh[*objId].vao);
		glDrawElements(mesh[*objId].type,mesh[*objId].numIndexes, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		popMatrix(MODEL);
	
}

void Cheerios::update(int delta_t){

}

