#ifndef ANIMALS_H
#define ANIMALS_H

#include <stdio.h>   /* printf(), puts() */
#include <stdlib.h>  /* calloc(), free(), rand() */
#include <string.h>  /* memcopy() */
#include <stdbool.h> /* bool, true, false */

#define ANIM_STARTING_HEALTH   100
#define ANIM_MAX_DISTANCE      100
#define ANIM_MOVE_OPTIONS        8
#define ANIM_CANATT_DIST        15 /* max dist from where animals can attack */

/* damage that animal causes to player */
#define ANIM_DAMAGE(att) ((att) * (1.0/5.0) + \
                          ((rand() % 5) + 1) - ((rand() % 5) + 1))

/* distance that animal go close/away to/from  player */
#define ANIM_DISTMOVE(spd) ((spd)* (1.0/6.0) + \
                            (((rand() % 5) + 1) - ((rand() % 5) + 1)))


enum animalid {
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
	MAX_ANIMALTYPES  /* their total count */
};

enum animalmood {
	ANIM_SCARED = 0,
	ANIM_NEUTRAL,
	ANIM_AGGRESSIVE,
	MAX_MOOD	 /* their total count */
};

enum animalmove {
	ANIMOVE_NTH = 0,
	ANIMOVE_ATT,     /* attack player */
	ANIMOVE_CLS,     /* go close to player */
	ANIMOVE_AWAY,    /* go away from player */
	MAX_ANIMMOVES    /* their total count */
};


struct animal {
	enum animalid  id;
	char  *name;
	int    attack;    /* 0 to 100 */
	int    defense;   /* 0 to 100 */
	int    speed;     /* 0 to 100 */
	int    value;     /* xp and gold that player gains when capture it */
                          /* 0 to 100 */
};

struct sceneanimal {
	int             id;        /* for identification in the scene */
	struct animal   type;
	enum animalmood mood;
	int             health;
	int             distance;  /* distance from player */
};

struct node {
	struct sceneanimal  animal;
	struct node         *next;
};

struct listanimals {
	struct node  *head;
	struct node  *tail;
	int           len;
	int           idcount;  /* count added animals */
};


bool animals_add(struct listanimals *list, struct animal animtable[]);
void animals_delete(struct listanimals *list, int id);
void animals_deleteall(struct listanimals *list);
struct sceneanimal *animals_find(struct node *list, int id);
void animals_look(struct listanimals list);
enum animalmove animals_decision(struct sceneanimal animal);

#endif

