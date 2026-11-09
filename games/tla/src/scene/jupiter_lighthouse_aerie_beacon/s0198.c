#include "types.h"
#include "actor.h"
#include "field_sprite.h"
#include "event_inline.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

extern u32 gFrameCount;
extern s32 gData_02006a24;
extern s32 gData_0200641c[];
extern s32 gData_02006470[];

u32 Random16Far(void);
void Func_02005764(struct Actor *object, const void *script);
void SceneActor_ParkRecord(struct Actor *object);
void Func_020000ec(void);

static inline void Event_PlayCue(s32 cue) { Sound_Play(cue); }

static inline void Work_SetValuesIfNonNegative(s32 first, s32 second,
                                               s32 third) {
  Map_SetScreenShake(first, second, third);
}

static inline void ColorBuffer_ApplyTarget(s32 value, s32 mode) {
  BattleEffect_ApplyColorToTargetBuffer(value, mode);
}

static inline void ColorBuffer_Interpolate(s32 frames) {
  BattleEffect_StartBufferInterpolation(frames);
}

static inline struct Actor *Actor_Create(s32 type, s32 x, s32 y, s32 z) {
  return ((struct Actor *)Import_ActorCreate(type, x, y, z));
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
    Actor_SetScript(36, gData_0200641c);
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
    FieldActor_Stop(36);
    Event_SetFlag(0x202);
    break;
  }
  if (spawn) {
    rise = ((u32)(Random16Far() * 80) >> 16) << 16;
    effect =
        Actor_Create(672, center->x, center->y - rise + 0xfff80000, center->z);
    if (effect != 0) {
      sprite = effect->animation;
      Func_02005764(effect, gData_02006470);
      Actor_SetSpriteColor(effect, 1);
      effect->motionFlags = 0;
      angle = Random16Far() & 0xffff000;
      effect->action = angle;
      effect->unk66 = 0;
      {
        u32 counter = Random16Far() >> 13;

        BYTE_AT(effect, 0x62) = counter;
      }
      effect->update = Func_020000ec;
      effect->speedLimit = Math_Sin((u32)(Random16Far() * 0xffff) >> 20) * 24;
      effect->speedLimit = center->speedLimit >> 16;
      sprite->nextFrame = 0;
      sprite->priority = 1;
    }
  }
  gData_02006a24++;
}
