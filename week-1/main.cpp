#include "SDL.h"
#include "ResourcePath.h"

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#include <memory>
using namespace std;

class Graphics
{
private:
    
    SDL_Window* _window;
    
public:
    
    Graphics(SDL_Window* window)
    {
        _window = window;
    }
    
    void update()
    {
        glClearColor(rand() % 255 / 255.0f, rand() % 255 / 255.0f, rand() % 255 / 255.0f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        SDL_GL_SwapWindow(_window);
    }
};

void UpdateFrame(void* param)
{
    Graphics* graphics = (Graphics*)param;
    graphics->update();
}

int EventFilter(void* userdata, SDL_Event* event)
{
    switch (event->type)
    {
        case SDL_FINGERMOTION:
            SDL_Log("Finger Motion");
            return 0;
            
        case SDL_FINGERDOWN:
            SDL_Log("Finger Down");
            return 0;
            
        case SDL_FINGERUP:
            SDL_Log("Finger Up");
            return 0;
    }
    
    return 1;
}


int main(int argc, char *argv[])
{
    /* initialize SDL */
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Could not initialize SDL\n");
        return 1;
    }
    
    SDL_DisplayMode displayMode;
    SDL_GetDesktopDisplayMode(0, &displayMode);
    
    /* create window and renderer */
    SDL_Window* window = SDL_CreateWindow(NULL, 0, 0, displayMode.w, displayMode.h, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN | SDL_WINDOW_RESIZABLE);
    if (!window) {
        printf("Could not initialize Window\n");
        return 1;
    }
    
    auto gl = SDL_GL_CreateContext(window);
    
    unique_ptr<Graphics> graphics = unique_ptr<Graphics>(new Graphics(window));
    
    SDL_iPhoneSetAnimationCallback(window, 1, UpdateFrame, graphics.get());
    SDL_AddEventWatch(EventFilter, NULL);
    
    SDL_Log("My resource path is %s", getResourcePath().c_str());
    
    //Game Loop
    SDL_Event event;
    auto done = false;
    while (!done)
    {
        SDL_PumpEvents();
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    done = true;
                    break;
                    
                case SDL_APP_DIDENTERFOREGROUND:
                    SDL_Log("SDL_APP_DIDENTERFOREGROUND");
                    break;
                    
                case SDL_APP_DIDENTERBACKGROUND:
                    SDL_Log("SDL_APP_DIDENTERBACKGROUND");
                    break;
                    
                case SDL_APP_LOWMEMORY:
                    SDL_Log("SDL_APP_LOWMEMORY");
                    break;
                    
                case SDL_APP_TERMINATING:
                    SDL_Log("SDL_APP_TERMINATING");
                    break;
                    
                case SDL_APP_WILLENTERBACKGROUND:
                    SDL_Log("SDL_APP_WILLENTERBACKGROUND");
                    break;
                    
                case SDL_APP_WILLENTERFOREGROUND:
                    SDL_Log("SDL_APP_WILLENTERFOREGROUND");
                    break;
                    
                case SDL_WINDOWEVENT:
                {
                    switch (event.window.event)
                    {
                        case SDL_WINDOWEVENT_RESIZED:
                        {
                            SDL_Log("Window %d resized to %dx%d", event.window.windowID, event.window.data1, event.window.data2);
                            
                            break;
                        }
                    }
                }
            }
        }
    }
    
    SDL_GL_DeleteContext(gl);
    
    // Done! Close the window, clean-up and exit the program.
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}
