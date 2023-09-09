#pragma once
#include<gl/glut.h>
#include<math.h>
#include<iostream>

void AnimatedTransformations(float px, float py, float pz, float rx, float ry, float rz, float sx, float sy, float sz)
{
	glTranslatef(px, py, pz);
	glRotatef(rz, 0, 0, 1);
	glRotatef(ry, 0, 1, 0);
	glRotatef(rx, 1, 0, 0);
	glScalef(sx, sy, sz);
}

class Objects
{
private:
	float posX, posY, posZ, rotX, rotY, rotZ, sX, sY, sZ;
	bool isCube;

public:
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
	float rot;

public:
	HumanoidSkeleton(float angle)
	{
		rot = angle;
		makeSkeleton();
		//makeHead();
		//makeBody();
		//makeLeftArm();
		//makeRightArm();
		//makeLeftLeg();
		//makeRightLeg();
	}

	void makeHead()
	{
		Objects Head(0,0,0,0,0,0,0.5,0.5,0.5,false), Neck(0,-0.75,0,0,0,0,0.25,0.5,0.25,true);
		glPushMatrix();
			AnimatedTransformations(0,1,0,0,0,0,1,1,1);
			glColor3f(0.3, 0.1, 0.1);
			Head.setObject();
			glColor3f(0.0, 0.2, 0.2);
			Neck.setObject();
		glPopMatrix();
	}

	void makeSkeleton()
	{
		Objects Chest(0,0,0,0,0,0,1.5,0.5,0.5,true), Abdomen(0,-0.68,0,0,0,0,1.25,0.85,0.5,true), Pelvis(0, -1.5, 0, 0, 0, 0, 1.0, 0.75, 0.5, true);
		glPushMatrix();
			AnimatedTransformations(0, 1, -20+rot *0.008, 0, 0, 0, 1, 1, 1);
			glColor3f(0.3, 0.1, 0.0);
			glPushMatrix();
			Pelvis.setObject(); //All are child to this
				glPushMatrix();
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

	void makeBody()
	{
		Objects Chest(0,0,0,0,0,0,1.5,0.5,0.5,true), Abdomen(0,-0.68,0,0,0,0,1.25,0.85,0.5,true), Pelvis(0, -1.5, 0, 0, 0, 0, 1.0, 0.75, 0.5, true);
		glPushMatrix();
		AnimatedTransformations(0, 1, 0, 0, 0, 0, 1, 1, 1);
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
		Objects Shoulder(0,0,0,0,0,0,0.75,0.7,0.5,true), Arm( -0.05, -0.8, 0, 0, 0, 0, 0.5, 1.2, 0.25, true), Pam(-0.05, -1.5, 0, 0, 0, 0, 0.25, 0.25, 0.25, false);
		glPushMatrix();
		AnimatedTransformations(-1.15, 0, 0, -sin(rot * 0.01) * 20, 0, 0, 1, 1, 1);
		
		glColor3f(0.4, 0.1, 0.9);
		Shoulder.setObject();
		glPushMatrix();
			glColor3f(0.2, 0.5, 0.6);
				Arm.setObject();
				glPushMatrix();
					glColor3f(0.35, 0.21, 0.24);
					Pam.setObject();
				glPopMatrix();
			glPopMatrix();
		glPopMatrix();
	}
	void makeRightArm()
	{
		Objects Shoulder(0, 0, 0, 0, 0, 0, 0.75, 0.7, 0.5, true), Arm(0.05, -0.8, 0, 0, 0, 0, 0.5, 1.2, 0.25, true), Pam(0.05, -1.5, 0, 0, 0, 0, 0.25, 0.25, 0.25, false);
		glPushMatrix();
		AnimatedTransformations(1.15, 0, 0, sin(rot*0.01)*20 , 0, 0, 1, 1, 1);
		
		glColor3f(0.4, 0.1, 0.9);
		Shoulder.setObject();
		glPushMatrix();
			glColor3f(0.2, 0.5, 0.6);
				Arm.setObject();
				glPushMatrix();
					glColor3f(0.35, 0.21, 0.24);
					Pam.setObject();
				glPopMatrix();
			glPopMatrix();
		glPopMatrix();
	}
	void makeLeftLeg()
	{
		Objects Thighs(0, -0.2, 0, 0, 0, 0, 1.1, 0.5, 0.5, true), Leg(-0.12, -1.45, 0, 0, 0, 0, 0.5, 2, 0.5, true), Feet(-0.12, -2.45, 0, 0, 0, 0, 0.3, 0.3, 0.3, false);
		glPushMatrix();
		AnimatedTransformations(-0.5, -1.95, 0, sin(rot * 0.01) * 20, 0, 0, 1, 1, 1);
		glColor3f(0.4, 0.1, 0.9);
			Thighs.setObject();
				glPushMatrix();
					glColor3f(0.35, 0.21, 0.24);
					Leg.setObject();
					glPushMatrix();
						glColor3f(0.2, 0.5, 0.6);
						Feet.setObject();
					glPopMatrix();
				glPopMatrix();
		glPopMatrix();
	}
	void makeRightLeg()
	{
		Objects Thighs(0, -0.2, 0, 0, 0, 0, 1.1, 0.5, 0.5, true), Leg(0.12, -1.45, 0, 0, 0, 0, 0.5, 2, 0.5, true), Feet(0.12, -2.45, 0, 0, 0, 0, 0.3, 0.3, 0.3, false);
		glPushMatrix();
		AnimatedTransformations(0.5, -1.95, 0, -sin(rot * 0.01) * 20, 0, 0, 1, 1, 1);
		glColor3f(0.4, 0.1, 0.9);
			Thighs.setObject();
				glPushMatrix();
					glColor3f(0.35, 0.21, 0.24);
					Leg.setObject();
					glPushMatrix();
						glColor3f(0.2, 0.5, 0.6);
						Feet.setObject();
					glPopMatrix();
				glPopMatrix();
		glPopMatrix();
	}
};

void SequeneceManager(float angle)
{
	HumanoidSkeleton S(angle);
}