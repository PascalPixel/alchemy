#ifndef TLA_FIELD_EVENT_H
#define TLA_FIELD_EVENT_H

#include "types.h"
#include "field_sprite.h"
#include "event_work.h"

#include "actor.h"

/// struct Actor as the two scene routines that keep a flag number at 0x64 read
/// it: as Actor.timer, the s16, their store of that number compiles
/// differently (Tundaria Tower's and the Mars Lighthouse entrance's
/// placement tables).
struct FieldActor {
  u8 unk00[0x55];
  u8 motionFlags;
  u8 unk56[3];
  u8 collisionFlags;
  u8 unk5a[0x0a];
  u16 timer;
};

#define ACTOR_NO_TARGET ((s32)0x80000000)

void Task_Wait(s32 frames);
struct Actor *Actor_Create(s32 type, s32 x, s32 y, s32 z);
void Sound_PlayFar(s32 cue);

void Import_EventStart(void);
void Import_EventEnd(void);
void Import_EventWait(s32 frames);
void Import_EventResolvePendingActions(s32 skip);
void Import_EventSetMessageId(s32 message);
s32 Import_FieldMessageShowAtActor(s32 speaker, s32 flags);
void Import_EventShowMessageAndWait(s32 speaker, s32 flags);
s32 Import_EventAskYesNoWithActorAnimation(s32 actor, s32 flags);
void Import_EventSetExitRequest(s32 countdown);
void Import_EventFinishScreenTransition(void);
void Import_EventWaitTransitionFrames(void);
s32 Import_PartyGetLeaderActor(void);
struct Actor *Import_ActorGet(s32 actor);
void Import_ActorSetSpeed(s32 actor, s32 speed, s32 acceleration);
void Import_ActorCenterAndWalk(s32 actor, s32 priority, s32 dz);
void Import_ActorSetAnimation(s32 actor, s32 animation);
void Import_ActorSetSpritePriority(s32 actor, s32 priority);
void Import_ActorReturnHome(s32 mode, s32 actor, s32 flag);
s32 Import_GameFlagIsSet(s32 flag);
void Import_GameFlagSet(s32 flag);
void Import_ShopRun(s32 shop, s32 keeper);
void Import_SoundPlay(s32 cue);
void Import_FieldMessageShow(s32 message, s32 flags);
void Import_MapCopyCellPictureIndicesRect(s32 srcX, s32 srcY, s32 destX, s32 destY, s32 width,
                           s32 height);
void Import_MapCopyCellAttributeRect(s32 srcX, s32 srcY, s32 width, s32 height,
                                  s32 destX, s32 destY);
struct Actor *Import_ActorCreate(s32 type, s32 x, s32 y, s32 z);
void Import_ActorSetMode(struct Actor *object, s32 mode);
void Import_ActorStartScript(struct Actor *object, const void *script);

#endif
