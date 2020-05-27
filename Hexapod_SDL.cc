#include "Hexapod_SDL.h"
#define SCALE (float)0.5

void logError(std::ostream &os, const std::string &msg){
	os << "Error at " << msg << "\n";
}

void draw_Vector3f_point(SDL_Renderer *renderer, Eigen::Vector3f joint){
	SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
	
	SDL_Rect rectangleXY = {(int)(joint[0]*SCALE-2+250),(int)(joint[1]*(-1)*SCALE+250-2),5,5};
	SDL_Rect rectangleXZ = {(int)(joint[0]*SCALE-2+750),(int)(joint[2]*(-1)*SCALE+250-2),5,5};
	SDL_RenderFillRect(renderer, &rectangleXY);
	SDL_RenderFillRect(renderer, &rectangleXZ);
	
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
}

void draw_2Vector3f_line(SDL_Renderer *renderer, Eigen::Vector3f joint1, Eigen::Vector3f joint2){
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	
	
	SDL_RenderDrawLine(renderer, joint1[0]*SCALE + 250, joint1[1]*(-1)*SCALE + 250, joint2[0]*SCALE + 250, joint2[1]*(-1)*SCALE + 250);
	SDL_RenderDrawLine(renderer, joint1[0]*SCALE + 750, joint1[2]*(-1)*SCALE + 250, joint2[0]*SCALE + 750, joint2[2]*(-1)*SCALE + 250);
	
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
}

void get_XYZ_transform(Eigen::Transform <float, 3, Eigen::Affine> *T, Eigen::Vector3f *j){
	*j << *T * Eigen::Vector3f::Zero();
}

void draw_leg(SDL_Renderer *renderer, Hexapod &hex, int legId, Eigen::Vector4f q){
	//DKT from center to base and all joints
	Eigen::Matrix<float, 5, 3> joints;
	for (int joint = 0; joint < 5; joint++)
	{
		Eigen::Transform <float, 3, Eigen::Affine> T;
		Eigen::Vector3f c;
		hex.leg_DKT(q, legId, &T, NULL, joint);
		get_XYZ_transform(&T, &c);
		joints.row(joint) = c;
	}
	Eigen::Vector3f center;
	center = Eigen::Vector3f::Zero();
	//draw the joints
	draw_Vector3f_point(renderer, center);
	for(int joint = 0; joint < 5; joint++){
		draw_Vector3f_point(renderer, joints.row(joint));
		if (joint == 0) { draw_2Vector3f_line(renderer, center, joints.row(joint)); }
		else { draw_2Vector3f_line(renderer, joints.row(joint-1), joints.row(joint)); }
	}
}

void draw_robotView(SDL_Renderer *renderer, Hexapod &hex, Eigen::Matrix<float, 6, 4> q){
	for (int legId = 0; legId < 6; legId++){
		draw_leg(renderer, hex, legId, q.row(legId));
	}
}

void draw_text(SDL_Renderer *renderer, TTF_Font *font, std::string text, int x, int y){
	SDL_Surface *textSurf  = TTF_RenderText_Solid(font, text.c_str(), {255, 255, 255});
	SDL_Texture *textTex = SDL_CreateTextureFromSurface(renderer, textSurf);
	SDL_Rect textRect = { x, y, textSurf->w, textSurf->h};
	SDL_RenderCopy(renderer, textTex, NULL, &textRect);
	cleanup(textSurf, textTex);
}

void draw_robotInfo(SDL_Renderer *renderer, Hexapod &hex, TTF_Font *font){
	//labels for user
	draw_text(renderer, font, "XY plane", 0, 0);
	draw_text(renderer, font, "XZ plane", 500, 0);
	draw_text(renderer, font, "Joint temperatures", 1000, 0);
	//temps
	Eigen::Matrix<float, 24, 1> tempW;
	hex.HAL_readTemperature(tempW);
	for (int i = 0; i < 24; i++)
	{
		draw_text(renderer, font, std::to_string(tempW[i]).c_str(), 1000, ((i+1)*TTF_FontHeight(font)));
	}
}

void draw_main(SDL_Renderer *renderer, Hexapod &hex, TTF_Font *font, Eigen::Matrix<float, 6, 4> q){
	SDL_RenderClear(renderer);
	
	draw_robotView(renderer, hex, q);
	draw_robotInfo(renderer, hex, font);

	SDL_RenderPresent(renderer);
}