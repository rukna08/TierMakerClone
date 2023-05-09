#include <SDL.h>
#include <SDL_image.h>

#include <iostream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

#define DEBUG 1

int main(int argc, char* argv[]) {
	std::cout << "Tier Maker Launched!\n";

	std::string path = "img/";
	for (const auto& entry : fs::directory_iterator(path)) {
        std::cout << entry.path() << std::endl;
	}

	SDL_Init(SDL_INIT_VIDEO);
	
	SDL_Window* window = SDL_CreateWindow("Tier Maker",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		960,
		540,
		0);

	SDL_Surface* surface = SDL_GetWindowSurface(window);

	SDL_FillRect(surface, 0, SDL_MapRGBA(surface->format,
		255,
		0,
		255,
		255));

	SDL_UpdateWindowSurface(window);

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	SDL_Texture* image = IMG_LoadTexture(renderer, "img/berry.jpg");
		
	if(image != 0) {
		std::cout << "Apple.jpg loaded successfully!\n";
	} else if(image == 0){
		std::cout << "Apple.jpg failed to load!\n";
	}

	int w, h;
	SDL_QueryTexture(image, 0, 0, &w, &h);
	SDL_Rect imageRect;
	imageRect.x = 200;
	imageRect.y = 200;
	imageRect.w = 130;
	imageRect.h = 100;

	SDL_Event e;
	bool quit = false;

	while(!quit) {
		while(SDL_PollEvent(&e)) {
			if(e.type == SDL_QUIT) quit = true;
		}

		SDL_RenderClear(renderer);

		SDL_RenderCopy(renderer, image, 0, &imageRect);

		SDL_RenderPresent(renderer);

		//SDL_SetRenderDrawColor(renderer, 40, 40, 40, 40);

		//SDL_RenderClear(renderer);
	}

	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}