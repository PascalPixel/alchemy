#ifndef MAGMA_ROCK_INSIDE_LOCAL_H
#define MAGMA_ROCK_INSIDE_LOCAL_H

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "iwram_call.h"
#include "scene_ids.h"
#include "frame.h"
#include "event_inline.h"
#include "map.h"

extern struct MapCell gMapCellBuffer[];

extern char SceneId_Ov692Room1;
extern char SceneId_Ov692Room2;
extern char SceneId_Ov692Room3;
extern char SceneId_Ov692Room4;
extern char SceneId_Ov692Room5;
extern char SceneId_Ov692Room6;
extern char SceneId_Ov692Room7;
extern char SceneId_Ov692Room8;

void Scene_SetActorSpritePriority(struct Actor *object, s32 priority);
void Animation_SetStateFlags(struct Actor *object, s32 flags);

extern const u8 gData_02005534[];

extern s32 gData_02006710;

void Func_02001190(s32 a, s32 b);
void Func_020024ac(void);
void Func_02004934(s32 a, s32 b, s32 owner);
void Func_02004c98(struct Actor *object, s32 a);
void Func_02004ca0(struct Actor *object, const void *data);
void Func_02004df8(s32 id);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ struct Actor *Create(s32 kind, s32 x, s32 y, s32 z) {
  return Import_ActorCreate(kind, x, y, z);
}

static __inline__ void Actor_ResetAndSetPositionInMode2_(s32 id, s32 x,
                                                                s32 z) {
  /* FAKEMATCH(wrapper): forwarding through the inline sets x between the two
   * halves of z, as the native does (a direct call builds z whole first); as a
   * direct call, 2 of 77 instructions of Func_020023e8 differ in tla-en. */
  Actor_ResetAndSetPositionInMode2(id, x, z);
}

#endif
