#ifndef TLA_TRIAL_ROAD_H
#define TLA_TRIAL_ROAD_H

#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "field_scene.h"
#include "scene_ids.h"

#define OBJECT_BYTE(object, offset) (((u8 *)(object))[offset])

struct TrialRoadMapLayer {
  u8 unk00[0x30];
  u8 *cells;
  u8 unk34[4];
};

struct TrialRoadMapWork {
  u8 unk000[0x108];
  struct TrialRoadMapLayer layers[3];
};

struct TrialRoadState {
  u8 unk00[0x10];
  s16 pattern;
  s16 mirrored;
  s16 field14;
  s16 field16[2];
  s16 field1a;
  s16 field1c;
};

extern u8 gData_020023c4[];

#define gTrialRoadState ((struct TrialRoadState *)(gData_020023c4 + 0x88))

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void WaitFrames(s32 frames);
struct ObjectRuntime *Object_GetById(s32 id);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);

void Func_0200062c(s32 id);
void Func_02000660(s32 id);
void Func_0200070c(s32 id, s32 value);
void Func_0200078c(s32 id);
void Func_020009ac(s32 a, s32 b);
void Func_02000a18(s32 id, s32 a, s32 b);
void Func_02000b7c(s32 id, s32 other, s32 flag, s32 side);
void Func_02000e40(s32 id, s32 dx, s32 dz, s32 axis);
void Func_02001104(s32 id, s32 a, s32 frame);
void Func_02004418(const void *data);
void Func_020045ac(const void *data, s32 a);
void Func_020057c8(struct ObjectRuntime *object, s32 a);
void Func_020057d0(struct ObjectRuntime *object, const void *script);
struct ObjectRuntime *Func_020057d8(s32 type, s32 x, s32 y, s32 z);
void Func_02005818(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02005820(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02005918(s32 id, s32 a, s32 b);
void Func_020059a0(s32 a, s32 b);
s32 Func_020059b8(s32 id);
void Func_020059c0(void);
void Func_020059c8(void);
s32 Func_02005a30(void);
void Func_02005a38(void);

struct PartyState254 {
  u8 unk000[0x254];
  u16 value;
};

void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Party_SetFields1eeAnd1f0(s32 scene, s32 entrance);
void Party_SetFields1f2And1f4(s32 scene, s32 entrance);
void Object_SetModeById(u32 id, s32 mode);
void Func_020006b4(s32 id);
void Func_020007b4(void);
void Func_02005810(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_020058e0(s32 id, s32 x, s32 z);
void Func_020058e8(s32 id, s32 x, s32 z, s32 angle);
void Func_02005998(s32 a, s32 b);
void Func_02005a50(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
void Func_02005a60(s32 a, s32 b, s32 c);
void Func_02005a68(s32 a);

static __inline__ struct ObjectRuntime *Object_Create(s32 type, s32 x, s32 y,
                                                      s32 z) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  return Func_020057d8(type, x, y, z);
}

static __inline__ void Map_CopyCells(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_02005818(a, b, c, d, e, f);
}

static __inline__ void Map_Stamp(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_02005820(a, b, c, d, e, f);
}

static __inline__ void Func_02005810_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_02005810(a, b, c, d, e, f);
}

static __inline__ void Func_02005a50_(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                      s32 g) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_02005a50(a, b, c, d, e, f, g);
}

/// The store-flag form of a logical not.
// The equality tests of this stage return 1 - (difference != 0) with no
// branch, which the compiler emits only for !value on a register (as plain
// comparisons they compile to a compare and branch).
static __inline__ s32 IsZero(s32 value) { return !value; }

#endif
