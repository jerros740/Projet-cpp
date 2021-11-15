#pragma once

#include "object.h"
#include "declarations.hh"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <vector>
#include <random>
#include <filesystem>

/// ANIMALS ///////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
class animal : public moving_object {
public:
  animal(const std::string& file_path, SDL_Surface* window_surface_ptr);
};

/// SHEEP ////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
class sheep : public animal 
{
private:
  unsigned time_childbirth;
public:
  sheep(SDL_Surface* window_surface_ptr);
  void interaction(const std::shared_ptr<interacting_object>& obj,
                   double distance, ground* g) override;
  void boost(bool left, bool right, bool up, bool down) override;
  void reset_boost(bool left, bool right, bool up, bool down) override;
  void set_time_childbirth(unsigned time) { this->time_childbirth = time; };

  unsigned get_time_childbirth() { return this->time_childbirth; };
};


/// WOLF /////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
  class wolf :  public animal {
  private:
    std::shared_ptr<moving_object> prey; // La cible du loup
    std::shared_ptr<moving_object> dog; // Le chien que doit éviter le loup
    unsigned time_eat;

  public:
    wolf(SDL_Surface* window_surface_ptr);

    void interaction(const std::shared_ptr<interacting_object>& obj, double distance, ground* g) override;
    void find_prey(ground* g) override;
    void find_dog(ground* g) override;
    void move() override;
    void set_prey(std::shared_ptr<moving_object> prey) { this->prey = prey; };
    void set_dog(std::shared_ptr<moving_object> dog) { this->dog = dog; };
    void set_time_eat(unsigned time) { this->time_eat = time; };

    std::shared_ptr<moving_object> get_dog() { return this->dog; };
    std::shared_ptr<moving_object> get_prey() { return this->prey; };
    unsigned get_time_eat() { return this->time_eat; };
    bool isStarving() { return (SDL_GetTicks() - time_eat) > 15000; }
  };

  /// SHEPHERD DOG /////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
  class dog : public animal {
  private:
    double angle;
    double radius;

    std::shared_ptr<moving_object> mov_obj_; // L'objet sur lequel le chient tournera en rond
  public:
    dog(SDL_Surface* window_surface_ptr, std::shared_ptr<moving_object> mov_obj);
    //dog(SDL_Surface* window_surface_ptr,moving_object* p1);
    void move() override;
    double get_angle() { return this->angle; };
    double get_radius() { return this->radius; };
    void set_angle(double angle) { this->angle = angle; };
    void set_radius(double radius) { this->radius = radius; };

    void interaction(const std::shared_ptr<interacting_object>& obj, double distance,  ground* g) override;
  };
