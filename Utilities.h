#pragma once
#include<gl/glut.h>
#include<math.h>
#include<iostream>
#include <fstream>
#include <sstream>
#include <string>



#define animationSequnence "csv_walking"



class Objects
{
private:
	bool isCube;

public:
	float posX, posY, posZ, rotX, rotY, rotZ, sX, sY, sZ;
	Objects(float posx, float posy, float posz, float rotx, float roty, float rotz, float sx, float sy,float sz, bool isCubeParams)
	{
		//position with respect to origin
		posX = posx;
		posY = posy;
		posZ = posz;

		//scale of the object
		sX = sx;
		sY = sy;
		sZ = sz;

		//local roation of the object
		rotX = rotx;
		rotY = roty;
		rotZ = rotz;


		//isCube
		isCube = isCubeParams;

	}

	void setRotation(float x,float y,float z)
	{
		rotX = x;
		rotY = y;
		rotZ = z;
	}

	void setTranslation(float x,float y,float z)
	{
		posX = x;
		posY = y;
		posZ = z;
	}



	void setObject()
	{
		glPushMatrix();
		glTranslatef(posX, posY, posZ);
		glRotatef(rotZ, 0, 0, 1);
		glRotatef(rotY, 0, 1, 0);
		glRotatef(rotX, 1, 0, 0);
		glScalef(sX, sY, sZ);

		if (isCube)
			glutSolidCube(1);
		else
			glutSolidSphere(1, 50, 50);
		glPopMatrix();
	}
};



void AnimatedTransformations(float px, float py, float pz, float sx, float sy, float sz)
{
	glTranslatef(px, py, pz);
	glScalef(sx, sy, sz);
}



void AnimatedRotation(Objects obj, float rx, float ry, float rz, bool local)
{
	if (local)
	{
		glTranslatef(obj.posX, obj.posY, obj.posZ);
		glRotatef(rz, 0, 0, 1);
		glRotatef(ry, 0, 1, 0);
		glRotatef(rx, 1, 0, 0);
		glTranslatef(-obj.posX, -obj.posY, -obj.posZ);
	}
	else {
		glRotatef(rz, 0, 0, 1);
		glRotatef(ry, 0, 1, 0);
		glRotatef(rx, 1, 0, 0);
	}
}

void ApplyPerspectiveProjection()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50.0, 1.0, 1.0, 100.0);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void LookAt(float x, float y, float z)
{
	gluLookAt(x, y, z, 0, 0, 0, 0, 1, 0);
}


class HumanoidSkeleton {
private:
	float rot=0;

	float frame = 1;
	



	//arrays instead of variables

	float leftLeg[2][3]={ {0.0,0.0,0.0},{0.0,0.0,0.0} };
	float rightLeg[2][3]={ {0,0,0},{0,0,0} };

	float leftArm[2][3]={ {0,0,0},{0,0,0} };
	float rightArm[2][3]={ {0,0,0},{0,0,0} };

	float leftArmJoint[2][3]={ {0,0,0},{0,0,0} };
	float rightArmJoint[2][3]={ {0,0,0},{0,0,0} };
	
	float leftLegJoint[2][3]={ {0,0,0},{0,0,0} };
	float rightLegJoint[2][3]={ {0,0,0},{0,0,0} };

	float chest[2][3] = { {0,0,0},{0,0,0} };
	float body[2][3] = { {0,0,0},{0,0,0} };
	

public:
	HumanoidSkeleton()
	{
		
	}

	void Update(float angle)
	{

		rot = angle;
		++frame;

		if (animationSequnence =="Walking")
		{

			//right leg walking motion: rotation along x axis
			rightLeg[1][0] = sin(rot * 0.01) * 30;
			rightLegJoint[1][0] = -sin(rot * 0.01) * 30 > 0 ? -sin(rot * 0.01) * 30 : -sin(rot * 0.01) * 5;


			//left leg walking motion: rotation along x axis
			leftLeg[1][0] = -sin(rot * 0.01) * 30;
			leftLegJoint[1][0] = sin(rot * 0.01) * 30 > 0 ? sin(rot * 0.01) * 30 : sin(rot * 0.01) * 5;


			//left arm walking motion: rotation along x axis
			leftArm[1][0] = sin(rot * 0.01) * 30;
			leftArmJoint[1][0] = sin(rot * 0.01) * 40 < 0 ? sin(rot * 0.01) * 40 : 0;


			//right arm walking motion: rotation along x axis
			rightArm[1][0] = -sin(rot * 0.01) * 30;
			rightArmJoint[1][0] = -sin(rot * 0.01) * 40 < 0 ? -sin(rot * 0.01) * 40 : 0;


			//limiting forward motion of the body
			if (!(sin(angle) > -0.2 and sin(angle) < 0.2))
				body[0][2] = -10 + (rot * 0.0035);


			body[0][1] = -sin(rot * 0.01) * 0.05;
			chest[1][1] = -sin(rot * 0.01) * 10;


			

		}
		else if (animationSequnence == "csv_walking")
		{
			readCSV("walking.csv", frame);
		}
		else if (animationSequnence == "jumping")
		{
			//Jumping animation through csv
			readCSV("jumping.csv", frame);

		}
		else {
			//stopped Animation
		}
		
		//std::cout << leftLeg[1][0] << std::endl;

		makeSkeleton();
		
	}

	void makeSkeleton()
	{



		Objects Chest(0,0,0,0,0,0,1.5,0.5,0.5,true), Abdomen(0,-0.68,0,0,0,0,1.25,0.85,0.5,true), Pelvis(0, -1.5, 0, 0, 0, 0, 1.0, 0.75, 0.5, true);
		glPushMatrix();
			AnimatedTransformations(0, 1, 0, 1, 1, 1);
			glColor3f(0.3, 0.1, 0.0);
			glPushMatrix();
			
			float angle = sin(rot * 0.01);
			
			AnimatedTransformations(0,body[0][1], body[0][2] ,1,1,1);
			AnimatedRotation(Pelvis,body[1][0], body[1][1], body[1][2], 1);

			//AnimatedRotation();
			Pelvis.setObject(); //All are child to this
				glPushMatrix();
				AnimatedTransformations(chest[0][0], chest[0][1],chest[0][2],1,1,1);
				AnimatedRotation(Chest, chest[1][0], chest[1][1],chest[1][2] ,  1);
					
					glColor3f(0.3, 0.1, 0.2);
					Chest.setObject(); //Abdomen and Arms are the child 
						glPushMatrix();
							glColor3f(0.3, 0.3, 0.1);
							Abdomen.setObject();
						glPopMatrix();

						glPushMatrix();
							makeLeftArm(); //Left Arm is connected to chest 
							makeRightArm(); //Right arm also
						glPopMatrix();

						glPushMatrix();
							makeHead();
						glPopMatrix();

				glPopMatrix();
						glPushMatrix();
							makeLeftLeg();
							makeRightLeg();
						glPopMatrix();
			glPopMatrix();
		glPopMatrix();
	}
	
	
	
	void makeHead()
	{
		Objects Head(0,0,0,0,0,0,0.5,0.5,0.5,false), Neck(0,-0.75,0,0,0,0,0.25,0.5,0.25,true);
		glPushMatrix();
			AnimatedTransformations(0,1,0,1,1,1);
			glColor3f(0.3, 0.1, 0.1);
			Head.setObject();
			glColor3f(0.0, 0.2, 0.2);
			Neck.setObject();
		glPopMatrix();
	}


	void makeBody()
	{
		Objects Chest(0,0,0,0,0,0,1.5,0.5,0.5,true), Abdomen(0,-0.68,0,0,0,0,1.25,0.85,0.5,true), Pelvis(0, -1.5, 0, 0, 0, 0, 1.0, 0.75, 0.5, true);
		glPushMatrix();
		AnimatedTransformations(0, 1, 0, 1, 1, 1);
		Pelvis.setObject();
			glPushMatrix();
				glColor3f(0.3, 0.1, 0.2);
				Chest.setObject();
				glColor3f(0.3, 0.3, 0.1);
				Abdomen.setObject();
				glColor3f(0.3, 0.1, 0.0);
			glPopMatrix();
		glPopMatrix();
	}


	void makeLeftArm()
	{
		
		Objects Shoulder(0,0,0,0,0,0,1.1,0.7,0.5,true), 
			Arm1( -0.1, -0.8, 0, 0, 0, 0, 0.5, 1.2, 0.25, true),
			ArmJoint(-0.1, -1.5, 0, 0, 0, 0, 0.25, 0.25, 0.25, false),
			Arm2(-0.1, -2.1, 0, 0, 0, 0, 0.5, 1.2, 0.25, true);



		glPushMatrix();
		AnimatedTransformations(-1.3, 0, 0, 1, 1, 1); //Shoulder

		AnimatedTransformations(leftArm[0][0], leftArm[0][1], leftArm[0][2], 1, 1, 1);
		AnimatedRotation(Shoulder,leftArm[1][0],leftArm[1][1],leftArm[1][2],1);
		//AnimatedRotation(Shoulder,leftHand,0,0,1);
		
		
		glColor3f(0.4, 0.1, 0.9);
		Shoulder.setObject();
		glPushMatrix();
			glColor3f(0.2, 0.5, 0.6);
				Arm1.setObject();  //Upper Arm
				glPushMatrix();
					//Elbow Joint Animation Here
					//AnimatedTransformations();
					
					AnimatedTransformations(leftArmJoint[0][0], leftArmJoint[0][1], leftArmJoint[0][2], 1, 1, 1);
					AnimatedRotation(ArmJoint, leftArmJoint[1][0], leftArmJoint[1][1], leftArmJoint[1][2], 1);

					//AnimatedRotation(ArmJoint,leftHandJoint, 0, 0, 1);


					glColor3f(0.35, 0.21, 0.24);
					ArmJoint.setObject();			//Elbow Joint
					glPushMatrix();
						glColor3f(0.2, 0.5, 0.6);
						Arm2.setObject(); //Lower Arm
					glPopMatrix();
				glPopMatrix();
			glPopMatrix();
		glPopMatrix();
	}
	void makeRightArm()
	{
		Objects Shoulder(0,0,0,0,0,0,1.1,0.7,0.5,true), 
			Arm1( 0.1, -0.8, 0, 0, 0, 0, 0.5, 1.2, 0.25, true), 
			ArmJoint(0.1, -1.5, 0, 0, 0, 0, 0.25, 0.25, 0.25, false),
			Arm2(0.1, -2.1, 0, 0, 0, 0, 0.5, 1.2, 0.25, true);


		glPushMatrix();
		AnimatedTransformations(1.3, 0, 0, 1, 1, 1); //Shoulder
		
		AnimatedTransformations(rightArm[0][0], rightArm[0][1], rightArm[0][2], 1, 1, 1);
		AnimatedRotation(Shoulder, rightArm[1][0], rightArm[1][1], rightArm[1][2], 1);
													 
		//AnimatedRotation(Shoulder,rightHand , 0, 0, 1);
		
		glColor3f(0.4, 0.1, 0.9);
		Shoulder.setObject();
		glPushMatrix();
			glColor3f(0.2, 0.5, 0.6);
				Arm1.setObject();
				
				glPushMatrix();
					//Elbow Joint Animation Here
					//AnimatedTransformations();
					AnimatedTransformations(rightArmJoint[0][0], rightArmJoint[0][1], rightArmJoint[0][2], 1, 1, 1);
					AnimatedRotation(ArmJoint, rightArmJoint[1][0], rightArmJoint[1][1], rightArmJoint[1][2], 1);
					//AnimatedRotation(ArmJoint, rightHandJoint, 0, 0, 1);

					

					glPushMatrix();
						glColor3f(0.35, 0.21, 0.24);
						ArmJoint.setObject(); //Elbow Joint
							glPushMatrix();
								glColor3f(0.2, 0.5, 0.6);
								Arm2.setObject();   //Lower Arm
							glPopMatrix();
					glPopMatrix();
				glPopMatrix();
			glPopMatrix();
		glPopMatrix();
	}


	void makeLeftLeg()
	{
		Objects Thighs(0, -0.2, 0, 0, 0, 0, 1.1, 0.5, 0.5, true), 
			Leg1(-0.12, -1.2, 0, 0, 0, 0, 0.5, 1.5, 0.5, true), 
			LegJoint(-0.12, -2.15, 0, 0, 0, 0, 0.3, 0.3, 0.3, false), 
			Leg2(-0.12, -3.1, 0, 0, 0, 0, 0.5, 1.5, 0.5, true);


		glPushMatrix();
		AnimatedTransformations(-0.5, -1.95, 0, 1, 1, 1);

		AnimatedTransformations(leftLeg[0][0], leftLeg[0][1], leftLeg[0][2],1,1,1);
		AnimatedRotation(Thighs, leftLeg[1][0], leftLeg[1][1], leftLeg[1][2], 1);

		//AnimatedRotation(Thighs,leftLeg, 0, 0, 1);
		glColor3f(0.4, 0.1, 0.9);
			Thighs.setObject();
				glPushMatrix();
					glColor3f(0.35, 0.21, 0.24);
					Leg1.setObject();		//Upper leg
					glPushMatrix();
						//Knee Joint Animation Here
						//AnimatedTransformations();
					AnimatedTransformations(leftLegJoint[0][0],leftLegJoint[0][1], leftLegJoint[0][2],1,1,1);
					AnimatedRotation(LegJoint,leftLegJoint[1][0],leftLegJoint[1][1], leftLegJoint[1][2],1);
					//AnimatedRotation(LegJoint,leftLegJoint, 0, 0, 1);
						
						glColor3f(0.2, 0.5, 0.6);
						LegJoint.setObject();   //Knee JOint
						glPushMatrix();
							glColor3f(0.35, 0.21, 0.24);
							Leg2.setObject();     //Lower Leg
						glPopMatrix();
					glPopMatrix();
				glPopMatrix();
		glPopMatrix();
	}
	void makeRightLeg()
	{
		Objects Thighs(0, -0.2, 0, 0, 0, 0, 1.1, 0.5, 0.5, true), Leg1(0.12, -1.2, 0, 0, 0, 0, 0.5, 1.5, 0.5, true), LegJoint(0.12, -2.15, 0, 0, 0, 0, 0.3, 0.3, 0.3, false), Leg2(0.12, -3.1, 0, 0, 0, 0, 0.5, 1.5, 0.5, true);
		glPushMatrix();
		AnimatedTransformations(0.5, -1.95, 0, 1, 1, 1);
		AnimatedTransformations(rightLeg[0][0], rightLeg[0][1], rightLeg[0][2],1,1,1);
		AnimatedRotation(Thighs, rightLeg[1][0], rightLeg[1][1], rightLeg[1][2], 1);
		//AnimatedRotation(Thighs,rightLeg , 0, 0, 1);
		
		glColor3f(0.4, 0.1, 0.9);
			Thighs.setObject();
				glPushMatrix();
					glColor3f(0.35, 0.21, 0.24);
					Leg1.setObject();		//Upper leg
					glPushMatrix();
						//Knee Joint Animation Here
						//AnimatedTransformations();
					
					AnimatedTransformations(rightLegJoint[0][0], rightLegJoint[0][1], rightLegJoint[0][2],1,1,1);
					AnimatedRotation(LegJoint, rightLegJoint[1][0], rightLegJoint[1][1], rightLegJoint[1][2], 1);
					//AnimatedRotation(LegJoint, rightLegJoint, 0, 0, 1);


						glColor3f(0.2, 0.5, 0.6);
						LegJoint.setObject();  //Knee Joint
						glPushMatrix();
							glColor3f(0.35, 0.21, 0.24);
							Leg2.setObject(); //Lower Leg
						glPopMatrix();
					glPopMatrix();
				glPopMatrix();
		glPopMatrix();
	}


	bool readCSV(const std::string& filename, int frame)
	{
		std::ifstream file(filename);

		if (!file.is_open())
		{
			std::cout << " Unable to open file !" << std::endl;
			return false;
		}

		std::string line;
		int row = 0;
		while (std::getline(file, line)) {
			
			std::istringstream iss(line);
			int col = 0;
			row++;
			
			if (row > frame)
				break;
			else if(frame==row)
			while (col < 60) {  
				std::string value;
				if (!std::getline(iss, value, ',')) {
					std::cerr << "Error: Unexpected end of line in CSV file." << std::endl;
					return false;
				}

				//std::cout << value << std::endl;
				float floatValue = std::stof(value);
				
			

					switch (col) {
						// Position and rotation for left leg
					case 0:
						leftLeg[0][0] = floatValue; // X position
						break;
					case 1:
						leftLeg[0][1] = floatValue; // Y position
						break;
					case 2:
						leftLeg[0][2] = floatValue; // Z position
						break;
					case 3:
						leftLeg[1][0] = floatValue; // X rotation
						break;
					case 4:
						leftLeg[1][1] = floatValue; // Y rotation
						break;
					case 5:
						leftLeg[1][2] = floatValue; // Z rotation
						break;


					case 6:
						leftLegJoint[0][0] = floatValue; // X position
						break;
					case 7:
						leftLegJoint[0][1] = floatValue; // Y position
						break;
					case 8:
						leftLegJoint[0][2] = floatValue; // Z position
						break;
					case 9:
						leftLegJoint[1][0] = floatValue; // X rotation
						break;
					case 10:
						leftLegJoint[1][1] = floatValue; // Y rotation
						break;
					case 11:
						leftLegJoint[1][2] = floatValue; // Z rotation
						break;

						// Position and rotation for right leg
					case 12:
						rightLeg[0][0] = floatValue; // X position
						break;
					case 13:
						rightLeg[0][1] = floatValue; // Y position
						break;
					case 14:
						rightLeg[0][2] = floatValue; // Z position
						break;
					case 15:
						rightLeg[1][0] = floatValue; // X rotation
						break;
					case 16:
						rightLeg[1][1] = floatValue; // Y rotation
						break;
					case 17:
						rightLeg[1][2] = floatValue; // Z rotation
						break;

					case 18:
						rightLegJoint[0][0] = floatValue; // X position
						break;
					case 19:
						rightLegJoint[0][1] = floatValue; // Y position
						break;
					case 20:
						rightLegJoint[0][2] = floatValue; // Z position
						break;
					case 21:
						rightLegJoint[1][0] = floatValue; // X rotation
						break;
					case 22:
						rightLegJoint[1][1] = floatValue; // Y rotation
						break;
					case 23:
						rightLegJoint[1][2] = floatValue; // Z rotation
						break;

						// Position and rotation for left arm
					case 24:
						leftArm[0][0] = floatValue; // X position
						break;
					case 25:
						leftArm[0][1] = floatValue; // Y position
						break;
					case 26:
						leftArm[0][2] = floatValue; // Z position
						break;
					case 27:
						leftArm[1][0] = floatValue; // X rotation
						break;
					case 28:
						leftArm[1][1] = floatValue; // Y rotation
						break;
					case 29:
						leftArm[1][2] = floatValue; // Z rotation
						break;

						// Position and rotation for left arm joint
					case 30:
						leftArmJoint[0][0] = floatValue; // X position
						break;
					case 31:
						leftArmJoint[0][1] = floatValue; // Y position
						break;
					case 32:
						leftArmJoint[0][2] = floatValue; // Z position
						break;
					case 33:
						leftArmJoint[1][0] = floatValue; // X rotation
						break;
					case 34:
						leftArmJoint[1][1] = floatValue; // Y rotation
						break;
					case 35:
						leftArmJoint[1][2] = floatValue; // Z rotation
						break;




					case 36:
						rightArm[0][0] = floatValue; // X position
						break;
					case 37:
						rightArm[0][1] = floatValue; // Y position
						break;
					case 38:
						rightArm[0][2] = floatValue; // Z position
						break;
					case 39:
						rightArm[1][0] = floatValue; // X rotation
						break;
					case 40:
						rightArm[1][1] = floatValue; // Y rotation
						break;
					case 41:
						rightArm[1][2] = floatValue; // Z rotation
						break;

						// Position and rotation for right arm joint
					case 42:
						rightArmJoint[0][0] = floatValue; // X position
						break;
					case 43:
						rightArmJoint[0][1] = floatValue; // Y position
						break;
					case 44:
						rightArmJoint[0][2] = floatValue; // Z position
						break;
					case 45:
						rightArmJoint[1][0] = floatValue; // X rotation
						break;
					case 46:
						rightArmJoint[1][1] = floatValue; // Y rotation
						break;
					case 47:
						rightArmJoint[1][2] = floatValue; // Z rotation
						break;

					

						// Position and rotation for chest
					case 48:
						chest[0][0] = floatValue; // X position
						break;
					case 49:
						chest[0][1] = floatValue; // Y position
						break;
					case 50:
						chest[0][2] = floatValue; // Z position
						break;
					case 51:
						chest[1][0] = floatValue; // X rotation
						break;
					case 52:
						chest[1][1] = floatValue; // Y rotation
						break;
					case 53:
						chest[1][2] = floatValue; // Z rotation
						break;

						// Position and rotation for body (pelvis)
					case 54:
						body[0][0] = floatValue; // X position
						break;
					case 55:
						body[0][1] = floatValue; // Y position
						break;
					case 56:
						body[0][2] = floatValue; // Z position
						break;
					case 57:
						body[1][0] = floatValue; // X rotation
						break;
					case 58:
						body[1][1] = floatValue; // Y rotation
						break;
					case 59:
						body[1][2] = floatValue; // Z rotation
						break;

					default:
						break;
					}

				col++;
			}

			
		}
		file.close();
		return true;
	}
};

HumanoidSkeleton S;


void SequeneceManager(float angle)
{
	S.Update(angle);
}