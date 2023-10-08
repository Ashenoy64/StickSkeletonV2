#pragma once
#include<gl/glut.h>
#include<math.h>
#include<iostream>
#include <fstream>
#include <sstream>
#include <string>


//walking : uses code calculations
//csv_walking : uses inefficient method to read csv for animation
//csv_walking_efficient : uses efficient csv reading method
//walking_calculation : 30 stages animated


#define viewAngle 1
#define EDITING false
#define PI 3.14159265358979323846
#define animationSequnence "walking_calculation"


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


void InitLight()
{

	GLfloat lightpos[] = { 0.0, 0.0, 15.0 };
	GLfloat lightcolor[] = { 0.5, 0.5, 0.5 };
	GLfloat ambcolor[] = { 0.2, 0.2, 0.2 };
	GLfloat specular[] = { 1.0, 1.0, 1.0 };

	glEnable(GL_LIGHTING);

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambcolor);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightcolor);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	glEnable(GL_COLOR_MATERIAL); // tells opengl to maintain the original color of the object
}

class HumanoidSkeleton {
private:
	float rot=0;

	float frame = 1;

	std::ifstream filePointer;
	int rowNo = 0;
	bool isFileOpen = true;


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
		if (animationSequnence == "csv_walking_efficient")
		{
			openCSV("walking_efficient.csv");
		}
	}

	void Update(float angle)
	{

		rot = angle;
		++frame;

		if (animationSequnence =="walking")
		{
			//variables to fine tune the movements: 
			float legFact = 0.018;
			float armFact = 0.018;
			float speedMul = 0.018;
			float riseHeight = 0.1, riseSpeed = 0.019 * 2;
			float chestRotate = 0.018;


			//right leg walking motion: rotation along x axis
			rightLeg[1][0] = sin(rot * legFact) * 27;
			rightLegJoint[1][0] = -sin(rot * legFact) * 30 > 0 ? -sin(rot * legFact) * 30 : -sin(rot * legFact) * 5;

			//left leg walking motion: rotation along x axis
			leftLeg[1][0] = -sin(rot * legFact) * 27;
			leftLegJoint[1][0] = sin(rot * legFact) * 30 > 0 ? sin(rot * legFact) * 30 : sin(rot * legFact) * 5;

			//left arm walking motion: rotation along x axis
			leftArm[1][0] = sin(rot * armFact) * 35;
			leftArmJoint[1][0] = sin(rot * armFact) * 40 < 0 ? sin(rot * armFact) * 40 : 0;

			//right arm walking motion: rotation along x axis
			rightArm[1][0] = -sin(rot * armFact) * 35;
			rightArmJoint[1][0] = -sin(rot * armFact) * 40 < 0 ? -sin(rot *armFact) * 40 : 0;

			
		
		
			if (!(sin(angle) > -0.001 and sin(angle) < 0.001)) {
				body[0][2] = -10 + (rot * speedMul);
			}

			body[0][1] = sin(rot * riseSpeed) * riseHeight;
	
			chest[1][1] = -sin(rot *chestRotate) * 10;

			if (!(sin(angle) > -0.2 and sin(angle) < 0.2))
				body[0][2] = -10 + (rot * 0.0035);


			body[0][1] = -sin(rot * 0.01) * 0.05;
			chest[1][1] = -sin(rot * 0.01) * 10;


			

		}
		else if (animationSequnence == "csv_walking")
		{
			readCSV("walking.csv", frame);
		}
		else if (animationSequnence == "csv_walking_efficient" and isFileOpen)
		{
			readCSV_Efficient(frame);
		}
		else if (animationSequnence == "jumping")
		{
			
			readCSV("jumping.csv", frame);

		}
		else if (animationSequnence == "walking_calculation")
		{
			//int stage = (int)(rot/20) % 9;
			int stage = (int)(rot/20) % 30;
			//int stage = 2;
			/*
			leftLeg = [-15, -30, 0, 15, 30, 0, -15, -30]
			leftLegJoint = [0, 30, 0, 0, 15, 30, 45, 15]

			rightLeg = [30, 0, -15, -30, -15, -30, 0, 15]
			rightLegJoint = [15, 30, 45, 15, 0, 30, 0, 0]

			upDown = [-0.11, -0.26, -0.11, +0.11]
			*/

			if (!(sin(angle) > -0.2 and sin(angle) < 0.2))
				body[0][2] = -10 + (2.276/8 * rot / 20);


			if (stage == 1)
			{
				leftLeg[1][0] = -15;
				leftLegJoint[1][0] = 0;

				rightLeg[1][0] = 30;
				rightLegJoint[1][0] = 15;

				body[0][1] = -0.11;


				rightArm[1][0] = -15;
				rightArmJoint[1][0] =-30 ;

				leftArm[1][0] = 15;
				leftArmJoint[1][0] = -30 ;
	

			}
			else if (stage == 2)
			{
				leftLeg[1][0] = -18.5;
				leftLegJoint[1][0] = 7.5;

				rightLeg[1][0] = 22.5;
				rightLegJoint[1][0] = 18.5;

				body[0][1] = -0.14;


				rightArm[1][0] = -11.25;
				rightArmJoint[1][0] = -26.25;

				leftArm[1][0] = 11.25;
				leftArmJoint[1][0] = -22.5;

			}
			else if (stage == 3)
			{
				leftLeg[1][0] = -22.5;
				leftLegJoint[1][0] = 15;

				rightLeg[1][0] = 15;
				rightLegJoint[1][0] = 22.5;

				body[0][1] = -0.18;


				rightArm[1][0] = -7.5;
				rightArmJoint[1][0] = -22.5;

				leftArm[1][0] = 7.5;
				leftArmJoint[1][0] = -15;
				
			}
			else if (stage == 4)
			{
				leftLeg[1][0] = -30;
				leftLegJoint[1][0] = 30;

				rightLeg[1][0] = 0;
				rightLegJoint[1][0] = 30;

				body[0][1] = -0.22;


				rightArm[1][0] = 0;
				rightArmJoint[1][0] = -15;

				leftArm[1][0] = 0;
				leftArmJoint[1][0] = 0;
				
			}
			else if (stage == 5) 
			{
				leftLeg[1][0] = -22.5;
				leftLegJoint[1][0] = 22.5;

				rightLeg[1][0] = -3.5;
				rightLegJoint[1][0] = 33.5;

				body[0][1] = -0.22;

				rightArm[1][0] = 4;
				rightArmJoint[1][0] = -11.5;

				leftArm[1][0] = -4;
				leftArmJoint[1][0] = -4;


			}
			else if (stage == 6)
			{
				leftLeg[1][0] = -15;
				leftLegJoint[1][0] = 15;

				rightLeg[1][0] = -7.5;
				rightLegJoint[1][0] = 37.5;

				body[0][1] = -0.18;

				rightArm[1][0] = 7.5;
				rightArmJoint[1][0] = -7.5;

				leftArm[1][0] = -7.5;
				leftArmJoint[1][0] = -7.5;


			}
			else if (stage == 7)
			{
				leftLeg[1][0] = -7.5;
				leftLegJoint[1][0] = 7.5;

				rightLeg[1][0] = -7.5;
				rightLegJoint[1][0] = 42;

				body[0][1] = -0.14;

				rightArm[1][0] = 11.5;
				rightArmJoint[1][0] = -4;

				leftArm[1][0] = -11.5;
				leftArmJoint[1][0] = -11.5;


			}
			else if (stage == 8)
			{
				leftLeg[1][0] = 0;
				leftLegJoint[1][0] = 0;

				rightLeg[1][0] = -15;
				rightLegJoint[1][0] = 45;

				body[0][1] = -0.11;

				rightArm[1][0] = 15;
				rightArmJoint[1][0] = 0;

				leftArm[1][0] = -15;
				leftArmJoint[1][0] = -15;

				
			}
			else if (stage == 9)
			{
				leftLeg[1][0] = 7.5;
				leftLegJoint[1][0] = 0;

				rightLeg[1][0] = -22.5;
				rightLegJoint[1][0] = 30;

				body[0][1] = 0;

				rightArm[1][0] = 22.5;
				rightArmJoint[1][0] = -7.5;

				leftArm[1][0] = -22.5;
				leftArmJoint[1][0] = -7.5;


			}
			else if (stage == 10)
			{
				leftLeg[1][0] = 11.5;
				leftLegJoint[1][0] = 0;

				rightLeg[1][0] = -27.5;
				rightLegJoint[1][0] = 22.5;

				body[0][1] = -0.055;

				rightArm[1][0] = 26;
				rightArmJoint[1][0] = -11.5;

				leftArm[1][0] = -26;
				leftArmJoint[1][0] = -4;


				}
			else if (stage == 11)
			{
				leftLeg[1][0] = 15;
				leftLegJoint[1][0] = 0;

				rightLeg[1][0] = -30;
				rightLegJoint[1][0] = 15;

				body[0][1] = -0.11;

				rightArm[1][0] = 30;
				rightArmJoint[1][0] =-15;

				leftArm[1][0] = -30;
				leftArmJoint[1][0] = 0;

				
			}
			else if (stage == 12)
			{
				leftLeg[1][0] = 18.5;
				leftLegJoint[1][0] = 3.75;

				rightLeg[1][0] = -26;
				rightLegJoint[1][0] = 11;

				body[0][1] = -0.05;

				rightArm[1][0] = 30;
				rightArmJoint[1][0] = -11.5;

				leftArm[1][0] = -30;
				leftArmJoint[1][0] = -4;
			}
			else if (stage == 13)
			{
				leftLeg[1][0] = 22.5;
				leftLegJoint[1][0] = 7.5;

				rightLeg[1][0] = -22.5;
				rightLegJoint[1][0] = 7.5;

				body[0][1] = 0;

				rightArm[1][0] = 30;
				rightArmJoint[1][0] = -7.5;

				leftArm[1][0] = -30;
				leftArmJoint[1][0] = -7.5;
			}
			else if (stage == 14)
			{
				leftLeg[1][0] = 26;
				leftLegJoint[1][0] = 11.25;

				rightLeg[1][0] = -18;
				rightLegJoint[1][0] = 4;

				body[0][1] = -0.05;

				rightArm[1][0] = 30;
				rightArmJoint[1][0] = -4;

				leftArm[1][0] = -30;
				leftArmJoint[1][0] = -11.25;
				}
			else if (stage == 15)
			{
				leftLeg[1][0] = 30;
				leftLegJoint[1][0] = 15;

				rightLeg[1][0] = -15;
				rightLegJoint[1][0] = 0;

				body[0][1] = -0.11;

				rightArm[1][0] = 30;
				rightArmJoint[1][0] = 0;

				leftArm[1][0] = -30;
				leftArmJoint[1][0] = -15;
			}
			else if (stage == 16)
			{
				leftLeg[1][0] = 22.5;
				leftLegJoint[1][0] = 18.5;

				rightLeg[1][0] = -18.5;
				rightLegJoint[1][0] = 7.5;

				body[0][1] = -0.145;

				rightArm[1][0] = 25.5;
				rightArmJoint[1][0] = 0;

				leftArm[1][0] = -27.5;
				leftArmJoint[1][0] = -15;
			}
			else if (stage == 17)
			{
				leftLeg[1][0] = 15;
				leftLegJoint[1][0] = 22.5;

				rightLeg[1][0] = -22.5;
				rightLegJoint[1][0] = 15;

				body[0][1] = -0.185;

				rightArm[1][0] = 22.5;
				rightArmJoint[1][0] = 0;

				leftArm[1][0] = -22.5;
				leftArmJoint[1][0] = -15;
			}
			else if (stage == 18)
			{
				leftLeg[1][0] = 7.5;
				leftLegJoint[1][0] = 27.5;

				rightLeg[1][0] = -27.5;
				rightLegJoint[1][0] = 22.5;

				body[0][1] = -0.22;

				rightArm[1][0] = 18;
				rightArmJoint[1][0] = 0;

				leftArm[1][0] = -18;
				leftArmJoint[1][0] = -15;
			}
			else if (stage == 19)
			{
				leftLeg[1][0] = 0;
				leftLegJoint[1][0] = 30;

				rightLeg[1][0] = -30;
				rightLegJoint[1][0] = 30;

				body[0][1] = -0.22;

				rightArm[1][0] = 15;
				rightArmJoint[1][0] = 0;

				leftArm[1][0] = -15;
				leftArmJoint[1][0] = -15;
			}
			else if (stage == 20)
			{
				leftLeg[1][0] = -4;
				leftLegJoint[1][0] = 37.5;

				rightLeg[1][0] = -15;
				rightLegJoint[1][0] = 15;

				body[0][1] = -0.185;

				rightArm[1][0] = -15;
				rightArmJoint[1][0] = -7.5;

				leftArm[1][0] = -7.5;
				leftArmJoint[1][0] = -15;
			}
			else if (stage == 21)
			{
				leftLeg[1][0] = -7.5;
				leftLegJoint[1][0] = 37.5;

				rightLeg[1][0] = -15;
				rightLegJoint[1][0] = 15;

				body[0][1] = -0.14;

				rightArm[1][0] = -15;
				rightArmJoint[1][0] = -7.5;

				leftArm[1][0] = -7.5;
				leftArmJoint[1][0] = -15;
			}
			else if (stage == 22)
			{
				leftLeg[1][0] = -11.5;
				leftLegJoint[1][0] = 41.5;

				rightLeg[1][0] = -7.5;
				rightLegJoint[1][0] = 7.5;

				body[0][1] = -0.11;

				rightArm[1][0] = -15;
				rightArmJoint[1][0] = -11.5;

				leftArm[1][0] = -4;
				leftArmJoint[1][0] = -15;
			}
			else if (stage == 23)
			{
				leftLeg[1][0] = -15;
				leftLegJoint[1][0] = 45;

				rightLeg[1][0] = 0;
				rightLegJoint[1][0] = 0;

				body[0][1] = -0.11;

				rightArm[1][0] = -15;
				rightArmJoint[1][0] = -15;

				leftArm[1][0] = 0;
				leftArmJoint[1][0] = -15;
			}
			else if (stage == 24)
			{
				leftLeg[1][0] = -18.5;
				leftLegJoint[1][0] = 37.5;

				rightLeg[1][0] = 3.75;
				rightLegJoint[1][0] = 0;

				body[0][1] = -0.05;


				rightArm[1][0] = -18.5;
				rightArmJoint[1][0] = -15;

				leftArm[1][0] = 3.75;
				leftArmJoint[1][0] = -18.5;
			}
			else if (stage == 25)
			{
				leftLeg[1][0] = -22.5;
				leftLegJoint[1][0] = 30;

				rightLeg[1][0] = 7.5;
				rightLegJoint[1][0] = 0;

				body[0][1] = 0;


				rightArm[1][0] = -22.5;
				rightArmJoint[1][0] = -15;

				leftArm[1][0] = 7.5;
				leftArmJoint[1][0] = -22.5;
			}
			else if (stage == 26)
			{
				leftLeg[1][0] = -27.5;
				leftLegJoint[1][0] = 22.5;

				rightLeg[1][0] = 11.5;
				rightLegJoint[1][0] = 0;

				body[0][1] = -0.05;


				rightArm[1][0] = -27.5;
				rightArmJoint[1][0] = -15;

				leftArm[1][0] = 11.5;
				leftArmJoint[1][0] = -27.5;
				}
			else if (stage == 27)
			{
				leftLeg[1][0] = -30;
				leftLegJoint[1][0] = 15;

				rightLeg[1][0] = 15;
				rightLegJoint[1][0] = 0;

				body[0][1] = -0.11;


				rightArm[1][0] = -30;
				rightArmJoint[1][0] = -15;

				leftArm[1][0] = 15;
				leftArmJoint[1][0] = -30;
			}
			else if (stage == 28) {
				leftLeg[1][0] = -27.5;
				leftLegJoint[1][0] = 11.5;

				rightLeg[1][0] = 18.5;
				rightLegJoint[1][0] = 3.75;

				body[0][1] = -0.05;


				rightArm[1][0] = -27.5;
				rightArmJoint[1][0] = -18.5;

				leftArm[1][0] = 15;
				leftArmJoint[1][0] = -30;
			}
			else if(stage == 29){
				leftLeg[1][0] = -22.5;
				leftLegJoint[1][0] = 7.5;

				rightLeg[1][0] = 22.5;
				rightLegJoint[1][0] = 7.5;

				body[0][1] = 0;


				rightArm[1][0] = -22.5;
				rightArmJoint[1][0] = -22.5;

				leftArm[1][0] = 15;
				leftArmJoint[1][0] = -30;
			}

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
		glColor3f(1,1,1);
		glLineWidth(5.0);
		glBegin(GL_LINES);
		glVertex3d(0, -4.9, -50);
		glVertex3d(0, -4.9, 50);
		glEnd();
		glPopMatrix();
		//std::cout << "hello"<<std::endl;

		glPushMatrix();
			AnimatedTransformations(0, 1, 0, 1, 1, 1);
			glColor3f(1,0.5,0.0);
			glPushMatrix();
			
			float angle = sin(rot * 0.01);
			
			AnimatedTransformations(0,body[0][1], body[0][2] ,1,1,1);
			AnimatedRotation(Pelvis,body[1][0], body[1][1], body[1][2], 1);

			//AnimatedRotation();
			Pelvis.setObject(); //All are child to this
				glPushMatrix();
				AnimatedTransformations(chest[0][0], chest[0][1],chest[0][2],1,1,1);
				AnimatedRotation(Chest, chest[1][0], chest[1][1],chest[1][2] ,  1);
					
					glColor3f(1, 0.5, 0.5);
					Chest.setObject(); //Abdomen and Arms are the child 
						glPushMatrix();
							glColor3f(1, 0.5, 0.0);
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
			glColor3f(0.5, 0.5,1 );
			Head.setObject();
			glColor3f(1, 0.5, 0.0);
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
	bool openCSV(const std::string& filename)
	{
		filePointer.open(filename);

		if (!filePointer.is_open())
		{
			std::cout << " Unable to open file !" << std::endl;
			isFileOpen = false;
			return false;
		}
		std::string line;
		if (std::getline(filePointer, line))
		{
			std::istringstream iss(line);
			rowNo++;
		}
	}
	bool readCSV_Efficient(int frame)
	{
		std::string line;
		
		if (std::getline(filePointer, line))
		{
			std::istringstream iss(line);
			rowNo++;
			int col = 0;
			while (col < 60) {
				std::string value;
				if (!std::getline(iss, value, ',')) {
					std::cerr << "Error: Unexpected end of line in CSV file." << std::endl;
					return false;
				}
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
		else {
			isFileOpen = false;
			filePointer.close();
		}

		
	}
};

HumanoidSkeleton S;


void SequeneceManager(float angle)
{
	InitLight();
	S.Update(angle);
}