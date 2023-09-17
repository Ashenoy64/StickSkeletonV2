#pragma once
#include<gl/glut.h>
#include<math.h>
#include<iostream>


#define animationSequnence "Walking"



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

	float bodyHorizontal=0;
	float bodyVertical = 0;
	float bodyRotation = 0;

	float rightLeg = 0;
	float rightLegJoint = 0;

	float leftLeg = 0;
	float leftLegJoint = 0;

	float leftHand = 0;
	float leftHandJoint = 0;

	float rightHand = 0;
	float rightHandJoint = 0;

	

public:
	HumanoidSkeleton()
	{
		makeSkeleton();
		//makeHead();
		//makeBody();
		//makeLeftArm();
		//makeRightArm();
		//makeLeftLeg();
		//makeRightLeg();
	}

	void Update(float angle)
	{

		rot = angle;

		if (animationSequnence =="Walking")
		{

			rightLegJoint = -sin(rot * 0.01) * 30 > 0 ? -sin(rot * 0.01) * 30 : -sin(rot * 0.01) * 5;
			rightLeg = sin(rot * 0.01) * 30;

			leftLeg = -sin(rot * 0.01) * 30;
			leftLegJoint = sin(rot * 0.01) * 30 > 0 ? sin(rot * 0.01) * 30 : sin(rot * 0.01) * 5;

			leftHand = sin(rot * 0.01) * 30;
			leftHandJoint = sin(rot * 0.01) * 40 < 0 ? sin(rot * 0.01) * 40 : 0;

			rightHand = -sin(rot * 0.01) * 30;
			rightHandJoint = -sin(rot * 0.01) * 40 < 0 ? -sin(rot * 0.01) * 40 : 0;


			if (!(sin(angle) > -0.2 and sin(angle) < 0.2))
				bodyHorizontal = -10 + (rot * 0.004);


			bodyVertical = -sin(rot * 0.01) * 0.05;
			bodyRotation = -sin(rot * 0.01) * 10;
		}
		else if (animationSequnence == "Jumping")
		{

		}
		else {
			//stopped Animation
		}
		

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
			
			
			AnimatedTransformations(0,bodyVertical, bodyHorizontal ,1,1,1);
			Pelvis.setObject(); //All are child to this
				glPushMatrix();
					
				AnimatedRotation(Chest, 0, bodyRotation, 0,  1);
					
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

		AnimatedRotation(Shoulder,leftHand,0,0,1);
		
		
		glColor3f(0.4, 0.1, 0.9);
		Shoulder.setObject();
		glPushMatrix();
			glColor3f(0.2, 0.5, 0.6);
				Arm1.setObject();  //Upper Arm
				glPushMatrix();
					//Elbow Joint Animation Here
					//AnimatedTransformations();
					
					AnimatedRotation(ArmJoint,leftHandJoint, 0, 0, 1);


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
		AnimatedRotation(Shoulder,rightHand , 0, 0, 1);
		
		glColor3f(0.4, 0.1, 0.9);
		Shoulder.setObject();
		glPushMatrix();
			glColor3f(0.2, 0.5, 0.6);
				Arm1.setObject();
				
				glPushMatrix();
					//Elbow Joint Animation Here
					//AnimatedTransformations();
					
					AnimatedRotation(ArmJoint, rightHandJoint, 0, 0, 1);

					

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
		AnimatedRotation(Thighs,leftLeg, 0, 0, 1);
		glColor3f(0.4, 0.1, 0.9);
			Thighs.setObject();
				glPushMatrix();
					glColor3f(0.35, 0.21, 0.24);
					Leg1.setObject();		//Upper leg
					glPushMatrix();
						//Knee Joint Animation Here
						//AnimatedTransformations();
					AnimatedRotation(LegJoint,leftLegJoint, 0, 0, 1);
						
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

		AnimatedRotation(Thighs,rightLeg , 0, 0, 1);
		
		glColor3f(0.4, 0.1, 0.9);
			Thighs.setObject();
				glPushMatrix();
					glColor3f(0.35, 0.21, 0.24);
					Leg1.setObject();		//Upper leg
					glPushMatrix();
						//Knee Joint Animation Here
						//AnimatedTransformations();
					AnimatedRotation(LegJoint, rightLegJoint, 0, 0, 1);


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
};

HumanoidSkeleton S;


void SequeneceManager(float angle)
{
	S.Update(angle);
}