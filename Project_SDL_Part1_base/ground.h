#pragma once

#include "declarations.hh"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>

class ground {
private:
  SDL_Surface* window_surface_ptr_;

  std::vector<std::shared_ptr<moving_object>> all_object_;

public:
  ground() = default;
  ground(SDL_Surface* window_surface_ptr);
  	
  void add_object(const std::shared_ptr<moving_object>& new_animal);
  void update();
  int size() { return all_object_.size(); }
  std::vector<std::shared_ptr<moving_object>> get_moving_object() {return all_object_;};
  int get_score();
};
