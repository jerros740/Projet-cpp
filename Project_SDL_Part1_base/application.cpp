#include "application.h"
#include "ground.h"
#include "animals.h"
#include "object.h"
#include <SDL.h>
#include <SDL_image.h>

void init() {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) < 0)
    throw std::runtime_error("init():" + std::string(SDL_GetError()));

  // Initialize PNG loading
  int imgFlags = IMG_INIT_PNG;
  if (!(IMG_Init(imgFlags) & imgFlags))
    throw std::runtime_error("init(): SDL_image could not initialize! "
                             "SDL_image Error: " +
                             std::string(IMG_GetError()));
}

application::application(unsigned n_sheep, unsigned n_wolf) : 
    window_ptr_{SDL_CreateWindow("Animal's farm", SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED, frame_width,frame_height, 0)}, // Création de la fenêtre
    window_surface_ptr_(SDL_GetWindowSurface(this->window_ptr_)), // Récupération de la surface de la fenêtre
    image_surface_ptr{IMG_Load("media/grass.png")}, // Le fond d'écran de la fenêtre
    score{0}, // Initialisation du score
    n_wolf{n_wolf}, 
    g{new ground(window_surface_ptr_)} // Création de ground
{
  // Vérification qu'on a bien l'image
  if (!this->image_surface_ptr) {
    printf("IMG_Load: %s\n", IMG_GetError());
  }

  // Met en fond l'image
  SDL_BlitScaled(this->image_surface_ptr, NULL, this->window_surface_ptr_,NULL);

  for (unsigned i = 0; i < n_sheep; ++i)
    g->add_object(std::make_shared<sheep>(window_surface_ptr_));
  
  for (unsigned i = 0; i < n_wolf; ++i)
    g->add_object(std::make_shared<wolf>(window_surface_ptr_));

  std::shared_ptr<shepherd> player_1 = std::make_shared<shepherd>(window_surface_ptr_);

  // Shepherd + shepherd dog
  g->add_object(std::make_shared<dog>(window_surface_ptr_, player_1));
  g->add_object(player_1);
  
};

application::~application()
{
  delete g;
  SDL_FreeSurface(window_surface_ptr_); 
  SDL_FreeSurface(image_surface_ptr);   
  SDL_DestroyWindow(window_ptr_);       
  window_surface_ptr_ = nullptr;        
  window_ptr_ = nullptr;                
  image_surface_ptr = nullptr;          
};

int application::loop(unsigned period) {

  unsigned start_ticks = SDL_GetTicks();
  last_ticks_ = SDL_GetTicks();
  bool keep_window_open = true;
  while (keep_window_open && ((SDL_GetTicks() - start_ticks) < period * 1000)) {
      
    while (SDL_PollEvent(&window_event_) > 0) {
      switch (window_event_.type) {
      case SDL_QUIT:
        keep_window_open = false;
        std::cout << "Score : " << g->get_score() << std::endl;
        break;
      }
    }
    // Refresh de la fenêtre
    SDL_BlitScaled(this->image_surface_ptr, NULL, this->window_surface_ptr_,
                   NULL);

    // Mise à jour des déplacements des objet
    g->update();

    // Mise à jour de la fenêtre
    SDL_UpdateWindowSurface(this->window_ptr_);
  }
  std::cout << "Score : " << g->get_score() << std::endl;
  SDL_DestroyWindow(this->window_ptr_);
  SDL_Quit();
  return 0;
};


