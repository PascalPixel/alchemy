#ifndef ALCHEMY_FIELD_EVENT_H
#define ALCHEMY_FIELD_EVENT_H

#include "LAYOUT_GUARD.H"

/*
 * What a field scene's event scripts ask of the engine: holding the party
 * still while an event plays, dialogue, actor motion, flags, sound, exits
 * and the shop, inn and sanctum services.
 */

/* The part of the saved game that records where the party is. */
struct GameState {
    u8 unknown_000[0x1c0];
    s16 scene;
    s16 entrance;
    s16 saved_scene;
    s16 saved_entrance;
};

LAYOUT_OFFSET_GUARD(GameState_Entrance, struct GameState, entrance, 0x1c2);

extern struct GameState gGameState;

/* The engine's event work. */
struct EventWork {
    u8 unknown_000[0x1c0];
    /* How the screen opens when the scene starts. */
    s32 start_transition;
};

LAYOUT_OFFSET_GUARD(EventWork_StartTransition, struct EventWork, start_transition, 0x1c0);

extern struct EventWork *gEventWork;

/* A scene transition is a style in the high byte and its variant in the low byte. */
enum SceneTransitionStyle {
    TRANSITION_FADE = 0,
    TRANSITION_WINDOW = 2
};

#define SCENE_TRANSITION(style, variant) ((style) << 8 | (variant))

/* The runtime actor as event scripts see it. */
struct FieldActor {
    u8 unknown_00[6];
    u16 facing;
    u8 unknown_08[0x51];
    u8 collision_flags;
};

LAYOUT_OFFSET_GUARD(FieldActor_Facing, struct FieldActor, facing, 0x06);
LAYOUT_OFFSET_GUARD(FieldActor_CollisionFlags, struct FieldActor, collision_flags, 0x59);

/* Holding the party still while an event plays. */
void Event_Begin(void);
void Event_End(void);
void Event_Wait(s32 frames);

/* Dialogue: choose the message, then let an actor say it. Each message shown
   advances to the next one. */
void Event_SetMessage(s32 message);
void Event_ShowMessage(s32 speaker, s32 flags);
s32 Event_AskYesNo(s32 speaker, s32 flags);

void Event_RequestExit(s32 exit);

struct FieldActor *Actor_Get(s32 actor);
void Actor_FaceActor(s32 actor, s32 target, s32 frames);
void Actor_FaceEachOther(s32 actor, s32 other, s32 frames);
s32 Actor_SetAnimationAndWait(s32 actor, s32 animation);
void Actor_StartRepeatedMotion(s32 actor, s32 repeats);
void Actor_RunRepeatedMotion(s32 actor, s32 repeats);
void Actor_SetSpriteFlags(struct FieldActor *actor, s32 flags);

s32 GameFlag_IsSet(s32 flag);
s32 GameFlag_Set(s32 flag);
void GameFlag_Clear(s32 flag);

void Audio_PlayCue(s32 cue);

void Shop_Open(s32 shop, s32 keeper);
void Inn_Open(s32 inn, s32 keeper);
void Sanctum_Open(s32 priest);

#endif
