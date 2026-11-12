#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "macros.h"
#include "version.h"
#include "sprite.h"

void Actor_SetSingleChildFlagsFar(struct Actor *object, u32 flags);

#if TLA_PATCH_1
#define LEAP_SOUND 153
#else
#define LEAP_SOUND 136
#endif

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
  ((struct AnimationObject *)djinni->animation)->image.priority = 0;
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
  sprite = ((struct AnimationObject *)djinni->animation)->entries[0]->animId;
  tint = 9;
  if (sprite == 243)
    tint = 2;
  if (sprite == 245)
    tint = 10;
  // DEAD: tint is already 9 for sprite 244; the source keeps the test.
  if (sprite == 244)
    tint = 9;
  sharedSprite = NULL;
  prev = djinni;
  for (i = 0; i < 8; i++) {
    copy = Func_080200c0(sprite, djinni->x, djinni->y, djinni->z);
    // DEAD: nothing reads copies; the source keeps the store.
    copies[i] = copy;
    if (copy != NULL) {
      copy->scaleY = 0xf000;
      copy->scaleX = 0xf000;
      copy->motionFlags = 0;
      copy->priorityFlags = 2;
      copy->actionFlags |= 1;
      copy->update = (void (*)(void))Actor_MoveHalfwayTowardTarget;
      copy->angle = djinni->angle;
      ((struct AnimationObject *)copy->animation)->image.priority = 0;
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
  ((struct AnimationObject *)djinni->animation)->image.priority = 2;
#if TLA_PATCH_1
  djinni->gravity = 0x10000;
#endif
  Event_End();
}
