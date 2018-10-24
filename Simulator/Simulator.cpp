#using <System.dll>

#include <zmq.hpp>
#include <turbojpeg.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>
#include <conio.h>
#include <stdio.h>

#include "SMFcn.h"
#include "SMObject.h"
#include "SMStructs.h"
#include "SMFcn.cpp"
#include "SMObject.cpp"

#include <Windows.h>

void display();
void idle();

GLuint tex;

zmq::context_t context(1);
zmq::socket_t subscriber(context, ZMQ_SUB);

int main(int argc, char** argv)
{
	TCHAR ProM[] = TEXT("ProcessManagement");
	TCHAR SIMULATOR[] = TEXT("Simulator");
	SMObject PMObj(ProM, sizeof(ProcessManagement));
	SMObject SimObj(SIMULATOR, sizeof(Simulator));
	PMObj.SMAccess();
	SimObj.SMAccess();
	ProcessManagement* PM = (ProcessManagement*)PMObj.pData;
	Simulator* sim = (Simulator*)SimObj.pData;

	while (!PM->ShutDown.Flags.Simulator) {
		PM->HeartBeat.Flags.Simulator = 1;

		const int WINDOW_WIDTH = 800;
		const int WINDOW_HEIGHT = 600;

		glutInit(&argc, (char**)(argv));
		glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
		glutInitWindowPosition(0, 0);
		glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
		glutCreateWindow("MTRN3500 - GL");

		glutDisplayFunc(display);
		glutIdleFunc(idle);

		glGenTextures(1, &tex);

		subscriber.connect("tcp://192.168.1.200:26000");
		subscriber.setsockopt(ZMQ_SUBSCRIBE, "", 0);

		glutMainLoop();
	}
	PM->HeartBeat.Flags.Simulator = 0;
	return 0;
}

void display() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);

	glBegin(GL_QUADS);
		glTexCoord2f(0, 1); glVertex2f(-1, -1);
		glTexCoord2f(1, 1); glVertex2f(1, -1);
		glTexCoord2f(1, 0); glVertex2f(1, 1);
		glTexCoord2f(0, 0); glVertex2f(-1, 1);
	glEnd();
	glutSwapBuffers();

}

void idle() {

	TCHAR ProM[] = TEXT("ProcessManagement");
	SMObject PMObj(ProM, sizeof(ProcessManagement));
	PMObj.SMAccess();
	ProcessManagement* PM = (ProcessManagement*)PMObj.pData;
	if (PM->ShutDown.Flags.Simulator == 1) {
		exit(0);
	}

	zmq::message_t update;
	if (subscriber.recv(&update, ZMQ_NOBLOCK)) {
		std::cout << "Received data is " << update.size() << std::endl;

		long unsigned int _jpegSize = update.size();
		unsigned char* _compressedImage = static_cast<unsigned char*>(update.data());

		int jpegSubsamp, width, height;

		tjhandle _jDecompressor = tjInitDecompress();
		tjDecompressHeader2(_jDecompressor, _compressedImage, _jpegSize, &width, &height, &jpegSubsamp);
		unsigned char* buffer = new unsigned char[width*height * 3];

		std::cout << "Dimensions are " << width << "*" << height << std::endl;

		tjDecompress2(_jDecompressor, _compressedImage, _jpegSize, buffer, width, 0, height, TJPF_RGB, TJFLAG_FASTDCT);
		tjDestroy(_jDecompressor);

		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, buffer);

		delete[] buffer;
	}

	display();

}