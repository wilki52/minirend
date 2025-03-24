#include <iostream>
#include "tgaimage.h"
#include <cmath>
constexpr TGAColor white = {255, 255, 255, 255};
constexpr TGAColor green = {0, 255, 0, 255};
constexpr TGAColor red = {0, 0, 255, 255};
constexpr TGAColor blue = {255, 128, 64, 255};
constexpr TGAColor yellow  = {  0, 200, 255, 255};

void simple_line(int ax, int ay, int bx, int by, TGAColor color, TGAImage &framebuffer){
	//Creating a line
        //1. using functions
        /*
         * x(t) = a_x+t(b_x-a_x)`
         * y(t) = a_y+t(b_y-a_y)`
         */
        for (float t=0; t<1; t+=0.01){
                int x= ax+t*(bx-ax);
                int y= ay+t*(by-ay);
                framebuffer.set(x, y, color);
        }

}

//calculates t as a function of x : t(x) = (x-ax)/(bx-ax) ; derived from isolating t in our first equation.
//We start at ax and increment until we reach bx, and we use x to calculate t, which we use to calculate our y coordinate.
void line(int ax, int ay, int bx, int by, TGAColor color, TGAImage &framebuffer){
	//bool steep = (abs(bx-ax)<abs(by-ay));
	//STEEP check must happen before the second check. Why? Because transposing the line may make B come before A, so we must swap again.	
	bool steep = (abs(bx-ax)<abs(by-ay));
	if (steep){
                std::swap(ax, ay);
                std::swap(bx, by);
	}
	if (bx < ax){
                std::swap(ax, bx);
                std::swap(ay, by);
        }
	for (int x=ax; x<bx; x++){
		float t = (x-ax)/(float)(bx-ax);
		int y = ay+t*(by-ay);
		std::cout <<"t: " << t << " " << x << ", "<< y << std::endl;
		if (steep){
			framebuffer.set(y, x, color);
		}
		else {
			framebuffer.set(x, y, color);
		}
		
	}

}

int main(int argc, char** argv){
	constexpr int width = 64;
	constexpr int height = 64;
	TGAImage framebuffer(width, height, TGAImage::RGB);

	int ax=7, ay=3;
	int bx=12, by=37;
	int cx=62, cy=53;
	
	int dx=15, dy=3;
	int ex=62, ey=20;	
	framebuffer.set(ax, ay, white);
	framebuffer.set(bx, by, white);
	framebuffer.set(cx, cy, blue);
		
	line(ax, ay, bx, by, blue, framebuffer);
	line(cx, cy, bx, by, green, framebuffer);
	line(ax, ay, cx, cy, red, framebuffer);
	line(cx, cy, ax, ay, yellow, framebuffer);
	
	framebuffer.write_tga_file("framebuffer.tga");


}
