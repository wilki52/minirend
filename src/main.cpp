#include <iostream>
#include <fstream>
#include "tgaimage.h"
#include <cmath>
#include "string_util.h"
#include <vector>
#include <map>
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
	float y = ay;
	for (int x=ax; x<bx; x++){
		float t = (x-ax)/(float)(bx-ax);
		//int y = ay+t*(by-ay);
		//std::cout <<"t: " << t << " " << x << ", "<< y << std::endl;
		if (steep){
			framebuffer.set(y, x, color);
		}
		else {
			framebuffer.set(x, y, color);
		}
		y+=(by-ay)/(float)(bx-ax); //OPTIMIZATION 1 : works because we nolonger have to calculate t. now just a simple division addition.
		
	}

}

int main(int argc, char** argv){
	constexpr int width = 1024;
	constexpr int height = 1024;
	TGAImage framebuffer(width, height, TGAImage::RGB);

	int ax=7, ay=3;
	int bx=12, by=37;
//	int cx=62, cy=53;
//	
//	int dx=15, dy=3;
//	int ex=62, ey=20;	
//	framebuffer.set(ax, ay, white);
//	framebuffer.set(bx, by, white);
//	framebuffer.set(cx, cy, blue);
//		
	
//`	line(cx, cy, bx, by, green, framebuffer);
//	line(ax, ay, cx, cy, red, framebuffer);
//	line(cx, cy, ax, ay, yellow, framebuffer);
//
//	for (int i =0; i<1000000; i++){
//		int x1= rand()%width, y1=rand()%height;
//		int x2= rand()&width, y2=rand()%height;
//		line(x1, y1, x2, y2, {(uint8_t)rand()%256, (uint8_t)rand()%256,(uint8_t) rand()%256, (uint8_t)rand()%256}, framebuffer);
//	}	
	
	std::string file_line;
	std::ifstream wavefile("../obj/wireframe.obj");
	int v_index = 0;
	std::map<int, std::vector<int>> v_map;
	while (std::getline(wavefile, file_line)){
		std::vector<std::string> vec = split(file_line," ");
		if(vec.at(0)=="v"){
			//for (int i=0; i<vec.size(); i++){
                        //        std::cout<< i << ": " << vec.at(i) << std::endl;
                        //}
              		//check
			if (vec.size()<4) continue;
			int x = static_cast<int>((std::stof(vec[1])+1.0)*width/2);
			int y= static_cast<int>((std::stof(vec[2])+1.0)*height/2);
		
			v_map[v_index] = std::vector<int>{x, y};
		
			framebuffer.set(x,y, white);		
		}
		else if (vec.at(0)==("f")){
			//grab first number from each triplet, put into v_indices
			std::vector<int> v_indices;
			std::cout << "face found: "<< vec[1] << std::endl;
			for (int i =1; i<vec.size(); i++){
				std::vector<std::string> indices = split(vec[i], "/");
				//std::cout << vec[1] << std::endl;
				std::cout << indices[0] << std::endl;
				v_indices.push_back(std::stoi(indices[0])-1);
				std::cout << "after" << std::endl;
			}	
			//for each number in v_indices, create a line to the next one.
			std::cout << "Now creating lines" << std::endl;
			for (int i=1; i< v_indices.size(); i++){
				//std::cout << v_indices[i-1] << std::endl;
				std::vector<int> point_1 = v_map[v_indices[i-1]];
				std::vector<int> point_2 = v_map[v_indices[i]];
				//std::cout << point_1[0] << std::endl;
				std::cout << typeid(point_1[0]).name() << " " << point_1[1]<< " "  << point_2[0] << " " << point_2[1] << std::endl;	
				std::cout << point_1[0] << " " << point_1[1]<< " "  << point_2[0] << " " << point_2[1] << std::endl;	
				line(point_1[0], point_1[1], point_2[0], point_2[1], red, framebuffer);

			}
			std::vector<int> last = v_map[v_indices.back()];
			std::vector<int> first = v_map[v_indices[0]];		
			line(last[0], last[1], first[0], first[1], red, framebuffer);
				//on the last one, make it connect ot the first.
			//
		}
		v_index ++;
	}

	wavefile.close();
	
	framebuffer.write_tga_file("framebuffer.tga");


}
