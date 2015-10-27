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

	//cereals
	float amb[]= {0.02f, 0.02f, 0.02f, 1.0f};
	float diff[] = {0.95f, 0.84f, 0.65f, 1.0f};
	float spec[] = {0.71f, 0.66f, 0.73f, 1.0f};
	

	float emissive[] = {0.0f, 0.0f, 0.0f, 1.0f};
	float shininess= 100.0f;
	int texcount = 0;

	*objId=3;
	memcpy(mesh[*objId].mat.ambient, amb,4*sizeof(float));
	memcpy(mesh[*objId].mat.diffuse, diff,4*sizeof(float));
	memcpy(mesh[*objId].mat.specular, spec,4*sizeof(float));
	memcpy(mesh[*objId].mat.emissive, emissive,4*sizeof(float));
	mesh[*objId].mat.shininess = shininess;
	mesh[*objId].mat.texCount = texcount;
	createTorus(0.1f, 0.5f, 20, 20);
}

void Track::draw(struct MyMesh* mesh, VSShaderLib& shader, GLint& pvm_uniformId, GLint& vm_uniformId, GLint& normal_uniformId, GLint& texMode_uniformId, int *objId){
	*objId=3;
	GLint loc;
	//int track;
	// send the material

	int aux;
	int aux2 = 0;
	int aux3 = 0;
	int aux4 = 0;
	int aux5 = 0;
	for(aux=0; aux < 115; ++aux){
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
		glUniform4fv(loc, 1, mesh[*objId].mat.ambient);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
		glUniform4fv(loc, 1, mesh[*objId].mat.diffuse);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
		glUniform4fv(loc, 1, mesh[*objId].mat.specular);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
		glUniform1f(loc,mesh[*objId].mat.shininess);
		pushMatrix(MODEL);
		if(aux<=16){
			translate(MODEL, -11.0f + aux * 1.5f, 0.2f, -13.0f);
		}else if (16 < aux && aux <= 28) {
			translate(MODEL, -33.5f + aux * 1.5f, 0.2f, -10.0f);
		}else if(28 < aux && aux<=46){
			translate(MODEL, 13.0f, 0.2f, -11.5f + aux2 * 1.5f);
			aux2++;
		}else if (46 < aux && aux <= 60) {
			translate(MODEL, 10.0f, 0.2f, -36.5f + aux2 * 1.5f);
			aux2++;
		//direita
		}else if(60 < aux && aux<=68){
			translate(MODEL, 0.5f + aux3 * 1.5f, 0.2f, 14.0f);
			aux3++;
		}else if (68 < aux && aux <= 72) {
			translate(MODEL, -9.5f + aux3 * 1.7f, 0.2f, 11.0f);
			aux3++;
		//aaixo
		}else if(72 < aux && aux <= 82){
			translate(MODEL, -12.5f, 0.2f, -13.0f + aux4 * 1.5f);
			aux4++;
		}else if (82 < aux && aux <= 89) {
			translate(MODEL, -9.25f, 0.2f, -24.75f + aux4 * 1.5f);
			aux4++;
		//diagonal
		}else if(89 < aux && aux <= 102) {
			translate(MODEL, -12.5f + aux5 * 1.0f, 0.2f, 1.5f + aux5 * 1.0f);
			aux5++;
		}else if (102 < aux && aux <= 115) {
			translate(MODEL, -21.5f + aux5 * 1.0f, 0.2f, -12.75f + aux5 * 1.0f);
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