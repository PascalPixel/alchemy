#ifndef TAOPO_SWAMP_LOCAL_H
#define TAOPO_SWAMP_LOCAL_H

#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "iwram_call.h"
#include "field_sprite.h"
#include "event_inline.h"
#include "frame.h"

extern u32 gSceneExits[];

extern char SceneId_Ov678Room1;
extern char SceneId_Ov678Room2;
extern char SceneId_Ov678Room3;
extern char SceneId_Ov678Room4;
extern char SceneId_Ov678Room5;
extern char SceneId_Ov678Room6;
extern char SceneId_Ov678Room7;
extern char SceneId_Ov678Room8;
extern char SceneId_Ov678Room9;
extern char SceneId_Ov678Room10;
extern char SceneId_Ov678Room11;

void GameFlag_SetByte(s32 flag, s32 value);
void Func_02002764(s32 id, s32 a);
void Func_02002778(s32 a, s32 b);
void Event_Start(void);
void Event_End(void);
void Actor_JumpDownLedge(s32 id, s32 a, s32 b);
void Func_02002928(void);
void Field_RequestBattleFromEncounterTable(s32 a, s32 b);
void Actor_ReturnHome(s32 a, s32 b, s32 c);
void Func_02002990(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
void Func_020029a8(s32 a, s32 b, s32 c);
void Actor_StartScript(struct Actor *object, const void *data);
void Animation_SetStateFlags(struct Actor *object, u32 value);

extern s32 gData_02003458;
extern s32 gData_0200345c;
extern s32 gData_02003460;
extern const u8 gData_020029c8[];
void Func_02002948(void);
void Func_02002084(s32 a);
void Func_020029a0(s32 message);
void Func_020027f8(struct Actor *object, s32 a);
void Actor_SetSpritePriorityDirect(struct Actor *object, s32 a);
s32 Func_02002730(void);
void Func_0200271c(s32 a, s32 b);
s32 Func_0200273c(s32 a);
void Func_02002978(s32 a);

struct SpriteWork {
  u8 unk00[17];
  u8 layer : 2;
  u8 level : 6;
  u8 unk12[7];
  u8 dirty;
  u8 mode;
  u8 unk1b[17];
  u8 *child;
};

struct SceneState {
  s16 mode;
  s16 timer;
  s32 x;
  s32 z;
};

extern struct SceneState gSceneState;

typedef volatile u16 vu16;

struct IoReg16 {
  u16 value;
};

#define REG_BLDALPHA ((struct IoReg16 *)0x04000052)
struct VentEntry {
  s16 active;
  s16 timer;
  s32 x;
  s32 y;
  s32 z;
  s32 value;
};

struct VentWork {
  s32 counter;
  s32 (*hook)(void);
  struct VentEntry entries[16];
  u16 ids[4];
};

extern u8 gData_020023c4[];
#define VENT_WORK ((struct VentWork *)(gData_020023c4 + 0x188))
#define DIFF(a, b) ((a) - (b) >= 0 ? (a) - (b) : (b) - (a))

s32 Math_Cosine(s32 angle);
void Func_02002400(struct Actor *object);
extern const u8 gData_02002af4[];
extern s32 gData_020035f0;
extern u8 gMapShapeGrid[];
void Map_CenterOnFollowedPoint(void);
void DisplayBlend_EnableRunScript(void);
void Func_02001eb8(void);
void Func_02001ff0(s32 x, s32 z);
void Func_0200237c(void);
void Func_02000920(s32 a, s32 id);
extern const u8 gData_02002a60[];
extern const u8 gData_02002a6c[];
void Func_02000810(s32 id);
void Func_020008d8(s32 id);
extern const u8 gData_02002a48[];
extern const u8 gData_02002a54[];
extern u8 MsgIThoughtI;
void Func_02000c24(void);
void Func_02000ca0(void);
void Func_02000cec(void);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ s32 GameFlag_GetByte_(s32 flag) {
  return GameFlag_GetByte(flag);
}

static __inline__ void
Actor_OffsetPositionAndResetMotion_(s32 id, s32 dx, s32 dz) {
  Actor_OffsetPositionAndResetMotion(id, dx, dz);
}

static __inline__ void Func_02002840_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  Map_CopyCellAttributeRect(a, b, c, d, e, f);
}

static __inline__ void Func_020028d0_(s32 id, s32 x, s32 z) {
  Actor_SetHorizontalPositionWithTerrain(id, x, z);
}

static __inline__ void Func_020028e0_(s32 id, s32 a, s32 b) {
  Actor_JumpDownLedge(id, a, b);
}

static __inline__ void Func_02002910_(s32 id, s32 a) {
  FieldEffect_SetAttached(id, a);
}

static __inline__ void Func_02002958_(s32 a, s32 b, s32 c) {
  Actor_ReturnHome(a, b, c);
}

static __inline__ void Func_02002990_(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                      s32 g) {
  Func_02002990(a, b, c, d, e, f, g);
}

static __inline__ struct Actor *Func_02002808_(s32 type, s32 x, s32 y,
                                                       s32 z) {
  return Import_ActorCreate(type, x, y, z);
}

static __inline__ void Func_02002838_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  Map_CopyCellsRect(a, b, c, d, e, f);
}

static __inline__ void Actor_SetSpeedParameters_(s32 id, s32 speed,
                                                        s32 acceleration) {
  Actor_SetSpeedParameters(id, speed, acceleration);
}

static __inline__ void Func_02002848_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  Func_0802b450(a, b, c, d, e, f);
}

static __inline__ void Func_02002818_(struct Actor *object, s32 x,
                                      s32 y, s32 z) {
  Actor_SetMoveTarget(object, x, y, z);
}

static __inline__ void Func_020029a8_(s32 a, s32 b, s32 c) {
  Func_020029a8(a, b, c);
}

#endif
