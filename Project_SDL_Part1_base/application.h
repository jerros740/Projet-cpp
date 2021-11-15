#include "ground.h"
#include "declarations.hh"
#include "object.h"
#include "player.h"

// Helper function to initialize SDL
void init();

class application {
private:
  SDL_Window* window_ptr_;
  SDL_Surface* image_surface_ptr;
  SDL_Surface* window_surface_ptr_;
  SDL_Event window_event_;

  unsigned score;
  unsigned n_wolf;
  unsigned last_ticks_;
  ground* g;

public:
  application(unsigned n_sheep, unsigned n_wolf);
  ~application();
  int loop(unsigned period);
  void set_score(int score) { this->score = score; };
  unsigned get_score() { return this->score; }
  void set_nwolf(int n_wolf) { this->n_wolf = n_wolf; }
  unsigned get_nwolf() { return this->n_wolf; }
};
