#ifndef INFO_H
#define INFO_H

#define APP_NAME        "Hunt"
#define APP_VERSION     "0.01"

#define WELCOME_MSG                                                       \
    "*     *   *     *   **    *   *******\n"                             \
    "*     *   *     *   * *   *      *\n"                                \
    "*******   *     *   *  *  *      *\n"                                \
    "*     *   *     *   *   * *      *\n"                                \
    "*     *   *******   *    **      *\n"                                \
    "--------------------------------------\n\n"                          \
    "It's a happy sunny day in the forest.\n"                             \
    "A bright sun is up in the sky and birds chirping in the air.\n"      \
    "There are trees, bushes, flowers, grass, and fruits all around you." \
    "\n\nWhat a wonderful place!\nBut beware! You're not alone!\n"

#define HELP_MSG \
"Only the following commands are understood by the program:\n\n"                      \
"  Command             Description\n"                                                 \
" -------------------------------------------------------------------------\n"        \
"  shoot <id>          shoots the animal with the corresponding id\n"                 \
"  look                displays info about all animals on the scene\n"                \
"  status              displays info about player\n"                                  \
"  buy <type> <spef>   buys the requested stuff\n"                                    \
"                         examples: buy bullets 5, buy drugs 1, buy weapon rifle\n" \
"  help, ? <table>     if called without parameter, prints this help text\n"          \
"                         you can user either \"help animals\" or \"help weapons\"\n" \
"                         to print a table of all animals or weapons\n"               \
"  exit, quit          exits the program\n"                                           \

#endif
