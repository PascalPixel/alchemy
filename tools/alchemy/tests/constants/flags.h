// alchemy camelotize constants: a flags.h written as pure #defines.
#ifndef FLAGS_H
#define FLAGS_H

#define FLAG_PARTY_ROBIN 0x000
#define FLAG_PARTY_GERALD 0x001
#define FLAG_AUTO_BATTLE 0x16a
#define FLAG_DJINN_TUTORIAL 0x16e
#define FLAG_DJINNI_MET 0x16e // a second name for one flag: neither is used
#define FLAG_VISITED_START 0x180
#define FLAG_VISITED_VALE (FLAG_VISITED_START + 2)
#define FLAG_VISITED(group) (FLAG_VISITED_START + (group))
#define FLAG_VISITED_COUNT 0x20 // a count: it names no flag
#define NUM_DJINN_FLAGS 0x30
#define FLAG_SCENE_TEMP_00 0x240
#define FLAG_SHOP_STOCKED_START 0x400
#define FLAG_SHOP_STOCKED_VALE 0x400
#define FLAG_SOL_SANCTUM_OPEN 0x814 /* proven */
#define FLAG_ALTIN_FREED \
	0x909

#endif
