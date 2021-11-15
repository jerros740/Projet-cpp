
#include "object.h"
#include "animals.h"
#include "ground.h"
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <numeric>
#include <random>
#include <string>
#include <type_traits>

namespace {
// Defining a namespace without a name -> Anonymous workspace
// Its purpose is to indicate to the compiler that everything
// inside of it is UNIQUELY used within this source file.


SDL_Surface* load_surface_for(const std::string& path,
                              SDL_Surface* window_surface_ptr) {
  // The final optimized image
  SDL_Surface* optimizedSurface = NULL;

  // Load image at specified path
  SDL_Surface* loadedSurface = IMG_Load(path.c_str());
  if (!loadedSurface)
    throw std::runtime_error("load_surface_for() :"
                             "Unable to load image " +
                             path + ".\n Error: " + IMG_GetError());

  // Convert surface to screen format
  optimizedSurface =
      SDL_ConvertSurface(loadedSurface, window_surface_ptr->format, 0);
  //  optimizedSurface = SDL_ConvertSurfaceFormat(loadedSurface,
  //  SDL_GetWindowPixelFormat(window_ptr), 0);
  if (!optimizedSurface)
    throw std::runtime_error("load_surface_for(): Unable to optimize image," +
                             std::string(SDL_GetError()));

  // Get rid of old loaded surface
  SDL_FreeSurface(loadedSurface);

  return optimizedSurface;
}
} // namespace



///// INTERACTING OBJECT
interacting_object::interacting_object() 
{ 
};

void interacting_object::interaction(const std::shared_ptr<interacting_object>& obj, double distance, ground* g){};
void interacting_object::find_prey(ground* g) {}
void interacting_object::find_dog(ground* g) {}
    
///// RENDERED_OBJECT
rendered_object::rendered_object(const std::string& file_path,SDL_Surface* window_surface_ptr): 
    pos_x(frame_boundary + (rand() % (frame_width - 2 * frame_boundary))),
    pos_y(frame_boundary + (rand() % (frame_height - 2 * frame_boundary))) 
{
    this->window_surface_ptr_ = window_surface_ptr;
    this->image_ptr_ = load_surface_for(file_path, window_surface_ptr);
};

rendered_object::~rendered_object() {
  SDL_FreeSurface(image_ptr_);
  image_ptr_ = nullptr;
};

void rendered_object::draw()
{
  SDL_Rect rect;
  rect.h = frame_object_height;
  rect.w = frame_object_width;
  rect.x = get_pos_x();
  rect.y = get_pos_y();

  // Met en l'image sur le fond
  SDL_BlitScaled(this->image_ptr_, NULL, this->window_surface_ptr_, &rect);
  
};

double rendered_object::distance(const std::shared_ptr<rendered_object>& obj) {
  double x1 = this->get_pos_x();
  double y1 = this->get_pos_y();

  double x2 = obj->get_pos_x();
  double y2 = obj->get_pos_y();

  return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0);
};

///// MOVING_OBJECT
moving_object::moving_object(const std::string& file_path, SDL_Surface* window_surface_ptr) : 
    rendered_object(file_path, window_surface_ptr), vel_x(0), vel_y(0){};

void moving_object::set_vel_x(double vx) 
{ 
    this->vel_x = vx; 
    set_left(vx < 0);
    set_right(vx > 0);
};
void moving_object::set_vel_y(double vy) 
{ 
    this->vel_y = vy; 
    set_up(vy < 0);
    set_down(vy > 0);
}

void moving_object::boost(bool left, bool right, bool up, bool down){};
void moving_object::reset_boost(bool left, bool right, bool up, bool down){};

void moving_object::move()
{ 
    double new_pos_y = get_pos_y() + get_vel_y();
    double new_pos_x = get_pos_x() + get_vel_x();
    
    if (new_pos_x < (frame_width - frame_boundary) && new_pos_x > frame_boundary) 
    {
      set_pos_x(new_pos_x);
    }
    else 
    {
      set_vel_x(-get_vel_x()); // Changement de direction si on touche la bordure de droite ou de gauche
    }

    if (new_pos_y < (frame_height - frame_boundary) && new_pos_y > frame_boundary) 
    {
      set_pos_y(new_pos_y);
    } 
    else 
    {
      set_vel_y(-get_vel_y()); // Changement de direction si on touche la bordure du haut ou du bas
    }
}


