#ifndef JUPITER_LIGHTHOUSE_ROOM_3_LOCAL_H
#define JUPITER_LIGHTHOUSE_ROOM_3_LOCAL_H

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "iwram_call.h"
#include "dma.h"
#include "event_inline.h"

extern u32 gSceneExits[];

struct PushedBlock {
  s32 a;
  s32 b;
  s32 c;
  s32 d;
  s32 e;
  s32 f;
};

void Scene_SetArrivalFlags(s32 a);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_02002360(struct Actor *object, s32 a);
void Func_02002368();
void Func_020023d0(s32 id);
void Func_02002400(void);
void Func_02002440(s32 id, s32 a);
void Func_02002460(s32 x, s32 z, s32 a, s32 b);
void Func_02002468(s32 id);
void Func_02000054(struct Actor *object);
void Func_02000404(void);
void Func_02000c78(s32 id);
s32 Func_02000dcc(struct PushedBlock *block);
void Func_02001050(struct PushedBlock block);

extern s32 gData_02002cc0;
extern s32 gData_02002cc4;
extern const u16 gData_02002cec[];
extern const u16 gData_02002d0c[];
extern const u16 gData_02002d10[];

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Lift_(s32 x, s32 z, s32 a, s32 b) {
  Func_02002368(x, z, a, b);
}

static __inline__ s32 GameFlag_Test_(s32 flag) { return GameFlag_Test(flag); }

static __inline__ void Func_020023b0_(s32 id, s32 x, s32 z) {
  Actor_SetHorizontalPositionWithTerrain(id, x, z);
}

#endif
