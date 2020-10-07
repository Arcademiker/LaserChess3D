//
// Created by arcademiker on 20.07.18.
//

#ifndef LASERCHESS_CMAP_H
#define LASERCHESS_CMAP_H


// Include GLFW
//#include <GLFW/glfw3.h>
#include <external/glfw-3.1.2/include/GLFW/glfw3.h>
#include <algorithm>
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
    int get_commandU_counter();
    int get(int x, int y);
    CUnit* get_unit(int x, int y);
    CUnit* get_unit(int id);
    void add_unit(int type, int x, int y);
    bool kill_unit(int id);
    bool move(int x, int y, int to_x, int to_y);
    int distance(int x, int y, int to_x, int to_y);
    bool is_inbound(int x, int y);
    std::unordered_map<int,CUnit*>* get_enemys_list();
    std::unordered_map<int,CUnit*>* get_unit_list();
    void print();
    void print(std::map<int,CUnit*>* UMap);
    void listAllUnits();
};


#endif //LASERCHESS_CMAP_H
