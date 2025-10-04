#pragma once

#include <string>
#include <vector>

struct Activity {
  std::string name;
  int minutes;
};

void clearConsole();
void printBarChartPercent(const std::vector<Activity> &activities);
void saveToFile(const std::string &filename, std::vector<Activity> &active);
void loadFromFile(const std::string &filename, std::vector<Activity> &active);
void show(std::vector<Activity> &active);
void add(std::vector<Activity> &active);
void dell(std::vector<Activity> &active);
void startTimer(std::vector<Activity> &active);
