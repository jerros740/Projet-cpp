#pragma once

#include "ground.h"
#include "animals.h"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

///// GROUND

ground::ground(SDL_Surface* window_surface_ptr) : window_surface_ptr_{window_surface_ptr} {};

void ground::add_object(const std::shared_ptr<moving_object>& new_object) 
{
  all_object_.push_back(new_object);
}

void ground::update() {

  // Copie du vecteur pour ne pas modifier le vecteur en même temps que l'itération
  std::vector<std::shared_ptr<moving_object>> copy_all_object_(all_object_.size());
  std::copy(all_object_.begin(), all_object_.end(), copy_all_object_.begin());

  std::vector<std::shared_ptr<moving_object>>::iterator it;
  for (auto a1 : copy_all_object_) 
  {
    // Si l'objet est en vie
    if (a1->isAlive()) 
    {
      if (a1->isHunter()) 
      {
        a1->find_prey(this);
        a1->find_dog(this);
      }
      a1->move();
      a1->draw();
      for (const auto& a2 : copy_all_object_) 
      {
        double distance = a1->distance(a2);

        // Interaction en fonction de la distance
        a1->interaction(a2, distance,this);
      }
    } 
    else
    {
      // Recupère l'objet à supprimer
      it = find(all_object_.begin(), all_object_.end(), a1);

      // Suppression
      all_object_.erase(it);
    }
  }
}

int ground::get_score() 
{
  int score = 0;
  for (auto a1 : all_object_) 
  {
    if (a1->isSheep())
      score += 1;
  }

  return score;
}