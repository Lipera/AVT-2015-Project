#include "Butter.h"

/// The storage for matrices
extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];

/// The normal matrix
extern float mNormal3x3[9];


//constructor
Butter::Butter(){
    //to do
}

//destructor
Butter::~Butter(){
    //to do
}

void Butter::create (struct MyMesh* mesh){

	float amb[]= {0.0f, 0.0f, 0.0f, 1.0f};
	float diff[] = {1.0f, 0.8f, 0.1f, 1.0f};
	float spec[] = {0.9f, 0.7f, 0.3f, 1.0f};

	float emissive[] = {0.0f, 0.0f, 0.0f, 1.0f};
	float shininess= 100.0f;
	int texcount = 0;

	int objId=2;
	memcpy(mesh[objId].mat.ambient, amb,4*sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff,4*sizeof(float));
	memcpy(mesh[objId].mat.specular, spec,4*sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive,4*sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	createCube();


}

void Butter::draw(struct MyMesh* mesh, VSShaderLib& shader, GLint& pvm_uniformId, GLint& vm_uniformId, GLint& normal_uniformId, GLint& texMode_uniformId){

	int objId=2;
	GLint loc;
		int aux2;
		for(aux2 = 0; aux2 < 4; ++aux2){
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
			if(aux2%2==0){
				translate(MODEL, -5.0f + aux2 * 2.0f, 0.0f, -7.0f + aux2 * 5.0f);
				rotate(MODEL, 10.0, 0.0, 1.0, 0.0);
			}
			else{
				translate(MODEL, -12.0f + aux2 * 1.0f, 0.0f, 5.0f + aux2 * 2.0f);
				rotate(MODEL, -10.0, 0.0, 1.0, 0.0);
			}
			scale(MODEL, 4.0f, 1.5f, 3.0f);

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

			glBindVertexArray(mesh[objId].vao);
			glDrawElements(mesh[objId].type,mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			popMatrix(MODEL);
		}
}