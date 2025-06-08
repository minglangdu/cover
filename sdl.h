#include <iostream>
#include <vector>
#include <string>
#include <SDL2/SDL.h>

// taken from my other project, stockship.
// copy-and-pasated to reduce tedium in implementing sdl
namespace SDLH {
    extern bool DID_INIT;
    extern int WINDOWS;
    class Display {
        /*
        Generic display 
        */
        public:
            SDL_Window* window;
            SDL_Renderer* renderer;
            bool quit;

            Display(std::string title, int windowsize);
            ~Display();
            bool update();
            virtual void getInputs(SDL_Event e);
            virtual void renderObjects();
    };
    class SoundPlot : public Display {
        /*
        Based on an array of `short`s, creates a 
        graph of the sound on a window.
        */
        public:
            std::vector<short> vals;
            std::pair<short, short> range;

            SoundPlot(std::string title, std::vector<short> graph);
            ~SoundPlot();
            void change(std::vector<short> graph);
            void renderObjects();
            bool update();
    };
}