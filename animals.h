#ifndef ANIMALS_H
#define ANIMALS_H

#include <stdio.h>   // printf(), puts()
#include <stdlib.h>  // calloc(), free(), rand()
#include <string.h>  // memcopy()
#include <stdbool.h> // bool, true, false

#define STARTING_ANIM_HEALTH   100
#define MAX_ANIM_DISTANCE      100

typedef enum {
    ANIM_INVALID = -1,
    ANIM_LION,
    ANIM_TIGER,
    ANIM_CHEETAH,
    ANIM_WOLF,
    ANIM_BEAR,
    ANIM_ELEPHANT,
    ANIM_BOAR,
    ANIM_ALLIGATOR,
    ANIM_PYTHON,
    ANIM_FOX,
    ANIM_DEER,
    ANIM_ZEBRA,
    MAX_ANIMALTYPES // their total count
} AniTypeId;

typedef enum {ANIM_SCARED = 0,
              ANIM_NEUTRAL,
              ANIM_AGGRESSIVE,
              MAX_MOOD // their total count
} AniMood;

typedef enum {
    ANIMOVE_NOTHING = 0,
    ANIMOVE_ATTACK,
    ANIMOVE_CLOSE,  // go close to player
    ANIMOVE_GOAWAY, // go away from player
    MAX_ANIMMOVES   // their total count
} AniMove;


typedef struct {
    AniTypeId  id;
    char      *name;
    int        attack;    // 0 to 100
    int        defense;   // 0 to 100
    int        speed;     // 0 to 100
    int        value;     // xp and gold that player gains when capture
                          // such animal --- 0 to 100
} AnimalType;

typedef struct {
    int             id;       // for identification in the scene
    AnimalType      type;
    int             health;
    AniMood         mood;
    int             distance; // distance from player
} SceneAnimal;

typedef struct node {
    SceneAnimal   animal;
    struct node  *next;
} Node;

typedef struct {
    Node  *head;
    Node  *tail;
    int    len;
    int    idcount; // count added animals
} List;


/********************************************************************
 * animals_addanimal: Adds a new Node to list and increase its len
 *                    field by 1. Returns true if Node stored properly,
 *                    else false.
 ********************************************************************/
bool animals_addanimal(List *list, AnimalType animtable[]);

/********************************************************************
 * animals_kill: Deletes a Node from list and decrease its len field
 *               by 1.
 ********************************************************************/
void animals_kill(List *list, int id);

/********************************************************************
 * animals_killall: Destroys a linked list. Deallocates each of
 *                  its nodes.
 ********************************************************************/
void animals_killall(Node *list);

/********************************************************************
 * animals_find: Search each node of a linked list for a SceneAnimal
 *               with the corresponding id. Returns a pointer to the
 *               node that contains that animal if found, else NULL.
 ********************************************************************/
SceneAnimal *animals_find(Node *list, int id);

/********************************************************************
 * animals_look: Displays info about each Node of the list.
 ********************************************************************/
void animals_look(List list);

/********************************************************************
 * animals_attack: Returns the amount of damage that animal causes.
 ********************************************************************/
int animals_attack(SceneAnimal *animal);

/********************************************************************
 * animals_goclose: Decreases animal's distance from the player.
 ********************************************************************/
void animals_goclose(SceneAnimal *animal);

/********************************************************************
 * animals_goaway: Increases animal's distance from the player.
 ********************************************************************/
void animals_goaway(SceneAnimal *animal);

#endif
