#ifndef ANIMALS_H
#define ANIMALS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

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


typedef struct {
    AniTypeId  id;
    char      *name;
    int        attack;    // 0 to 100
    int        defense;   // 0 to 100
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

bool animals_addanimal(List *list, AnimalType animtable[]) ;
void animals_look(List list);
void animals_destroy(Node *list);

#endif
