# LaserChess3D
![laserchess](https://user-images.githubusercontent.com/15786772/43614702-cf3fff8e-96b4-11e8-8b98-b3633cb95fdd.png)

```
Game Overview:
The Game has 3 Levels:
Player Units are Blue.
The Units are Red.
The Player is granted the first turn.
Each Unit can moved once in one turn.


Controlls:
left mouse click to choose, move and attack with Units.
right mouse click to abort move.
left click on aktive Unit during attack skips attack phase of this Unit

Press Enter to get next free Unit.
Press Enter while accesing a Unit to do no move or no attack with current Unit.


The Units:

The Human pieces:

Grunt:
  Moves 1 space orthogonally 
  Shoots once, diagonally at any range. 
  Attack power: 1 
  Hitpoints: 2 
  
Jumpship:
  Moves like the knight in Chess 
  Attacks all enemy pieces in the 4 orthogonally adjacent spaces simultaneously. 
  Attack power: 2 
  Hitpoints: 2 

Tank:
  Moves like the Queen in chess, up to a maximum of 3 spaces. 
  Shoots once, orthogonally at any range. 
  Attack power: 2 
  Hitpoints: 4 
  
  
The AI pieces:

Drone:
  Moves forward 1 space from its side of the board (like a pawn, but never moves diagonally). 
  Shoots once, diagonally at any range. 
  Attack power: 1 
  Hitpoints: 2 
  Behaviour: 
    Drones move before any other AI piece. 
    They must all move and attack if possible. 
    They must shoot at a target if possible after attempting to move 
    
Dreadnought:
  Moves 1 space in any direction: 
  Attacks all adjacent enemy units; 
  Attack power 2: 
  Hitpoints: 5 
  Behaviour: 
    Dreadnoughts move after all drones have moved. 
    It must move 1 space, if possible, and must move towards the nearest enemy unit. It must try to 
    attack after moving. 
    
Command Unit:
  The Command Unit must move after Dreadnoughts have moved. 
  It can only move 1space in two possible directions ­ parallel to the AIs side of the board (i.e. it stays 
  the same distance from the enemy side of the board). 
  It cannot shoot or attack. 
  Attack power: 0 
  Hitpoints: 5 
  Behaviour: 
   It must avoid getting hit, if possible, so it must make the best move out of the three options available 
   (move one way, move the other way, or stay still). 


limitations of this version:
Forced MSAA and Vsync
Forced 1080p resolution
```
