//
// Created by arcademiker on 20.07.18.
//


#include "CMap.h"
#include "CUnit_AI.h"
#include "CUnit_Player.h"
#include "units/CGrunt.h"
#include "units/CJumpship.h"
#include "units/CTank.h"
#include "units/CDrone.h"
#include "units/CDreadnought.h"
#include "units/CCommandUnit.h"


void CMap::set(int x, int y, int id) {
    this->grid[y][x] = id;
}

CMap::CMap() {
    this->unitCounter = 0;
    this->enemyCounter = 0;
    this->commandU_counter = 0;
    this->grid = std::vector<std::vector<int>>(this->size, std::vector<int>(this->size, 0));
}

CMap::~CMap() {
    unit_list.clear();
    enemy_list.clear();
}


bool CMap::is_inbound(int x, int y) {
    return y >= 0 && x >=0 && y < this->grid.size() && x < this->grid[0].size();
}

unsigned int CMap::get_size() {
    return this->size;
}

int CMap::get_commandU_counter() {
    return this->commandU_counter;
}

/// get unit id that occupated field at position (x,y) or 0 if free
int CMap::get(int x, int y) {
    return this->grid[y][x];
}

CUnit* CMap::get_unit(int x, int y) {
    int id = this->get(x,y);
    if(id != 0) {
        return this->get_unit(id);
    }
    else {
        return nullptr;
    }
}

CUnit* CMap::get_unit(int id) {
    if(id < 0) {
        return this->enemy_list.at(id);
    }
    else if(id > 0){
        return this->unit_list.at(id);
    }
    else {
        return nullptr;
    }
}

/// warning: replaces unit if space is occupied
/// buggy but user doesn't add units manually. cannot occur ingame if level is createtd properly
void CMap::add_unit(int type, int x, int y) {
    switch(type) {
        case 1:
            this->enemyCounter++;
            this->enemy_list.insert(std::make_pair( -this->enemyCounter, new CDrone(type, x, y, *this)));
            this->grid[y][x] = -enemyCounter;
            break;
        case 2:
            this->enemyCounter++;
            this->enemy_list.insert(std::make_pair( -this->enemyCounter, new CDreadnought(type, x, y, *this)));
            this->grid[y][x] = -enemyCounter;
            break;
        case 3:
            this->enemyCounter++;
            this->enemy_list.insert(std::make_pair( -this->enemyCounter, new CCommandUnit(type, x, y, *this)));
            this->grid[y][x] = -enemyCounter;
            this->commandU_counter++;
            break;
        case 4 :
            this->unitCounter++;
            this->unit_list.insert(std::make_pair( this->unitCounter, new CGrunt(type, x, y, *this)));
            this->grid[y][x] = unitCounter;
            break;
        case 5 :
            this->unitCounter++;
            this->unit_list.insert(std::make_pair( this->unitCounter, new CJumpship(type, x, y, *this)));
            this->grid[y][x] = unitCounter;
            break;
        case 6 :
            this->unitCounter++;
            this->unit_list.insert(std::make_pair( this->unitCounter, new CTank(type, x, y, *this)));
            this->grid[y][x] = unitCounter;
            break;
        default:
            //todo: throw exception
            break;
    }
}

bool CMap::kill_unit(int id) {
    std::unordered_map<int,CUnit*>* pList;
    if(id<0) {
        if(this->get_unit(id)->get_type()==3) {
            this->commandU_counter--;
        }
        pList = &this->enemy_list;
    }
    else {
        pList = &this->unit_list;
    }
    if (pList->count(id) != 0) {
        this->set(pList->at(id)->get_x(), pList->at(id)->get_y(), 0);
        pList->erase(id);
        return true;
    }
    return false;

}

//attention this function doesn't update the new location in the unit class
bool CMap::move(int x, int y, int to_x, int to_y) {
    int id = this->get(x, y);
    if(this->get(to_x, to_y)==0){
        this->set(x, y, 0);
        this->set(to_x, to_y, id);
        return true;
    }
    else {
        return false;
    }
}

int CMap::distance(int x, int y, int to_x, int to_y) {
    return std::max(std::abs(x-to_x),std::abs(y-to_y)); /// diagonal distance
}

std::unordered_map<int,CUnit*>* CMap::get_enemys_list() {
    return &this->enemy_list;
}
std::unordered_map<int,CUnit*>* CMap::get_unit_list() {
    return &this->unit_list;
}

void CMap::print() {
    std::cout << "   0 1 2 3 4 5 6 7" << std::endl << std::endl;
    for(int y = 0; y < this->grid.size(); ++y) {
        std::cout << y << " ";
        for(int x = 0; x < this->grid[0].size(); ++x) {
            if(this->get(x,y) < 0) {
                std::cout << this->get(x,y);
            }
            else if(this->get(x,y) > 0) {
                std::cout << "+" << this->get(x,y);
            }
            else {
                std::cout << " 0";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void CMap::print(std::map<int,CUnit*>* UMap) {
    std::cout << "   0 1 2 3 4 5 6 7" << std::endl << std::endl;
    for(int y = 0; y < this->grid.size(); ++y) {
        std::cout << y << " ";
        for(int x = 0; x < this->grid[0].size(); ++x) {
            if(this->get(x,y) < 0) {
                std::cout << this->get(x,y);
            }
            else if(this->get(x,y) != 0 && UMap->count(this->get(x,y)) != 0) {
                std::cout << "+" << this->get(x,y);
            }
            else if(this->get(x,y) != 0) {
                std::cout << "!" << this->get(x,y);
            }
            else {
                std::cout << " 0";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void CMap::listAllUnits() {
    for ( auto& U: this->unit_list ) {
        std::cout << "+" << U.first << ": type = " << U.second->get_type()
                  << " - (x, y) = (" << U.second->get_x() << ", " << U.second->get_y() << ")"
                  << " - hp = " << U.second->get_health() << std::endl;
    }
    std::cout << std::endl;
    for ( auto& U: this->enemy_list ) {
        std::cout << U.first << ": type = " << U.second->get_type()
                  << " - (x, y) = (" << U.second->get_x() << ", " << U.second->get_y() << ")"
                  << " - hp = " << U.second->get_health() << std::endl;
    }
    std::cout << std::endl;
}










