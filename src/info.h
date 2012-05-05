#ifndef INFO_H
#define INFO_H

#include <libintl.h> // gettext()
#define _(string)       gettext(string)

#define APP_NAME        "Hunt"
#define APP_VERSION     "0.3"
#define AUTHOR          "Ilias Stamatis"
#define AUTHOR_MAIL     "stamatis.iliass@gmail.com"

#define START_MENU                         _(\
    "              %s\n"                     \
    "    ------------------------\n\n"       \
    "      S  Start the game\n\n"            \
    "      C  Show credits\n\n"              \
    "      I  Info about playing\n\n"        \
    "      Q  Quit\n\n")

#define CREDITS                                                            _(\
    "Name: %s\n"                                                             \
    "Version: %s\n\n"                                                        \
    "Copyright (C) 2012 %s <%s>\n\n"                                         \
    "%s is free software: you can redistribute it and/or modify\n"           \
    "it under the terms of the GNU General Public License as published by\n" \
    "the Free Software Foundation, either version 3 of the License, or\n"    \
    "(at your option) any later version.\n\n"                                \
    "Written by %s.\n\n"                                                     \
    "Special thanks to migf1.\n\n")

#define INFO_ABOUT_PLAYING                                               _(\
    "Info about playing\n"                                                 \
    "-------------------\n"                                                \
    "Commands consist of sentences with one, two or three words.\n"        \
    "Only the following commands are understood by the program:\n"         \
    "HELP, QUIT, SHOOT, LOOK, BUY, STATUS.\n\n"                            \
    "Try HELP to see a description of each command.\n\n"                   \
    "Your target is to capture as many animals as you can.\n"              \
    "For each animal you capture you earn gold and gain some xp.\n"        \
    "You can buy bullets, better weapons and drugs in case you get hurt "  \
    "by an animal.\n\n"                                                    \
    "When your health drops below 0, the game is over.\n")

#define WELCOME_MSG                                                      _(\
    "*     *   *     *   **    *   *******\n"                              \
    "*     *   *     *   * *   *      *\n"                                 \
    "*******   *     *   *  *  *      *\n"                                 \
    "*     *   *     *   *   * *      *\n"                                 \
    "*     *   *******   *    **      *\n"                                 \
    "--------------------------------------\n\n"                           \
    "It's a happy sunny day in the forest.\n"                              \
    "A bright sun is up in the sky and birds chirping in the air.\n"       \
    "There are trees, bushes, flowers, grass, and fruits all around you."  \
    "\n\nWhat a wonderful place!\nBut beware! You're not alone!\n")

#define HELP_MSG                                                                       _(\
    "Only the following commands are understood by the program:\n\n"                     \
    " Command             Description\n"                                                 \
    "-------------------------------------------------------------------------\n"        \
    " shoot <id>          shoots the animal with the corresponding id\n"                 \
    " look                displays info about all animals on the scene\n"                \
    " status              displays info about player\n"                                  \
    " buy <type> <spef>   buys the requested stuff\n"                                    \
    "                        examples: buy bullets 5, buy drugs 1, buy weapon rifle\n"   \
    " help, ? <table>     if called without parameter, prints this help text\n"          \
    "                        you can user either \"help animals\" or \"help weapons\"\n" \
    "                        to print a table of all animals or weapons\n"               \
    " exit, quit          exits the program\n")

#endif
