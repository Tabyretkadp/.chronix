#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

struct Activity {
  std::string name;
  int minutes;
};

void clearConsole() {
#ifdef _WIN32
  std::system("cls");
#else
  std::system("clear");
#endif
}

// =========== visual-start

void menu() {
  std::cout << R"(
        ░▒▓██████▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓███████▓▒░ ░▒▓██████▓▒░░▒▓███████▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░ 
       ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░ 
       ░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░ 
       ░▒▓█▓▒░      ░▒▓████████▓▒░▒▓███████▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓██████▓▒░  
       ░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░ 
░▒▓██▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░ 
░▒▓██▓▒░░▒▓██████▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░░▒▓██████▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░ 
                                                                                             
[ 1 ] - Add                                                                [ q ] - Skip/qiut
[ 2 ] - Dell                                                                   [ ^C ] - Exit
[ 3 ] - Show                                                                            v1.0
[ 4 ] - Timer
  )";
std::cout << std::endl;
}

void printBarChartPercent(const std::vector<Activity> &activities) {
  if (activities.empty())
    return;

  int maxHeight = 10;
  int total_minutes = 0;

  for (const auto &a : activities)
    total_minutes += a.minutes;

  for (int level = maxHeight; level > 0; --level) {
    for (const auto &a : activities) {
      int height = (a.minutes * maxHeight) / total_minutes;
      if (height >= level)
        std::cout << " █ ";
      else if (height == 0 && a.minutes > 0 && level == 1)
        std::cout << " ▎ ";
      else
        std::cout << "   ";
    }
    std::cout << std::endl;
  }

  for (size_t i = 0; i < activities.size(); i++)
    std::cout << "---";
  std::cout << std::endl;
  ;

  for (const auto &a : activities)
    std::cout << " " << a.name[0] << " ";
  std::cout << std::endl;
  ;
}

// =========== visual-end

// =========== file-start

void saveToFile(const std::string &filename, std::vector<Activity> &active) {
  std::ofstream fout(filename, std::ios::out | std::ios::trunc);
  for (const auto &a : active) {
    fout << a.name << " " << a.minutes << std::endl;
  }
}

void loadFromFile(const std::string &filename, std::vector<Activity> &active) {
  std::ifstream fin(filename);
  if (!fin.is_open())
    return;

  active.clear();
  std::string line;
  while (std::getline(fin, line)) {
    std::istringstream iss(line);
    std::string name;
    int minutes;
    if (!(iss >> name >> minutes))
      continue;

    bool found = false;
    for (auto &a : active) {
      if (a.name == name) {
        a.minutes += minutes;
        found = true;
        break;
      }
    }
    if (!found)
      active.push_back({name, minutes});
  }
}

// =========== file-end

// =========== main-start

void show(std::vector<Activity> &active) {
  std::cout << std::endl;
  for (const auto &a : active) {
    int hours = a.minutes / 60;
    int mins = a.minutes % 60;

    if (hours > 0) {
      std::cout << a.name << " : " << hours << "h " << mins << "min"
                << std::endl;
    } else {
      std::cout << a.name << " : " << mins << " min" << std::endl;
    }
  }

  std::cout << std::endl;
}

void add(std::vector<Activity> &active) {
  show(active);
  std::string name;
  int minutes;

  while (true) {
    std::cout << "Name active: ";
    std::cin >> name;

    if (name == "q") {
      break;
    }

    if (name.find(' ') != std::string::npos) {
      std::cout << "Error! use something like ReadBook" << std::endl;
      ;
    } else {
      break;
    }
  }

  while (true) {
    std::cout << "Minutes: ";
    std::string input;
    std::cin >> input;

    if (input == "q") {
      break;
    }

    std::stringstream ss(input);
    if (ss >> minutes && ss.eof() && minutes >= 0) {
      break;
    } else {
      std::cout << "Error! in int only" << std::endl;
    }
  }

  bool found = false;
  for (auto &a : active) {
    if (a.name == name) {
      a.minutes += minutes;
      found = true;
      break;
    }
  }

  if (!found) {
    active.push_back({name, minutes});
  }
}

void dell(std::vector<Activity> &active) {
  std::string name;
  for (auto &a : active) {
    std::cout << a.name << " : " << a.minutes << " min" << std::endl;
  }

  std::cout << std::endl;

  std::cout << "Name:";
  std::cin >> name;
  if (name == "q") {
    name = "you **** boy";
  }
  for (auto it = active.begin(); it < active.end(); it++) {
    if (it->name == name) {
      active.erase(it);
      break;
    }
  }
}

void startTimer(std::vector<Activity> &active) {
  using namespace std::chrono;
  std::string activeName;

  show(active);
  std::cout << "Choosing where the time will go: ";
  std::cin >> activeName;

  if (activeName == "q") {
    return;
  }

  clearConsole();

  std::cout << "Timer started. Press Enter to stop..." << std::endl;
  ;

  auto start = steady_clock::now();

  std::cin.ignore();
  std::cin.get();

  auto end = steady_clock::now();
  auto elapsed = duration_cast<minutes>(end - start);

  int total_minutes = elapsed.count();
  int hours = total_minutes / 60;
  int minutes = total_minutes % 60;

  std::cout << "time: ";
  if (hours > 0)
    std::cout << hours << "h ";
  std::cout << minutes << " min" << std::endl;

  for (int i = 0; i < active.size(); i++) {
    if (activeName == active[i].name) {
      active[i].minutes += total_minutes;
      std::cout << active[i].name << " = " << active[i].minutes << std::endl;
      ;
      break;
    }
  }
}

// =========== main-end

// =========== START =========== //

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
