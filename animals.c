#include "animals.h"

static bool add_node(Node **list, Node **tail, AnimalType animtable[], int id)
{
    Node *new_node = calloc(1, sizeof(Node));
    if (new_node == NULL)
        return false; // out of memory

    new_node->next = NULL;
    new_node->animal.id = id;
    new_node->animal.type.id = rand() % MAX_ANIMALTYPES;
    memcpy(&new_node->animal.type, &animtable[new_node->animal.type.id], sizeof(AnimalType));
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

void animals_kill(Node **list, int id)
{
    Node *cur = *list, *prev = NULL;

    for (; cur != NULL && cur->animal.id < id;
           prev = cur, cur = cur->next)
        ; // empty loop

    if (cur == NULL || cur->animal.id != id)
        return; // not found

    if (prev == NULL)
        *list = cur->next;
    else
        prev->next = cur->next;
    free(cur);
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
    printf("The following %d animals are on the scene now: \n\n", list.len);
    for (Node *an = list.head; an != NULL; an = an->next) {
        printf("%d:%s  (health=%d att=%d def=%d value=%d dist=%d)\n",
            an->animal.id, an->animal.type.name,
            an->animal.health, an->animal.type.attack,
            an->animal.type.defense, an->animal.type.value,
            an->animal.distance);
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
