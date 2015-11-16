#include "Pot.h"

/// The storage for matrices
extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];

/// The normal matrix
extern float mNormal3x3[9];

//constructor
Pot::Pot(){
    //to do
}

//destructor
Pot::~Pot(){
    //to do
}

//desenha mesa
void Pot::create(struct MyMesh* mesh, int *objId){

	float amb[]= {0.2f, 0.2f, 0.2f, 1.0f};
	float diff[] = {0.5f, 0.3f, 0.2f, 1.0f};
	float spec[] = {0.6f, 0.7f, 0.6f, 1.0f};

	float emissive[] = {0.0f, 0.0f, 0.0f, 0.7f};
	float shininess= 100.0f;
	int texcount = 0;

	*objId=18;
	memcpy(mesh[*objId].mat.ambient, amb,4*sizeof(float));
	memcpy(mesh[*objId].mat.diffuse, diff,4*sizeof(float));
	memcpy(mesh[*objId].mat.specular, spec,4*sizeof(float));
	memcpy(mesh[*objId].mat.emissive, emissive,4*sizeof(float));
	mesh[*objId].mat.shininess = shininess;
	mesh[*objId].mat.texCount = texcount;
	createCylinder(0.5f, 0.5f, 20);

}

void Pot::draw(struct MyMesh* mesh, VSShaderLib& shader, GLint& pvm_uniformId, GLint& vm_uniformId, GLint& normal_uniformId, GLint& texMode_uniformId, int *objId){
	GLint loc;

	*objId=18;

	 for(int j = 1; j < 3; j++) {
           
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
				translate(MODEL,-7.0f, 0.25f,-9.85f+j*5);
	
		// send matrices to OGL
		computeDerivedMatrix(PROJ_VIEW_MODEL);
		glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
		glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
		computeNormalMatrix3x3();
		glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

		// Render mesh
		glUniform1i(texMode_uniformId, 0); // modulate Phong color with texel color
		//glUniform1i(texMode_uniformId, 1); // só componente especular
		//glUniform1i(texMode_uniformId, 2); // multitexturing

		glBindVertexArray(mesh[*objId].vao);
		glDrawElements(mesh[*objId].type,mesh[*objId].numIndexes, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		popMatrix(MODEL);
	 }
}