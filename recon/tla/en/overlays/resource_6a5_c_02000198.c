// Under -mthumb-slow-loads (2026-10-26): differ 8 of 204; left: two loads issue
// one row early, and the sprite's priority byte loads before the radius store,
// as in overlay 69e's Func_020001a4.
// Near miss, differ 16 of 204: the twin of ☀️'s VenusLighthouseAerie_RunTransitionStep
// (scene/venus_lighthouse_aerie.c), with object 36, type 672, no rise counters in cases 25
// and 26 and no gated rise step at the end, its imported calls through inlines
// as ☀️'s scene layer has them (direct: differ 75). The volatile reads of the
// centre's y in cases 25 and 26 stop GCSE from reusing the compared y in the
// shared rising tail, where the native reloads it after the cue (plain: differ
// 38). Left, four sched2 order swaps: case 26 loads y before the limit's shift
// (native after); the rise's third shift comes before the y load (native
// after); the effect's address copy for the 0x66 store comes before the pool
// zero's load; and the sprite's copy out of r10 comes after the centre's speed
// load (native before). Tried 2026-10-15: the case 26 limit in a local set
// before the decrement (same); the rise inline in the call (same); barriers
// before the y updates (62, 73); alchemy permute 10 minutes (the counter
// temporary, 18 to 16).
#include "types.h"
#include "object_runtime.h"
#include "field_sprite.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

extern u32 gFrameCount;
extern s32 gData_02006a24;
extern s32 gData_0200641c[];
extern s32 gData_02006470[];

u32 Random16Far(void);
s32 Math_Sine(u32 angle);
struct ObjectRuntime *Object_GetById(s32 id);
struct ObjectRuntime *Func_0200576c(s32 type, s32 x, s32 y, s32 z);
void Func_02005764(struct ObjectRuntime *object, const void *script);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);
void Func_020059f4(s32 cue);
void Func_020057ac(s32 a, s32 b, s32 c);
void Func_02005954(s32 target, s32 mode);
void Func_02005964(s32 frames);
void SceneActor_ParkRecord(struct ObjectRuntime *object);
void ObjectMotion_EnableActionAndSetCallback(s32 id, const void *script);
void ObjectMotion_EnableActionAndResetMotion(s32 id);
void GameFlag_SetBit(s32 flag);
void Func_020000ec(void);

static inline void Event_PlayCue(s32 cue) { Event_PlayCue(cue); }

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

static inline struct ObjectRuntime *Object_Create(s32 type, s32 x, s32 y,
                                                  s32 z) {
  return Func_0200576c(type, x, y, z);
}

void Func_02000198(void) {
  struct ObjectRuntime *center;
  struct ObjectRuntime *effect;
  struct FieldSprite *sprite;
  s32 spawn;
  u32 rise;
  s32 angle;

  center = Object_GetById(36);
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
    ObjectMotion_EnableActionAndSetCallback(36, gData_0200641c);
    break;
  case 25:
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
    ObjectMotion_EnableActionAndResetMotion(36);
    GameFlag_SetBit(0x202);
    break;
  }
  if (spawn) {
    rise = ((u32)(Random16Far() * 80) >> 16) << 16;
    effect =
        Object_Create(672, center->x, center->y - rise + 0xfff80000, center->z);
    if (effect != 0) {
      sprite = effect->animation;
      Func_02005764(effect, gData_02006470);
      Object_SetPartAttribute(effect, 1);
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
