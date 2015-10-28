#include "Track.h"

/// The storage for matrices
extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];

/// The normal matrix
extern float mNormal3x3[9];


//constructor
Track::Track(){
    //to do
}

//destructor
Track::~Track(){
    //to do
}

void Track::create(struct MyMesh* mesh, int *objId){
//nothing to do
}

void Track::draw(struct MyMesh* mesh, VSShaderLib& shader, GLint& pvm_uniformId, GLint& vm_uniformId, GLint& normal_uniformId, GLint& texMode_uniformId, int *objId){
	*objId=12;
	GLint loc;
	//int track;
	// send the material

	int aux;
	int aux2 = 0;
	int aux3 = 0;
	int aux4 = 0;
	int aux5 = 0;
	for(aux=1; aux < 69; ++aux){
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
		glUniform4fv(loc, 1, mesh[*objId].mat.ambient);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
		glUniform4fv(loc, 1, mesh[*objId].mat.diffuse);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
		glUniform4fv(loc, 1, mesh[*objId].mat.specular);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
		glUniform1f(loc,mesh[*objId].mat.shininess);
		pushMatrix(MODEL);
		if(aux<=9){
			translate(MODEL, -11.0f + aux * 2.5f, 0.2f, -13.0f);
		}else if (9 < aux && aux <= 17) {
			translate(MODEL, -33.5f + aux * 2.5f, 0.2f, -10.0f);
		}else if(17 < aux && aux<=28){
			translate(MODEL, 13.0f, 0.2f, -11.5f + aux2 * 2.5f);
			aux2++;
		}else if (28 < aux && aux <= 36) {
			translate(MODEL, 10.0f, 0.2f, -36.5f + aux2 * 2.5f);
			aux2++;
		//direita
		}else if(36 < aux && aux<=41){
			translate(MODEL, 0.5f + aux3 * 2.5f, 0.2f, 14.0f);
			aux3++;
		}else if (41 < aux && aux <= 44) {
			translate(MODEL, -8.0f + aux3 * 2.5f, 0.2f, 11.0f);
			aux3++;
		//aaixo
		}else if(44 < aux && aux <= 50){
			translate(MODEL, -12.5f, 0.2f, -13.0f + aux4 * 2.5f);
			aux4++;
		}else if (50 < aux && aux <= 54) {
			translate(MODEL, -9.25f, 0.2f, -23.75f + aux4 * 2.5f);
			aux4++;
		//diagonal
		}else if(54 < aux && aux <= 61) {
			translate(MODEL, -12.5f + aux5 * 1.85f, 0.2f, 1.5f + aux5 * 1.85f);
			aux5++;
		}else if (61 < aux && aux <= 68) {
			translate(MODEL, -21.5f + aux5 * 1.85f, 0.2f, -12.75f + aux5 * 1.85f);
			aux5++;
		} 

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
}