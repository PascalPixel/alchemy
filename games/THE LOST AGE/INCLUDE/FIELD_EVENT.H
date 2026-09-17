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
    u8 unknown_00[8];
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
    u8 unknown_55;
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

#endif
