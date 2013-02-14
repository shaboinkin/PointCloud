/******************************************************************************************************
 *
 * Basis of OpenGL stuff was created by Vijay Kalivarapu (I think) at Iowa State University
 * for the SPIRE-REU, 2011.
 * 
 * Modifications added in by: Daniel Kulas
 *									11/7/12
 *******************************************************************************************************/

#ifndef _RENDER_H
#define _RENDER_H

//Place cstdlib above glut, causes redefinition error with exit()
#include <cstdlib>
#include "glut.h"

#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <sstream>
#include <iterator>
#include <stack>


using namespace std;

class Render
{
public:
	//constructor definition
	Render();
	//destructor definition
	~Render();

	// These functions are defined public so they can be called by the main function in main.cpp
	void display();
	void init(string file);
	void reshape(int w, int h);
	void mouseClick(int button, int state, int x, int y);
	void mouseMove(int x, int y);
	void keyPos(unsigned char key, int x, int y);

	//Draws an individual box
	void drawBox(float x, float y, float z, float red, float green, float blue);

	//Draws the entire scene
	void drawSurface();


private:
	// These functions are private and cannot be called anywhere outside of render class

	vector<string> lineval;
	vector<string> newline;

	string s_x, s_y, s_z,
		   sred, sgreen, sblue,
		   srow, scol;

	float x_pos, y_pos, z_pos;
	   
	int	   red, green, blue,
		   row, col;

	std::vector<float> x;
	std::vector<float> y;
	std::vector<float> z;
	std::vector<float> cred;
	std::vector<float> cgreen;
	std::vector<float> cblue;
	std::vector<int> crow;
	std::vector<int> ccol;


	void drawTextureBox(void);
	void clamp(float v0, float v1, float v2);
	void OpenFile(string file);

	// Variables used in the member functions of render class
	float eye[3];
	float rot[3];
	int Wx, Wy;
	int LEFT, MIDDLE, RIGHT;
	int mButton;
	int mOldX, mOldY;
	
	
};

#endif
