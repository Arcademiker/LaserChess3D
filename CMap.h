//
// Created by arcademiker on 20.07.18.
//

#ifndef LASERCHESS_CMAP_H
#define LASERCHESS_CMAP_H


// Include GLFW
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <map>

#include "CUnit.h"




class CUnit;
class CMap {
private:

    unsigned int size = 8;
    std::vector<std::vector<int>> grid;
    std::unordered_map<int,CUnit*> unit_list;
    std::unordered_map<int,CUnit*> enemy_list;
    unsigned int unitCounter;
    unsigned int enemyCounter;
    int commandU_counter;

    //helper funktions:
    void set(int x, int y, int id);

public:
    CMap();
    ~CMap();
    unsigned int get_size();
    /// get number of CommandUnits (for player victory conditions)
    int get_commandU_counter();
    /// get unit id that occupated field at position (x,y) or 0 if free
    int get(int x, int y);
    /// get unit on field x,y or if empty = 0
    CUnit* get_unit(int x, int y);
    /// get Unit with id
    CUnit* get_unit(int id);
    /// add Unit with specific type on map and generate an id for the Unit
    void add_unit(int type, int x, int y);
    /// kill unit with id and remove it from the board and the unit lists
    bool kill_unit(int id);
    /// move unit on board from one location x,y to loaction x,y
    bool move(int x, int y, int to_x, int to_y);
    /// get diagonal distance on the map between two points
    int distance(int x, int y, int to_x, int to_y);
    /// check if x,y is a place on the map inside the boundaries
    bool is_inbound(int x, int y);
    std::unordered_map<int,CUnit*>* get_enemys_list();
    std::unordered_map<int,CUnit*>* get_unit_list();
    void print();
    void print(std::map<int,CUnit*>* UMap);
    void listAllUnits();
};


#endif //LASERCHESS_CMAP_H
