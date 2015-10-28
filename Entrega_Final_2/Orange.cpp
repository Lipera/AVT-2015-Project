#include "Orange.h"

/// The storage for matrices
extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];

/// The normal matrix
extern float mNormal3x3[9];

//constructor
Orange::Orange(float x, float y, float z){
    _position = new Vector3(x, y, z);
    _speed = new Vector3();
	setAngle(0.0f);
	_bottomLeft = new Vector3(-1.25f, 0.0f, -1.25f);
	_topRight = new Vector3(1.25f, 0.0f, 1.25f);
}

//destructor
Orange::~Orange(){
    //to do
}


void Orange::create(struct MyMesh* mesh, int *objId){

	float amb[]= {0.2f, 0.2f, 0.2f, 1.0f};
	float diff[] = {1.0f, 0.43f, 0.09f, 1.0f};
	float spec[] = {0.1f, 0.2f, 0.1f, 1.0f};

	float emissive[] = {0.0f, 0.0f, 0.0f, 1.0f};
	float shininess= 50.0f;
	int texcount = 0;

	*objId=6;
	memcpy(mesh[*objId].mat.ambient, amb,4*sizeof(float));
	memcpy(mesh[*objId].mat.diffuse, diff,4*sizeof(float));
	memcpy(mesh[*objId].mat.specular, spec,4*sizeof(float));
	memcpy(mesh[*objId].mat.emissive, emissive,4*sizeof(float));
	mesh[*objId].mat.shininess = shininess;
	mesh[*objId].mat.texCount = texcount;
	createSphere(1.25f, 20);
}

void Orange::draw(struct MyMesh* mesh, VSShaderLib& shader, GLint& pvm_uniformId, GLint& vm_uniformId, GLint& normal_uniformId, GLint& texMode_uniformId, int *objId){
	*objId=6;
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
			
			translate(MODEL, _position->getX(), _position->getY(), _position->getZ());
			//translate(MODEL, orangeX, orangeY, orangeZ);
			rotate(MODEL, -getAngle(), 1.0f, 0.0f, 0.0f);
			//rotate(MODEL, _angle, 1.0f, 1.0f, 0.0f);
			// send matrices to OGL
			computeDerivedMatrix(PROJ_VIEW_MODEL);
			glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
			glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
			computeNormalMatrix3x3();
			glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

			// Render mesh
			glUniform1i(texMode_uniformId, 3); // apenas o texel
			//glUniform1i(texMode_uniformId, 1); // só componente especular
			//glUniform1i(texMode_uniformId, 2); // multitexturing

			glBindVertexArray(mesh[*objId].vao);
			glDrawElements(mesh[*objId].type,mesh[*objId].numIndexes, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			popMatrix(MODEL);
}


void Orange::update(int delta_t){

	float orangesPos[3] = {-14.0f, 6.0f, 14.0f};
	float new_x;
    float new_y;
    float new_z;
	float z_speed; 
	float angle;

	angle = getAngle();

    new_x = _position->getX();
    new_y = _position->getY();
    new_z = _position->getZ();
	
	z_speed = getSpeed()->getZ();

	if(z_speed <= 0.02){
		z_speed += 0.00001f;
		setSpeed(_speed->getX(), _speed->getY(), z_speed);
	}

	//printf("%f\n", angle);		
	if (angle <360.0){
			angle = angle + 5.0f;
			setAngle(angle);
	}
	else if (angle >=360.0) {
		angle= 0.0f;
		setAngle(angle);
	}
	
	new_z -= (delta_t * z_speed);
	if(new_z <= -16.5f){
		new_z= 16.5f;
		new_x= orangesPos[(rand()%3)];
	}
    
    new_z -= _speed->getZ() * delta_t; 
    
    setPosition(new_x, new_y, new_z);
}


