// SDL_Test.h: Includedatei für Include-Standardsystemdateien
// oder projektspezifische Includedateien.

#pragma once

#include "declarations.hh"
#include "ground.h"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <vector>
#include <random>
#include <filesystem>
#include <any>

// Defintions
constexpr double frame_rate = 60.0; // refresh rate
constexpr double frame_time = 1. / frame_rate;
constexpr unsigned frame_width = 1400; // Width of window in pixel
constexpr unsigned frame_height = 900; // Height of window in pixel
// Minimal distance of animals to the border
// of the screen
constexpr unsigned frame_boundary = 100;
constexpr int frame_object_width = 50; // Largeur de l'objet
constexpr int frame_object_height = 50; // Longueur de l'objet

class interacting_object 
{
private:
  std::map<std::string, std::string> properties; // Propriété de l'objet
  std::map<std::string, bool> direction; // Direction de l'objet
  double stamina; // Endurance

public:
    interacting_object();
    virtual void interaction(const std::shared_ptr<interacting_object>& obj, double distance, ground* g) = 0;
    virtual void find_prey(ground* g);
    virtual void find_dog(ground* g);

    bool isPrey() { return get_properties("role") == "prey"; }
    bool isAlive() { return get_properties("alive") == "true"; }
    bool isMale() { return get_properties("gender") == "male"; }
    bool isSheep() { return get_properties("object") == "sheep"; }
    bool isMom() { return get_properties("mom") == "true"; }
    bool isHunter() { return get_properties("role") == "hunter"; }
    bool isDog() { return get_properties("object") == "dog"; }
    bool isLeft() { return direction["left"]; }
    bool isRight() { return direction["right"]; }
    bool isUp() { return direction["up"]; }
    bool isDown() { return direction["down"]; }
    
    void set_left(bool left) { direction.insert_or_assign("left", left); }
    void set_right(bool right) { direction.insert_or_assign("right", right); }
    void set_up(bool up) { direction.insert_or_assign("up", up); }
    void set_down(bool down) { direction.insert_or_assign("down", down); }
    void set_stamina(double stamina) { this->stamina = stamina; };
       
    std::string get_properties(std::string key) { return properties[key]; }
    void set_properties(std::string key, std::string value) { properties.insert_or_assign(key, value); }
    double get_stamina() { return this->stamina; };
};

class rendered_object :  public interacting_object // Positionnement de l'objet
{
private:
  SDL_Surface* window_surface_ptr_; // Pointeur de la surface sur laquelle on va ajouter nos objets
  SDL_Surface* image_ptr_;  // Pointeur de la texture de l'objet
  double pos_x, pos_y;

public:
  rendered_object(const std::string& file_path,SDL_Surface* window_surface_ptr);
  ~rendered_object();
  void draw(); // Fonction pour placer l'objet dans la fenêtre

  void set_pos_x(double x) { this->pos_x = x; };
  void set_pos_y(double y) { this->pos_y = y; };
  void set_image_surface(SDL_Surface* image_surface) { this->image_ptr_ = image_surface; };
  void set_window_surface(SDL_Surface* window_surface) {this->window_surface_ptr_ = window_surface;};

  double get_pos_x() { return this->pos_x; };
  double get_pos_y() { return this->pos_y; };
  SDL_Surface* get_image_surface() { return image_ptr_; };
  SDL_Surface* get_window_surface() { return window_surface_ptr_; };

  double distance(const std::shared_ptr<rendered_object>& obj); // Distance entre cet objet et un autre objet
};


class moving_object : public rendered_object // Mouvement de l'objet (Vitesse, sens)
{
private: 
    double vel_x, vel_y;

public:
    moving_object(const std::string& file_path,SDL_Surface* window_surface_ptr);
    virtual void move();

    void set_vel_x(double vx);
    void set_vel_y(double vy);

    double get_vel_x() { return this->vel_x; };
    double get_vel_y() { return this->vel_y; };

    virtual void boost(bool left, bool right, bool up,bool down); // Boost de vitesse vers la direction choisi
    virtual void reset_boost(bool left, bool right, bool up, bool down); // Retabli la vitesse dans la même direction
};
