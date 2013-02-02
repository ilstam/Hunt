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

#include <stdio.h>     /* printf(), puts(), fgets(), putchar() */
#include <stdlib.h>    /* srand(), rand(), system(), strtol(), exit() */
#include <string.h>    /* strcmp(), memcpy() */
#include <stdbool.h>   /* bool, true, false */
#include <time.h>      /* time() */

#include "animals.h"   /* s_tolower(), s_tokenize() */
#include "misc.h"      /* s_tolower(), s_tokenize() */
#include "weapons.h"
#include "info.h"

#define MAX_INPUT            255 /* max accepted len of stdin */

#define MAX_CMD_ALIASES        2
#define MAX_CMD_PARAMS         3

#define STARTING_ANIMALS       5
#define ADD_ANIM_ROUNDS        4 /* rounds period for new animal to be added */

#define DRUG_HEALTH           15 /* health that player gains when uses drugs */
#define DRUG_COST             20
#define BULLET_COST            5


enum commandid {
        CMD_INVALID = -1,
        CMD_SHOOT,
        CMD_LOOK,
        CMD_BUY,
        CMD_STATUS,
        CMD_HELP,
        CMD_EXIT,
        MAX_COMMANDS  /* their total count */
};

struct command {
        enum  commandid  id;
        char *aliases[MAX_CMD_ALIASES];
        int   nparams; /* number of parameters */
        char *params[MAX_CMD_PARAMS];
};

struct player {
        int health; /* 0 to 100 */
        int xp;
        int gold;
        int bullets;
        struct weapon wep;
        enum animalid capedanimals[MAX_ANIMALTYPES]; /* captured animals */
};

enum buyitemid {
        BUY_INVALID = -1,
        BUY_WEAPON,
        BUY_BULLETS,
        BUY_DRUGS
};

struct buyitem {
        enum buyitemid id;
        union {
                enum weaponid wep;
                int quantity;
        } value;
        int cost;
};


/*
 * hit:
 *    Check if animal distance is in player's weapon range.
 *    If so, decrease health of the animal with the corresponding id.
 *    If animals' health is below 0, kill animal and update player attributes.
 */
void hit(int id, struct listanimals *list, struct player *player)
{
        struct sceneanimal *animal = NULL;
        int damage;

        if ((animal = animals_find(list->head, id)) == NULL)
                return;

        puts("BOOM!");
        player->bullets--;
        if (player->wep.range < animal->distance)
                return;

        damage = player->wep.attack + 50 - animal->type.defense;
        /* a divergence 0 to 10 from the standard */
        damage += ((rand() % 5) + 1) - ((rand() % 5) + 1);
        animal->health -= damage;

        if (animal->health <= 0) {
                printf("The %s is dead! You earn %d gold!\n",
                animal->type.name, animal->type.value);
                player->gold += animal->type.value;
                player->xp += animal->type.value;
                player->capedanimals[animal->type.id]++;
                animals_delete(list, id);
        }
}

/*
 * check_shoot:
 *    Check if:
 *       - command followed by a parameter
 *       - first parameter corresponds to a valid animal id
 *       - player ran of bullets
 *
 *    Return a valid animal id if command is proper, else a negative integer.
 */
int check_shoot(struct command cmd, struct listanimals *list, int bullets)
{
        int id;

        if (cmd.params[0] == NULL) {
                puts("Be more specific.");
                return -1;
        }

        id = strtol(cmd.params[0], NULL, 10);
        if (id == 0 || animals_find(list->head, id) == NULL) {
                printf("%s is not a valid animal id\n", cmd.params[0]);
                return -1;
        }

        if (!bullets) {
                puts("I have no bullets! I can't shoot!");
                return -1;
        }

        return id;
}

/*
 * lookup_weapon:
 *    Convert a weapon name into weapon id and return it.
 */
enum weaponid lookup_weapon(struct weapon weaptable[], char *wep)
{
        register int i;

        if (wep == NULL)
                return WEAP_INVALID;

        for (i = 0; i < MAX_WEAPONS; i++) {
                if (!strcmp(weaptable[i].name, wep))
                        return weaptable[i].id;
        }

        return WEAP_INVALID;
}

/*
 * buy:
 *    Check if:
 *       - command followed by a parameter
 *       - first parameter corresponds to a valid buyitemid
 *       - parameters are sufficient (eg. "buy drugs" is invalid)
 *    For weapons purchases:
 *       - if there is corresponding weaponid
 *    For bullets or drugs purchases:
 *       - if parameter is a valid integer
 *
 *    Update bitem values accordingly.
 */
void buy(struct command cmd, struct buyitem *bitem, struct weapon weaptable[])
{
        if (cmd.params[0] == NULL) {
                puts("What do you want to buy?");
                bitem->id = BUY_INVALID;
                return;
        }

        if (strcmp(cmd.params[0], "bullets")
        && strcmp(cmd.params[0], "drugs")
        && strcmp(cmd.params[0], "weapon")) {
                printf("You can't buy %s.\n", cmd.params[0]);
                bitem->id = BUY_INVALID;
                return;
        }

        if (cmd.params[1] == NULL) {
                puts("Please be more specific.");
                bitem->id = BUY_INVALID;
                return;
        }

        if (!strcmp(cmd.params[0], "weapon")) {
                bitem->value.wep = lookup_weapon(weaptable, cmd.params[1]);
                if (bitem->value.wep == WEAP_INVALID) {
                        puts("There is no weapon with such name.");
                        bitem->id = BUY_INVALID;
                        return;
                }
                bitem->id = BUY_WEAPON;
                bitem->cost = weaptable[bitem->value.wep].value;
        } else { /* "bullets" or "drugs" */
                bitem->value.quantity = strtol(cmd.params[1], NULL, 10);
                if (bitem->value.quantity <= 0) {
                        printf("%s is not a valid positive integer.\n",
                        cmd.params[1]);
                        bitem->id = BUY_INVALID;
                        return;
                }

                if (!strcmp(cmd.params[0], "bullets")) {
                        bitem->id = BUY_BULLETS;
                        bitem->cost = BULLET_COST * bitem->value.quantity;
                } else {
                        bitem->id = BUY_DRUGS;
                        bitem->cost = DRUG_COST * bitem->value.quantity;
                }
        }
}

/*
 * pay_and_get:
 *    Check if player can afford the purchase. If not return.
 *    Else prompts user to confirm his decision.
 *    If user accepts, decrease player's gold.
 *
 *    In weapons purchases: update player's weapon values.
 *    In bullets purchases: increase number of player's bullets.
 *    In drugs purchases: increase number of player's health.
 */
void pay_and_get(struct buyitem bitem,
                 struct player *player,
                 struct weapon weaptable[]
)
{
        char input[MAX_INPUT+1]; /* user's stdin */
        /* to be filled by s_tokenize() with NUL-terminated string tokens */
        char *tokens[2] = {NULL};

        printf("You need %d gold in order to buy this stuff.\n", bitem.cost);
        if (player->gold < bitem.cost) {
                puts("Sorry, you don't have enough gold.");
                return;
        }

        printf("Are you sure that you want to proceed? (y/n) ");
        fgets(input, sizeof(input), stdin);
        s_tokenize(input, tokens, 2, " \n");

        if (tokens[0] == NULL || tokens[1] != NULL
        || (strcmp(s_tolower(tokens[0]), "y")
        && strcmp(s_tolower(tokens[0]), "yes"))) {
                puts("canceled");
                return;
        }

        switch(bitem.id) {
        case BUY_WEAPON:
                /* copy fields of specific weapon type */
                memcpy(&player->wep, &weaptable[bitem.value.wep],
                sizeof(*weaptable));
                break;
        case BUY_BULLETS:
                player->bullets += bitem.value.quantity;
                break;
        case BUY_DRUGS:
                player->health += bitem.value.quantity * DRUG_HEALTH;
                if (player->health > 100)
                        player->health = 100; /* must not exceed 100 */
                break;
        case BUY_INVALID:
                break;
        }
        player->gold -= bitem.cost;
        puts("Done!");

        return;
}

/*
 * animal_move:
 *    Perform a move (attack, go close/away) for each animal.
 *    If animal attacks, decrease player's health.
 *    If animal go close or away from player: change animals' distance.
 *    If animal goes too far away, kill it.
 */
void animal_move(struct listanimals *list, struct player *player)
{
        struct node *an = NULL;
        enum animalmove move;

        for (an = list->head; an != NULL && player->health > 0; an = an->next) {
                move = animals_decision(an->animal);
                switch (move) {
                case ANIMOVE_ATT:
                        printf("\nBEWARE! An angry %s attacks you! "
                        "Oh, that hurts!\n", an->animal.type.name);
                        player->health -= ANIM_DAMAGE(an->animal.type.attack);
                        break;
                case ANIMOVE_CLS:
                        an->animal.distance -= ANIM_DISTMOVE(
                        an->animal.type.speed);
                        if (an->animal.distance < 0) /* must not be < 0 */
                                an->animal.distance = 0;
                        break;
                case ANIMOVE_AWAY:
                        an->animal.distance += ANIM_DISTMOVE(
                        an->animal.type.speed);
                        if (an->animal.distance > 100) { /* out of scene */
                                animals_delete(list, an->animal.id);
                                puts("\nOops! "
                                "A terrified animal left the scene.");
                        }
                        break;
                case ANIMOVE_NTH:
                default:
                        break;
                }
        }
}

/*
 * player_status:
 *    Display player attributes.
 */
void player_status(struct player player)
{
        printf("Health: %d, XP: %d, Gold: %d\n"
        "Weapon: %s (attack=%d, distance=%d), Bullets: %d\n",
        player.health, player.xp, player.gold, player.wep.name,
        player.wep.attack, player.wep.range, player.bullets);
}

/*
 * help:
 *    If called without parameters display a help message.
 *    If called with "animals" or "weapons" as parameter display the
 *    corresponding table. Else display an error message.
 */
void help(struct command cmd,
          struct animal animtable[],
          struct weapon weptable[]
)
{
        register int i;

        if (cmd.params[0] == NULL) {
                puts(HELP_MSG);
                return;
        }

        if (!strcmp(cmd.params[0], "animals")) {
                puts("name    \tatt\tdef\tspd\tval");
                puts("--------------------------------------------");
                for (i = 0; i < MAX_ANIMALTYPES; i++)
                        printf("%s     \t%3d\t%3d\t%3d\t%3d\n",
                        animtable[i].name, animtable[i].attack,
                        animtable[i].defense, animtable[i].speed,
                        animtable[i].value);
        } else if (!strcmp(cmd.params[0], "weapons")) {
                puts("name    \tatt\tdist\t val");
                puts("-------------------------------------");
                for (i = 0; i < MAX_WEAPONS; i++)
                        printf("%s     \t%3d\t%3d\t%4d\n", weptable[i].name,
                        weptable[i].attack, weptable[i].range,
                        weptable[i].value);
        } else
                printf("%s: invalid parameter\n", cmd.params[0]);
}

/*
 * exit_msg:
 *    Display a goodbye message, player stats and captured animals if any.
 */
void exit_msg(struct player player, struct animal animtable[], int rounds)
{
        register int i;
        bool havecaped = false;  /* captured any animals */

        printf("\nThis adventure is over.\nYou've made %d xp points and died "
        "holding a %s after %d rounds.\n", player.xp, player.wep.name, rounds);

        for (i = 0; i < MAX_ANIMALTYPES; i++)
                if (player.capedanimals[i] > 0)
                        havecaped = true;

        if (!havecaped) {
                puts("\nYou haven't captured any animals.\n");
                return;
        }

        printf("\nYou have also captured: ");
        for (i = 0; i < MAX_ANIMALTYPES; i++)
                if (player.capedanimals[i] > 0) {
                        printf("%d %s",
                        player.capedanimals[i], animtable[i].name);
                if (player.capedanimals[i] > 1)
                        putchar('s');  /* plural */
                printf(", ");
            }
        puts("\n");
}

/*
 * lookup_alias:
 *    Convert a command alias into command id and return it.
 */
enum commandid lookup_alias(struct command cmdtable[], char *alias)
{
        register int i, y;

        if (alias == NULL)
                return CMD_INVALID;

        for (i = 0; i < MAX_COMMANDS; i++) {
                for (y = 0; y < MAX_CMD_ALIASES; y++)
                        if (cmdtable[i].aliases[y] != NULL &&
                        !strcmp(cmdtable[i].aliases[y], alias))
                                return cmdtable[i].id;
        }

        return CMD_INVALID;
}


/*
 * input_parser:
 *    Parse the command line arguments and update accordingly the values
 *    of cmd.
 */
void input_parser(char *input, struct command *cmd, struct command cmdtable[])
{
        register int i;
        enum commandid cmdid; /* for command identification */
        /* to be filled by s_tokenize() with NUL-terminated string tokens */
        char *tokens[MAX_CMD_PARAMS + 1] = {NULL};
        int ntokens = 0; /* number of tokens */

        /* max desired tokens for s_tokenize are MAX_CMD_PARAMS + 2 */
        /* 1 token for holding cmd name and 1 to check if extra invalid */
        /* parameters were passed to a valid cmd*/
        ntokens = s_tokenize(input, tokens, MAX_CMD_PARAMS + 1 + 1, " \n");

        cmdid = lookup_alias(cmdtable, tokens[0]);
        if (cmdid == CMD_INVALID) {
                cmd->id = CMD_INVALID;
                return;
        }

        /* copy fields of specific command type */
        memcpy(cmd, &cmdtable[cmdid], sizeof(*cmd));
        for (i = 0; i < (ntokens - 1); i++)
                cmd->params[i] = tokens[i+1];
}

/*
 * init_scene:
 *    Add some animals on the scene and print them.
 */
void init_scene(struct listanimals *animals, struct animal animtable[])
{
        register int i;
        struct node *an;

        for (i = 0; i < STARTING_ANIMALS; i++)
                animals_add(animals, animtable);

        puts("There are the following animals on the scene: ");
        for (an = animals->head; an != NULL; an = an->next)
                printf("%d:%s, ", an->animal.id, an->animal.type.name);
        puts("\n");
}

/*
 * start_menu:
 *    Print available options and prompt user for an option.
 *    Return only when user enters "s".
 *    If user enters "q" terminate program, else display program info.
 */
void start_menu(void)
{
        char input[MAX_INPUT + 1];  /* user's stdin */
        char *tokens[2] = {NULL};   /* to be filled by s_tokenize() */
                                    /* with NUL-terminated string tokens*/

        for (;;) {
                system("clear");
                printf(START_MENU, APP_NAME);

                fgets(input, sizeof(input), stdin);
                s_tolower(input);
                s_tokenize(input, tokens, 2, " \n");

                if (tokens[0] == NULL || tokens[1] != NULL)
                        continue;

                if (!strcmp(tokens[0], "s")) {
                        return;
                } else if (!strcmp(tokens[0], "c")) {
                        system(SYSTEM_CLEAR);
                        printf(CREDITS, APP_NAME, APP_VERSION, AUTHOR,
                        AUTHOR_MAIL, APP_NAME, AUTHOR);
                        puts("[Press Enter.]");
                        fgets(input, sizeof(input), stdin);
                } else if (!strcmp(tokens[0], "i")) {
                        system(SYSTEM_CLEAR);
                        puts(INFO_ABOUT_PLAYING);
                        puts("[Press Enter.]");
                        fgets(input, sizeof(input), stdin);
                } else if (!strcmp(tokens[0], "q"))
                        exit(EXIT_SUCCESS);
        }
}


int main(void)
{
        char input[MAX_INPUT + 1];  /* user's stdin */
        int rounds = 0;             /* rounds played */
        int id;                     /* animal id used by check_shoot() */

        struct command cmdtable[MAX_COMMANDS] = {
                /* .id             .aliases     .nparams       .params */
                {CMD_SHOOT,    {"shoot",  "sh"},    1,    {NULL, NULL, NULL}},
                {CMD_LOOK,     {"look",   "lk"},    0,    {NULL, NULL, NULL}},
                {CMD_BUY,      {"buy",    NULL},    2,    {NULL, NULL, NULL}},
                {CMD_STATUS,   {"status", "st"},    0,    {NULL, NULL, NULL}},
                {CMD_HELP,     {"help",    "?"},    1,    {NULL, NULL, NULL}},
                {CMD_EXIT,     {"exit", "quit"},    0,    {NULL, NULL, NULL}},
        };

        struct animal animtable[MAX_ANIMALTYPES] = {
                /* .id            .name       .attack .defense .speed .value */
                {ANIM_LION,      "lion",        100,      90,     80,   100},
                {ANIM_TIGER,     "tiger",        95,      80,     85,    90},
                {ANIM_CHEETAH,   "cheetah",      95,      80,     85,    90},
                {ANIM_WOLF,      "wolf",         90,      70,     80,    80},
                {ANIM_BEAR,      "bear",         75,      75,     50,    75},
                {ANIM_ELEPHANT,  "elephant",     70,      80,     45,    75},
                {ANIM_BOAR,      "boar",         90,      55,     55,    70},
                {ANIM_ALLIGATOR, "alligator",    65,      50,     30,    60},
                {ANIM_PYTHON,    "python",       60,      55,     40,    60},
                {ANIM_FOX,       "fox",          55,      65,     65,    60},
                {ANIM_DEER,      "deer",         50,      70,     90,    60},
                {ANIM_ZEBRA,     "zebra",        50,      65,     80,    55}
        };

        struct weapon weaptable[MAX_WEAPONS] = {
                /* .id             .name    .attack  .distance .value */
                {WEAP_SLING,     "sling",      50,       30,       0},
                {WEAP_HANDGUN,   "handgun",    60,       50,     100},
                {WEAP_RIFLE,     "rifle",      80,       80,     500},
                {WEAP_SHOTGUN,   "shotgun",   100,      100,    1000}
        };
                          /* .health  .xp  .gold .bullets .wep .capedanimals */
        struct player player = {100,    0,    100,   5,  weaptable[0],   {0}};

        struct command cmd = {CMD_INVALID, {NULL, NULL}, 0, {NULL, NULL}};
        struct listanimals animals = {NULL, NULL, 0, 0};
        struct buyitem bitem = {BUY_INVALID, {0}, 0};


        srand((unsigned) time(NULL)); /* init pseudo-random seed */

        start_menu();
        system(SYSTEM_CLEAR);
        puts(WELCOME_MSG);
        init_scene(&animals, animtable);

        /* internal command line loop */
        for (;;) {
                printf(">>> ");
                fgets(input, sizeof(input), stdin);
                s_tolower(input);
                input_parser(input, &cmd, cmdtable);

                /* check if valid command but more parameters than expected*/
                if (cmd.id != CMD_INVALID && cmd.params[cmd.nparams] != NULL) {
                        puts("Invalid parameters");
                        continue;
                }

                switch (cmd.id) {
                case CMD_SHOOT:
                        id = check_shoot(cmd, &animals, player.bullets);
                        if (id > 0) {
                                hit(id, &animals, &player);
                                goto animals_turn;
                        }
                        break;
                case CMD_LOOK:
                        animals_look(animals);
                        goto animals_turn;
                        break;
                case CMD_BUY:
                        buy(cmd, &bitem, weaptable);
                        switch (bitem.id) {
                        case BUY_INVALID:
                                break;
                        default:
                                pay_and_get(bitem, &player, weaptable);
                                break;
                        }
                        break;
                case CMD_STATUS:
                        player_status(player);
                        break;
                case CMD_HELP:
                        help(cmd, animtable, weaptable);
                        break;
                case CMD_EXIT:
                        exit_msg(player, animtable, rounds);
                        goto exit_success;
                        break;
                case CMD_INVALID:
                default:
                        puts("Unknown command");
                        break;
                }
                continue;

        animals_turn:
                rounds++;
                animal_move(&animals, &player);

                if (player.health <= 0) {
                        puts("\nAfter many hard fights you're finally dead!");
                        exit_msg(player, animtable, rounds);
                        goto exit_success;
                }

                if (animals.len < 3)
                        if (animals_add(&animals, animtable))
                                puts("\nBe careful! "
                                "A new animal appeared from nowhere!");
                /* for first 50 rounds add a new animal after each x rounds */
                /* after that add a new animal after each x-2 rounds */
                if ((rounds > 50 && !(rounds % (ADD_ANIM_ROUNDS - 2)))
                || !(rounds % ADD_ANIM_ROUNDS))
                        if (animals_add(&animals, animtable))
                                puts("\nBe careful! "
                                "A new animal appeared from nowhere!");
        }

exit_success:
        animals_deleteall(&animals);
        exit(EXIT_SUCCESS);
}

