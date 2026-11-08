// Matches: differ 0 (tla-en, 2026-11-08); tla-ja, tla-de, tla-es, tla-fr and
// tla-it differ 0 too (--symbol Func_080d7524 outside tla-en and tla-it).
// The last tie (sched2 put the sprite's reload copy `mov r8, r1` before the
// `djinni->angle` strh) fell to reading the sprite id as The Broken Seal's
// twin does, through its layer's struct (`entries[0]->animationId`), with the
// update and angle stores before the read. A halfword member takes its
// struct's alias set, disjoint from struct Actor's, so the angle store no
// longer has to precede the ldrsh and wins the tie by RTL order. Read as a
// bare s16 (short's set, which Actor's halfword members conflict with) the
// store before the read is forced ahead of the loads (6); the struct read
// with the read first keeps the tie (2).
// Japanese is the older form: no bounce sound (152) on the three falls, the
// leap's sound 136 for 153, and no gravity reset at the end, each behind
// TLA_PATCH_1.
// The Broken Seal twin (Djinn_RunBattleStartHop):
// the Djinni bounces twice, turns away from the leader, spins up and falls
// back three times (Func_080d47b4's effect 0x103 first), then leaps 16 pixels
// at the party trailing eight afterimages tinted for its sprite (243, 244 and
// 245) and waits up to 120 frames to come within 20 pixels of the leader's
// height. Through a cached u8 pointer the motion flags cost an extra zero
// (differ 25).
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "macros.h"
#include "version.h"

#if TLA_PATCH_1
#define LEAP_SOUND 153
#else
#define LEAP_SOUND 136
#endif

struct DjinniSpriteLayer {
  s16 animationId; ///< its sprite id, as in The Broken Seal's SpriteAnimation
};

struct DjinniSprite {
  u8 unk00[9];
  u8 unused09 : 2;
  u8 priority : 2;
  u8 unk09 : 4;
  u8 unk0a[30];
  struct DjinniSpriteLayer *entries[1];
};

struct Actor *Actor_GetById(s32 id);
void Event_Start(void);
void Event_End(void);
void wait(s32 frames);
void Motion_SetVarCbAndRefresh(s32 id, s32 variant);
void Actor_ArmCallback(s32 id, s32 angle, s32 frames);
void Actor_Jump(s32 id, s32 speed, s32 frames);
void *Func_080d47b4(s32 id, s32 flags, s32 state);
void FieldEffect_AdvanceSpinAngle(struct Actor *object);
struct Actor *Func_080200c0(s32 kind, s32 x, s32 y, s32 z);
void Animation_ApplyChildValuesFar(struct Actor *object, s32 value);
void Actor_SetSingleChildFlagsFar(struct Actor *object,
                                           s32 flags);
void *Sprite_ShareVramCacheEntry(void *sprite, void *last);
void Motion_SetTargetPositionFromMagnitudeAngle(struct Actor *object,
                                                s32 magnitude, s32 angle);

void Func_080d7524(s32 djinniId) {
  struct Actor *djinni;
  struct Actor *leader;
  struct Actor *copy;
  struct Actor *prev;
  void *sharedSprite;
  s32 dir;
  s32 back;
  s32 sprite;
  s32 tint;
  s32 i;
  struct Actor *copies[8];

  djinni = Actor_GetById(djinniId);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  dir = (leader->angle + 0x2000) & 0xc000;
  Event_Start();
  wait(10);
  Sound_PlayFar(173);
  Motion_SetVarCbAndRefresh(djinniId, 1);
  Sound_PlayFar(175);
  Motion_SetVarCbAndRefresh(djinniId, 1);
  wait(10);
  back = dir + 0x8000;
  Actor_ArmCallback(djinniId, back, 0);
  wait(10);
  djinni->angle = back;
  Func_080d47b4(djinniId, 0x103, 50);
  Actor_GetById(djinniId)->actionFlags &= ~1;
  djinni->motionFlags = 2;
  djinni->update = (void (*)(void))FieldEffect_AdvanceSpinAngle;
#if TLA_PATCH_1
  Sound_PlayFar(152);
#endif
  djinni->gravity = 0x4000;
  djinni->velocityY = 0x48000;
  wait(33);
  ((struct DjinniSprite *)djinni->animation)->priority = 0;
#if TLA_PATCH_1
  Sound_PlayFar(152);
#endif
  djinni->velocityY = 0x4cccc;
  wait(35);
#if TLA_PATCH_1
  Sound_PlayFar(152);
#endif
  djinni->velocityY = 0x4e666;
  wait(38);

  djinni->update = NULL;
  djinni->angle = back;
  sprite = ((struct DjinniSprite *)djinni->animation)->entries[0]->animationId;
  tint = 9;
  if (sprite == 243)
    tint = 2;
  if (sprite == 245)
    tint = 10;
  if (sprite == 244)
    tint = 9;
  sharedSprite = NULL;
  prev = djinni;
  for (i = 0; i < 8; i++) {
    copy = Func_080200c0(sprite, djinni->x, djinni->y, djinni->z);
    copies[i] = copy;
    if (copy != NULL) {
      copy->scaleY = 0xf000;
      copy->scaleX = 0xf000;
      copy->motionFlags = 0;
      copy->priorityFlags = 2;
      copy->actionFlags |= 1;
      copy->update = (void (*)(void))Actor_MoveHalfwayTowardTarget;
      copy->angle = djinni->angle;
      ((struct DjinniSprite *)copy->animation)->priority = 0;
      Animation_ApplyChildValuesFar(copy, tint);
      Actor_SetMode(copy, 0);
      Actor_SetSingleChildFlagsFar(copy, 0);
      sharedSprite = Sprite_ShareVramCacheEntry(copy->animation, sharedSprite);
      copy->watchedObject = prev;
      prev = copy;
    }
  }

  djinni->speedLimit = 0x4ccc;
  djinni->acceleration = 0x4ccc;
  Motion_SetTargetPositionFromMagnitudeAngle(djinni, 0x100000, dir + 0x8000);
  Sound_PlayFar(LEAP_SOUND);
  Actor_Jump(djinniId, 6, 5);
  wait(24);
  for (i = 0; i < 120; i++) {
    if (ABS(djinni->y - leader->y) < 0x140000)
      break;
    wait(1);
  }
  djinni->motionFlags = 0;
  djinni->velocityX = 0;
  djinni->velocityZ = 0;
  djinni->velocityY = 0;
  djinni->targetX = 0x80000000;
  djinni->targetZ = 0x80000000;
  djinni->targetY = 0x80000000;
  Actor_SetMode(djinni, 0);
  ((struct DjinniSprite *)djinni->animation)->priority = 2;
#if TLA_PATCH_1
  djinni->gravity = 0x10000;
#endif
  Event_End();
}
