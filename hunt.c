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

#include <stdio.h>     // printf(), puts(), fgets(), putchar()
#include <stdlib.h>    // srand(), rand(), system(), strtol(), exit()
#include <string.h>    // strcmp(), memcpy()
#include <stdbool.h>   // bool, true, false
#include <time.h>      // time()
#include <locale.h>    // setlocale()
#include <libintl.h>   // gettext(), bindtextdomain(), textdomain()

#include "mylibrary.h" // s_tolower(), s_tokenize()
#include "animals.h"   // animals_*()
#include "weapons.h"
#include "info.h"

#define PACKAGE                "hunt"
#define LOCALEDIR              "/usr/share/locale"
#define _(string)              gettext(string)

#define MAX_INPUT              255

#define STARTING_ANIMALS         5
#define ADD_ANIM_ROUNDS          4  // add new animal after each x rounds

#define DRUG_HEALTH             15
#define DRUG_COST               20
#define BULLET_COST              5

#define MAX_CMD_ALIASES          2
#define MAX_CMD_PARAMS           2


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


/********************************************************************
 * shoot: Checks is commands' params is a valid animal id. If so,
 *        and player has at least 1 bullet, shoots the animal.
 ********************************************************************/
bool shoot(CommandType command, List *list, Player *player)
{
    SceneAnimal *animal = NULL;
    int id, damage;

    if (command.params[0] == NULL) {
        puts(_("Please be more specific."));
        return false;
    }

    if (command.params[command.nparams] != NULL) {
        puts(_("You can't shoot two animals at once."));
        return false;
    }

    id = strtol(command.params[0], NULL, 10);
    if (id == 0 || (animal = animals_find(list->head, id)) == NULL) {
        printf(_("%s is not a valid animal id\n"), command.params[0]);
        return false;
    }

    if (!player->bullets) {
        puts(_("I have no bullets! I can't shoot!"));
        return false;
    }

    puts("BOOM!");
    player->bullets--;
    if (player->weapon.distance < animal->distance)
        return true;

    damage = player->weapon.attack + 50 - animal->type.defense;
    // a divergence 0 to 10 from the standard
    damage += ((rand() % 5) + 1) - ((rand() % 5) + 1);
    animal->health -= damage;

    if (animal->health <= 0) {
        printf(_("The %s is dead! You earn %d gold!\n"),
            animal->type.name, animal->type.value);
        player->gold += animal->type.value;
        player->xp   += animal->type.value;
        player->capedanimals[animal->type.id]++;
        animals_kill(list, id);
    };
    return true;
}

/********************************************************************
 * buy: Checks commands' params and does the corresponding actions.
 ********************************************************************/
void buy(CommandType command, Player *player, WeaponType weaptable[])
{
    char input[MAX_INPUT+1], *tokens[2] = {NULL};
    int i, num, cost;
    bool found;

    if (command.params[0] == NULL) {
        puts(_("What do you want to buy?"));
        return;
    }

    if (!strcmp(command.params[0], "bullets") ||
        !strcmp(command.params[0], "drugs")   ||
        !strcmp(command.params[0], "weapon")) {

        if (command.params[1] == NULL) {
            puts(_("Be more specific."));
            return;
        }

        if (!strcmp(command.params[0], "weapon")) {
            for (i = 0; i < MAX_WEAPONS; i++)
                if (! strcmp(weaptable[i].name, command.params[1])) {
                    found = true;
                    break;
                }
            if (!found) {
                puts(_("There is no weapon with such name."));
                return;
            }
            cost = weaptable[i].value;
        }
        else {
            num = strtol(command.params[1], NULL, 10);
            if (num <= 0) {
                printf(_("%s is not a valid positive integer.\n"), command.params[1]);
                return;
            }
            cost = !strcmp(command.params[0], "bullets") ? BULLET_COST:DRUG_COST;
            cost *= num;
        }

        printf(_("You need %d gold in order to buy this stuff.\n"), cost);
        if (player->gold < cost) {
            puts(_("Sorry, you don't have enough gold."));
            return;
        }

        printf(_("Are you sure that you want to proceed? (y/n) "));
        fgets(input, sizeof(input), stdin);
        s_tokenize(input, tokens, 2, " \n");

        if (tokens[0] == NULL || tokens[1] != NULL ||
                (strcmp(s_tolower(tokens[0]), "y") &&
                strcmp(s_tolower(tokens[0]), "yes"))) {
            puts(_("canceled"));
            return;
            }

        if (!strcmp(command.params[0], "bullets")) {
            player->gold -= cost;
            player->bullets += num;
        }
        else if (!strcmp(command.params[0], "drugs")){
            player->gold -= cost;
            player->health += num * DRUG_HEALTH;
            if (player->health > 100)
                player->health = 100;
        }
        else {
            player->gold -= cost;
            memcpy(&player->weapon, &weaptable[i], sizeof(WeaponType));
        }
        puts(_("Done!"));
        return;
    }
    printf(_("You can't buy %s.\n"), command.params[0]);
}

/********************************************************************
 * player_status: Displays player's attributes.
 ********************************************************************/
void player_status(Player player)
{
    printf(_("Health: %d, XP: %d, Gold: %d\n"
           "Weapon: %s (attack=%d, distance=%d), Bullets: %d\n"),
           player.health, player.xp, player.gold, player.weapon.name,
           player.weapon.attack, player.weapon.distance, player.bullets);
}

/********************************************************************
 * help: If called without parameter, displays a help text. If called
 *       with animals or weapons as a parameter then prints a table of
 *       all animals or weapons.
 ********************************************************************/
void help(CommandType command, AnimalType animtable[], WeaponType weaptable[])
{
    if (command.params[0] == NULL) {
        puts(HELP_MSG);
        return;
    }

    register int i;

    if (! strcmp(command.params[0], "animals")) {
        puts(_("name    \tatt\tdef\tspd\tval"));
        puts("--------------------------------------------");
        for (i = 0; i < MAX_ANIMALTYPES; i++)
            printf("%s     \t%3d\t%3d\t%3d\t%3d\n", animtable[i].name,
            animtable[i].attack, animtable[i].defense,
            animtable[i].speed, animtable[i].value);
    }

    else if (! strcmp(command.params[0], "weapons")) {
        puts(_("name    \tatt\tdist\t val"));
        puts("-------------------------------------");
        for (i = 0; i < MAX_WEAPONS; i++)
            printf("%s     \t%3d\t%3d\t%4d\n", weaptable[i].name,
            weaptable[i].attack, weaptable[i].distance, weaptable[i].value);
    }

    else
        printf(_("%s: invalid parameter\n"), command.params[0]);
}

/********************************************************************
 * exit_msg: Displays players' achievements.
 ********************************************************************/
void exit_msg(Player player, AnimalType animtable[], int rounds)
{
    printf(_("\nThis adventure is over.\nYou've made %d xp points and died "
           "holding a %s after %d rounds.\n"),
           player.xp, player.weapon.name, rounds);

    for (register int y = 0; y < MAX_ANIMALTYPES; y++)
        if (player.capedanimals[y] > 0)
            goto print_capedanimals;
    puts(_("\nYou haven't captured any animals.\n"));
    return;

print_capedanimals:
    printf(_("\nYou have also captured: "));
    for (register int i = 0; i < MAX_ANIMALTYPES; i++)
        if (player.capedanimals[i] > 0) {
            printf("%d %s", player.capedanimals[i], animtable[i].name);
            if (player.capedanimals[i] > 1)
                putchar('s'); // plural
            printf(", ");
        }
    puts("\n");
}

/********************************************************************
 * animal_move: Chooses and plays a move for a SceneAnimal. The choice
 *              is random but depends on animal's mood too.
 ********************************************************************/
void animal_move(Player *player, SceneAnimal *animal)
{
    AniMove move;
    int random = rand() % 8; // move choices are 8
    bool canattack = animal->distance <= 15 ? true : false;

    #define nth   ANIMOVE_NOTHING
    #define att   ANIMOVE_ATTACK
    #define cls   ANIMOVE_CLOSE
    #define away  ANIMOVE_GOAWAY

    switch (animal->mood) {
        case ANIM_AGGRESSIVE:
            if (canattack)
                move = ((AniMove []) \
                    {att, att, att, att, att, cls, away, nth})[random];
            else
                move = ((AniMove []) \
                    {cls, cls, cls, cls, cls, cls, away, nth})[random];
            break;
        case ANIM_SCARED:
            if (canattack)
                move = ((AniMove []) \
                    {away, away, away, away, away, att, cls, nth})[random];
            else
                move = ((AniMove []) \
                {away, away, away, away, away, away, cls, nth})[random];
            break;
        case ANIM_NEUTRAL: default:
            if (canattack)
                move = ((AniMove []) \
                    {att, att, cls, cls, away, away, nth, nth})[random];
            else
                move = ((AniMove []) \
                {cls, cls, cls, away, away, away, nth, nth})[random];
            break;
    }

    switch (move) {
        case ANIMOVE_ATTACK:
            player->health -= animals_attack(animal);
            break;
        case ANIMOVE_CLOSE:
            animals_goclose(animal);
            break;
        case ANIMOVE_GOAWAY:
            animals_goaway(animal);
            break;
        case ANIMOVE_NOTHING: default: break;
    }
}


/********************************************************************
 * lookup_alias: Checks if alias is an existing alias of a command
 *               in cmdtable. Returns the id of the corresponding
 *               command if found, else invalid.
 ********************************************************************/
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

/********************************************************************
 * parser: Lowerizes input and spilts it into tokens. Checks if the
 *         first token corresponds to an alias of CommandType in
 *         cmdtable. If so, sets command's id to the appropriate value
 *         and fills its params. Else, sets command's id to invalid.
 ********************************************************************/
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
        // .id            .name        .attack .defense .speed .value
        {ANIM_LION,      _("lion"),      100,      90,     80,   100},
        {ANIM_TIGER,     _("tiger"),      95,      80,     85,    90},
        {ANIM_CHEETAH,   _("cheetah"),    95,      80,     85,    90},
        {ANIM_WOLF,      _("wolf"),       90,      70,     80,    80},
        {ANIM_BEAR,      _("bear"),       75,      75,     50,    75},
        {ANIM_ELEPHANT,  _("elephant"),   70,      80,     45,    75},
        {ANIM_BOAR,      _("boar"),       90,      55,     55,    70},
        {ANIM_ALLIGATOR, _("alligator"),  65,      50,     30,    60},
        {ANIM_PYTHON,    _("python"),     60,      55,     40,    60},
        {ANIM_FOX,       _("fox"),        55,      65,     65,    60},
        {ANIM_DEER,      _("deer"),       50,      70,     90,    60},
        {ANIM_ZEBRA,     _("zebra"),      50,      65,     80,    55}
    };

    WeaponType weaptable[MAX_WEAPONS] = {
        // .id             .name     .attack  .distance .value
        {WEAP_SHOTGUN,  _("shotgun"),  100,      100,    1000},
        {WEAP_RIFLE,    _("rifle"),     80,       80,     500},
        {WEAP_HANDGUN,  _("handgun"),   60,       50,     100},
        {WEAP_SLING,    _("sling"),     50,       30,       0}
    };

    CommandType cmdtable[MAX_COMMANDS] = {
        // .id             .aliases     .nparams     .params
        {CMD_SHOOT,    {"shoot",  NULL},    1,    {NULL, NULL}},
        {CMD_LOOK,     {"look",   NULL},    0,    {NULL, NULL}},
        {CMD_BUY,      {"buy",    NULL},    2,    {NULL, NULL}},
        {CMD_STATUS,   {"status", NULL},    0,    {NULL, NULL}},
        {CMD_HELP,     {"help",    "?"},    1,    {NULL, NULL}},
        {CMD_EXIT,     {"exit", "quit"},    0,    {NULL, NULL}},
    };

               // .health   .xp  .gold .bullets .weapon  .capendanimals
    Player player = {100,    0,    100,   5,  weaptable[3],   {0}};
    List animals = {NULL, NULL, 0, 0};

    char input[MAX_INPUT+1];
    int rounds = 0;
    CommandType command = {CMD_INVALID, {NULL, NULL}, 0, {NULL, NULL}};

    // init pseudo-random seed
    srand((unsigned) time(NULL));

    // init the gettext translation system
    setlocale(LC_ALL, "");
    bindtextdomain(PACKAGE, LOCALEDIR);
    textdomain(PACKAGE);

    // start menu
    for (;;) {
        system(SYSTEM_CLEAR);
        printf(START_MENU, APP_NAME);
        fgets(input, sizeof(input), stdin);

        char *tokens[2] = {NULL};
        s_tolower(input);
        s_tokenize(input, tokens, 2, " \n");

        if (tokens[0] == NULL || tokens[1] != NULL)
            continue;

        if (!strcmp(tokens[0], "s"))
            break;
        else if (!strcmp(tokens[0], "c")) {
            system(SYSTEM_CLEAR);
            printf(CREDITS, APP_NAME, APP_VERSION, AUTHOR, AUTHOR_MAIL,
                   APP_NAME, AUTHOR);
            puts(_("[Press Enter.]"));
            fgets(input, sizeof(input), stdin);
        }
        else if (!strcmp(tokens[0], "i")) {
            system(SYSTEM_CLEAR);
            puts(INFO_ABOUT_PLAYING);
            puts(_("[Press Enter.]"));
            fgets(input, sizeof(input), stdin);
        }
        else if (!strcmp(tokens[0], "q"))
            goto exit_success;
    }


    // add some animals on the scene
    for (register int i = 0; i < STARTING_ANIMALS; i++)
        animals_addanimal(&animals, animtable);

    system(SYSTEM_CLEAR);
    puts(WELCOME_MSG);
    puts(_("There are the following animals on the scene: "));
    for (Node *an = animals.head; an != NULL; an = an->next)
        printf("%d:%s, ", an->animal.id, an->animal.type.name);
    puts("\n");

    // internal command line loop
    for (;;) {
        printf(">>> ");
        fgets(input, sizeof(input), stdin);
        parser(input, &command, cmdtable);

        switch (command.id) {
            case CMD_SHOOT:
                if (shoot(command, &animals, &player))
                    goto animals_turn;
                break;
            case CMD_LOOK:
                animals_look(animals);
                goto animals_turn;
                break;
            case CMD_BUY:
                buy(command, &player, weaptable);
                break;
            case CMD_STATUS:
                player_status(player);
                break;
            case CMD_HELP:
                help(command, animtable, weaptable);
                break;
            case CMD_EXIT:
                exit_msg(player, animtable, rounds);
                goto exit_success;
                break;
            case CMD_INVALID: default:
                puts(_("Unknown command"));
                break;
        }
        continue;

    animals_turn:
        rounds++;

        for (Node *an = animals.head; an != NULL; an = an->next) {
            animal_move(&player, &an->animal);
            if (an->animal.distance > 100) {
                animals_kill(&animals, an->animal.id);
                puts(_("\nOops! A terrified animal left the scene."));
            }
            if (player.health <= 0) {
                puts(_("\nAfter many hard fights you're finally dead!"));
                exit_msg(player, animtable, rounds);
                goto exit_success;
            }
        }
        if (player.health < 40)
            puts(_("\nYou are seriously hurt. You should really get some drugs."));


        if (animals.len < 3)
            if (animals_addanimal(&animals, animtable))
                puts(_("\nBe careful! Α new animal appeared from nowhere!"));
        // at the beggining add a new animal after each x years
        // after 50 rounds add a new animal after each x-2 years
        if ((rounds > 50 && !(rounds % ADD_ANIM_ROUNDS - 2)) ||
            !(rounds % ADD_ANIM_ROUNDS))
            if (animals_addanimal(&animals, animtable))
                puts(_("\nBe careful! Α new animal appeared from nowhere!"));
    }

exit_success:
    animals_killall(animals.head);
    exit(EXIT_SUCCESS);
}
