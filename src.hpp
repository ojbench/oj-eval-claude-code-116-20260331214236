#ifndef SRC_HPP
#define SRC_HPP
#include <iostream>
#include <utility>
#include <deque>

struct Map;
enum class instruction{UP, DOWN, LEFT, RIGHT, NONE};
//the instruction of the snake

bool is_food(Map* map, int x, int y);
//return true if the cell at (x,y) is food

bool is_wall(Map* map, int x, int y);
//return true if the cell at (x,y) is wall

void eat_food(Map* map, int x, int y);
//remove the food at (x,y)

int get_height(Map* map);
//return the height of the map

int get_width(Map* map);
//return the width of the map


struct Snake{
  // store the necessary information of the snake
  std::deque<std::pair<int, int>> body; // positions of snake body (head is front)
  instruction current_direction;

  void initialize(int x, int y, instruction ins){
    // (x,y) is the position of the head of the snake, ins is the initial orientation of the snake
    body.clear();
    body.push_back({x, y});
    current_direction = ins;
  }

  int get_length(){
    //return the length of the snake
    return body.size();
  }

  bool move(Map* map, instruction ins){
    //the current map and the next instruction
    //return false if the snake is dead
    //return true if the snake is alive

    // Determine next direction
    instruction next_dir = (ins == instruction::NONE) ? current_direction : ins;

    // Check for reverse direction (death condition)
    if ((current_direction == instruction::UP && next_dir == instruction::DOWN) ||
        (current_direction == instruction::DOWN && next_dir == instruction::UP) ||
        (current_direction == instruction::LEFT && next_dir == instruction::RIGHT) ||
        (current_direction == instruction::RIGHT && next_dir == instruction::LEFT)) {
      return false; // Reverse direction = death
    }

    current_direction = next_dir;

    // Calculate new head position
    int head_x = body.front().first;
    int head_y = body.front().second;
    int new_x = head_x, new_y = head_y;

    switch(current_direction) {
      case instruction::UP:
        new_x--;
        break;
      case instruction::DOWN:
        new_x++;
        break;
      case instruction::LEFT:
        new_y--;
        break;
      case instruction::RIGHT:
        new_y++;
        break;
      default:
        break;
    }

    // Check boundaries
    if (new_x < 0 || new_x >= get_height(map) || new_y < 0 || new_y >= get_width(map)) {
      return false; // Hit boundary
    }

    // Check if hit wall
    if (is_wall(map, new_x, new_y)) {
      return false; // Hit wall
    }

    // Check if hit self body (excluding tail which will be removed)
    for (size_t i = 0; i < body.size() - 1; i++) {
      if (body[i].first == new_x && body[i].second == new_y) {
        return false; // Hit self
      }
    }

    // Add new head
    body.push_front({new_x, new_y});

    // Check if ate food
    if (is_food(map, new_x, new_y)) {
      eat_food(map, new_x, new_y);
      // Don't remove tail - snake grows
    } else {
      // Remove tail - normal move
      body.pop_back();
    }

    return true;
  }

  std::pair<int, std::pair<int,int>*> get_snake(){
    //return the length of the snake and a pointer to the array of the position of the snake
    //you can store the head as the first element
    int len = body.size();
    std::pair<int, int>* arr = new std::pair<int, int>[len];
    for (int i = 0; i < len; i++) {
      arr[i] = body[i];
    }
    return {len, arr};
  }
};

const int MaxWidth = 20;
struct Map{
  // store the necessary information of the map
  bool wall[MaxWidth][MaxWidth];
  bool food[MaxWidth][MaxWidth];
  int width, height;

  int get_height(){
    //return the height of the map
    return height;
  }

  int get_width(){
    //return the width of the map
    return width;
  }

  bool is_food(int x, int y){
    //return true if the cell at (x,y) is food
    return food[x][y];
  }

  void eat_food(int x, int y){
    //eat the food at (x,y)
    food[x][y] = false;
  }

  bool is_wall(int x, int y){
    //return true if the cell at (x,y) is wall
    return wall[x][y];
  }

  void initialize(Snake *snake){
    char str[MaxWidth];
    int head_x = -1 , head_y = -1;
    instruction ins;

    // Read map dimensions
    std::cin >> height >> width;

    // Initialize arrays
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        wall[i][j] = false;
        food[i][j] = false;
      }
    }

    // Read map layout
    for (int i = 0; i < height; i++) {
      std::cin >> str;
      for (int j = 0; j < width; j++) {
        if (str[j] == '#') {
          wall[i][j] = true;
        } else if (str[j] == '*') {
          food[i][j] = true;
        } else if (str[j] == '@') {
          head_x = i;
          head_y = j;
        }
      }
    }

    // Read initial direction
    std::cin >> str;
    switch(str[0]) {
      case 'U':
        ins = instruction::UP;
        break;
      case 'D':
        ins = instruction::DOWN;
        break;
      case 'L':
        ins = instruction::LEFT;
        break;
      case 'R':
        ins = instruction::RIGHT;
        break;
      default:
        ins = instruction::NONE;
    }

    snake->initialize(head_x, head_y, ins);
  }

  void print(Snake *snake){
    auto snake_body = snake->get_snake();
    int len = snake_body.first;
    std::pair<int, int>* positions = snake_body.second;

    // Create a temporary grid for printing
    char grid[MaxWidth][MaxWidth];

    // Fill with map data
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        if (wall[i][j]) {
          grid[i][j] = '#';
        } else if (food[i][j]) {
          grid[i][j] = '*';
        } else {
          grid[i][j] = '.';
        }
      }
    }

    // Add snake body
    for (int i = 1; i < len; i++) {
      grid[positions[i].first][positions[i].second] = 'o';
    }

    // Add snake head (head is first element)
    if (len > 0) {
      grid[positions[0].first][positions[0].second] = '@';
    }

    // Print grid
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        std::cout << grid[i][j];
      }
      std::cout << std::endl;
    }

    // Clean up
    delete[] positions;
  }
};

struct Game{
  Map *map;
  Snake *snake;
  int score;
  int round;

  void initialize(){
    map = new Map();
    snake = new Snake();
    map->initialize(snake);
    score = 0;
    round = 0;
  }

  bool step()
  {
    char str[MaxWidth];
    std::cin >> str;
    instruction ins;
    switch(str[0]){
      case 'U':
        ins = instruction::UP;
        break;
      case 'D':
        ins = instruction::DOWN;
        break;
      case 'L':
        ins = instruction::LEFT;
        break;
      case 'R':
        ins = instruction::RIGHT;
        break;
      default:
        ins = instruction::NONE;
    }
    if(snake->move(map, ins)){
      score++;
    }else{
      return false;
    }
    return true;
  }

  void print(){
    std::cout<< "Round: " << round << std::endl;
    map->print(snake);
  }

  void play()
  {
    while(step()){
      round++;
      print();
    }
    score += snake->get_length() ;
    std::cout << "Game Over" << std::endl;
    std::cout << "Score: " << score << std::endl;
  }
};
#endif
