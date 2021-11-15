#include "player.h"
#include "object.h"


player::player(const std::string& file_path, SDL_Surface* window_surface_ptr) : 
    moving_object(file_path, window_surface_ptr)
{
  set_properties("alive", "true");
  set_properties("role", "player");
  set_properties("object", "player");
};

void player::move()
{
  const Uint8* state = SDL_GetKeyboardState(NULL);

  if (state[SDL_SCANCODE_UP] && !state[SDL_SCANCODE_DOWN] &&
      !state[SDL_SCANCODE_LEFT] && !state[SDL_SCANCODE_RIGHT]) // Haut seulement 
  {
    set_vel_y(-1);
    set_vel_x(0);
  } 

  if (state[SDL_SCANCODE_UP] && !state[SDL_SCANCODE_DOWN] &&
      !state[SDL_SCANCODE_LEFT] && state[SDL_SCANCODE_RIGHT]) // Haut + Droite
  {
    set_vel_y(-0.75);
    set_vel_x(0.75);
  }

  if (state[SDL_SCANCODE_UP] && !state[SDL_SCANCODE_DOWN] &&
      state[SDL_SCANCODE_LEFT] && !state[SDL_SCANCODE_RIGHT]) // Haut + gauche
  {
    set_vel_y(-0.75);
    set_vel_x(-0.75);
  } 

  if (!state[SDL_SCANCODE_UP] && state[SDL_SCANCODE_DOWN] &&
      !state[SDL_SCANCODE_LEFT] && !state[SDL_SCANCODE_RIGHT]) // Bas seulement
  {
    set_vel_y(1);
    set_vel_x(0);
  } 

  if (!state[SDL_SCANCODE_UP] && state[SDL_SCANCODE_DOWN] &&
      !state[SDL_SCANCODE_LEFT] && state[SDL_SCANCODE_RIGHT]) // Bas + Droite
  {
    set_vel_y(0.75);
    set_vel_x(0.75);
  } 

  if (!state[SDL_SCANCODE_UP] && state[SDL_SCANCODE_DOWN] &&
      state[SDL_SCANCODE_LEFT] && !state[SDL_SCANCODE_RIGHT]) // Bas + gauche
  {
    set_vel_y(0.75);
    set_vel_x(-0.75);
  } 

  if (!state[SDL_SCANCODE_UP] && !state[SDL_SCANCODE_DOWN] && !state[SDL_SCANCODE_LEFT] && !state[SDL_SCANCODE_RIGHT]) // Aucun
  {
    set_vel_y(0);
    set_vel_x(0);
  } 

  if (!state[SDL_SCANCODE_UP] && !state[SDL_SCANCODE_DOWN] &&
      state[SDL_SCANCODE_LEFT] &&
      !state[SDL_SCANCODE_RIGHT]) // Gauche seulement
  {
    set_vel_y(0);
    set_vel_x(-1);
  } 

  if (!state[SDL_SCANCODE_UP] && !state[SDL_SCANCODE_DOWN] &&
      !state[SDL_SCANCODE_LEFT] &&
      state[SDL_SCANCODE_RIGHT]) // Droite seulement
  {
    set_vel_y(0);
    set_vel_x(1);
  } 
 
  moving_object::move();
};

//// SHEPHERD
shepherd::shepherd(SDL_Surface* window_surface_ptr) : 
    player("media/shepherd.png", window_surface_ptr) {}
void shepherd::interaction(const std::shared_ptr<interacting_object>& obj, double distance,ground* g){};