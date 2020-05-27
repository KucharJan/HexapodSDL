#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>
#include "Hexapod.h"

template<typename T, typename... Args>
void cleanup(T *t, Args&&... args){
	//Cleanup the first item in the list
	cleanup(t);
	//Recurse to clean up the remaining arguments
	cleanup(std::forward<Args>(args)...);
}
template<>
inline void cleanup<SDL_Window>(SDL_Window *window){
	if (!window){
		return;
	}
	SDL_DestroyWindow(window);
}
template<>
inline void cleanup<SDL_Renderer>(SDL_Renderer *renderer){
	if (!renderer){
		return;
	}
	SDL_DestroyRenderer(renderer);
}
template<>
inline void cleanup<SDL_Texture>(SDL_Texture *texture){
	if (!texture){
		return;
	}
	SDL_DestroyTexture(texture);
}
template<>
inline void cleanup<SDL_Surface>(SDL_Surface *surface){
	if (!surface){
		return;
	}
	SDL_FreeSurface(surface);
}
template<>
inline void cleanup<TTF_Font>(TTF_Font *font){
	if (!font){
		return;
	}
	TTF_CloseFont(font);
}

/**
 * Log an error to the output stream of choice
 * os: The output stream to write the error to
 * msg: The error message to log
 */
void logError(std::ostream &os, const std::string &msg);

/**
 * Draws a circle on a given 3D coordinate, with x y being a position and z color of the point
 * renderer: SDL renderer
 * joint: Vector3f coordinate
 */
void draw_Vector3f_point(SDL_Renderer *renderer, Eigen::Vector3f joint);

/**
 * Draws a line between two given 3D coordinates
 * renderer: SDL renderer
 * joint1: first Vector3f coordinate
 * joint2: second Vector3f coordinate
 */
void draw_2Vector3f_line(SDL_Renderer *renderer, Eigen::Vector3f joint1, Eigen::Vector3f joint2);

/**
 * Computes the coordinates of the joints from given transformation
 * T: Transform matrix
 * c: the vector where the coordinates will be written
 */
void get_XYZ_transform(Eigen::Transform <float, 3, Eigen::Affine> *T, Eigen::Vector3f *j);

/**
 * Draws a leg, given the legId
 * renderer: SDL renderer
 * hex: hexapod class
 * legId: id of the leg
 */
void draw_leg(SDL_Renderer *renderer, Hexapod &hex, int legId, Eigen::Vector4f q);

/*
 * Draws the bones and joints of the robot
 * renderer: SDL_renderer
 * hex: Hexapod class object
 */
void draw_robotView(SDL_Renderer *renderer, Hexapod &hex, Eigen::Matrix<float, 6, 4> q);

/**
 * Displays text on a coordinate with a given font
 * renderer: SDL renderer
 * font: TTF font
 * text: text to display
 * x: x coordinate
 * y: y coordinate
 */
void draw_text(SDL_Renderer *renderer, TTF_Font *font, std::string text, int x, int y);

/*
 * Draws the info of the robot (temps, battery status, etc.)
 * renderer: SDL renderer
 * hex: Hexapod class object
 * font: TTF_Font
 */
void draw_robotInfo(SDL_Renderer *renderer, Hexapod &hex, TTF_Font *font);

/**
 * The main draw function: draws robot view and info
 * renderer: SDL renderer
 * hex: Hexapod class object
 * font: TTF_font
 */
void draw_main(SDL_Renderer *renderer, Hexapod &hex, TTF_Font *font, Eigen::Matrix<float, 6, 4> q);