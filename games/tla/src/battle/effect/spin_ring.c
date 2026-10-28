#include "types.h"
#include "actor.h"

struct Actor *Actor_GetById(u32 actor);
struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Actor_SetSingleChildFlagsFar(struct Actor *object,
                                           s32 flags);
void Event_Start(void);
void Event_ResolvePendingActions(s32 skip);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void WaitFrames(s32 frames);
void Event_WaitFrames(s32 frames);
void Actor_Destroy(struct Actor *object);
void BattleEffect_SpinObject(void);

/// An event command, the arrival counterpart of Func_080e3698. The actor
/// spins (BattleEffect_SpinObject) at zero width and its sprite is hidden while
/// object 280 grows on it for 32 frames; object 280 then snaps to 1.5 by 1
/// (cue 136, mode 6) while the actor widens back from half size over 16 frames
/// and shows again. Object 280 is destroyed and the actor's update and facing
/// are restored.
void Func_080e37e0(s32 actor) {
  struct Actor *object = Actor_GetById(actor);
  s32 angle = object->angle;
  struct Actor *ring;
  s32 i;

  object->update = BattleEffect_SpinObject;
  object->scaleX = 0;
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetSingleChildFlagsFar(Actor_GetById(actor), 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  ring = Actor_Spawn(280, object->x, object->y, object->z);
  Actor_SetMode(ring, 2);
  ring->scaleX = 0x8000;
  ring->scaleY = 0xc000;
  for (i = 0; i < 32; i++) {
    ring->scaleX += 0x800;
    /* FAKEMATCH: a one-pass block, so scaleY is loaded before the 0x200 step
       is built (differ 2 without) */
    do {
      ring->scaleY += 0x200;
    } while (0);
    WaitFrames(1);
  }
  ring->scaleX = 0x18000;
  ring->scaleY = 0x10000;
  Sound_PlayFar(136);
  Actor_SetMode(ring, 6);
  object->scaleY = object->scaleX = 0x8000;
  for (i = 0; i < 16; i++) {
    object->scaleY = object->scaleX = object->scaleX + 0x800;
    WaitFrames(1);
  }
  Actor_SetSingleChildFlagsFar(Actor_GetById(actor), 1);
  Event_WaitFrames(10);
  Actor_Destroy(ring);
  object->update = NULL;
  object->angle = angle;
}
