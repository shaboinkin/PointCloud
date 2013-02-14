/******************************************************************************************************
 *
 * Basis of OpenGL stuff was created by Vijay Kalivarapu (I think) at Iowa State University
 * for the SPIRE-REU, 2011.
 * 
 * Modifications added in by: Daniel Kulas
 *									11/7/12
 *******************************************************************************************************/

#include "../include/render.h"

#define TIMER 33

using namespace std;

void recomputeFrame(int value);
float myrot;	

void recomputeFrame(int value)
{
	myrot += 1.0f;
	if(myrot > 360.0f)
		myrot -= 360.0f;
	glutPostRedisplay();
}


// Default constructor
// used for initializing any user defined variables
Render::Render()
{
	// specify a default location for the camera
	eye[0] = 0.0f;
	eye[1] = 0.0f;
	eye[2] = 300.0f;

	// specify default values to the rotational values in the transformation matrix
	rot[0] = 0.0f;
	rot[1] = 0.0f;
	rot[2] = 0.0f;

	LEFT = 0;
	MIDDLE = 1;
	RIGHT = 2;
	mButton = -1;
}

Render::~Render()
{


}

void Render::init(string file)
{
	OpenFile(file);
	// clear the background to the color values specified in the parentheses
	glClearColor (0.0, 0.0, 0.0, 0.0);

	// this is a 3D scene. so, clear the depth
	glClearDepth(1.0);

	// I need depth. enable depth
	glEnable(GL_DEPTH_TEST);

	glShadeModel (GL_SMOOTH);

	// Initialize lighting
	GLfloat LightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f};
	GLfloat LightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glLightfv (GL_LIGHT0, GL_AMBIENT, LightAmbient);
	glLightfv (GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
	glEnable (GL_LIGHT0);
	glEnable (GL_LIGHTING);
	glEnable (GL_COLOR_MATERIAL);
	glColorMaterial (GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

}

void Render::OpenFile(string file)
{
	ifstream input;

	string line;
	string errHandle;

	stringstream ss;
	string fileFormat = ".pts";
	string finalName;

	ss << file << fileFormat;
	finalName = ss.str();	
	input.open("./PTSFiles/" + finalName, ios::in);
	
	/*
	 * if running binary file by itself, change to 
	 * input.open("../PTSFiles/" + finalName, ios::in);
	 */


	if(!input)
	{

		cout << "Error opening file. Bad file name or lack of file in directory. Try again." << endl;
		cout << "Press any key, then enter, to exit" << endl;
		while( cin >> errHandle)
		{
			exit(1);
		}
	}

	cout << "Opened file, " << finalName << endl;
	cout << "Please wait..." << endl;

	//read through file
	while(getline(input, line))
	{
		istringstream iss(line);
		copy(istream_iterator<string>(iss),	//get string
				istream_iterator<string>(),	//get space between values
				back_inserter<vector<string> >(lineval));	//place string in vector
	}

	for(int i = 0; i < lineval.size(); i++)
	{
		//since there are 8 strings, just mod what you want
		if( i % 8 == 0)
		{
			s_x = lineval.at(i);
			istringstream get_x(s_x);
			get_x >> x_pos;
			x.push_back(x_pos);
		}
		if( i % 8 == 1)
		{
			s_y = lineval.at(i);
			istringstream get_y(s_y);
			get_y >> y_pos;
			y.push_back(y_pos);
		}
		if( i % 8 == 2)
		{
			s_z = lineval.at(i);
			istringstream get_z(s_z);
			get_z >> z_pos;
			z.push_back(z_pos);
		}
		if( i % 8 == 3)
		{
			sred = lineval.at(i);
			istringstream get_red(sred);
			get_red >> red;
			cred.push_back(red);
		}
		if( i % 8 == 4)
		{
			sgreen = lineval.at(i);
			istringstream get_green(sgreen);
			get_green >> green;
			cgreen.push_back(green);
		}
		if( i % 8 == 5)
		{
			sblue = lineval.at(i);
			istringstream get_blue(sblue);
			get_blue >> blue;
			cblue.push_back(blue);
		}
		if( i % 8 == 6)
		{
			srow = lineval.at(i);
			istringstream get_row(srow);
			get_row >> row;
			crow.push_back(row);
		}
		if( i % 8 == 7)
		{
			scol = lineval.at(i);
			istringstream get_col(scol);
			get_col >> col;
			ccol.push_back(col);
		}
	}

	cout << "Finished reading and converting file" << endl;
}

void Render::reshape(int w, int h)
{
	glViewport(0,0,(GLsizei) w, (GLsizei) h);
	Wx = w;
	Wy = h;
}

void Render::mouseClick(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN) 
	{
		mOldX = x;
		mOldY = y;
        switch (button)  
		{
			case GLUT_LEFT_BUTTON:
				mButton = LEFT;
				break;
			case GLUT_MIDDLE_BUTTON: 
				mButton = MIDDLE; 
				break;
			case GLUT_RIGHT_BUTTON:
				mButton = RIGHT;
				break;
			default:
				break;
		}
	}
	else if (state == GLUT_UP)
	{
		mButton = -1;
	}
}

void Render::mouseMove(int x, int y)
{
	if (mButton == LEFT) 
	{
		rot[0] -= ((mOldY - y) * 180.0f) / 1000.0f;
		rot[1] -= ((mOldX - x) * 180.0f) / 1000.0f;
		clamp(rot[0], rot[1], rot[2]);
	}
	else if (mButton == MIDDLE) 
	{
		eye[2] -= ((mOldY - y) * 180.0f) / 500.0f;
		clamp(rot[0], rot[1], rot[2]);
	}
	else if (mButton == RIGHT)
	{
		eye[0] += ((mOldX - x) * 180.0f) / 1000.0f;
		eye[1] -= ((mOldY - y) * 180.0f) / 1000.0f;
		clamp(rot[0], rot[1], rot[2]);
	}	 
	mOldX = x; 
	mOldY = y;

}

void Render::keyPos(unsigned char key, int x, int y)
{
	switch (key)
	{
		default:
			break;
	    case 27:	// this is for the 'Esc' key on the keyboard, quit the program when the esc key is pressed
			exit (0);
	}
}

void Render::display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Specifies which matrix stack is the target for subsequent matrix operations
	// In this example, the projection matrix is set to perspective projection matrix stack
	glMatrixMode(GL_PROJECTION);
	// all matrix values from previous frames set to identity
	glLoadIdentity();
	
	// perspective projection loaded with new values for Wx and Wy updated
	gluPerspective(45, (GLfloat) Wx/(GLfloat) Wy, 1.0, 10000.0);
	// use glOrtho for a 3D orthogonal projection
	//glOrtho(-100, 100, -100, 100, -100, 100);

	// Applies subsequent matrix operations to the modelview matrix stack.
	glMatrixMode(GL_MODELVIEW);
	// Clears all the previously loaded values in the modelview matrix
	glLoadIdentity();
	// Now, add a bunch of transformation operations
	glTranslatef(-eye[0], -eye[1], -eye[2]);
    glRotatef(rot[0], 1.0f, 0.0f, 0.0f);
    glRotatef(rot[1], 0.0f, 1.0f, 0.0f);
    glRotatef(rot[2], 0.0f, 0.0f, 1.0f);

	//Draws the scene
	drawSurface();
	
	//	Perform rotating cube animation
	glutTimerFunc(TIMER, recomputeFrame, 0);

	// this allows opengl to wait for the draw buffer to be ready in the background for the next frame
	// therefore, while the current buffer is being drawn in the current frame, a buffer is set ready to draw on frame+1
	// this call is effective only when GL_DOUBLE is enabled in glutInitDisplayMode in the main function
	// It is recommended to use glutSwapBuffers and double buffering always
	glutSwapBuffers();
}

//function will draw the surface that the Bumblebee2 camera is currently viewing
void Render::drawBox(float x, float y, float z, float red, float green, float blue)
{
	glColor3f(red, green, blue);
	glPushMatrix();
		glTranslated(x, y, z);		
		glutSolidCube(1);
	glPopMatrix();
}

void Render::drawSurface()
{
	for(int i = 0; i < ccol.size(); i++)
	{
		//pop stuff off of stacks and draw cubes, multiplying for scaling purposes
		float xval = x.at(i) * 75.0;
		float yval = y.at(i) * 75.0;
		float zval = z.at(i) * 75.0;

		float red = cred.at(i) / 255.0;
		float green = cgreen.at(i) / 255.0;
		float blue = cblue.at(i) / 255.0;

		int row = crow.at(i);
		int col = ccol.at(i);

		drawBox(xval, yval, zval, red, green, blue);
	}
}

// this is for clamping the numbers between 0 & 360. used for rotation values in the mouse move function
void Render::clamp(float v0, float v1, float v2)
{
	if (v0 > 360 || v0 < -360)
		v0 = 0;
	if (v1 > 360 || v1 < -360)
		v1 = 0;
	if (v2 > 360 || v2 < -360)
		v2 = 0;
}

