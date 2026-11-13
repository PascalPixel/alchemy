#ifndef MIKASALLA_LOCAL_H
#define MIKASALLA_LOCAL_H

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "event_inline.h"

extern u32 gSceneExits[];

void Func_02000620(s32 a);
void Event_Start(void);
void Event_End(void);

void Func_02000438(void);

extern u8 MsgFieldAreYouLookingForSomeone[];

#endif
