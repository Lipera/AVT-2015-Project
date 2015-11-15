#include "Car.h"

/// The storage for matrices
extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];

/// The normal matrix
extern float mNormal3x3[9];


//constructor
Car::Car(float x, float y, float z, int lives){
    _position = new Vector3(x, y, z);
    _speed = new Vector3();
	setAngle(0.0f);
	setVarAngle(0.0f);
	setAceleracao(0.0f);
	setVelocidade(0.0f);
	_bottomLeft = new Vector3(-0.65f, 0.0f, -0.3f);
	_topRight = new Vector3(0.9f, 0.0f, 0.9f);
	setLives(lives);
}

//destructor
Car::~Car(){
    //to do
}

void Car::create (struct MyMesh* mesh, int *objId){

	//wheels color (black rubber)
	float amb[]= {0.02f, 0.02f, 0.02f, 1.0f};
	float diff[] = {0.01f, 0.01f, 0.01f, 1.0f};
	float spec[] = {0.4f, 0.4f, 0.4f, 1.0f};

	//base block car color (red plastic)
	float amb1[]= {0.3f, 0.1f, 0.1f, 1.0f};
	float diff1[] = {0.5f, 0.0f, 0.0f, 1.0f};
	float spec1[] = {0.7f, 0.6f, 0.6f, 1.0f};

	//top block car color (blue glass)
	float amb2[]= {0.0f, 0.3f, 0.4f, 1.0f};
	float diff2[] = {0.07f, 0.68f, 0.89f, 1.0f};
	float spec2[] = {0.6f, 0.45f, 0.6f, 1.0f};

	//spindles color 
	float amb3[]= {0.3f, 0.3f, 0.3f, 1.0f};
	float diff3[] = {0.5f, 0.5f, 0.5f, 1.0f};
	float spec3[] = {0.55f, 0.55f, 0.55f, 1.0f};

	//yellow front lights
	float amb4[]= {0.0f, 0.0f, 0.0f, 1.0f};
	float diff4[] = {1.0f, 0.8f, 0.1f, 1.0f};
	float spec4[] = {0.9f, 0.7f, 0.3f, 1.0f};

	//orange back lights
	float amb5[]= {0.2f, 0.2f, 0.2f, 1.0f};
	float diff5[] = {1.0f, 0.43f, 0.09f, 1.0f};
	float spec5[] = {0.633f, 0.728f, 0.633f, 1.0f};

	float emissive[] = {0.0f, 0.0f, 0.0f, 1.0f};
	float shininess= 100.0f;
	int texcount = 0;

	//parte de baixo do carro
	*objId=7;
	memcpy(mesh[*objId].mat.ambient, amb1,4*sizeof(float));
	memcpy(mesh[*objId].mat.diffuse, diff1,4*sizeof(float));
	memcpy(mesh[*objId].mat.specular, spec1,4*sizeof(float));
	memcpy(mesh[*objId].mat.emissive, emissive,4*sizeof(float));
	mesh[*objId].mat.shininess = shininess;
	mesh[*objId].mat.texCount = texcount;
	createCube();
	
	//parte de cima do carro
	*objId=8;
	memcpy(mesh[*objId].mat.ambient, amb2,4*sizeof(float));
	memcpy(mesh[*objId].mat.diffuse, diff2,4*sizeof(float));
	memcpy(mesh[*objId].mat.specular, spec2,4*sizeof(float));
	memcpy(mesh[*objId].mat.emissive, emissive,4*sizeof(float));
	mesh[*objId].mat.shininess = shininess;
	mesh[*objId].mat.texCount = texcount;
	createCube();

	//eixos
	*objId=9;
	memcpy(mesh[*objId].mat.ambient, amb3,4*sizeof(float));
	memcpy(mesh[*objId].mat.diffuse, diff3,4*sizeof(float));
	memcpy(mesh[*objId].mat.specular, spec3,4*sizeof(float));
	memcpy(mesh[*objId].mat.emissive, emissive,4*sizeof(float));
	mesh[*objId].mat.shininess = shininess;
	mesh[*objId].mat.texCount = texcount;
	createCylinder(1.25f, 0.1f, 10);
	
	//rodas
	*objId=10;
	memcpy(mesh[*objId].mat.ambient, amb,4*sizeof(float));
	memcpy(mesh[*objId].mat.diffuse, diff,4*sizeof(float));
	memcpy(mesh[*objId].mat.specular, spec,4*sizeof(float));
	memcpy(mesh[*objId].mat.emissive, emissive,4*sizeof(float));
	mesh[*objId].mat.shininess = shininess;
	mesh[*objId].mat.texCount = texcount;
	createTorus(0.1f, 0.5f, 20, 20);
	
}

void Car::draw(struct MyMesh* mesh, VSShaderLib& shader, GLint& pvm_uniformId, GLint& vm_uniformId, GLint& normal_uniformId, GLint& texMode_uniformId, int *objId){
	
	*objId=7;
	GLint loc;
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
			//translate(MODEL, carX, carY, carZ);
			translate(MODEL, -1.5f, 0.5f, 0.0f);
			//rotate(MODEL, carAlpha, 0.0, 1.0, 0.0);
			scale(MODEL, 2.5f, 0.50f, 0.75f);

			//translate(MODEL, _position->getX(), _position->getY(), _position->getZ());
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
	
	*objId=8;
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
			translate(MODEL, -1.0f, 1.0f, 0.0f);
			scale(MODEL, 1.0f, 0.50f, 0.75f);
			//translate(MODEL, _position->getX(), _position->getY(), _position->getZ());
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


	*objId=9;
	int aux;
	for(aux=0; aux < 2; aux++){
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

			if(aux==0){
				translate(MODEL, 0.35f, 0.5f, 0.35f);
				rotate(MODEL, 90.0f, 1.0f, 0.0f, 0.0f);
			}else if (aux==1) {
				translate(MODEL, -0.9f, 0.5f, 0.35f);
				rotate(MODEL, 90.0f, 1.0f, 0.0f, 0.0f);
			} 
			//translate(MODEL, _position->getX(), _position->getY(), _position->getZ());
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

	*objId=10;
	int aux3;
	for(aux3=0; aux3 < 4; aux3++){
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
			if(aux3==0){
				translate(MODEL, 0.35f, 0.5f, 0.55f);
				rotate(MODEL, 90.0f, 1.0f, 0.0f, 0.0f);
			}else if (aux3==1) {
				translate(MODEL, -0.9f, 0.5f, 0.55f);
				rotate(MODEL, 90.0f, 1.0f, 0.0f, 0.0f);
			} else if (aux3==2) {
				translate(MODEL, 0.35f, 0.5f, -0.5f);
				rotate(MODEL, 90.0f, 1.0f, 0.0f, 0.0f);
			} else  {
				translate(MODEL, -0.9f, 0.5f, -0.5f);
				rotate(MODEL, 90.0f, 1.0f, 0.0f, 0.0f);
			} 
			translate(MODEL, 0.0f, 0.35f, 0.0f);
			//translate(MODEL, _position->getX(), _position->getY(), _position->getZ());
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

	loc = glGetUniformLocation(shader.getProgramIndex(), "isCarLife");
	glUniform1i(loc, _lives);

}

void Car::update(int delta_t){
	float new_x;
    float new_y;
    float new_z;
	float vel;
	float angle;

    new_x = _position->getX();
    new_y = _position->getY();
    new_z = _position->getZ();	

	vel = getVelocidade();
	angle = getAngle();



	new_x += delta_t * ( vel * cos(-angle * 3.14f / 180.0f));
	new_z += delta_t * ( vel * sin(-angle * 3.14f / 180.0f));
	_position->set(new_x, new_y, new_z);

}

void Car::updateAux(bool keyQ, bool keyA, bool keyO, bool keyP, int delta_t){
	
	float angle = getAngle();
	float velocidade = getVelocidade();
	float aceleracao = getAceleracao();
	float varAngle = getVarAngle();

	angle -= varAngle;
	if(angle >= 360) {
		angle = 0.0f;
	} else if(angle < 0) {
		angle = 360.0f;
	}

	setAngle(angle);

	if(keyQ && velocidade < 0.01f) {
		velocidade += aceleracao;
	} else if(keyA && _velocidade > -0.01f) {
		velocidade += aceleracao;
	} else if(!keyQ && !keyA){
		if(aceleracao < 0.0f && velocidade > 0.0f) {
			velocidade += aceleracao;
		} else if (aceleracao > 0.0f && velocidade < 0.0f) {
			velocidade += aceleracao;
		} else {
			velocidade = 0.0f;
		}
	}
	setVelocidade(velocidade);
	setAceleracao(aceleracao);

	update(delta_t);
}

void Car::setAceleracao(float aceleracao){
	_aceleracao = aceleracao;
}
float Car::getAceleracao(){
	return _aceleracao;
}
void Car::setVarAngle(float varAngle){
	_varAngle = varAngle;
}
float Car::getVarAngle(){
return _varAngle;
}

void Car::setVarVelocidade(float varVelocidade){
	_varVelocidade = varVelocidade;
}
float Car::getVarVelocidade(){
	return _varVelocidade;
}
void Car::setVelocidade(float velocidade){
	_velocidade = velocidade;
}
float Car::getVelocidade(){
	return _velocidade;
}

void Car::setLives(int lives){_lives=lives;}
int Car::getLives(){return _lives;}