#ifndef MIKASALLA_LOCAL_H
#define MIKASALLA_LOCAL_H

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "event_inline.h"

extern const struct SceneEntrance gSceneEntrances[];
extern const u32 gSceneExits[];
extern const struct ActorPlacement gActorPlacements[];
extern const struct Trigger gSceneEvents[];

void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Func_02000620(s32 a);
void Func_02000648(void);
void Func_02000650(void);

void Func_02000438(void);

extern u8 MsgFieldAreYouLookingForSomeone;

#endif
