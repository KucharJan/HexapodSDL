#include "Hexapod_SDL.h"

#include <chrono>
#include <ctime> 

#define SCREEN_WIDTH  1250
#define SCREEN_HEIGHT 500

int main(int argc, char **argv){
	#pragma region Handle_arguments
	bool offline = 0;
	int demo = 0;
	for (int args = 1; args < argc; args++){
		//Offline
		if(strcmp(argv[args], "--offline") == 0){
			offline = 1;
		}
		//Demo
		if(strcmp(argv[args], "--demo") == 0){
			if(args+1 != argc) {
				demo = atoi(argv[args+1]);
			}
			if(demo < 0 || demo > 4){
				demo = 0;
				std::cout<< "Wrong argument for demo! Defaulting to 0 \n";
			}
		}
	}
	#pragma endregion Handle_region

	#pragma region Init_SDL_TTF
	SDL_Window *window;
	SDL_Renderer *renderer;
	TTF_Font *font;
	SDL_Event ev;
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		std::string buf = "SDL_Init: ";
		buf.append(SDL_GetError());
		logError(std::cout, buf);
		return 1;
	}
	if (TTF_Init() != 0){
		std::string buf = "TTF_Init: ";
		buf.append(TTF_GetError());
		logError(std::cout, buf);
		return 1;
	}
	window = SDL_CreateWindow("Robot view", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == nullptr){
		std::string buf = "SDL_CreateWindow: ";
		buf.append(SDL_GetError());
		logError(std::cout, buf);
		SDL_Quit();
		return 1;
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr){
		std::string buf = "SDL_CreateRenderer: ";
		buf.append(SDL_GetError());
		logError(std::cout, buf);
		cleanup(window);
		SDL_Quit();
		return 1;
	}
	font = TTF_OpenFont("/usr/share/fonts/truetype/freefont/FreeSans.ttf", 20);
	if (font == NULL)
	{
		std::string buf = "TTF_OpenFont: ";
		buf.append(TTF_GetError());
		logError(std::cout, buf);
		cleanup(window, renderer);
		TTF_Quit();
		SDL_Quit();
		return 1;
	}
	#pragma endregion Init_SDL_TTF

	#pragma region Init_robot
	Hexapod hex(!offline, 1);
	if(offline)(std::cout << "Offline demo " << demo << "\n");
	#pragma endregion Init_robot

	#pragma region main_loop
	bool isRunning = true;
	while (isRunning)
	{
		auto start = std::chrono::system_clock::now();

		#pragma region Get_Matrix_Q
		Eigen::Vector4f q[6];
		if(!hex.isOnline()){
			for (int legId = 0; legId < 6; legId++){
				hex.HAL_leg_read(q[legId], legId);
			}
		}
		else
		{
			switch (demo)
			{
				case 0:{
					//This is a demo of 0 position of all servos
					for (int legId = 0; legId < 6; legId++){
						q[legId] = Eigen::Vector4f::Zero(); //define q-space as just zeros
					}
					break;
				}
				case 1:{
					//This is a demo of the home position of the robot
					float off = 0;
					float safetyOff = 0.206;
					float base = 0.512;
					q[0] = { base, base, base+off, base-off};
					q[1] = { base, base - safetyOff, base+off, base-off};
					q[2] = { base, base, base+off, base-off};
					q[3] = { base, base + safetyOff, base+off, base-off};
					q[4] = { base, base, base+off, base-off};
					q[5] = { base, base, base+off, base-off};
					break;
				}
				case 2:{
					uint64_t msSinceEpoch = std::chrono::system_clock::now().time_since_epoch().count();
					uint64_t weirdthing = msSinceEpoch%20000000000;
					double change = sin(((double)weirdthing/10000000000)*180*DEG2RAD);
					float move = (float)change;
					q[0] = {0,0,0,move};
					for (int legId = 1; legId < 6; legId++){
						q[legId] = Eigen::Vector4f::Zero(); //define q-space as just zeros
					}
					break;
				}
				case 3:{
					uint64_t msSinceEpoch = std::chrono::system_clock::now().time_since_epoch().count();
					uint64_t weirdthing = msSinceEpoch%10000000000;
					double change = sin(((double)weirdthing/10000000000)*180*DEG2RAD);
					float move = (float)change;
					for (int legId = 0; legId < 6; legId++){
						q[legId] = {move,move,move,move}; //define q-space as just zeros
					}
					break;
				}
			}
		}
		Eigen::Matrix<float, 6, 4> qMatrix;
		for (int legId = 0; legId < 6; legId++)
		{
			qMatrix.row(legId) = q[legId];
		}
		#pragma endregion Get_Matrix_Q

		draw_main(renderer, hex, font, qMatrix);
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = end-start;
		std::cout << elapsed_seconds.count()*1000 << " ms\n";
		SDL_Delay(50);
		while(SDL_PollEvent(&ev) != 0)
		{
			if(ev.type == SDL_QUIT) { isRunning = false; }
		}
	}
	#pragma endregion main_loop

	#pragma region exit_code
	cleanup(renderer, window, font);
	TTF_Quit();
	SDL_Quit();
	return 0;
	#pragma endregion exit_code
}