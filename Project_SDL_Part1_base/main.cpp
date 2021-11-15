#include "application.h"
#include <stdio.h>
#include <string>
#include <stdexcept>
#include <random>
#include "object.h"

int main(int argc, char* argv[]) {

  std::cout << "Starting up the application" << std::endl;

   if (argc != 4)
    throw std::runtime_error("Need three arguments - "
                             "number of sheep, number of wolves, "
                             "simulation time\n");

  init();

  srand(time(0)); // Génération  d'une position aléatoire différente entre chaque éxécution

  std::cout << "Done with initilization" << std::endl;

  application my_app(std::stoul(argv[1]), std::stoul(argv[2]));
  std::cout << "Created window" << std::endl;

  int retval = my_app.loop(std::stoul(argv[3]));
  std::cout << "Exiting application with code " << retval << std::endl;

  return retval;
}