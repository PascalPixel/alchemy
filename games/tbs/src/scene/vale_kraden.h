#ifndef VALE_KRADEN_H
#define VALE_KRADEN_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "facing_object.h"

enum CottageActor
{
	ACTOR_KRADEN = ACTOR_FIRST_PLACED + 5,
	ACTOR_SATUROS,
	ACTOR_MENARDI
};

enum CottageFlag
{
	FLAG_SANCTUM_VISIT_PLANNED = 0x801,
	FLAG_MET_SATUROS_AND_MENARDI = 0x808
};

#define NULL ((void *)0)

#endif
