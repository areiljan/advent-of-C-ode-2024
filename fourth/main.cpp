#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <regex>
#include <chrono>

std::vector<std::string> read_file_into_vector() 
{
  auto start = std::chrono::high_resolution_clock::now();

  std::ifstream read_file("input.txt");
  std::vector<std::string> vector {};

  std::string line {};
  while (getline(read_file, line)) {
    vector.push_back(line);
  }
  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
  std::cout << "File read in " << duration << " µs." << "\n";
  return vector;
}

void print_vector(std::vector<std::string> vector) 
{
  for (int x = 0; x < vector.size(); x++) {
    std::cout << x << ". " << vector.at(x) << "\n";
  }
}

std::string user_input() 
{
  std::string input{};
  std::cout << "What are you looking for: ";
  std::cin >> input;
  return input;
}

enum Direction 
{
  // means that the direction is heading there on the matrix
  RIGHT,
  LEFT
};

std::string get_diagonal(const std::vector<std::string>& vector, std::pair<int, int> starting_coordinates, enum Direction direction = RIGHT) 
{
  std::string diagonal {};
  int y = starting_coordinates.second;
  if (direction == LEFT) {
    for (int x = starting_coordinates.first; x >= 0; x--) 
    {
      if (vector.size() > y) {
        diagonal = diagonal + (vector[y].at(x));

        y++;
      } else {
        return diagonal;
      }
    }
    return diagonal;
  } 
  else 
  {
    for (int x = starting_coordinates.first; x < vector[0].size(); x++) {
      if (vector.size()> y) {
        diagonal = diagonal + (vector[y].at(x));

        y++;
      } else {
        return diagonal;
      }
    }
    return diagonal;
  }
}

std::vector<std::string> transpose_and_add(const std::vector<std::string>& vector) {
  std::vector<std::string> new_vector {vector};
  // transposing is impossible with less than O(n) :/
  for (int x = 0; x < vector[0].size(); x++) 
  {
    std::string column {};
    for (int y = 0; y < vector.size(); y++)
    {
      column = column + vector[y].at(x); 
    }
    if (column.empty()) {
      continue;
    }
    new_vector.push_back(column);
  }
  return new_vector;
}

int find_match_amount(const std::vector<std::string>& vector, std::string searchInput)
{

  auto start = std::chrono::high_resolution_clock::now();
  std::string rev {searchInput};
  std::reverse(rev.begin(), rev.end());
  const std::regex criteria("(?=(" + searchInput + "|" + rev + "))");

  int counter = 0;
  for (int index = 0; index < vector.size(); index++) {
    std::string text = vector.at(index);
    std::ptrdiff_t const match_count(std::distance(
          std::sregex_iterator(text.begin(), text.end(), criteria),
          std::sregex_iterator()));
    counter = counter + match_count;
  }

  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
  std::cout << "Found all of the instances in " << duration << " µs." << "\n";

  return counter;
}

int main() 
{
  std::vector<std::string> lines{read_file_into_vector()};


  auto start = std::chrono::high_resolution_clock::now();

  std::vector<std::string> combinedStrings {transpose_and_add(lines)};
  for (int x = 0; x < lines[0].size(); x++) {
    std::pair<int, int> coordinates = {x, 0};
    combinedStrings.push_back(get_diagonal(lines, coordinates, LEFT));
    combinedStrings.push_back(get_diagonal(lines, coordinates, RIGHT));
  }
  for (int y = 1; y < lines.size(); y++) {
    std::pair<int, int> coordinates = {0, y};
    combinedStrings.push_back(get_diagonal(lines, coordinates, RIGHT));
    coordinates = {lines[0].size() - 1, y};
    combinedStrings.push_back(get_diagonal(lines, coordinates, LEFT));
  }

  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
  std::cout << "Preprocessed the file for optimal search in " << duration << " µs." << "\n";

  std::string searchInput{user_input()};

  int matchCases =  find_match_amount(combinedStrings, searchInput); 
  std::cout << "This is how many examples of " << searchInput << " we have: " << matchCases << "\n";
  return 0;
}

