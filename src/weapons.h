#ifndef WEAPONS_H
#define WEAPONS_H

enum weaponid {
	WEAP_INVALID = -1,
	WEAP_SLING,
	WEAP_HANDGUN,
	WEAP_RIFLE,
	WEAP_SHOTGUN,
	MAX_WEAPONS /* their total count */
};

struct weapon {
	enum weaponid id;
	char *name;
	int   attack;  /* 0 to 100 */
	int   range;   /* distance to which is able to shoot - 0 to 100 */
	int   value;   /* purchase price */
};

#endif

