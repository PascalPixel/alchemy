#ifndef TLA_FIELD_EVENT_H
#define TLA_FIELD_EVENT_H

#include "types.h"
#include "field_sprite.h"
#include "eventwrk.h"

#include "object_runtime.h"
#define Object_SetMode ((void (*)(struct FieldActor *, s32))Object_SetMode)

struct FieldActor {
    u8 unknown_00[6];
    u16 facing;
    s32 x;
    s32 y;
    s32 z;
    s32 terrain_height;
    u8 unknown_18[0x0b];
    u8 priority_flags;
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    s32 speed;
    s32 acceleration;
    s32 target_x;
    s32 target_y;
    s32 target_z;
    u8 unknown_44[0x0c];
    struct FieldSprite *sprite;
    u8 animation_kind : 4;
    u8 unknown_54_high : 4;
    u8 motion_flags;
    u8 unknown_56[3];
    u8 collision_flags;
    u8 unknown_5a[0x0a];
    u16 timer;
};

#define ACTOR_NO_TARGET ((s32)0x80000000)

void Task_Wait(s32 frames);
void GameFlag_Set(s32 flag);
void Event_Wait(s32 frames);
void Event_Begin(void);
void Event_End(void);
struct FieldActor *Actor_Get(s32 actor);
void Actor_SetAnimation(s32 actor, s32 animation);
struct FieldActor *Object_Create(s32 type, s32 x, s32 y, s32 z);
void Object_SetScript(struct FieldActor *object, const void *script);
void Object_SetPalette(struct FieldActor *object, s32 palette);
void Actor_SetSpriteFlags(struct FieldActor *actor, s32 flags);
void Map_CopyCellsTo(s32 src_x, s32 src_y, s32 dest_x, s32 dest_y, s32 width, s32 height);
void Map_CopyCellAttributes(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y);
void Audio_PlayCue(s32 cue);

void Engine_EventBegin(void);
void Engine_EventEnd(void);
void Engine_EventWait(s32 frames);
void Engine_EventResolvePendingActions(s32 skip);
void Engine_EventSetMessage(s32 message);
s32 Engine_EventOpenMessage(s32 speaker, s32 flags);
void Engine_EventShowMessage(s32 speaker, s32 flags);
s32 Engine_EventChooseYesNo(s32 actor, s32 flags);
void Engine_EventSetCountdown(s32 countdown);
void Engine_EventCloseScreen(void);
void Engine_EventWaitDelay(void);
s32 Engine_PartyGetLeaderActor(void);
struct FieldActor *Engine_ActorGet(s32 actor);
void Engine_ActorSetSpeed(s32 actor, s32 speed, s32 acceleration);
void Engine_ActorCenterAndWalk(s32 actor, s32 priority, s32 dz);
void Engine_ActorSetAnimation(s32 actor, s32 animation);
void Engine_ActorSetSpritePriority(s32 actor, s32 priority);
void Engine_ActorReturnHome(s32 mode, s32 actor, s32 flag);
s32 Engine_GameFlagIsSet(s32 flag);
void Engine_GameFlagSet(s32 flag);
void Engine_ShopOpen(s32 shop, s32 keeper);
void Engine_AudioPlayCue(s32 cue);
void Engine_MessageShowCentered(s32 message, s32 flags);
void Engine_MapCopyCellsTo(s32 src_x, s32 src_y, s32 dest_x, s32 dest_y, s32 width, s32 height);
void Engine_MapCopyCellAttributes(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y);
struct FieldActor *Engine_ObjectCreate(s32 type, s32 x, s32 y, s32 z);
void Engine_ObjectSetMode(struct FieldActor *object, s32 mode);
void Engine_ObjectSetScript(struct FieldActor *object, const void *script);

#endif
