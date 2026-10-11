#ifndef KYUDEN_H
#define KYUDEN_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "facing_object.h"

#define NULL ((void *)0)

enum
{
	FLAG_REWARD_TAKEN = 0x84e
};

extern struct SceneEntrance Placement_Scripts[];
extern u32					Placement_Messages[];
extern s32					Kyuden_FacingActions[];
extern s32					Kyuden_PacingActions[];

#endif
