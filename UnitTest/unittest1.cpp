#include "stdafx.h"
#include "CppUnitTest.h"
#include <list> // Using list arrays
#include "../3D Game/Box.h"
#include "../3D Game/Camera.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		// Try testing if we can generate random floats
		TEST_METHOD(Test_RandomFloat) {
			try {
				GLfloat x = RandomFloat(-15.0f, 15.0f);
				GLfloat y = RandomFloat(-15.0f, 15.0f);
				GLfloat z = RandomFloat(-15.0f, 15.0f);
			} catch (...) {
				Assert::Fail(L"Error in RandomFloat!", LINE_INFO());
			}
		}

		// Try adding 100 boxes
		TEST_METHOD(Test_BoxSetup) {
			try {
				list<Box*> Boxes;
				for (GLint i = 0; i < 100; i++) {
					Box* nBox = new Box();
					GLfloat x = RandomFloat(-15.0f, 15.0f);
					GLfloat y = RandomFloat(-15.0f, 15.0f);
					GLfloat z = RandomFloat(-15.0f, 15.0f);
					nBox->pos = vec3(x, y, z);
					Boxes.push_back(nBox);
				}
			} catch (...) {
				Assert::Fail(L"Error in creating 100 boxes.", LINE_INFO());
			}
		}

		// Try forcing a box to jump
		TEST_METHOD(Test_BoxJump) {
			try {
				Box* box = new Box();
				box->Jump();
			} catch (...) {
				Assert::Fail(L"Error in Box Jump.", LINE_INFO());
			}
		}

		// Try testing if the box can jump
		TEST_METHOD(Test_BoxCanJump) {
			try {
				Box* box = new Box();
				box->CanJump();
			} catch (...) {
				Assert::Fail(L"Error in Box CanJump!", LINE_INFO());
			}
		}

		// Try testing if we can create camera
		TEST_METHOD(Test_CameraCreate) {
			try {
				Camera camera(vec3(0.0f, 0.0f, 3.0f));
			} catch (...) {
				Assert::Fail(L"Error in Camera Create!", LINE_INFO());
			}
		}

		// Try testing if we can get Camera Zoom
		TEST_METHOD(Test_CameraZoom) {
			try {
				Camera camera(vec3(0.0f, 0.0f, 3.0f));
				camera.GetZoom();
			} catch (...) {
				Assert::Fail(L"Error in Camera Zoom!", LINE_INFO());
			}
		}

		// Try testing if we can get Camera View Matrix
		TEST_METHOD(Test_CameraViewMatrix) {
			try {
				Camera camera(vec3(0.0f, 0.0f, 3.0f));
				camera.GetViewMatrix();
			} catch (...) {
				Assert::Fail(L"Error in Camera View Matrix!", LINE_INFO());
			}
		}

		// Try testing if the Camera can Move
		TEST_METHOD(Test_CameraMove) {
			try {
				Camera camera(vec3(0.0f, 0.0f, 3.0f));
				camera.ProcessKeyboard(FORWARD, 2.0f);
				camera.ProcessKeyboard(BACKWARD, 2.0f);
				camera.ProcessKeyboard(LEFT, 2.0f);
				camera.ProcessKeyboard(RIGHT, 2.0f);
			} catch (...) {
				Assert::Fail(L"Error in Camera Mo!", LINE_INFO());
			}
		}

		// Try testing if the Camera can Move Mouse
		TEST_METHOD(Test_CameraMouse) {
			try {
				Camera camera(vec3(0.0f, 0.0f, 3.0f));
				camera.ProcessMouseMovement(1.0f, 1.0f, true);
			} catch (...) {
				Assert::Fail(L"Error in Camera Move Mouse!", LINE_INFO());
			}
		}

		// Try testing if the Camera can use Scroll
		TEST_METHOD(Test_CameraScroll) {
			try {
				Camera camera(vec3(0.0f, 0.0f, 3.0f));
				camera.ProcessMouseScroll(1.0f);
			} catch (...) {
				Assert::Fail(L"Error in Camera Scroll!", LINE_INFO());
			}
		}

		// Try testing if the Camera can get Position
		TEST_METHOD(Test_CameraPosition) {
			try {
				Camera camera(vec3(0.0f, 0.0f, 3.0f));
				camera.GetPosition();
			} catch (...) {
				Assert::Fail(L"Error in Camera Position!", LINE_INFO());
			}
		}

		// Try testing if the Camera can get Front
		TEST_METHOD(Test_CameraFront) {
			try {
				Camera camera(vec3(0.0f, 0.0f, 3.0f));
				camera.GetFront();
			} catch (...) {
				Assert::Fail(L"Error in Camera Front!", LINE_INFO());
			}
		}

		// Try testing if the box vertices are correct
		TEST_METHOD(Test_Shader) {
			try {
				GLfloat vertices[] = {
					// Positions            // Normals              // Texture Coords
					-0.25f, -0.25f, -0.25f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
					0.25f, -0.25f, -0.25f,     0.0f,  0.0f, -1.0f,     1.0f,  0.0f,
					0.25f,  0.25f, -0.25f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
					0.25f,  0.25f, -0.25f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
					-0.25f,  0.25f, -0.25f,    0.0f,  0.0f, -1.0f,     0.0f,  1.0f,
					-0.25f, -0.25f, -0.25f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,

					-0.25f, -0.25f,  0.25f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
					0.25f, -0.25f,  0.25f,     0.0f,  0.0f,  1.0f,     1.0f,  0.0f,
					0.25f,  0.25f,  0.25f,     0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
					0.25f,  0.25f,  0.25f,     0.0f,  0.0f,  1.0f,  	1.0f,  1.0f,
					-0.25f,  0.25f,  0.25f,    0.0f,  0.0f,  1.0f,     0.0f,  1.0f,
					-0.25f, -0.25f,  0.25f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,

					-0.25f,  0.25f,  0.25f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,
					-0.25f,  0.25f, -0.25f,    -1.0f,  0.0f,  0.0f,    1.0f,  1.0f,
					-0.25f, -0.25f, -0.25f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
					-0.25f, -0.25f, -0.25f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
					-0.25f, -0.25f,  0.25f,    -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,
					-0.25f,  0.25f,  0.25f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,

					0.25f,  0.25f,  0.25f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
					0.25f,  0.25f, -0.25f,     1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
					0.25f, -0.25f, -0.25f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
					0.25f, -0.25f, -0.25f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
					0.25f, -0.25f,  0.25f,     1.0f,  0.0f,  0.0f,     0.0f,  0.0f,
					0.25f,  0.25f,  0.25f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,

					-0.25f, -0.25f, -0.25f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
					0.25f, -0.25f, -0.25f,     0.0f, -1.0f,  0.0f,     1.0f,  1.0f,
					0.25f, -0.25f,  0.25f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
					0.25f, -0.25f,  0.25f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
					-0.25f, -0.25f,  0.25f,    0.0f, -1.0f,  0.0f,     0.0f,  0.0f,
					-0.25f, -0.25f, -0.25f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,

					-0.25f,  0.25f, -0.25f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,
					0.25f,  0.25f, -0.25f,     0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
					0.25f,  0.25f,  0.25f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
					0.25f,  0.25f,  0.25f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
					-0.25f,  0.25f,  0.25f,    0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
					-0.25f,  0.25f, -0.25f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f
				};
			} catch (...) {
				Assert::Fail(L"Error in Vertices!", LINE_INFO());
			}
		}

	};
}