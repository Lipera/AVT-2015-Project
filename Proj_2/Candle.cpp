#include "Candle.h"

/// The storage for matrices
extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];

/// The normal matrix
extern float mNormal3x3[9];


//constructor
Candle::Candle(){
    //to do
}

//destructor
Candle::~Candle(){
    //to do
}

void Candle::create (struct MyMesh* mesh){

	float amb[] = { 0.25f, 0.25f, 0.25f, 1.0f };
	float diff[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	float spec[] = { 0.774597f, 0.774597f, 0.774597f, 1.0f };

	float emissive[] = {0.0f, 0.0f, 0.0f, 1.0f};
	float shininess= 100.0f;
	int texcount = 0;

	//vela
	int objId=4;
	memcpy(mesh[objId].mat.ambient, amb,4*sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff,4*sizeof(float));
	memcpy(mesh[objId].mat.specular, spec,4*sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive,4*sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	createCylinder(3.0f, 0.7f, 20);

	//chama base
	objId=5;
	memcpy(mesh[objId].mat.ambient, amb,4*sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff,4*sizeof(float));
	memcpy(mesh[objId].mat.specular, spec,4*sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive,4*sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	createSphere(0.15f, 10);

	//chama top
	objId=6;
	memcpy(mesh[objId].mat.ambient, amb,4*sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff,4*sizeof(float));
	memcpy(mesh[objId].mat.specular, spec,4*sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive,4*sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	createCone(0.25f, 0.15f, 10);
}

void Candle::draw(struct MyMesh* mesh, VSShaderLib& shader, GLint& pvm_uniformId, GLint& vm_uniformId, GLint& normal_uniformId, GLint& texMode_uniformId){

	int objId=4;
	GLint loc;
	int aux;
	for(aux=0; aux < 6; aux++){
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

			if(aux == 0){
				translate(MODEL, 8.0f, 0.0f, -3.0f);
			}else if(aux == 1){
				translate(MODEL, -7.5f, 0.0f, -8.0f);
			}else if(aux == 2){
				translate(MODEL, 8.5f, 0.0f, 9.0f);
			}else if(aux == 3){
				translate(MODEL, -4.0f, 0.0f, 2.75f);
			}else if(aux == 4){
				translate(MODEL, 2.0f, 0.0f, -8.0f);
			}else if(aux == 5){
				translate(MODEL, 3.0f, 0.0f, 9.0f);
			}

			// send matrices to OGL
			computeDerivedMatrix(PROJ_VIEW_MODEL);
			glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
			glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
			computeNormalMatrix3x3();
			glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

			// Render mesh
			//glUniform1i(texMode_uniformId, 0); // modulate Phong color with texel color
			//glUniform1i(texMode_uniformId, 1); // s� componente especular
			//glUniform1i(texMode_uniformId, 2); // multitexturing

			glBindVertexArray(mesh[objId].vao);
			glDrawElements(mesh[objId].type,mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			popMatrix(MODEL);
	}

	objId=5;
	int aux2;
	for(aux2=0; aux2 < 6; aux2++){
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

			if(aux == 0){
				translate(MODEL, 8.0f, 3.0f, -3.0f);
			}else if(aux == 1){
				translate(MODEL, -7.5f, 3.0f, -8.0f);
			}else if(aux == 2){
				translate(MODEL, 8.5f, 3.0f, 9.0f);
			}else if(aux == 3){
				translate(MODEL, -4.0f, 3.0f, 2.75f);
			}else if(aux == 4){
				translate(MODEL, 2.0f, 3.0f, -8.0f);
			}else if(aux == 5){
				translate(MODEL, 3.0f, 3.0f, 9.0f);
			}

			// send matrices to OGL
			computeDerivedMatrix(PROJ_VIEW_MODEL);
			glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
			glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
			computeNormalMatrix3x3();
			glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

			// Render mesh
			//glUniform1i(texMode_uniformId, 0); // modulate Phong color with texel color
			//glUniform1i(texMode_uniformId, 1); // s� componente especular
			//glUniform1i(texMode_uniformId, 2); // multitexturing

			glBindVertexArray(mesh[objId].vao);
			glDrawElements(mesh[objId].type,mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			popMatrix(MODEL);
	}

	objId=6;
	int aux3;
	for(aux3=0; aux3 < 6; aux3++){
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

			if(aux == 0){
				translate(MODEL, 8.0f, 4.0f, -3.0f);
			}else if(aux == 1){
				translate(MODEL, -7.5f, 4.0f, -8.0f);
			}else if(aux == 2){
				translate(MODEL, 8.5f, 4.0f, 9.0f);
			}else if(aux == 3){
				translate(MODEL, -4.0f, 4.0f, 2.75f);
			}else if(aux == 4){
				translate(MODEL, 2.0f, 4.0f, -8.0f);
			}else if(aux == 5){
				translate(MODEL, 3.0f, 4.0f, 9.0f);
			}

			// send matrices to OGL
			computeDerivedMatrix(PROJ_VIEW_MODEL);
			glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
			glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
			computeNormalMatrix3x3();
			glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

			// Render mesh
			//glUniform1i(texMode_uniformId, 0); // modulate Phong color with texel color
			//glUniform1i(texMode_uniformId, 1); // s� componente especular
			//glUniform1i(texMode_uniformId, 2); // multitexturing

			glBindVertexArray(mesh[objId].vao);
			glDrawElements(mesh[objId].type,mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			popMatrix(MODEL);
	}


}