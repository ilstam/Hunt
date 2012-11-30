#ifndef WEAPONS_H
#define WEAPONS_H

typedef enum {
    WEAP_SLING = 0,
    WEAP_HANDGUN,
    WEAP_RIFLE,
    WEAP_SHOTGUN,
    MAX_WEAPONS  // their total count
} WeapTypeId;

typedef struct {
    WeapTypeId  id;
    char       *name;
    int         attack;   // 0 to 100
    int         distance; // distance to which is able to shoot - 0 to 100
    int         value;    // purchase price
} WeaponType;

#endif

