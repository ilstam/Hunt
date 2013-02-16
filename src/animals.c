/*
* Copyright (C) 2012-2013 Ilias Stamatis <stamatis.iliass@gmail.com>
*
* This file is part of Hunt.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "animals.h"


/*
 * animals_add:
 *    Add a new node in an ordered (by id) linked list.
 *    Give node fields semi-random values and increase len of list.
 *
 *    Return true if node stored properly, else false.
 */
bool animals_add(struct listanimals *list, struct animal animtable[])
{
	struct node *new_node;

	if ((new_node = malloc(sizeof(*new_node))) == NULL)
		return false;  /* out of memory */

	new_node->next = NULL;
	new_node->animal.id = ++list->idcount;
	new_node->animal.type.id = rand() % MAX_ANIMALTYPES; /* random value */
	/* copy attributes of specific animal type */
	memcpy(&new_node->animal.type,
	       &animtable[new_node->animal.type.id], sizeof(*animtable));
	new_node->animal.health = ANIM_STARTING_HEALTH;
	new_node->animal.mood = rand () % MAX_MOOD;              /* random */
	new_node->animal.distance = rand () % ANIM_MAX_DISTANCE; /* random */

	list->len++;
	if (list->head == NULL) {
		list->head = list->tail = new_node;
		return true;
	}

	list->tail->next = new_node;
	list->tail = new_node;

	return true;
}

/*
 * animals_delete:
 *    Delete the node with the corresponding id from an ordered (by id)
 *    linked list and decrease len of list.
 */
void animals_delete(struct listanimals *list, int id)
{
	struct node *cur = list->head, *prev = NULL;

	for (; cur != NULL && cur->animal.id < id;
	     prev = cur, cur = cur->next)
		; /* void */

	if (cur == NULL || cur->animal.id != id)
		return; /* not found */

	list->len--;
	if (prev == NULL) {
		list->head = cur->next;
	} else  {
		prev->next = cur->next;
		if (cur->next == NULL)
			list->tail = prev;
	}
	free(cur);
}

/*
 * animals_deleteall:
 *    Destroy a linked list by deallocating all of its nodes.
 */
void animals_deleteall(struct listanimals *list)
{
	struct node *dummy = NULL;

	for (; list->head != NULL; list->head = dummy) {
		dummy = list->head->next;
		free(list->head);
	}
	list->len = 0;
}

/*
 * animals_find:
 *    Search an ordered (by id) linked list for an animal with the
 *    corresponding id. Return a pointer to the node that contain
 *    requested animal if found, else NULL.
 */
struct sceneanimal *animals_find(struct node *list, int id)
{
	struct node *anim = list;
	for (; anim != NULL && anim->animal.id < id; anim = anim->next)
		; /* void */

	if (anim != NULL && anim->animal.id == id)
		return &anim->animal;
	return NULL;
}

/*
 * animals_look:
 *    Check if there are animals on the list.
 *    If so, display information of each animal.
 */
void animals_look(struct listanimals list)
{
	struct node *an;

	if (!list.len) {
		puts("There are no animals on the scene.");
		return;
	}

	printf("The following %d animals are on the scene now: \n\n", list.len);
	for (an = list.head; an != NULL; an = an->next) {
		printf("%d:%s           \t"
		       "(hlth=%d att=%d def=%d spd=%d dist=%d mood=",
		       an->animal.id, an->animal.type.name, an->animal.health,
		       an->animal.type.attack, an->animal.type.defense,
		       an->animal.type.speed, an->animal.distance);

		switch (an->animal.mood) {
		case ANIM_SCARED:
			puts("scared)");
			break;
		case ANIM_NEUTRAL:
			puts("neutral)");
			break;
		case ANIM_AGGRESSIVE:
			puts("aggressive)");
			break;
		default:
			break;
	    }
	}
}

/*
 * animals_decision:
 *    Return a random choise of animal move depending on its mood and
 *    its position.
 */
enum animalmove animals_decision(struct sceneanimal animal)
{
	int random = rand() % ANIM_MOVE_OPTIONS; /* random choise */
	bool canatt = animal.distance <= ANIM_CANATT_DIST ? true : false;

	#define nth   ANIMOVE_NTH
	#define att   ANIMOVE_ATT
	#define cls   ANIMOVE_CLS
	#define away  ANIMOVE_AWAY

	switch(animal.mood) {
	case ANIM_AGGRESSIVE:
		if (canatt)
			return ((enum animalmove []) {att, att, att, att, att,
			        cls, away, nth})[random];
		else
			return ((enum animalmove []) {cls, cls, cls, cls, cls,
			        cls, away, nth})[random];
		break;
	case ANIM_SCARED:
		if (canatt)
			return ((enum animalmove []) {away, away, away, away,
			        away, att, cls, nth})[random];
		else
			return ((enum animalmove []) {away, away, away, away,
			        away, away, cls, nth})[random];
		break;
	case ANIM_NEUTRAL:
		if (canatt)
			return ((enum animalmove []) {att, att, cls, cls, away,
			        away, nth, nth})[random];
		else
			return ((enum animalmove []) {cls, cls, cls, away,away,
			        away, nth, nth})[random];
		break;
	default: /* to satisfy compiler -- not used */
		return 0;
		break;
	}
}

