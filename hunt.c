#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mylibrary.h"
#include "animals.h"
#include "weapons.h"

#define MAX_INPUT              255

#define STARTING_ANIMALS         5
#define ADD_ANIM_ROUNDS          5  // add animal after each x rounds

#define MAX_CMD_ALIASES          2
#define MAX_CMD_PARAMS           3

typedef enum {
    CMD_INVALID = -1,
    CMD_SHOOT,
    CMD_LOOK,
    CMD_BUY,
    CMD_STATUS,
    CMD_HELP,
    CMD_EXIT,
    MAX_COMMANDS // their total count
} ComTypeId;

typedef struct {
    ComTypeId   id;
    char        *aliases[MAX_CMD_ALIASES];
    int         nparams;  // num of params
    char        *params[MAX_CMD_PARAMS];
} CommandType;


typedef struct {
    int         health; // 0 to 100
    int         xp;
    int         gold;
    int         bullets;
    WeaponType  weapon;
    AniTypeId   capedanimals[MAX_ANIMALTYPES]; // captured animals
} Player;


void player_status(Player player)
{
    printf("Health: %d, XP: %d, Gold: %d\n"
           "Weapon: %s (attack=%d, distance=%d, value=%d), Bullets: %d\n",
           player.health, player.xp, player.gold, player.weapon.name,
           player.weapon.attack, player.weapon.distance,
           player.weapon.value, player.bullets);
}

void shoot(CommandType command, List *list, Player *player)
{    
    SceneAnimal *animal = NULL;    
    int id = strtol(command.params[0], NULL, 10), damage;
    
    if (command.params[command.nparams] != NULL) {
        puts("You can't shoot two animals at once.");
        return;
    }
    
    if (id == 0 || (animal = animals_find(list->head, id)) == NULL) {
        printf("%s is not a valid animal id\n", command.params[0]);
        return;
    }
    
    damage = player->weapon.attack + 50 - animal->type.defense;
    // a divergence 0 to 10 from the standard
    damage += ((rand() % 5) + 1) - ((rand() % 5) + 1);    
    animal->health -= damage;
    
    if (animal->health <= 0) {
        printf("The %s is dead! You gain %d gold and xp!\n", 
            animal->type.name, animal->type.value);
        player->gold += animal->type.value;
        player->xp += animal->type.value;
        animals_kill(&list->head, id);        
    };
}

ComTypeId lookup_alias(CommandType cmdtable[], char *alias)
{
    if (alias == NULL)
        return CMD_INVALID;

    for (register int i = 0; i < MAX_COMMANDS; i++) {
        for (register int y = 0; y < MAX_CMD_ALIASES; y++)
            if (cmdtable[i].aliases[y] != NULL &&
                !strcmp(cmdtable[i].aliases[y], alias))
                return cmdtable[i].id;
    }
    return CMD_INVALID;
}

void parser(char *input, CommandType *command, CommandType cmdtable[])
{
    ComTypeId ret;
    char *tokens[MAX_CMD_PARAMS+1] = {NULL};
    int ntokens = 0;

    s_tolower(input);
    ntokens = s_tokenize(input, tokens, MAX_CMD_PARAMS+1, " \n");

    ret = lookup_alias(cmdtable, tokens[0]);
    if (ret == CMD_INVALID) {
        command->id = CMD_INVALID;
        return;
    }

    memcpy(command, &cmdtable[ret], sizeof(CommandType));
    for (register int i = 0; i < (ntokens - 1); i++)
        command->params[i] = tokens[i+1];
}


int main(void)
{
    AnimalType animtable[MAX_ANIMALTYPES] = {
        // .id            .name      .attack  .defense .value
        {ANIM_LION,      "lion",       100,      90,    100},
        {ANIM_TIGER,     "tiger",       95,      80,     90},
        {ANIM_CHEETAH,   "cheetah",     95,      80,     90},
        {ANIM_WOLF,      "wolf",        90,      70,     80},
        {ANIM_BEAR,      "bear",        75,      75,     75},
        {ANIM_ELEPHANT,  "elephant",    70,      80,     75},
        {ANIM_BOAR,      "boar",        90,      55,     70},
        {ANIM_ALLIGATOR, "alligator",   65,      50,     60},
        {ANIM_PYTHON,    "python",      60,      55,     60},
        {ANIM_FOX,       "fox",         55,      65,     60},
        {ANIM_DEER,      "deer",        50,      70,     60},
        {ANIM_ZEBRA,     "zebra",       50,      65,     55}
    };

    WeaponType weaptable[MAX_WEAPONS] = {
        // .id             .name    .attack  .distance .value
        {WEAP_SHOTGUN,   "Shotgun",   100,      100,     200},
        {WEAP_RIFLE,     "Rifle",      85,       55,     100},
        {WEAP_HANDGUN,   "Handgun",    65,       30,      50},
        {WEAP_SLING,     "Sling",      50,       10,       0}
    };

    CommandType cmdtable[MAX_COMMANDS] = {
        // .id             .aliases     .nparams        .params
        {CMD_SHOOT,    {"shoot",  NULL},    1,    {NULL, NULL, NULL}},
        {CMD_LOOK,     {"look",   NULL},    0,    {NULL, NULL, NULL}},
        {CMD_BUY,      {"buy",    NULL},    3,    {NULL, NULL, NULL}},
        {CMD_STATUS,   {"status", NULL},    0,    {NULL, NULL, NULL}},
        {CMD_HELP,     {"help",    "?"},    0,    {NULL, NULL, NULL}},
        {CMD_EXIT,     {"exit", "quit"},    0,    {NULL, NULL, NULL}}
    };

               // .health   .xp  .gold .bullets .weapon  .capendanimals
    Player player = {100,    0,    100,    0,  weaptable[3],   {0}};
    List animals = {NULL, NULL, 0, 0};

    char input[MAX_INPUT+1];
    int rounds = 0;
    CommandType command = {CMD_INVALID, {NULL, NULL}, 0, {NULL, NULL, NULL}};

    srand((unsigned) time(NULL));

    for (register int i = 0; i < STARTING_ANIMALS; i++)
        animals_addanimal(&animals, animtable);

    for (;;) {
        printf(">>> ");
        fgets(input, sizeof(input), stdin);
        parser(input, &command, cmdtable);

        switch (command.id) {
            case CMD_SHOOT: shoot(command, &animals, &player); break;
            case CMD_LOOK: animals_look(animals); break;
            case CMD_BUY: puts("cmdbuy"); break;
            case CMD_STATUS: player_status(player); continue; break;
            case CMD_HELP: puts("cmdhelp"); continue; break;
            case CMD_EXIT: puts("cmdexit"); goto exit_success; break;
            case CMD_INVALID: default: puts("Unkown command"); continue; break;
        }

        //ANIMALS TURN
        
        rounds++;
        if (!(rounds % ADD_ANIM_ROUNDS))
            if (animals_addanimal(&animals, animtable))
                puts("\nBe careful! Α new animal appeared from nowhere!");
    }

exit_success:
    animals_killall(animals.head);
    exit(EXIT_SUCCESS);
}
