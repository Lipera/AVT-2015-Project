#include "Coin.h"

extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];

/// The normal matrix
extern float mNormal3x3[9];

//constructor of the Billboard
Coin::Coin(int value){
	setType(1);
	setNumber(value);
}

//destructor of the Billboard
Coin::~Coin(){}

void Coin::setReflection(int reflection){
	_reflection = reflection;
}

//_____________________________________Get and Set__________________________________________

void Coin::setCam(float camX, float camY, float camZ){
	_camX = camX;
	_camY = camY;
	_camZ = camZ;
}

void Coin::setPos(float posX, float posY, float posZ){
	_posX = posX;
	_posY = posY;
	_posZ = posZ;
}

float Coin::getCamX(){
	return _camX;
}

float Coin::getCamY(){
	return _camY;
}

float Coin::getCamZ(){
	return _camZ;
}

float Coin::getPosX(){
	return _posX;
}

float Coin::getPosY(){
	return _posY;
}

float Coin::getPosZ(){
	return _posZ;
}

void Coin::setType(int i){
	_type=i;
}

int Coin::getType(){
	return _type;
}

//___________________________________________________________________________________________

void Coin::create(struct MyMesh* mesh, int *objId){

	float spec[] = {0.9f, 0.9f, 0.9f, 1.0f};
	float emissive[] = {0.0f, 0.0f, 0.0f, 1.0f};
	float shininess= 1000.0f;
	int texcount = 0;
		
	*objId=17;
	memcpy(mesh[*objId].mat.specular, spec,4*sizeof(float));
	memcpy(mesh[*objId].mat.emissive, emissive,4*sizeof(float));
	mesh[*objId].mat.shininess = shininess;
	mesh[*objId].mat.texCount = texcount;
	createQuad(6,6);

}

void Coin::draw(struct MyMesh* mesh, VSShaderLib& shader, GLint& pvm_uniformId, GLint& vm_uniformId, GLint& normal_uniformId, GLint& texMode_uniformId, int *objId){
		
	float modelview[16];
	float pos[3];
	//pos[0] = getPosX(); pos[1] = getPosY(); pos[2] = getPosZ();
	float cam[3];
	cam[0] = getCamX(); cam[1] = getCamY(); cam[2] = getCamZ();
	GLint loc;

	//Draw trees billboards

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
    glUniform1i(texMode_uniformId, 1); // draw textured quads

        for(int j = 1; j < 3; j++) {
            pushMatrix(MODEL);
            translate(MODEL,-7.0,0.0,-10.0+j*5);

           pos[0] = -7.0; pos[1] = 0; pos[2] = -10.0+j*5;

            if (getType() == 2){
                l3dBillboardSphericalBegin(cam,pos);
			}
            else if (getType() == 3){
                l3dBillboardCylindricalBegin(cam,pos);
			}
    
            *objId=17;  //quad for tree

            //diffuse and ambient color are not used in the tree quads
            loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
            glUniform4fv(loc, 1, mesh[*objId].mat.specular);
            loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
            glUniform1f(loc,mesh[*objId].mat.shininess);
    
            pushMatrix(MODEL);
            translate(MODEL, 0.0, 3.0, 0.0f);
  
            // send matrices to OGL
            if (getType() == 0 || getType() == 1) {     //Cheating matrix reset billboard techniques
                computeDerivedMatrix(VIEW_MODEL);
                memcpy(modelview, mCompMatrix[VIEW_MODEL], sizeof(float) * 16);  //save VIEW_MODEL in modelview matrix

                //reset VIEW_MODEL
                if(getType()==0){
					BillboardCheatSphericalBegin();   
				}
                else{
					BillboardCheatCylindricalBegin();
				}

                computeDerivedMatrix_PVM(); // calculate PROJ_VIEW_MODEL
            }
            else computeDerivedMatrix(PROJ_VIEW_MODEL);

			glUniform1i(texMode_uniformId, 13);

            glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
            glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
            computeNormalMatrix3x3();
            glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);
            glBindVertexArray(mesh[*objId].vao);
            glDrawElements(mesh[*objId].type,mesh[*objId].numIndexes, GL_UNSIGNED_INT, 0);
            popMatrix(MODEL);

            popMatrix(MODEL);
        }
		glDisable(GL_BLEND);
		
}


/*-----------------------------------------------------------------
The objects motion is restricted to a rotation on a predefined axis
The function bellow does cylindrical billboarding on the Y axis, i.e.
the object will be able to rotate on the Y axis only.
-----------------------------------------------------------------*/
/*
void Coin::l3dBillboardLocalToWorld(float *cam, float *worldPos) {
	float *modelview;

    // get the current modelview matrix
    modelview = get(VIEW_MODEL);

// The local origin's position in world coordinates
	worldPos[0] = cam[0] + modelview[12]*modelview[0] + modelview[13] * modelview[1] + modelview[14] * modelview[2];
	worldPos[1] = cam[1] + modelview[12]*modelview[4] + modelview[13] * modelview[5] + modelview[14] * modelview[6];
	worldPos[2] = cam[2] + modelview[12]*modelview[8] + modelview[13] * modelview[9] + modelview[14] * modelview[10];
}
*/

/*-----------------------------------------------------------------
The following two methods get the right and up vectors of the 
inverse of the top 3x3 submetrix from modelview.
-----------------------------------------------------------------*/

/*
void Coin::l3dBillboardGetRightVector(float *right) {

	float modelview[16];

	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

	right[0] = modelview[0];
	right[1] = modelview[4];
	right[2] = modelview[8];
}

void l3dBillboardGetUpRightVector(float *up,float *right) {

	float modelview[16];

	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

	right[0] = modelview[0];
	right[1] = modelview[4];
	right[2] = modelview[8];

	up[0] = modelview[1];
	up[1] = modelview[5];
	up[2] = modelview[9];
}

*/
/*-----------------------------------------------------------------
The objects motion is restricted to a rotation on a predefined axis
The function bellow does cylindrical billboarding on the Y axis, i.e.
the object will be able to rotate on the Y axis only.
-----------------------------------------------------------------*/
void Coin::l3dBillboardCylindricalBegin(float *cam, float *worldPos) {

	float lookAt[3]={0,0,1},objToCamProj[3],upAux[3],angleCosine;

// objToCamProj is the vector in world coordinates from the local origin to the camera
// projected in the XZ plane
	objToCamProj[0] = cam[0] - worldPos[0] ;
	objToCamProj[1] = 0;
	objToCamProj[2] = cam[2] - worldPos[2] ;


// normalize both vectors to get the cosine directly afterwards
	mathsNormalize(objToCamProj);

// easy fix to determine wether the angle is negative or positive
// for positive angles upAux will be a vector pointing in the 
// positive y direction, otherwise upAux will point downwards
// effectively reversing the rotation.

	mathsCrossProduct(upAux,lookAt,objToCamProj);

// compute the angle
	angleCosine = mathsInnerProduct(lookAt,objToCamProj);

// perform the rotation. The if statement is used for stability reasons
// if the lookAt and v vectors are too close together then |aux| could
// be bigger than 1 due to lack of precision
	if ((angleCosine < 0.99990) && (angleCosine > -0.9999))
		rotate(MODEL,acos(angleCosine)*180/3.14,upAux[0], upAux[1], upAux[2]);
}


/*----------------------------------------------------------------
True billboarding. With the spherical version the object will 
always face the camera. It requires more computational effort than
the cylindrical billboard though. The parameters camX,camY, and camZ,
are the target, i.e. a 3D point to which the object will point.
----------------------------------------------------------------*/

void Coin::l3dBillboardSphericalBegin(float *cam, float *worldPos) {

	float lookAt[3]={0,0,1},objToCamProj[3],objToCam[3],upAux[3],angleCosine;

// objToCamProj is the vector in world coordinates from the local origin to the camera
// projected in the XZ plane
	objToCamProj[0] = cam[0] - worldPos[0] ;
	objToCamProj[1] = 0;
	objToCamProj[2] = cam[2] - worldPos[2] ;

// normalize both vectors to get the cosine directly afterwards
	mathsNormalize(objToCamProj);

// easy fix to determine wether the angle is negative or positive
// for positive angles upAux will be a vector pointing in the 
// positive y direction, otherwise upAux will point downwards
// effectively reversing the rotation.

	mathsCrossProduct(upAux,lookAt,objToCamProj);

// compute the angle
	angleCosine = mathsInnerProduct(lookAt,objToCamProj);

// perform the rotation. The if statement is used for stability reasons
// if the lookAt and v vectors are too close together then |aux| could
// be bigger than 1 due to lack of precision
	if ((angleCosine < 0.99990) && (angleCosine > -0.9999))
		rotate(MODEL,acos(angleCosine)*180/3.14,upAux[0], upAux[1], upAux[2]);


// The second part tilts the object so that it faces the camera

// objToCam is the vector in world coordinates from the local origin to the camera
	objToCam[0] = cam[0] - worldPos[0] ;
	objToCam[1] = cam[1] - worldPos[1] ;
	objToCam[2] = cam[2] - worldPos[2] ;

// Normalize to get the cosine afterwards
	mathsNormalize(objToCam);

// Compute the angle between v and v2, i.e. compute the
// required angle for the lookup vector
	angleCosine = mathsInnerProduct(objToCamProj,objToCam);


// Tilt the object. The test is done to prevent instability when objToCam and objToCamProj have a very small
// angle between them
	if ((angleCosine < 0.99990) && (angleCosine > -0.9999))
		if (objToCam[1] < 0)
			rotate(MODEL,acos(angleCosine)*180/3.14,1,0,0);
		else
			rotate(MODEL,acos(angleCosine)*180/3.14,-1,0,0);

}


/* --------------------------------------------------------
Simplest form of Sperical Billboarding. It is not real billboarding 
since the object won't face the camera in the general case.
The object will face a plane perpendicular to the cameras
"look at" vector. It is the fastest of them all though.
---------------------------------------------------------*/

void Coin::BillboardCheatSphericalBegin() {
	
	
	int i,j;

	// undo all rotations
	// beware all scaling is lost as well 
	for( i=0; i<3; i++ ) 
		for( j=0; j<3; j++ ) {
			if ( i==j )
				mCompMatrix[VIEW_MODEL][i*4+j] = 1.0;
			else
				mCompMatrix[VIEW_MODEL][i*4+j] = 0.0;
		}
}


/* -------------------------------------------------------
The comments above apply in here as well but this is the
cylindrical version, i.e. the up vector is not changed
---------------------------------------------------------*/
void Coin::BillboardCheatCylindricalBegin() {

	int i,j;
 
	// Note that a row in the C convention is a column 
	// in OpenGL convention (see the red book, pg.106 in version 1.2)
	// right vector is [1,0,0]  (1st column)
	// lookAt vector is [0,0,1] (3d column)
	// leave the up vector unchanged (2nd column)
	// notice the increment in i in the first cycle (i+=2)
	for( i=0; i<3; i+=2 ) 
		for( j=0; j<3; j++ ) {
			if ( i==j )
				mCompMatrix[VIEW_MODEL][i*4+j] = 1.0;
			else
				mCompMatrix[VIEW_MODEL][i*4+j] = 0.0;
		}
}

/* -------------------------------------------------------
This function is provided just for the sake of clean code.
Since it is implemented as an inline there is no penalty 
for calling this function.
--------------------------------------------------------*/
 
void Coin::BillboardEnd(float *m) {

	// restore the previously stored modelview matrix
	memcpy(mCompMatrix[VIEW_MODEL], m, sizeof(float) * 16);
}
