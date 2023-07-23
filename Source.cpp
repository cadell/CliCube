#include <iostream>
#include <array>
#include <vector>
#include <cmath>
#include <algorithm>
#include <thread>
//#include <unistd.h>

struct vec3d
{
	float x, y, z;
};

struct triangle
{
	vec3d p[3];
};

struct mesh
{
	std::vector<triangle> tris;
};

struct mat4x4
{
	float m[4][4] = { 0 };
};



std::vector<std::vector<char>> CreateWindow(unsigned int width, unsigned int hight)
{
	std::vector<std::vector<char>> window;
	return window;
}


class Window
{

private:
	unsigned int width_;
	unsigned int height_;

	unsigned int width_half;
	unsigned int height_half;



	void plotLineLow(int x0, int y0, int x1, int y1, char fill = '+');
	void plotLineHigh(int x0, int y0, int x1, int y1, char fill = '+');

	void MultiplyMatrixVector(vec3d& i, vec3d& o, mat4x4& m)
	{
		o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
		o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
		o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
		float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

		if (w != 0.0f)
		{
			o.x /= w; 
			o.y /= w; 
			o.z /= w;
		}

	}



public:
	std::vector<std::vector<char>> window_buffer;
	mesh meshCube;
	mat4x4 matProj;
	float fTheta;

	template<unsigned int width, unsigned int height>
	void CreateWindow()
	{
		width_ = width;
		height_ = height;

		width_half = width_ / 2;
		height_half = height_ / 2;

		// Initialize 2D Buffer
		//std::vector<char> h_(height, " ");
		window_buffer = std::vector<std::vector<char>>(width, std::vector<char>(height, ' '));

		meshCube.tris = {

			// SOUTH
			{ 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },

			// EAST                                                      
			{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
			{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

			// NORTH                                                     
			{ 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
			{ 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

			// WEST                                                      
			{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

			// TOP                                                       
			{ 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
			{ 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

			// BOTTOM                                                    
			{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
			{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },

		};

		// Projection Matrix
		float fNear = 0.1f;
		float fFar = 1000.0f;
		float fFov = 90.0f;
		float fAspectRatio = (float)height_ / (float)width_;
		float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

		matProj.m[0][0] = fAspectRatio * fFovRad;
		matProj.m[1][1] = fFovRad;
		matProj.m[2][2] = fFar / (fFar - fNear);
		matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
		matProj.m[2][3] = 1.0f;
		matProj.m[3][3] = 0.0f;
	}
	
	// TODO::Operator overloading for [] and = for setter and getter
	Window()
	{

	}


	void DrawLine(int x0, int y0, int x1, int y1, char fill = '+');
	void DrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2);
	void ClearBuffer() { window_buffer = std::vector<std::vector<char>>(width_, std::vector<char>(height_, ' ')); };
	void DisplayWindow();
	void Ontest();
};

using namespace std::chrono_literals;

void Window::Ontest()
{

	// Set up rotation matrices

	for (int time =0; time < 1000; time++)
	{
		mat4x4 matRotZ, matRotX;
		fTheta += 1.0f *  (time % 180) * ( 3.1415 / 180.0);// *0.5;// fElapsedTime;

		// Rotation Z
		matRotZ.m[0][0] = cosf(fTheta);
		matRotZ.m[0][1] = sinf(fTheta);
		matRotZ.m[1][0] = -sinf(fTheta);
		matRotZ.m[1][1] = cosf(fTheta);
		matRotZ.m[2][2] = 1;
		matRotZ.m[3][3] = 1;

		// Rotation X
		matRotX.m[0][0] = 1;
		matRotX.m[1][1] = cosf(fTheta * 0.5f);
		matRotX.m[1][2] = sinf(fTheta * 0.5f);
		matRotX.m[2][1] = -sinf(fTheta * 0.5f);
		matRotX.m[2][2] = cosf(fTheta * 0.5f);
		matRotX.m[3][3] = 1;

		// Draw Triangles
		for (auto tri : meshCube.tris)
		{
			triangle triProjected, triTranslated, triRotatedZ, triRotatedZX;

			// Rotate in Z-Axis
			MultiplyMatrixVector(tri.p[0], triRotatedZ.p[0], matRotZ);
			MultiplyMatrixVector(tri.p[1], triRotatedZ.p[1], matRotZ);
			MultiplyMatrixVector(tri.p[2], triRotatedZ.p[2], matRotZ);

			// Rotate in X-Axis
			MultiplyMatrixVector(triRotatedZ.p[0], triRotatedZX.p[0], matRotX);
			MultiplyMatrixVector(triRotatedZ.p[1], triRotatedZX.p[1], matRotX);
			MultiplyMatrixVector(triRotatedZ.p[2], triRotatedZX.p[2], matRotX);

#if 1
			// Offset into the screen
			triTranslated = triRotatedZX;
			triTranslated.p[0].z = triRotatedZX.p[0].z + 3.0f;
			triTranslated.p[1].z = triRotatedZX.p[1].z + 3.0f;
			triTranslated.p[2].z = triRotatedZX.p[2].z + 3.0f;

			// Project triangles from 3D --> 2D
			MultiplyMatrixVector(triTranslated.p[0], triProjected.p[0], matProj);
			MultiplyMatrixVector(triTranslated.p[1], triProjected.p[1], matProj);
			MultiplyMatrixVector(triTranslated.p[2], triProjected.p[2], matProj);
#endif



			//// Scale into view
			triProjected.p[0].x += 1.0f; triProjected.p[0].y += 1.0f;
			triProjected.p[1].x += 1.0f; triProjected.p[1].y += 1.0f;
			triProjected.p[2].x += 1.0f; triProjected.p[2].y += 1.0f;
			triProjected.p[0].x *= 0.5f * (float)width_;
			triProjected.p[0].y *= 0.5f * (float)height_;
			triProjected.p[1].x *= 0.5f * (float)width_;
			triProjected.p[1].y *= 0.5f * (float)height_;
			triProjected.p[2].x *= 0.5f * (float)width_;
			triProjected.p[2].y *= 0.5f * (float)height_;

			// Rasterize triangle
			DrawTriangle(triProjected.p[0].x, triProjected.p[0].y,
				triProjected.p[1].x, triProjected.p[1].y,
				triProjected.p[2].x, triProjected.p[2].y);

		}

		DisplayWindow();
		//std::this_thread::sleep_for(100ms);
		system("cls");
		ClearBuffer();
	}


}

void Window::plotLineHigh(int x0, int y0, int x1, int y1, char fill)
{
	auto dx = x1 - x0;
	auto dy = y1 - y0;
	auto xi = 1;

	if (dx < 0)
	{
		xi = -1;
		dx = -dx;
	}
	auto D = (2 * dx) - dy;
	auto x = x0;

	for (int y = y0; y < (y1); y++)
	{
		//if (abs(x) >= width_half || abs(y) >= height_half) [[unlikely]]
			//break;

		//window_buffer[width_half + x][height_half + y] = fill;
		window_buffer[x][y] = fill;
		//std::cout << x << " " << y << std::endl;

		if (D > 0)
		{
			x += xi;
			D = D + (2 * (dx - dy));
		}
		else
			D = D + 2 * dx;

	}

}


void Window::plotLineLow(int x0, int y0, int x1, int y1, char fill)
{
	auto dx = x1 - x0;
	auto dy = y1 - y0;
	int yi = 1;
	
	if (dy < 0)
	{
		yi = -1;
		dy = -dy;
	}
	auto D = (2 * dy) - dx;
	auto y = y0;

	for (int x = x0; x < (x1); x++)
	{
		//if (abs(x) >= width_half || abs(y) >= height_half) [[unlikely]]
			//break;

		//window_buffer[width_half + x][height_half + y] = fill;
		window_buffer[x][y] = fill;
		//std::cout << x << " " << y << std::endl;
		if (D > 0)
		{
			y += yi;
			D = D + (2 * (dy - dx));
		}
		else
			D = D + 2 * dy;
	}
}


//*******************************************************************************
// To add data to the buffer we may want to do ((width/2) + x)   and ((height/2) + y) 
// where x or y could be either positive or negative
// the abs x and abs y must be smaller than width/2 and height/2 respectively 
//*******************************************************************************
void Window::DrawLine(int x0, int y0, int x1, int y1, char fill)
{
#if 0
	auto delta_x = x1 - x0;
	auto delta_y = y1 - y0;
	auto D = 2 * delta_y - delta_x;
	auto y = y0;

	for (int x = x0; x < (x1); x++)
	{
		if (abs(x) > width_half || abs(y) > height_half) [[unlikely]]
			break;
		
		window_buffer[width_half + x][height_half + y] = fill;
		std::cout << x << " " << y << std::endl;
		if (D > 0)
		{
			y += 1;
			D = D - 2 * delta_x;
		}
		D = D + 2 * delta_y;
	}
#endif

	if (abs(y1 - y0) < abs(x1 - x0))
	{
		if (x0 > x1)
		{
			plotLineLow(x1, y1, x0, y0, fill);
		}
		else
		{
			plotLineLow(x0, y0, x1, y1, fill);
		}
	}
	else
	{
		if (y0 > y1)
		{
			plotLineHigh(x1, y1, x0, y0, fill);
		}
		else
		{
			plotLineHigh(x0, y0, x1, y1, fill);
		}
	}

}

void Window::DrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2)
{
	DrawLine(x0, y0, x1, y1, '-');
	DrawLine(x1, y1, x2, y2, '-');
	DrawLine(x2, y2, x0, y0, '-');

}

//***********************************************************************
// We can't just iterate over memory like we normally do, since we write 
// to memory sequentially we need to start at end and work our way up
//***********************************************************************
void Window::DisplayWindow()
{
	for (int i = 0; i < width_; i++)
	{
		std::for_each(window_buffer[i].rbegin(), window_buffer[i].rend(), [](const char n) { std::cout << n << " "; });
		std::cout << std::endl;
	}
}

int main ()
{
	//std::array< std::array<char, 512>, 512> windowHandle{};
	//std::array<char, 512> arr{};
	//arr.fill(' ');
	//windowHandle.fill(arr);
	//DrawLine(0,10,5,15,windowHandle);
	//DisplayWindow(windowHandle);





	Window window;
	window.CreateWindow<86, 86>();
	//window.DrawTriangle(0, 10, 5, 15,5,5);

	//while(true)
	//{
	//	//window.fTheta += 1 % 360;
	//	for (auto tri : window.meshCube.tris)
	//	{
	//		window.DrawTriangle(tri.p[0].x, tri.p[0].y,
	//			tri.p[1].x, tri.p[1].y,
	//			tri.p[2].x, tri.p[2].y);
	//	}
	//	window.DisplayWindow();
	//	std::this_thread::sleep_for(100ms);
	//	system("cls");
	//}

	window.Ontest();

	
}