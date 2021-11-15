#include "object.h"
#include "declarations.hh"

class player : public moving_object 
{
public:
  player(const std::string& file_path, SDL_Surface* window_surface_ptr);
  void move() override;
};


/// SHEPHERD
class shepherd : public player 
{
public:
  shepherd(SDL_Surface* window_surface_ptr);
  void interaction(const std::shared_ptr<interacting_object>& obj,double distance, ground* g) override;
};