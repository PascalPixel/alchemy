// Near miss, differ 6 of 204 (tla-en, 2026-10-31): two sched2 orders in the
// spawned effect's tail, both from -mthumb-slow-loads's cost model. The
// sprite's copy out of r10 (mov r1, sl) issues after the centre's speed load
// (movs r2, #50; ldrsh) where the native issues it before: the sched2 dump
// readies both one cycle after the sine's store to speedLimit and ranks the
// load first (priority 9 against 7, the load heading r3's chain through the
// next four stores); the native's order needs the load to wait a second cycle
// after that store, and the option's load-after-store wait skips a
// sign-extending load. And, as in overlay 69e's Func_020001a4
// (resource_69e_c_020001a4.c), the sprite's priority byte loads before the
// second speedLimit store; the native loads it after the store and after the
// zero's copy out of r8. Tried 2026-10-31: the speed read as the s16 at offset
// 50 (14); the two sprite stores swapped (15); the sprite read from
// effect->animation after the stores (28); the sprite stores through a block
// local (6); the two sprite stores in a one-pass block (4: the priority byte
// then loads where the native does, but the copy out of r10 moves after the
// zero's copy and the effect's and 1's setup for Actor_SetPartAttribute swap);
// a one-pass block before the second speed store (73: the function's
// registers reallocate); alchemy permute 4 minutes (nothing under 6; its best
// weighted, differ 8, a one-pass block).
// The twin of ☀️'s VenusLighthouseAerie_UpdateBeaconRise
// (scene/venus_lighthouse_aerie.c), with object 36, type 672, no rise counters
// in cases 25 and 26 and no gated rise step at the end, its imported calls
// through inlines as ☀️'s scene layer has them (direct: differ 45,
// 2026-10-31). The volatile reads of the centre's y in cases 25 and 26 stop
// GCSE from reusing the compared y in the shared rising tail, where the native
// reloads it after the cue (plain: differ 28, 2026-10-31; either one alone
// plain: 57 or 59). Tried 2026-10-15, under the earlier options: the case 26
// limit in a local set before the decrement (same); the rise inline in the
// call (same); barriers before the y updates (62, 73); alchemy permute 10
// minutes (the counter temporary, 18 to 16).
#include "types.h"
#include "actor.h"
#include "field_sprite.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

extern u32 gFrameCount;
extern s32 gData_02006a24;
extern s32 gData_0200641c[];
extern s32 gData_02006470[];

u32 Random16Far(void);
s32 Math_Sine(u32 angle);
struct Actor *Actor_GetById(s32 id);
struct Actor *Func_0200576c(s32 type, s32 x, s32 y, s32 z);
void Func_02005764(struct Actor *object, const void *script);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void Func_020059f4(s32 cue);
void Func_020057ac(s32 a, s32 b, s32 c);
void Func_02005954(s32 target, s32 mode);
void Func_02005964(s32 frames);
void SceneActor_ParkRecord(struct Actor *object);
void Actor_SetMovementTypeById(s32 id, const void *script);
void Actor_Stop(s32 id);
void GameFlag_SetBit(s32 flag);
void Func_020000ec(void);

static inline void Event_PlayCue(s32 cue) { Func_020059f4(cue); }

static inline void Work_SetValuesIfNonNegative(s32 first, s32 second,
                                               s32 third) {
  Func_020057ac(first, second, third);
}

static inline void ColorBuffer_ApplyTarget(s32 value, s32 mode) {
  Func_02005954(value, mode);
}

static inline void ColorBuffer_Interpolate(s32 frames) {
  Func_02005964(frames);
}

static inline struct Actor *Actor_Create(s32 type, s32 x, s32 y,
                                                  s32 z) {
  return Func_0200576c(type, x, y, z);
}

void Func_02000198(void) {
  struct Actor *center;
  struct Actor *effect;
  struct FieldSprite *sprite;
  s32 spawn;
  u32 rise;
  s32 angle;

  center = Actor_GetById(36);
  spawn = 0;
  switch (gData_02006a24) {
  case 0:
    Event_PlayCue(220);
    Work_SetValuesIfNonNegative(0x60000, 0x60000, 0x10000);
    ColorBuffer_ApplyTarget(0x2063ff, 1);
    ColorBuffer_Interpolate(8);
    break;
  case 8:
    ColorBuffer_ApplyTarget(0x10000, 1);
    ColorBuffer_Interpolate(8);
    break;
  case 16:
    Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
    break;
  case 24:
    center->x = 152 << 17;
    center->y = -0x1680000;
    center->z = 164 << 16;
    center->scaleX = 0x10000;
    center->scaleY = 0x10000;
    SceneActor_ParkRecord(center);
    Actor_SetMovementTypeById(36, gData_0200641c);
    break;
  case 25:
    /* FAKEMATCH(volatile): the centre's y read through a volatile pointer in
     * cases 25 and 26; as center->y in both, 28 of 204 instructions differ in
     * tla-en. */
    gData_02006a24--;
    if (*(volatile s32 *)&center->y > 0) {
      ColorBuffer_ApplyTarget(0x203210, 0);
      ColorBuffer_Interpolate(16);
      gData_02006a24++;
    } else {
      if ((gFrameCount & 7) == 0)
        Event_PlayCue(246);
      spawn = 1;
      center->y += 0x24000;
    }
    break;
  case 26:
    gData_02006a24--;
    if (*(volatile s32 *)&center->y > (160 << 14)) {
      ColorBuffer_ApplyTarget(0x10000, 0);
      ColorBuffer_Interpolate(40);
      gData_02006a24++;
    } else {
      if ((gFrameCount & 7) == 0)
        Event_PlayCue(246);
      spawn = 1;
      center->y += 0x24000;
    }
    break;
  case 27:
  case 28:
  case 29:
  case 30:
  case 31:
  case 32:
  case 33:
  case 34:
    spawn = 1;
    break;
  case 36:
    Event_PlayCue(187);
    ColorBuffer_ApplyTarget(0x7fff, 0);
    ColorBuffer_Interpolate(12);
    break;
  case 48:
    Actor_Stop(36);
    GameFlag_SetBit(0x202);
    break;
  }
  if (spawn) {
    rise = ((u32)(Random16Far() * 80) >> 16) << 16;
    effect =
        Actor_Create(672, center->x, center->y - rise + 0xfff80000, center->z);
    if (effect != 0) {
      sprite = effect->animation;
      Func_02005764(effect, gData_02006470);
      Actor_SetPartAttribute(effect, 1);
      effect->motionFlags = 0;
      angle = Random16Far() & 0xffff000;
      effect->action = angle;
      effect->unk66 = 0;
      {
        u32 counter = Random16Far() >> 13;

        BYTE_AT(effect, 0x62) = counter;
      }
      effect->update = Func_020000ec;
      effect->speedLimit = Math_Sine((u32)(Random16Far() * 0xffff) >> 20) * 24;
      effect->speedLimit = center->speedLimit >> 16;
      sprite->nextFrame = 0;
      sprite->priority = 1;
    }
  }
  gData_02006a24++;
}
