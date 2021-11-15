#include "object.h"
#include "animals.h"
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <numeric>
#include <random>
#include <string>
#include <type_traits>

///// ANIMAL ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
animal::animal(const std::string& file_path, SDL_Surface* window_surface_ptr)
    : moving_object(file_path, window_surface_ptr) {
  // Valeur aléatoire
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<float> dist(0.0, 1.0);

  if (dist(mt) < 0.5) 
  {
    set_properties("gender", "male");
  } else 
  {
    set_properties("gender", "female");
  }
  
  set_properties("alive", "true");
  set_properties("mom", "false");
  
};


///// SHEEP ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
sheep::sheep(SDL_Surface* window_surface_ptr)
    : animal("media/sheep.png", window_surface_ptr) , 
    time_childbirth{0} 
{
  // Valeur aléatoire
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<double> dist(0.35, 0.5);

  set_vel_x(dist(mt));
  set_vel_y(dist(mt));
  set_stamina(100.0);

  set_properties("object", "sheep");
  set_properties("role", "prey");
}

void sheep::interaction(const std::shared_ptr<interacting_object>& obj, double distance, ground* g) {
  
  // Interaction entre 2 moutons
  if (obj->isSheep() && distance < 75 && distance != 0)
  {
    if (obj->isMale() && !this->isMale() ) 
    {
      unsigned time_now = SDL_GetTicks();
      if (!this->isMom() || (this->isMom() && (time_now - this->get_time_childbirth()) > 20000)) 
      {
        // Position de la mère
        double mom_pos_x = this->get_pos_x();
        double mom_pos_y = this->get_pos_y();

        // Position de l'enfant en dessous de la mère à la naissance
        auto s1 = std::make_shared<sheep>(get_window_surface());
        s1->set_pos_x(mom_pos_x);
        s1->set_pos_y(mom_pos_y + 25);

        // Ajout dans ground
        g->add_object(s1);
        this->set_properties("mom", "true");
        set_time_childbirth(time_now);
      }
    }
  }
  
  // Interaction entre 1 mouton et 1 prédateur
  if (obj->isHunter() && distance < 125 &&  distance != 0 && get_stamina() == 100.0)
  {
    // Boost de la vitesse tant qu'on a de la stamina dans la même direction que le prédateur
    boost(obj->isLeft(), obj->isRight(), obj->isUp(), obj->isDown());
    set_stamina(0);
  }
  
  if (obj->isHunter() && distance > 200 && distance != 0 && get_stamina() < 100) 
  {
    // Reset de la vitesse quand on est loin du prédateur dans la même direction
    reset_boost(isLeft(),isRight(),isUp(),isDown());
  }
};

void sheep::boost(bool left, bool right, bool up, bool down) {
  // Valeur aléatoire
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<float> dist(1.3,1.45);

  if (left)
    set_vel_x(-dist(mt));
  if (right)
    set_vel_x(dist(mt));
  if (up)
    set_vel_y(-dist(mt));
  if (down)
    set_vel_y(dist(mt));
};

void sheep::reset_boost(bool left, bool right, bool up, bool down) {
  // Valeur aléatoire
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<float> dist(0.35, 0.5);

  if (left)
    set_vel_x(-dist(mt));
  if (right)
    set_vel_x(dist(mt));
  if (up)
    set_vel_y(-dist(mt));
  if (down)
    set_vel_y(dist(mt));
}


    ///// WOLF ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
wolf::wolf(SDL_Surface* window_surface_ptr)
    : animal("media/wolf.png", window_surface_ptr), 
    time_eat{0} 
{
  // Valeur aléatoire
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<double> dist(0.9, 1.15);

  set_vel_x(dist(mt));
  set_vel_y(dist(mt));

  set_properties("role", "hunter");
  set_properties("object", "wolf");
}

void wolf::interaction(const std::shared_ptr<interacting_object>& obj, double distance, ground* g) {
  
  if (obj->isPrey() && distance < 75 && distance != 0) 
  {
    obj->set_properties("alive", "false");
    this->set_time_eat(SDL_GetTicks());
    set_prey(nullptr);
  }
};

// Recherche d'une proie la plus proche
void wolf::find_prey(ground* g) 
{ 
    for (auto obj : g->get_moving_object()) 
    {
 
        if (obj->isPrey()) 
        {
            if (get_prey() == nullptr) 
            {
                this->set_prey(obj);
            }
          if (this->distance(obj) < this->distance(get_prey())) 
          {
            this->set_prey(obj);
          }
        }
    }
}

void wolf::find_dog(ground* g) 
{
  if (get_dog() == nullptr) 
  {
    for (auto obj : g->get_moving_object()) 
    {
      if (obj->isDog()) 
      {
        set_dog(obj);
      }
    }
  }
}

void wolf::move() 
{
  if (!(get_prey()->isAlive())) 
  {
    this->set_prey(nullptr);
  } 
  else 
  {
    if (isStarving()) // Meurt si il a faim 
    {
      this->set_properties("alive", "false");
    } 
    else 
    {
      // Position du loup
      double wolf_pos_x = this->get_pos_x();
      double wolf_pos_y = this->get_pos_y();

      // Evite le chien en priorité
      if (this->distance(get_dog()) < 100) {
        std::shared_ptr<moving_object> dog = get_dog();

        // Position
        double dog_pos_x = dog->get_pos_x();
        double dog_pos_y = dog->get_pos_y();

        // Vitesse
        double wolf_vel_x = abs(get_vel_x());
        double wolf_vel_y = abs(get_vel_x());

        if (wolf_pos_x > dog_pos_x) // Chien à gauche => Dirige vers la droite
        {
          set_pos_x(get_pos_x() + wolf_vel_x);

          // Se dirige vers la droite
          set_right(true);
          set_left(false);
        } else if (wolf_pos_x < dog_pos_x) // Chien à droite => Dirige vers la gauche
        {
          set_pos_x(get_pos_x() - wolf_vel_x);

          // Se dirige vers la gauche
          set_right(false);
          set_left(true);
        } else // Chien en dessous ou au dessus
        {
          set_pos_x(get_pos_x());

          // Se dirige ni vers la droite ni vers la gauche
          set_right(false);
          set_left(false);
        }

        if (wolf_pos_y > dog_pos_y) // Chien au dessus
        {
          set_pos_y(get_pos_y() + wolf_vel_y);

          // Se dirige vers le bas
          set_up(false);
          set_down(true);
        } else if (wolf_pos_y < dog_pos_y) // Chien en dessous
        {
          set_pos_y(get_pos_y() - wolf_vel_y);

          // Se dirige vers le bas
          set_up(false);
          set_down(true);
        } else // Chien à droite ou à gauche
        {
          set_pos_y(get_pos_y());

          // Se dirige ni vers le bas ni vers le haut
          set_up(false);
          set_down(false);
        }
      } else 
      {
        // Position de la proie ciblée
        std::shared_ptr<moving_object> prey = get_prey();
        double prey_pos_x = prey->get_pos_x();
        double prey_pos_y = prey->get_pos_y();

        if (wolf_pos_x > prey_pos_x) // Proie à gauche
        {
          set_pos_x(get_pos_x() - get_vel_x());

          // Se dirige vers la gauche
          set_right(false);
          set_left(true);
        } else if (wolf_pos_x < prey_pos_x) // Proie à droite
        {
          set_pos_x(get_pos_x() + get_vel_x());

          // Se dirige vers la droite
          set_right(true);
          set_left(false);
        } else // Proie en dessous ou au dessus
        {
          set_pos_x(get_pos_x());

          // Se dirige ni vers la droite ni vers la gauche
          set_right(false);
          set_left(false);
        }

        if (wolf_pos_y > prey_pos_y) // Proie au dessus
        {
          set_pos_y(get_pos_y() - get_vel_y());

          // Se dirige vers le haut
          set_up(true);
          set_down(false);
        } else if (wolf_pos_y < prey_pos_y) // Proie en dessous
        {
          set_pos_y(get_pos_y() + get_vel_y());

          // Se dirige vers le bas
          set_up(false);
          set_down(true);
        } else // Proie à droite ou à gauche
        {
          set_pos_y(get_pos_y());

          // Se dirige ni vers le bas ni vers le haut
          set_up(false);
          set_down(false);
        }
      }
    }
  }
}
///// DOG ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
dog::dog(SDL_Surface* window_surface_ptr, std::shared_ptr<moving_object> mov_obj): 
    animal("media/shepherd_dog.png", window_surface_ptr),
    mov_obj_(mov_obj), 
    // Angle et rayon pour le deplacement circulaire du chien
    angle{1}, 
    radius{100} 
{
  // Le chien se trouve à coté de l'objet qu'il doit suivre en rond
  set_pos_x(mov_obj_->get_pos_x() + 60);
  set_pos_y(mov_obj_->get_pos_y() + 60);

  set_properties("role", "hunter's ennemy");
  set_properties("object", "dog");
}

void dog::move() 
{ 
    set_angle(get_angle()+0.015);
    double x = mov_obj_->get_pos_x() + radius * cos(get_angle());
    double y = mov_obj_->get_pos_y() + radius * sin(get_angle());
    set_pos_x(x);
    set_pos_y(y);
}
void dog::interaction(const std::shared_ptr<interacting_object>& obj, double distance, ground* g){};
