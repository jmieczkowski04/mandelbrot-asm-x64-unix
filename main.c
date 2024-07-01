#include <SDL2/SDL.h>
#include <stdio.h>

#define SCREEN_WIDTH 360
#define SCREEN_HEIGHT 360


#define STANDARD_WIDTH 3.0
#define STANDARD_HEIGHT 2.0


void mandelbrot2(unsigned char* mem, int width, int height, double axis_x_start, double axis_x_len, double axis_y_start, double axis_y_len);

int main(int argc, char** args)
{
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Texture* texture = NULL;
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "could not initialize sdl2: %s\n", SDL_GetError());
        return 1;
    }
    window = SDL_CreateWindow(
			    "ARKO Mandelbrot",
			    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			    SCREEN_WIDTH, SCREEN_HEIGHT,
			    SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN
			    );

    if (window == NULL)
    {
        fprintf(stderr, "could not create window: %s\n", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    char need_rerender = 1;
    unsigned char* texture_mem = malloc(SCREEN_WIDTH * SCREEN_HEIGHT * 4);
    
    SDL_Event e;
    uint8_t run = 1;
    int width = SCREEN_WIDTH;
    int height = SCREEN_HEIGHT;
    double center_x = -0.5;
    double center_y = 0;
    double zoom = 0;

    while (run)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            switch (e.type)
            {
            case SDL_QUIT:
                run = 0;
                need_rerender = 0;
                break;
            
            case SDL_WINDOWEVENT:
                if (e.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    need_rerender = 1;
                    SDL_GetWindowSize(window, &width, &height);
                    free(texture_mem);
                    texture_mem = malloc(width * height * 4);
                    printf("resized\n");
                }
                break;

            case SDL_KEYDOWN:
                switch (e.key.keysym.sym)
                {
                case SDLK_w:
                    zoom += .1f;
                    need_rerender = 1;
                    break;
                case SDLK_s:
                    zoom -= .1f;
                    need_rerender = 1;
                    break;

                case SDLK_UP:
                    center_y -= (1/powf(2, zoom))/10;
                    need_rerender = 1;
                    break;
                case SDLK_DOWN:
                    center_y += (1/powf(2, zoom))/10;
                    need_rerender = 1;
                    break;
                case SDLK_LEFT:
                    center_x -= (1/powf(2, zoom))/10;
                    need_rerender = 1;
                    break;
                case SDLK_RIGHT:
                    center_x += (1/powf(2, zoom))/10;
                    need_rerender = 1;
                    break;
                
                default:
                    break;
                }
                break;

            default:
                break;
            }
        }

        SDL_RenderClear(renderer);
        /* rendering here */
        if (need_rerender)
        {
            need_rerender = 0;
            double start_x = center_x - (STANDARD_WIDTH/powf(2, zoom))/2;
            double start_y = center_y - (STANDARD_HEIGHT/powf(2, zoom))/2;
            mandelbrot2(texture_mem, width, height, start_x, STANDARD_WIDTH/powf(2, zoom), start_y, STANDARD_HEIGHT/powf(2, zoom));
            SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(texture_mem, width, height, 32, width * 4, 0xff000000, 0xff0000, 0xff00, 0xff);
            SDL_DestroyTexture(texture);
            texture = SDL_CreateTextureFromSurface(renderer, surface);

            SDL_FreeSurface(surface);
            printf("updated\n");
        }
        SDL_RenderCopy(renderer, texture, NULL, NULL);

        SDL_RenderPresent(renderer);
    }

    free(texture_mem);

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}



