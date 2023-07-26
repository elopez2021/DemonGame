#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
#include <math.h>
#include <time.h>

#define FILAS 23
#define COL 90

// Define constants for the player, demon, shield, destroyer, life, weapon, wall character
#define PLAYER '@'
#define WALL '|'
#define DEMON 'D'
#define SHIELD 'S'
#define DESTROYER 'T'
#define LIFE 3
#define WEAPON 'K'

// Define constants for exits
#define EXIT_A 'A'
#define EXIT_B 'B'
#define EXIT_C 'C'

//Define demons constants
#define NUM_DEMONS 4
#define MAX_STEPS_TO_KILL 3

//Define structure for demon position
typedef struct {
    int x;
    int y;
} Demon;
//Define structure for player
typedef struct {
    int x;
    int y;
    int steps;
    int lifes;
    bool hasShield, hasDestroyer, hasWeapon;
} Player;

// Define constants for the movement keys
#define KEY_LEFT_DOWN '1'
#define KEY_DOWN '2'
#define KEY_RIGHT_DOWN '3'
#define KEY_LEFT '4'
#define KEY_RIGHT '6'
#define KEY_LEFT_UP '7'
#define KEY_UP '8'
#define KEY_RIGHT_UP '9'

//Define constants variables for shield
#define SHIELD_TIMER_MAX 5

//Define constants variables for destroyer
#define DESTROYER_TIMER_MAX 5

//Define constants variables for lives
#define LIFE_TIMER_MAX 5

//Define constants variables for weapon
#define WEAPON_TIMER_MAX 5
#define WEAPON_MAX_SPACES 5

//Define structures for items
typedef struct {
  int x;
  int y;
} Point;

typedef struct {
    Point location;
    bool available;
    int steps;
    int timer;
} Shield;

typedef struct {
    Point location;
    int steps;
    int timer;
    bool available;
} Destroyer;

typedef struct {
    Point location;
    int timer;
    bool available;
} Weapon;

typedef struct {
    Point location;
    int timer;
    bool available;
} Life;

typedef struct{
    Shield shield;
    Destroyer destroyer;
    Weapon weapon;
    Life life;
} Items;

void clear(char m[FILAS][COL]){
 	int i,j;
 	for(i=0; i<FILAS;i++)
	 	for (j=0; j<COL; j++)
 			m[i][j]=' ';

}

bool is_wall(char m[FILAS][COL], int newx, int newy){
    // Check if the coordinates are out of bounds or there is a wall
    if(newx < 0 || newx >= FILAS || newy < 0 || newy >= COL || m[newx][newy] == WALL || m[newx][newy] == 'X'){
        return true;
    }
    return false;
}

bool is_exit(char m[FILAS][COL], int newx, int newy){
    if(m[newx][newy]==EXIT_A || m[newx][newy]==EXIT_B || m[newx][newy]==EXIT_C){
        return true;
    }
    return false;
}

// function to add a demon to the array
void addDemon(int x, int y, Demon* demons, int *numDemons, int *capacity) {
    if (*numDemons < *capacity) {
        demons[*numDemons].x = x;
        demons[*numDemons].y = y;
        (*numDemons)++;
    } else {
        *capacity *= 2;
        demons = realloc(demons, *capacity * sizeof(Demon));
        demons[*numDemons].x = x;
        demons[*numDemons].y = y;
        (*numDemons)++;
    }
}

void drawmap(char m[FILAS][COL], Demon* demons, int* numDemons, int* capacity){

	int i, f, c, dx, dy;
	int p = FILAS*COL*0.25;

	srand(time(NULL));

    //generate walls
	for(i=0; i<p; i++){
		f=rand()%FILAS;
		c=rand()%COL;
		if(m[f][c]==' ' && c != COL-2) //c != COL-2 is used to avoid creating a wall near an exit
			m[f][c]=WALL;
		else
			i--;
	}

	// Add exits to the board
    // Place exit A at the top right corner
    m[0][COL-1] = EXIT_A;
    // Place exit B at the center right corner
    m[FILAS/2][COL-1] = EXIT_B;
    // Place exit C at the bottom right corner
    m[FILAS-1][COL-1] = EXIT_C;

     //Add demons to the board
	for(int i = 0; i<NUM_DEMONS; i++){
        do{
            dx=rand()%FILAS;
            dy=rand()%COL;
        }while(is_wall(m,dx,dy) && m[dx][dy]== DEMON && m[dx][dy] == SHIELD && is_exit(m,dx,dy));
         addDemon(dx,dy,demons,numDemons,capacity); //add demon to the array
         m[dx][dy]=DEMON; //place demon
	}


}
void printboard(char m[FILAS][COL]){
 	int i, j;
 	bool exit;
    system("cls");

    // Print the top border
    for (i = 0; i < COL + 4; i++) {
        printf("*");
    }
    printf("\n");

    // Print the map with left and right borders
    for (i = 0; i < FILAS; i++) {
        printf("*");
        for (j = 0; j < COL; j++) {
            if ( (i == 0 && j == COL-2) || (i == FILAS/2 && j == COL-2) || (i == FILAS-1 && j == COL-2)  ) {
                printf("  %c  ", m[i][j]);
            } else {
                printf("%c", m[i][j]);
            }
        }

        printf("*\n");
    }


    // Print the bottom border
    for (i = 0; i < COL + 4; i++) {
        printf("*");
    }

    printf("\n");
}

//function to desactivate shield
Shield desactivateShield() {
    Shield shield;
    shield.location.x = -1;
    shield.location.y = -1;
    shield.steps = 0;
    return shield;
}

//function to desactivate destroyer
Destroyer desactivateDestroyer() {
    Destroyer destroyer;
    destroyer.location.x = -1;
    destroyer.location.y = -1;
    destroyer.steps = 0;
    return destroyer;
}


void pickUpItems(char m[FILAS][COL],int x, int y,  Player* player, Items* items){
     switch(m[x][y]){
        case SHIELD:
            player->hasShield = true;
            items->shield.available = false;
            items->shield.location.x = -1;
            items->shield.location.y = -1;
            items->shield.timer = 0;
            break;
        case DESTROYER:
            player->hasDestroyer = true;
            items->destroyer.available = false;
            items->destroyer.location.x = -1;
            items->destroyer.location.y = -1;
            items->destroyer.timer = 0;
            break;
        case LIFE:
            player->lifes++;
            items->life.available = false;
            items->life.location.x = -1;
            items->life.location.y = -1;
            items->life.timer = 0;
            break;
        case WEAPON:
            player->hasWeapon = true;
            items->weapon.available = false;
            items->weapon.location.x = -1;
            items->weapon.location.y = -1;
            items->weapon.timer = 0;
            break;
     }

}
//function to move the player
void moveplayer(char m[FILAS][COL], char key, Player* player, Items* items, bool *win, bool *moved){
    int tempx=player->x, tempy=player->y;
    switch(key){
        case KEY_LEFT_DOWN:
            tempx++;
            tempy--;
            break;
        case KEY_DOWN:
            tempx++;
            break;
        case KEY_RIGHT_DOWN:
            tempx++;
            tempy++;
            break;
        case KEY_LEFT:
            tempy--;
            break;
        case KEY_RIGHT:
            tempy++;
            break;
        case KEY_LEFT_UP:
            tempx--;
            tempy--;
            break;
        case KEY_UP:
            tempx--;
            break;
        case KEY_RIGHT_UP:
            tempx--;
            tempy++;
            break;
    }

    if(!is_wall(m, tempx, tempy) && m[tempx][tempy] != DEMON || (player->hasDestroyer && tempx >= 0 && tempx <= FILAS && tempy >= 0 && tempy <= COL && m[tempx][tempy] != DEMON)){
        if(is_exit(m, tempx, tempy)) *win = true;

        *moved = true;


        // check if the player has picked up an item
        pickUpItems(m,tempx,tempy,player,items);

        m[player->x][player->y]=' ';
        m[tempx][tempy]=PLAYER;
        player->x=tempx;
        player->y=tempy;
        //count steps
        player->steps++;

        if(player->hasShield) items->shield.steps++;
        if(player->hasDestroyer) items->destroyer.steps++;
        if (items->shield.steps == 10){
            items->shield = desactivateShield();
            player->hasShield = false;
        }
        if (items->destroyer.steps == 10){
            items->destroyer = desactivateDestroyer();
            player->hasDestroyer = false;
        }
    }
}
//function to move demons after the user has moved
void moveAllDemons(char (*m)[COL], Demon* demons, int numDemons) {
    int tempx, tempy, choice;
    for (int i = 0; i < numDemons; i++) {
        tempx = demons[i].x;
        tempy = demons[i].y;

        srand(time(NULL));
        do {
            choice = rand() % 9 + 1; //generate a random number between 1-9 excluding 5 because it's an invalid move
        } while(choice == 5);

        switch(choice) {
            case 1:
                tempx++;
                tempy--;
                break;
            case 2:
                tempx++;
                break;
            case 3:
                tempx++;
                tempy++;
                break;
            case 4:
                tempy--;
                break;
            case 6:
                tempy++;
                break;
            case 7:
                tempx--;
                tempy--;
                break;
            case 8:
                tempx--;
                break;
            case 9:
                tempx--;
                tempy++;
                break;
        }

        if (!is_wall(m, tempx, tempy) && m[tempx][tempy] != DEMON && m[tempx][tempy] != SHIELD && !is_exit(m,tempx,tempy) && m[tempx][tempy] != DESTROYER && m[tempx][tempy] != LIFE && m[tempx][tempy] != WEAPON && m[tempx][tempy] != PLAYER) {
            m[demons[i].x][demons[i].y] = ' ';
            m[tempx][tempy] = DEMON;
            demons[i].x = tempx;
            demons[i].y = tempy;
        }
    }
}

//check if there's a clear path between demon and player
bool isClearPath(char m[FILAS][COL], int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1); //gives the horizontal distance between the two points
    int dy = abs(y2 - y1); //gives the vertical distance
    //determine the direction of movement
    int sx = (x1 < x2) ? 1 : -1;
    // If x1 is less than x2, then sx is set to 1 which means that we need to move towards the right. If x1 is greater than x2, then sx is set to -1 which means that we need to move towards the left.
    int sy = (y1 < y2) ? 1 : -1;
    // if y1 is less than y2, then sy is set to 1 which means that we need to move downwards. If y1 is greater than y2, then sy is set to -1 which means that we need to move upwards.
    int err = dx - dy; //calculate the error or the deviation of the line from the ideal path(straight-line distance) at each step.
    int x = x1;
    int y = y1;

    while (true) {
        if (m[x][y] == WALL) {
            return false;
        }
        if(m[x][y] == 'X'){
            return false;
        }
        if (x == x2 && y == y2) {
            return true;
        }
        if (dx == dy) { // diagonal path
            x += sx;
            y += sy;
        } else if (err * 2 > -dy) { //determine whether to move horizontally or vertically on the next step when finding a clear path
            err -= dy;
            x += sx;
        } else if (err * 2 < dx) {
            err += dx;
            y += sy;
        }
    }
}

//function to check if demon can kill the player
bool demonKillPlayer(char m[FILAS][COL], int px, int py, bool hasShield) {
    int i,j;
    for (i = px - MAX_STEPS_TO_KILL; i <= px + MAX_STEPS_TO_KILL; i++) {
        for (j = py - MAX_STEPS_TO_KILL; j <= py + MAX_STEPS_TO_KILL; j++) {
            if (m[i][j] == DEMON && isClearPath(m,i,j,px,py) && !hasShield) {
                return true;
            }
        }
    }

    return false;
}

//function to destroy spaces using the weapon
void useWeapon(char (*m)[COL], int x, int y, Demon* demons, int* numDemons){
    bool found_demon = false;
    for(int i = x-WEAPON_MAX_SPACES; i <= x+WEAPON_MAX_SPACES; i++){
        for(int j = y-WEAPON_MAX_SPACES; j <= y+WEAPON_MAX_SPACES; j++){
            if (isClearPath(m,i,j,x,y) && !is_exit(m,i,j)){
                if(m[i][j] == DEMON){
                    m[i][j] = 'X';
                    for (int k = 0; k < *numDemons; k++) {
                        if (demons[k].x == i && demons[k].y == j) {
                            // Remove the demon from the array
                            for (int l = k; l < *numDemons - 1; l++) {
                                demons[l] = demons[l+1]; //shift all the elements in the demons array to the left
                            }
                            (*numDemons)--;
                            break;
                        }
                    }
                    found_demon = true;
                    break;
                }
            }
        }
        if(found_demon) break;
    }
}

// function to create a shield
void createShield(char (*m)[COL], Items* items) {
    int sx, sy;
    //creating a shield
    do{
        sx = rand() % FILAS;
        sy = rand() % COL;
    }while(is_wall(m, sx, sy) || m[sx][sy] == SHIELD || m[sx][sy] == DEMON || is_exit(m, sx, sy) || m[sx][sy] == DESTROYER || m[sx][sy] == LIFE || m[sx][sy] == WEAPON);

    m[sx][sy] = SHIELD; //place shield
    items->shield.steps = 0;
    items->shield.location.x = sx;
    items->shield.location.y = sy;
    items->shield.timer = 0;
    items->shield.available = true;
}

//function to create a destructor
void createDestroyer(char (*m)[COL], Items* items){
    int bx, by;
    //creating a destroyer
    do{
        bx = rand() % FILAS;
        by = rand() % COL;
    }while(is_wall(m, bx, by) || m[bx][by] == SHIELD || m[bx][by] == DEMON || is_exit(m, bx, by) || m[bx][by] == DESTROYER || m[bx][by] == LIFE || m[bx][by] == WEAPON);

    items->destroyer.available = true;
    items->destroyer.location.x = bx;
    items->destroyer.location.y = by;
    items->destroyer.steps = 0;
    items->destroyer.timer = 0;
    m[bx][by] = DESTROYER; //place destroyer

}

//function to create a life
void createLife(char (*m)[COL], Items* items) {
        int x, y;
        do {
            x = rand() % FILAS;
            y = rand() % COL;
        }while(is_wall(m, x, y) || m[x][y] == SHIELD || m[x][y] == DEMON || is_exit(m, x, y) || m[x][y] == DESTROYER || m[x][y] == LIFE || m[x][y] == WEAPON);
        items->life.available = true;
        items->life.location.x = x;
        items->life.location.y = y;
        items->life.timer = 0;
        m[x][y] = LIFE; // place a life
}

//function to create a weapon
void createWeapon(char (*m)[COL], Items* items) {
        int x, y;
        do {
            x = rand() % FILAS;
            y = rand() % COL;
        }while(is_wall(m, x, y) || m[x][y] == SHIELD || m[x][y] == DEMON || is_exit(m, x, y) || m[x][y] == DESTROYER || m[x][y] == LIFE || m[x][y] == WEAPON);
        items->weapon.available = true;
        items->weapon.timer = 0;
        items->weapon.location.x = x;
        items->weapon.location.y = y;
        m[x][y] = WEAPON; // place a weapon
}


// function to print the number of steps and items the player has
void print_info(Player player){
    printf("Pasos: %d\tEscudo: %s\tDestructor: %s\tVidas: ", player.steps, player.hasShield ? "Si" : "No", player.hasDestroyer ? "Si" : "No");
    for(int i = 1; i<=player.lifes; i++){
        printf("%c",LIFE);
    }
}

//function to create items
void create_items(int steps, char (*m)[COL], Items* items){
    if(steps >= 10){
            //create a shield
            if(steps % 20 == 0) createShield(m,items);

            //create a destructor
            if(steps % 25 == 0) createDestroyer(m,items);

            //create a life
            if(steps % 30 == 0) createLife(m,items);

            //create a weapon
            if(steps % 40 == 0) createWeapon(m,items);
    }
}

//remove player items
void remove_items(char (*m)[COL], Items* items){
    //removing shield after 5 steps if it's not picked up
        if (items->shield.available) {
            items->shield.timer++;
            if (items->shield.timer > SHIELD_TIMER_MAX) { // remove shield if it's been available for too long
                m[items->shield.location.x][items->shield.location.y] = ' '; // replace the shield with a space
                items->shield.location.x = -1;
                items->shield.location.y = -1;
                items->shield.timer = 0; // reset the shield timer
                items->shield.available = false; // set the shield as not available
            }
        }

        //removing destroyer after 5 steps if it's not picked up
        if(items->destroyer.available){
            items->destroyer.timer++;
            if (items->destroyer.timer > DESTROYER_TIMER_MAX) { // remove shield if it's been available for too long
                m[items->destroyer.location.x][items->destroyer.location.y] = ' '; // replace the shield with a space
                items->destroyer.location.x = -1;
                items->destroyer.location.y = -1;
                items->destroyer.timer = 0; // reset the shield timer
                items->destroyer.available = false; // set the shield as not available
            }
        }

        //removing a life after 5 steps if it's not picked up
        if(items->life.available){
            items->life.timer++;
            if(items->life.timer > LIFE_TIMER_MAX) { // remove life if it's been available for too long
                m[items->life.location.x][items->life.location.y] = ' '; // replace the shield with a space
                items->life.location.x = -1;
                items->life.location.y = -1;
                items->life.timer = 0; // reset the shield timer
                items->life.available = false; // set the shield as not available
            }
        }

        //removing a weapon after 5 steps if it's not picked up
        if(items->weapon.available){
            items->weapon.timer++;
            if(items->weapon.timer > WEAPON_TIMER_MAX) { // remove weapon if it's been available for too long
                m[items->weapon.location.x][items->weapon.location.y] = ' '; // replace the shield with a space
                items->weapon.location.x = -1;
                items->weapon.location.y = -1;
                items->weapon.timer = 0; // reset the shield timer
                items->weapon.available = false; // set the shield as not available
            }
        }
}

int main()
{
	char m[FILAS][COL], key;
	bool win = false, invalid_move = false, killed = false, moved = false;
    int dx,dy;

    // create instance of Demon struct, demons array
    int numDemons = 0;
    int capacity = NUM_DEMONS;
    Demon* demons = malloc(NUM_DEMONS * sizeof(Demon));  // allocate memory for demons

    clear(m);
	drawmap(m,demons,&numDemons,&capacity);
    srand(time(NULL)); //returns the current time, which is used to seed the random number generator

    Player player = {FILAS/2,0,0,1,false,false,false};// player starts at center left corner
    m[player.x][player.y]=PLAYER; //place player

    //creating instances of the items
    Items items;
    printboard(m);

    while(!win && !killed){
        print_info(player);
        do{
            key=getch();
            //the user can't make invalid moves
            if(key < '1' || key > '9' || key=='5'){
                if(!invalid_move){ // check if message has already been displayed
                    printf("\tMovimiento invalido");
                    invalid_move = true;
                }
            }
        }while(key < '1' || key > '9' || key=='5');

        invalid_move = false;

        //allow the player to move
        moveplayer(m, key, &player,&items,&win,&moved);
        //move all demons after the player has moved
        if(moved) moveAllDemons(m,demons,numDemons);

        //check if player has a weapon to kill a demon
        if(player.hasWeapon){
            useWeapon(m,player.x,player.y,demons,&numDemons);
            player.hasWeapon = false;
        }

        //check if there's a demon within 3 steps
        if(demonKillPlayer(m,player.x,player.y,player.hasShield)) player.lifes--;

        //check if it has at least one life left
        if(player.lifes < 1) killed = true;

        //create another demon after 10 steps
        if(player.steps > 0 && player.steps % 10 == 0){
            do{
                dx=rand()%FILAS;
                dy=rand()%COL;
            }while(is_wall(m,dx,dy) && m[dx][dy]==DEMON && m[dx][dy]==SHIELD && m[dx][dy]==DESTROYER && m[dx][dy]==WEAPON && m[dx][dy]==LIFE && is_exit(m,dx,dy));
                addDemon(dx,dy,demons,&numDemons,&capacity); //add demon
                m[dx][dy]=DEMON; //place demon
        }

        if(moved){
            //create items on screen
            create_items(player.steps,m,&items);

            //remove items from screen
            remove_items(m,&items);
        }

        moved = false;

        //print board again
        printboard(m);
    }
    printf("Pasos: %d", player.steps);
    if(win) printf("\tGanaste!\n");
    if(killed) printf("\tPerdiste\n");
    return 0;
}
