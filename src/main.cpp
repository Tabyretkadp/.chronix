#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "../include/banner.hpp"
#include "../include/func.hpp"

int main() {
  char in_menu;
  const std::string filename = "data.txt";
  std::vector<Activity> active;

  loadFromFile(filename, active);

  while (true) {
    clearConsole();
    menu();
    std::cout << std::endl;
    std::cout << "Input: ";
    std::cin >> in_menu;

    switch (in_menu) {
    case '1': {
      clearConsole();
      menu();
      add(active);
      saveToFile(filename, active);
      break;
    }
    case '2': {
      clearConsole();
      menu();
      dell(active);
      saveToFile(filename, active);
      break;
    }
    case '3': {
      clearConsole();
      menu();
      printBarChartPercent(active);
      show(active);
      char goToNext;

      std::cout << "In '0' to exit: ";
      std::cin >> goToNext;
      break;
    }
    case '4': {
      clearConsole();
      startTimer(active);
      saveToFile(filename, active);
      char goToNext;

      std::cout << "In '0' to exit: ";
      std::cin >> goToNext;
      break;
    }
    default:
      break;
    }
  }

  return 0;
}
