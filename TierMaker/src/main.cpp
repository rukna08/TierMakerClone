#include <SDL.h>
#include <SDL_image.h>

#include <iostream>
#include <string>
#include <filesystem>
#include <vector>

#include <Windows.h>

namespace fs = std::filesystem;

#define DEBUG 0

int main(int argc, char* argv[]) {
	std::cout << "Tier Maker Launched!\n";

	std::vector<std::string> imagePaths;
	std::string path = "img/";
	for (const auto& entry : fs::directory_iterator(path)) {
        std::cout << entry.path() << std::endl;
		imagePaths.push_back(entry.path().generic_string());
	}

	std::cout << "Image count: " << imagePaths.size() << "\n";

	SDL_Init(SDL_INIT_VIDEO);
	
	SDL_Window* window = SDL_CreateWindow("Tier Maker",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		1280,
		920,
		0);

	SDL_Surface* surface = SDL_GetWindowSurface(window);

	SDL_FillRect(surface, 0, SDL_MapRGBA(surface->format,
		255,
		0,
		255,
		255));

	SDL_UpdateWindowSurface(window);

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	SDL_Texture* bgImage = IMG_LoadTexture(renderer, "BG.png");
	if(bgImage != 0) std::cout << "BG Loaded successfully!\n";

	std::vector<SDL_Texture*> images;
	for(int i = 0; i < imagePaths.size(); i++) {
		images.push_back(IMG_LoadTexture(renderer, imagePaths[i].c_str()));
	}

	std::vector<SDL_Rect> imageRects;
	for(int i = 0; i < imagePaths.size(); i++) {
		SDL_Rect rect;
		rect.x = i*110;
		rect.y = 750;
		rect.w = 100;
		rect.h = 100;

		imageRects.push_back(rect);
	}

	SDL_Rect bgRect;
	bgRect.x = 0;
	bgRect.y = 0;
	bgRect.w = 1280;
	bgRect.h = 720;

	SDL_Point mousePosition;

	SDL_Event e;
	bool quit = false;

	bool followMousePosition = false;
	std::vector<bool> followMousePositions;
	for(int i = 0; i < imagePaths.size(); i++) {
		followMousePositions.push_back(false);
	}

	SDL_Rect slotRects[6][10];
	
	for(int i = 0; i < 6; i++) {
		for(int j = 0; j < 10; j++) {
			
			slotRects[i][j].x = 170 + j * 110;
			slotRects[i][j].y = 10 + i * 120;
			slotRects[i][j].w = 100;
			slotRects[i][j].h = 100;

		}
	}

	while(!quit) {
		while(SDL_PollEvent(&e)) {
			if(e.type == SDL_QUIT) quit = true;
			else if(e.type == SDL_MOUSEBUTTONDOWN) {
				mousePosition.x = e.motion.x;
				mousePosition.y = e.motion.y;

				for(int i = 0; i < imagePaths.size(); i++) {
					if(SDL_PointInRect(&mousePosition, &imageRects[i])) {
						std::cout << "clicked inside image: " + i;
						std::cout << "\n";

						followMousePositions[i] = true;
					}
				}

				for(int i = 0; i < imagePaths.size(); i++) {
					if(SDL_PointInRect(&mousePosition, &imageRects[i])) {
						followMousePositions[i] = true;
					}
				}
			} else if(e.type == SDL_MOUSEBUTTONUP) {
				followMousePosition = false;

				for(int i = 0; i < 6; i++) {
					for(int j = 0; j < 10; j++) {
						
						for(int k = 0; k < imagePaths.size(); k++) {
							if(followMousePositions[k] == true) {
								if(SDL_PointInRect(&mousePosition, &slotRects[i][j])) {
									imageRects[k].x = slotRects[i][j].x;
									imageRects[k].y = slotRects[i][j].y;
									break;
								}
							}
						}

					}
				}

				for(int i = 0; i < imagePaths.size(); i++) {
					followMousePositions[i] = false;
				}
			} else if(e.type == SDL_KEYDOWN) {
				float offset = 100;

				if(e.key.keysym.sym == SDLK_LEFT) {
					for(int i = 0; i < imagePaths.size(); i++) {
						if(imageRects[i].y > 720) {
							imageRects[i].x += offset;
						}
					}
				} else if(e.key.keysym.sym == SDLK_RIGHT) {
					for(int i = 0; i < imagePaths.size(); i++) {
						if(imageRects[i].y > 720) {
							imageRects[i].x -= offset;
						}
					}
				}
			}

			mousePosition.x = e.motion.x;
			mousePosition.y = e.motion.y;
		}

		for(int i = 0; i < imagePaths.size(); i++) {
			if(followMousePositions[i] == true) {
				imageRects[i].x = mousePosition.x - 50;
				imageRects[i].y = mousePosition.y - 50;
			}
		}

		SDL_RenderCopy(renderer, bgImage, 0, &bgRect);

#if DEBUG
		SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);

		for(int i = 0; i < 6; i++) {
			for(int j = 0; j < 10; j++) {

				SDL_RenderDrawRect(renderer, &slotRects[i][j]);
			
			}
		}
#endif

		for(int i = 0; i < imagePaths.size(); i++) {
			SDL_RenderCopy(renderer, images[i], 0, &imageRects[i]);
		}

		SDL_RenderPresent(renderer);

		SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
	
		SDL_RenderClear(renderer);

		Sleep(1);
	}

	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}