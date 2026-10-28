// Under -mthumb-slow-loads (2026-10-26): differ 10 of 263 in tla-en,
// instruction order only. An empty asm before `copy->actionFlags |= 1;` lowers
// it to 8.
// Near miss, differ 14 of 263: The Broken Seal twin (Djinn_RunBattleStartHop):
// the Djinni bounces twice, turns away from the leader, spins up and falls
// back three times (Func_080d47b4's effect 0x103 first), then leaps 16 pixels
// at the party trailing eight afterimages tinted for its sprite (243, 244 and
// 245) and waits up to 120 frames to come within 20 pixels of the leader's
// height. Left: instruction order only, seven reordered pairs (the leader's
// angle load, the 0x8000, the sprite copy to r8, the copies' address store,
// three in the afterimage loop). Through a cached u8 pointer the motion flags
// cost an extra zero (differ 33).
#include "types.h"
#include "actor.h"
#include "adventure.h"
#include "macros.h"

struct DjinniSprite {
  u8 unk00[9];
  u8 unused09 : 2;
  u8 priority : 2;
  u8 unk09 : 4;
  u8 unk0a[30];
  s16 *entries[1];
};

struct Actor *Actor_GetById(s32 id);
void Event_Start(void);
void Event_End(void);
void WaitFrames(s32 frames);
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
  leader = Actor_GetById(gAdventure.currentOwner);
  dir = (leader->angle + 0x2000) & 0xc000;
  Event_Start();
  WaitFrames(10);
  Sound_PlayFar(173);
  Motion_SetVarCbAndRefresh(djinniId, 1);
  Sound_PlayFar(175);
  Motion_SetVarCbAndRefresh(djinniId, 1);
  WaitFrames(10);
  back = dir + 0x8000;
  Actor_ArmCallback(djinniId, back, 0);
  WaitFrames(10);
  djinni->angle = back;
  Func_080d47b4(djinniId, 0x103, 50);
  Actor_GetById(djinniId)->actionFlags &= ~1;
  djinni->motionFlags = 2;
  djinni->update = (void (*)(void))FieldEffect_AdvanceSpinAngle;
  Sound_PlayFar(152);
  djinni->gravity = 0x4000;
  djinni->velocityY = 0x48000;
  WaitFrames(33);
  ((struct DjinniSprite *)djinni->animation)->priority = 0;
  Sound_PlayFar(152);
  djinni->velocityY = 0x4cccc;
  WaitFrames(35);
  Sound_PlayFar(152);
  djinni->velocityY = 0x4e666;
  WaitFrames(38);

  sprite = *((struct DjinniSprite *)djinni->animation)->entries[0];
  djinni->update = NULL;
  djinni->angle = back;
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
  Sound_PlayFar(153);
  Actor_Jump(djinniId, 6, 5);
  WaitFrames(24);
  for (i = 0; i < 120; i++) {
    if (ABS(djinni->y - leader->y) < 0x140000)
      break;
    WaitFrames(1);
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
  djinni->gravity = 0x10000;
  Event_End();
}
