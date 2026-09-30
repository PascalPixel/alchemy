#ifndef TLA_FIELD_EVENT_H
#define TLA_FIELD_EVENT_H

#include "TYPES.H"

/*
 * What a field scene's scripts ask of the engine. Each entry is reached
 * through one of the overlay's import veneers, which the scene's translation
 * unit binds by name.
 */

/* The sprite an object draws; its two parts each carry a priority. */
struct FieldSprite {
    u8 unknown_00[9];
    u8 unknown_09 : 2;
    u8 priority : 2;
    u8 unknown_09_high : 4;
    u8 unknown_0a[8];
    u16 unknown_12;
    u8 unknown_14[6];
    u8 unknown_1a;
    u8 unknown_1b[10];
    u8 unknown_25 : 2;
    u8 part_priority : 2;
    u8 unknown_25_high : 4;
};

/* The runtime object behind an actor or a spawned effect. */
struct FieldActor {
    u8 unknown_00[6];
    /* The direction the actor faces; 0xc000 faces up the screen. */
    u16 facing;
    s32 x;
    s32 y;
    s32 z;
    s32 unknown_14;
    u8 unknown_18[0x0b];
    /* Bit 0 is cleared when a script sets the sprite priority. */
    u8 priority_flags;
    u8 unknown_24[0x2c];
    struct FieldSprite *sprite;
    u8 unknown_54 : 4;
    u8 unknown_54_high : 4;
    /* Keeps the actor on the terrain height and lets it fall; cleared to
       move it freely. */
    u8 motion_flags;
    u8 unknown_56[0x0e];
    /* Counts up while the object's update runs. */
    u16 timer;
};

void Task_Wait(s32 frames);
void GameFlag_Set(s32 flag);
void Event_Wait(s32 frames);
void Event_Begin(void);
void Event_End(void);
void Event_SetMode(s32 mode);
struct FieldActor *Actor_Get(s32 actor);
void Actor_SetAnimation(s32 actor, s32 animation);
s32 Party_GetLeaderActor(void);
void Message_Show(s32 message, s32 flags);
struct FieldActor *Object_Create(s32 type, s32 x, s32 y, s32 z);
void Object_SetMode(struct FieldActor *object, s32 mode);
void Object_SetScript(struct FieldActor *object, const void *script);
void Object_SetPalette(struct FieldActor *object, s32 palette);
void Actor_SetSpriteFlags(struct FieldActor *actor, s32 flags);
void Map_CopyCellsTo(s32 src_x, s32 src_y, s32 dest_x, s32 dest_y, s32 width, s32 height);
void Map_CopyCellAttributes(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y);
void Audio_PlayCue(s32 cue);

/* The engine's event work. */
struct EventWork {
    u8 unknown_000[0x154];
    /* The exit the current event leaves by. */
    s16 exit;
};

/*
 * The engine entry points a scene overlay imports, named as The Broken
 * Seal's scenes name them. Each is one of the overlay's import veneers.
 */
void Engine_EventBegin(void);
void Engine_EventEnd(void);
void Engine_EventWait(s32 frames);
/* Readies the party members who may speak in the event; 0 for all. */
void Engine_EventPrepareSpeakers(s32 flags);
void Engine_EventSetMessage(s32 message);
s32 Engine_EventOpenMessage(s32 speaker, s32 flags);
void Engine_EventShowMessage(s32 speaker, s32 flags);
s32 Engine_EventChooseYesNo(s32 actor, s32 flags);
void Engine_EventRequestExit(s32 exit);
void Engine_EventCloseScreen(void);
void Engine_EventWaitForScreen(void);
/* The actor that stands for the party: its leader. */
s32 Engine_PartyGetLeaderActor(void);
struct FieldActor *Engine_ActorGet(s32 actor);
void Engine_ActorSetSpeed(s32 actor, s32 speed, s32 acceleration);
void Engine_ActorCenterAndWalk(s32 actor, s32 priority, s32 dz);
void Engine_ActorSetAnimation(s32 actor, s32 animation);
void Engine_ActorSetSpritePriority(s32 actor, s32 priority);
/* Answers a Psynergy search: in mode 2 sets the flag and puts the actor
   back where the map placed it. */
void Engine_ActorReturnHome(s32 mode, s32 actor, s32 flag);
s32 Engine_GameFlagIsSet(s32 flag);
void Engine_GameFlagSet(s32 flag);
void Engine_ShopOpen(s32 shop, s32 keeper);
void Engine_AudioPlayCue(s32 cue);
void Engine_MapCopyCellsTo(s32 src_x, s32 src_y, s32 dest_x, s32 dest_y, s32 width, s32 height);
void Engine_MapCopyCellAttributes(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y);
struct FieldActor *Engine_ObjectCreate(s32 type, s32 x, s32 y, s32 z);
void Engine_ObjectSetMode(struct FieldActor *object, s32 mode);
void Engine_ObjectSetScript(struct FieldActor *object, const void *script);

#endif
