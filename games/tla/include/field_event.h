#ifndef TLA_FIELD_EVENT_H
#define TLA_FIELD_EVENT_H

#include "types.h"
#include "field_sprite.h"
#include "event_work.h"

#include "actor.h"
#define Actor_SetMode ((void (*)(struct FieldActor *, s32))Actor_SetMode)

struct FieldActor {
  u8 unk00[6];
  u16 facing;
  s32 x;
  s32 y;
  s32 z;
  s32 terrainHeight;
  u8 unk18[0x0b];
  u8 priorityFlags;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 speed;
  s32 acceleration;
  s32 targetX;
  s32 targetY;
  s32 targetZ;
  u8 unk44[0x0c];
  struct FieldSprite *sprite;
  u8 animationKind : 4;
  u8 unk54High : 4;
  u8 motionFlags;
  u8 unk56[3];
  u8 collisionFlags;
  u8 unk5a[0x0a];
  u16 timer;
};

#define ACTOR_NO_TARGET ((s32)0x80000000)

void Task_Wait(s32 frames);
void GameFlag_Set(s32 flag);
void Event_Wait(s32 frames);
void Event_Start(void);
void Event_End(void);
struct FieldActor *Actor_Get(s32 actor);
void Actor_SetAnimation(s32 actor, s32 animation);
struct FieldActor *Actor_Create(s32 type, s32 x, s32 y, s32 z);
void Actor_SetScript(struct FieldActor *object, const void *script);
void Actor_SetPalette(struct FieldActor *object, s32 palette);
void Actor_SetSpriteFlags(struct FieldActor *actor, s32 flags);
void Map_CopyCellsTo(s32 srcX, s32 srcY, s32 destX, s32 destY, s32 width,
                     s32 height);
void Map_CopyCellAttributes(s32 srcX, s32 srcY, s32 width, s32 height,
                            s32 destX, s32 destY);
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
struct FieldActor *Import_ActorGet(s32 actor);
void Import_ActorSetSpeed(s32 actor, s32 speed, s32 acceleration);
void Import_ActorCenterAndWalk(s32 actor, s32 priority, s32 dz);
void Import_ActorSetAnimation(s32 actor, s32 animation);
void Import_ActorSetSpritePriority(s32 actor, s32 priority);
void Import_ActorReturnHome(s32 mode, s32 actor, s32 flag);
s32 Import_GameFlagIsSet(s32 flag);
void Import_GameFlagSet(s32 flag);
void Import_ShopOpen(s32 shop, s32 keeper);
void Import_SoundPlay(s32 cue);
void Import_FieldMessageShow(s32 message, s32 flags);
void Import_MapCopyCellPictureIndicesRect(s32 srcX, s32 srcY, s32 destX, s32 destY, s32 width,
                           s32 height);
void Import_MapCopyCellAttributeRect(s32 srcX, s32 srcY, s32 width, s32 height,
                                  s32 destX, s32 destY);
struct FieldActor *Import_ActorCreate(s32 type, s32 x, s32 y, s32 z);
void Import_ActorSetMode(struct FieldActor *object, s32 mode);
void Import_ActorSetScript(struct FieldActor *object, const void *script);

#endif
