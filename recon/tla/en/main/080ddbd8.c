// Draft, differ 15 of 208, skeleton gap 7 (tla-en, 2026-11-01)
// Twin of ☀️'s FieldEffect_SpawnBurstParticleField (Growth's particles): same
// flow, 24 particles, same offsets, same part copy and calls. ⚓️ spawns object
// 672, keeps the sprite's second part 28 bytes in (☀️: right after the first),
// and reads the angle as a halfword. Taken from the twin: the child flags call
// before the destination is set. The angle is read through a u16 pointer: as
// (u16)work->angle CSE keeps the compare's value for the 0xc000 call, but the
// listing rereads it, and that extra use of work is what gives it r9. A pointer
// for the second half of the loop (p) brings the entry's registers to the
// listing's. Counting down from 23 spills the count to the stack word.
// Left, both scheduling, so a near miss for now (W4):
// - entry: the listing has movs r2,#4 / movs r3,#23 / add r2,sp; here the first
//   and third rows are adjacent and the Thumb peephole folds them into
//   add r2,sp,#4, one instruction short, which also moves three branch offsets.
//   n = 23 before p = pos gives add r3 (19); a do loop changes nothing; counting
//   up keeps one more skeleton row (17).
// - palette copy: movs r3,#15 comes before the two ldrb and they swap; bitfields
//   in u16, a plain byte read and swapping the copies do not move it.
// One pointer for the whole loop is far worse (183).
#include "types.h"
#include "system.h"
#include "objdisp.h"
#include "object_effect_scripts.h"
#include "field_event.h"
#include "field_psynergy_work.h"
#include "ram_buffer.h"

struct BurstParticleVisual {
  u8 reserved00[5];
  u8 flagsALow : 5;
  u8 flip : 1;
  u8 flagsAHigh : 2;
  u8 reserved06;
  u8 flagsBLow : 6;
  u8 flagsBHigh : 2;
  union {
    struct {
      u16 shape : 10;
      u16 unused : 6;
    } bits;
    struct {
      u8 lo;
      u8 highLow : 4;
      u8 palette : 4;
    } bytes;
  } attributes;
};

// ⚓️'s sprite keeps its second part 28 bytes in.
struct BurstParticleVisualGroup {
  struct BurstParticleVisual primary;
  u8 reserved0c[16];
  struct BurstParticleVisual child;
};

void BattleEffect_InitializeSharedScene(void);
struct FieldActor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Vector_AddPolarOffset(s32 magnitude, s32 angle, s32 *pos);
void Animation_ApplyChildValuesFar(struct FieldActor *object, u32 value);
void Actor_SetSingleChildFlagsFar(struct FieldActor *object,
                                           s32 flags);

void Func_080ddbd8(void) {
  struct BattleFxScene *work;
  struct FieldActor *user;
  s32 pos[3];
  s32 n;
  s32 *p;

  work = Ram_HeapSlots->effectWork;
  user = work->mainObject;
  BattleEffect_InitializeSharedScene();

  p = pos;
  for (n = 23; n >= 0; n--) {
    struct FieldActor *obj;
    struct BurstParticleVisual *visual;
    struct BurstParticleVisual *child;
    s32 dist;

    if (*(u16 *)&work->angle == 0x4000) {
      pos[0] = user->x;
      pos[1] = user->y + 0xa0000;
      pos[2] = user->z;
    } else if (*(u16 *)&work->angle == 0xc000) {
      pos[0] = user->x;
      pos[1] = user->y + 0x180000;
      pos[2] = user->z;
    } else {
      pos[0] = user->x;
      pos[1] = user->y + 0xa0000;
      pos[2] = user->z;
      Vector_AddPolarOffset(0xa0000, *(u16 *)&work->angle, pos);
    }

    obj = Actor_Spawn(672, pos[0], pos[1], pos[2]);
    visual = &((struct BurstParticleVisualGroup *)obj->sprite)->primary;
    child = &((struct BurstParticleVisualGroup *)obj->sprite)->child;
    child->flip = visual->flip;
    child->flagsBHigh = visual->flagsBHigh;
    child->flagsAHigh = visual->flagsAHigh;
    child->attributes.bits.shape = visual->attributes.bits.shape;
    child->attributes.bytes.palette = visual->attributes.bytes.palette;

    if (obj != 0) {
      ((s32 *)obj)[7] = 0xb333;
      ((s32 *)obj)[6] = 0xb333;
      obj->acceleration = 0x18000;
      obj->speed = 0x18000;
      obj->motionFlags = 0;
      Animation_ApplyChildValuesFar(obj, 11);
      Actor_SetMode(obj, 7);
      Actor_StartScriptFar((struct ActorView *)obj,
                                   gBattleEffect_SharedParticleScript + 1);

      Actor_SetSingleChildFlagsFar(obj, 1);

      p[0] = work->x;
      p[1] = work->y;
      p[2] = work->z;
      if (*(u16 *)&work->angle == 0xc000)
        Vector_AddPolarOffset(0xe0000, *(u16 *)&work->angle, p);
      dist = Random16() * 6 + 0x40000;
      Vector_AddPolarOffset(dist, Random16(), p);
      Actor_SetTargetPosition((struct Actor *)obj, p[0], p[1], p[2]);
    }
    Sound_PlayFar(131);
    wait(2);
  }
  wait(8);
}
