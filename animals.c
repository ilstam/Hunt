/*
 * Copyright (C) 2012 Ilias Stamatis <stamatis.iliass@gmail.com>
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

static bool add_node(Node **list, Node **tail, AnimalType animtable[], int id)
{
    Node *new_node = calloc(1, sizeof(Node));
    if (new_node == NULL)
        return false; // out of memory

    new_node->next = NULL;
    new_node->animal.id = id;
    new_node->animal.type.id = rand() % MAX_ANIMALTYPES;
    memcpy(&new_node->animal.type, &animtable[new_node->animal.type.id], 
                                                    sizeof(AnimalType));
    new_node->animal.health = STARTING_ANIM_HEALTH;
    new_node->animal.mood = rand () % MAX_MOOD;
    new_node->animal.distance = rand () % MAX_ANIM_DISTANCE;

    if (*list == NULL) {
        *list = *tail = new_node;
        return true;
    }

    (*tail)->next = new_node;
    *tail = new_node;

    return true;
}

bool animals_addanimal(List *list, AnimalType animtable[])
{
    list->len++;
    return add_node(&list->head, &list->tail, animtable, ++list->idcount);
}

static void delete_node(Node **list, Node **tail, int id)
{
    Node *cur = *list, *prev = NULL;

    for (; cur != NULL && cur->animal.id < id;
           prev = cur, cur = cur->next)
        ; // empty loop

    if (cur == NULL || cur->animal.id != id)
        return; // not found

    if (prev == NULL)
        *list = cur->next;
    else  {
        prev->next = cur->next;
        if (cur->next == NULL)
            *tail = prev;
    }
    free(cur);
}

void animals_kill(List *list, int id)
{
    list->len--;
    delete_node(&list->head, &list->tail, id);
}

void animals_killall(Node *list)
{
    Node *dummy = NULL;
    for (; list != NULL; list = dummy) {
        dummy = list->next;
        free(list);
    }
}

void animals_look(List list)
{
    if (!list.len) {
        puts("There are no animals on the scene.");
        return;
    }
    
    printf("The following %d animals are on the scene now: \n\n", list.len);
    for (Node *an = list.head; an != NULL; an = an->next) {
        printf("%d:%s           \t(hlth=%d att=%d def=%d spd=%d dist=%d mood=",
            an->animal.id, an->animal.type.name,
            an->animal.health, an->animal.type.attack,
            an->animal.type.defense, an->animal.type.speed, an->animal.distance);
        switch (an->animal.mood) {
            case ANIM_SCARED: puts("scared)"); break;
            case ANIM_NEUTRAL: puts("neutral)"); break;
            case ANIM_AGGRESSIVE: puts("aggressive)"); break;
            default: break;
        }
    }
}

SceneAnimal *animals_find(Node *list, int id)
{
    Node *anim = list;
    for (; anim != NULL && anim->animal.id < id; anim = anim->next)
        ; // empty loop

    if (anim != NULL && anim->animal.id == id)
        return &anim->animal;
    return NULL;
}
