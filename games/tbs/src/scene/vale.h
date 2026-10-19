#include "mem.h"
#ifndef GUARD_VALE_H
#define GUARD_VALE_H

#include "types.h"
#include "callback_scheduler.h"

#define ACTOR_A 12
#define ACTOR_B 13
#define ACTOR_C 14
#define ACTOR_D 11

#include "staged_actor.h"
#include "field_event.h"
#include "field_scene.h"
#include "item_ids.h"

enum { ACTOR_DORA = 21 };

extern struct SceneEvent gVale_EventsAfterKalay[];
extern s32 gJennaAction[];

#include "global_cells.h"

#endif
