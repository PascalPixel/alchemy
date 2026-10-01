/* NONMATCHING: 2026-10-01 brief Wave2 one-device macro attempt.
 * Removing QueueTransfer's one-pass boundary at source line 87 changes:
 * Scene_RunScene371SequenceA: mov r0, r3 => strh r5, [r5] (622/622 assembly lines).
 * Production source retains and tags this measured scheduling boundary.
 */
#include "../../../../../../../games/THE BROKEN SEAL/SRC/FIELD/WORLD_MAP/STORY.H"

/* Preserve the original measured FIELD_EVENT adapter context of this draft. */
static inline void Event_Begin(void)
{
    Engine_EventBegin();
}

static inline void Event_End(void)
{
    Engine_EventEnd();
}

static inline void Event_Wait(s32 frames)
{
    Engine_EventWait(frames);
}

static inline void Task_Wait(s32 frames)
{
    Engine_TaskWait(frames);
}

static inline void Event_SetMessage(s32 message)
{
    Engine_EventSetMessage(message);
}

static inline void Event_RequestExit(s32 exit)
{
    Engine_EventRequestExit(exit);
}

static inline void Event_OpenScreen(void)
{
    Engine_EventOpenScreen();
}

static inline void Event_CloseScreen(void)
{
    Engine_EventCloseScreen();
}

static inline void Event_WaitForScreen(void)
{
    Engine_EventWaitForScreen();
}

static inline void Actor_WaitForMove(s32 actor)
{
    Engine_ActorWaitForMove(actor);
}

static inline void Actor_SetAnimation(s32 actor, s32 animation)
{
    Engine_ActorSetAnimation(actor, animation);
}

static inline void Actor_SetAnimationAndWait(s32 actor, s32 animation)
{
    Engine_ActorSetAnimationAndWait(actor, animation);
}

static inline void Actor_StartRepeatedMotion(s32 actor, s32 repeats)
{
    Engine_ActorStartRepeatedMotion(actor, repeats);
}

static inline void Actor_RunRepeatedMotion(s32 actor, s32 repeats)
{
    Engine_ActorRunRepeatedMotion(actor, repeats);
}

static inline void Actor_SetSpriteFlags(struct FieldActor *actor, s32 flags)
{
    Engine_ActorSetSpriteFlags(actor, flags);
}

static inline void Camera_FollowActor(s32 actor, s32 keep_position)
{
    Engine_CameraFollowActor(actor, keep_position);
}

static inline s32 Math_Sin(s32 angle)
{
    return Engine_MathSin(angle);
}

static inline void Object_SetAnimation(struct FieldActor *object, s32 animation)
{
    Object_SetMode(object, animation);
}

static inline void Object_SetPartPalettes(struct FieldActor *object, s32 palette)
{
    Engine_ObjectSetPartPalettes(object, palette);
}

static inline void Actor_EnableActionCallback(s32 actor, const u8 *table)
{
    Engine_ActorEnableActionCallback(actor, table);
}

static inline void ColorBuffer_Interpolate(s32 frames)
{
    Engine_ColorBufferInterpolate(frames);
}

static inline void Actor_Jump(s32 actor, s32 height, s32 frames)
{
    Engine_ActorJump(actor, height, frames);
}

#include "CALL.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "SCENE_IDS.H"
#include "FIELD_EFFECT.H"
#include "IO_REG.H"

extern u8 MsgWorldMapSukuretaHowLongWillIsland[];
extern u8 gPresentGuide9[];
extern u8 gPresentGuide8[];
extern u8 gPresentGuide5[];

enum {
    TRANSITION_EFFECT_TYPE = 222,
    TRANSITION_EFFECT_LEFT = 0x17b0,
    TRANSITION_EFFECT_TOP = 0x0c4c,
    TRANSITION_EFFECT_WIDTH = 40,
    TRANSITION_EFFECT_DEPTH = 30
};

/* The motion script each transition effect runs. */
extern const s32 gTransitionSparkScript[];
s32 __umodsi3(u32 value, s32 divisor);
extern u32 gFrameCount;
extern s32 gActorEightPuffScript[];

struct Sprite371 {
    u8 pad[9];
    u8 lo : 2;
    u8 layer : 2;
    u8 hi : 4;
};

struct Flags35 {
    u8 pad[35];
    u8 flags;
};

struct Flags38 {
    u8 pad[38];
    u8 flags;
};

struct Flags85 {
    u8 pad[85];
    u8 flags;
};

void Event_SetPairWork1c0(s32 scene, s32 entrance);

struct DisplayTransfer {
    const void *source;
    void *destination;
    u32 control;
};

struct DisplayTransferQueue {
    u16 count;
    u16 unknown_02;
    struct DisplayTransfer entries[32];
};

extern struct DisplayTransferQueue gIoWriteQueue;
u16 gWorldMapBlend;
extern const u8 gWorldMapPalettes[];
extern const u8 gWorldMapPackedTiles[];
extern const u8 gWorldMapPackedFrames[];
void *Runtime_BumpAllocateAlternatePool(s32 size);
void Sys_Free(void *buffer);
s32 Resource_DecodeType01(const void *source, void *destination);
void Map_ResumeAnimation(void);
void Map_LoadAreaGraphics(void);
void SceneEffect_RestoreBlendRegisters(void);
void FieldScene_RunLateSequence(void);

/* FAKEMATCH: the one-pass read preserves the saved-IME copy before masking.
 * Each request preserves IME while publishing one complete DMA transfer.
 * The original writes 0x0208 to IME; its enable bit is clear.
 * FAKEMATCH: the one-pass restore keeps subsequent callback setup outside
 * the queue-publication control boundary. */
#define QueueTransfer(source, destination, control) \
{ \
    u32 saved; \
    u32 *p; \
    s32 n; \
    { saved = *ime; } \
    *ime = (u16)(u32)ime; \
    n = *(u16 *)&gIoWriteQueue; \
    if (n < 32) { \
        p = (u32 *)&q->entries[n]; \
        *(u16 *)&gIoWriteQueue = n + 1; \
        *p++ = (u32)(source); \
        *p++ = (u32)(destination); \
        *p = (control); \
    } \
    do { /* FAKEMATCH: removing this one-pass boundary changes measured instruction scheduling; see its retained draft. */ *ime = saved; } while (0); \
}

#define QueueFrame(buffer, offset) QueueTransfer((buffer) + (offset), 0x06002000, 0x84000140)

/* Entrance 80's scene: Jasmine and Sukureta talk on the world map until
   actor 9 arrives and guides them off; it ends by sending the game to the
   title scene's entrance 10. */
void FieldScene_RunActorPresentationSequence(void)
{
    u8 *state = (u8 *)&gGameState;

    PaletteGlow_Update(state[0x205], state[0x206]);
    Event_Begin();
    BattleFx_ScheduleRatioTransition(0x10000, 0x12c);
    Camera_MoveTo(-1, -1, -1, 0);
    Actor_SetAnimation(ACTOR_JASMINE, 19);
    Actor_SetAnimation(8, 5);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Task_Wait(1);
    BattleFx_ScheduleRatioTransition(0x18000, 16);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    ColorBuffer_ApplyTarget(0x10003, 1);
    *(s32 *)((u8 *)gEventWork + 0x1c8) = 16;
    Event_OpenScreen();
    Event_WaitForDisplayField358Clear();
    Battle_SetObjectFlag5bWhenMode3();
    Event_Wait(40);
    Actor_RunRepeatedMotion(ACTOR_JASMINE, 1);
    Event_Wait(20);
    Event_SetMessage((s32)MsgWorldMapSukuretaHowLongWillIsland);
    Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 20);
    Actor_RunRepeatedMotion(8, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_ShowEmote(ACTOR_JASMINE, 0x107, 20);
    Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 20);
    Actor_ShowEmote(8, 0x105, 80);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_RunRepeatedMotion(ACTOR_JASMINE, 2);
    Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 20);
    Actor_ShowEmote(8, 0x105, 100);
    Actor_ShowEmote(ACTOR_JASMINE, 0x105, 40);
    Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 20);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_ShowEmote(ACTOR_JASMINE, 0x102, 20);
    Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 10);
    Actor_SetAttachedEffect(8, 0x102);
    Event_Wait(80);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_ShowEmote(ACTOR_JASMINE, 0x105, 80);
    Event_ShowMessageAndWait(8, 0, 120);
    Actor_RunRepeatedMotion(ACTOR_JASMINE, 1);
    Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 40);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_ShowEmote(ACTOR_JASMINE, 0x105, 40);
    Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 120);
    Actor_SetSpeed(9, 0x6666, 0x3333);
    Actor_SetPosition(9, 0x1ddc0000, 0xd840000);
    Actor_WalkToAndWait(9, 0x1d94, 0xd8c);
    Actor_WalkToAndWait(9, 0x1d88, 0xda0);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x6009, 0, 20);
    Actor_ShowEmote(8, 0x101, 0);
    Actor_ShowEmote(ACTOR_JASMINE, 0x101, 60);
    Actor_SetAnimationAndWait(9, 3);
    Event_ShowMessage(0x6009, 0);
    Map_LoadDefaultCellsAndUpdateBlock();
    Event_Wait(20);
    Actor_EnableActionCallback(9, gPresentGuide9);
    Event_Wait(80);
    Actor_SetAnimation(8, 1);
    Actor_Jump(8, 4, 40);
    Actor_SetAnimation(ACTOR_JASMINE, 1);
    Actor_Jump(ACTOR_JASMINE, 4, 60);
    Actor_FaceDirection(8, 0x3000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0xb000, 40);
    Actor_SetSpeed(8, 0x9999, 0x4ccc);
    Actor_SetSpeed(ACTOR_JASMINE, 0x9999, 0x4ccc);
    Actor_EnableActionCallback(8, gPresentGuide8);
    Event_Wait(20);
    Camera_SetSpeed(0xb333, 0x1666);
    Camera_MoveTo(0x1e380000, -1, 0xdc80000, 1);
    Engine_ActorEnableActionCallback(5, gPresentGuide5);
    do {
        Actor_SetAnimation(10, 6);
        Actor_SetAnimation(6, 8);
        Task_Wait(1);
    } while (*(s16 *)((u8 *)Object_GetById(5) + 100) == 0);
    Event_Wait(20);
    Actor_FaceDirection(9, 0x8000, 20);
    Actor_SetAttachedEffect(8, 0x102);
    Actor_SetAttachedEffect(ACTOR_JASMINE, 0x102);
    Event_Wait(40);
    Battle_SetObjectFlag5bWhenMode3();
    Call11(Engine_EventShowTwoMessagesAndWait, 5, 7, 13, 2, 12, 8, 9, 4, 4, 3, 0);
    Event_Wait(20);
    Map_LoadDefaultCellsAndUpdateBlock();
    Camera_SetSpeed(0x10000, 0x2000);
    Camera_MoveTo(0x1e580000, -1, 0xdc80000, 1);
    Actor_FaceDirection(9, 0x3000, 0);
    Actor_SetSpeed(8, 0x19999, 0xcccc);
    Actor_SetSpeed(ACTOR_JASMINE, 0x19999, 0xcccc);
    Actor_WalkTo(8, 0x1e7c, 0xdb8);
    Actor_WalkToAndWait(ACTOR_JASMINE, 0x1e6c, 0xdd8);
    Actor_SetAnimation(8, 1);
    Battle_SetObjectFlag5bWhenMode3();
    Event_Wait(80);
    Actor_RunRepeatedMotion(8, 1);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_JASMINE, 2);
    Event_ShowMessageAndWait(0x1005, 0, 40);
    Actor_FaceDirection(8, 0x8000, 20);
    Actor_StartRepeatedMotion(8, 2);
    Event_ShowMessageAndWait(8, 0, 60);
    Map_LoadDefaultCellsAndUpdateBlock();
    Audio_PlayCue(17);
    ColorBuffer_ApplyTarget(0, 0);
    ColorBuffer_Interpolate(120);
    Task_Wait(120);
    Event_SetPairWork1c0((s32)&SceneId_Title, 10);
}

/* Stages the scene transition and arms the timed callback that drives it. */
void StoryScene_StartTransition(void)
{
    Event_Begin();
    Audio_PlayCue(141);
    ColorBuffer_ApplySource(0, 0);
    ColorBuffer_ApplyTarget(0, 0);
    ColorBuffer_Interpolate(1);
    Task_Wait(2);
    *(s32 *)(*(u8 **)&gEventWork + 456) = 1;
    Event_OpenScreen();
    Event_WaitForScreen();
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
    Task_Wait(1);
    Camera_SetSpeed(0x40000, 0x8000);
    {
        s32 transition_delay = 3200;
        void *transition_callback = (void *)StoryScene_UpdateTransitionEffect;
        Engine_TaskAddCallback(transition_callback, transition_delay);
    }
    ColorBuffer_ApplySource(0, 0);
    ColorBuffer_ApplyTarget(0x10004, 1);
    ColorBuffer_ApplyTarget(0x10000, 2);
    ColorBuffer_Interpolate(40);
    Event_Wait(240);
    ColorBuffer_ApplyTarget(0, 0);
    ColorBuffer_Interpolate(80);
    Task_Wait(90);
    Event_RequestExit(109);
    GameFlag_Set(282);
    Event_End();
}

/*
 * The timed callback the transition schedules. Each run places one effect at
 * a random point of a 40 by 30 pixel area with a random scale, and on every
 * third frame moves the camera to one of four nearby points chosen at random.
 */
void StoryScene_UpdateTransitionEffect(void)
{
    u32 x = (u32)Random_Next() * TRANSITION_EFFECT_WIDTH >> 16;
    u32 z = (u32)Random_Next() * TRANSITION_EFFECT_DEPTH >> 16;
    struct FieldActor *object;

    object = Object_Create(TRANSITION_EFFECT_TYPE, PIXELS(x) + PIXELS(TRANSITION_EFFECT_LEFT), 0,
                           PIXELS(z) + PIXELS(TRANSITION_EFFECT_TOP));
    if (object != NULL) {
        struct FieldSprite *sprite = object->sprite;
        s32 scale = (((u32)Random_Next() << 15) >> 16) + 0x13333;

        sprite->flags = 0;
        sprite->priority = 2;
        object->motion_flags = 0;
        object->scale_x = scale;
        object->scale_y = scale;
        Object_SetAnimation(object, 1);
        Object_SetScript(object, gTransitionSparkScript);
    }
    if (gFrameCount % 3 == 0) {
        switch (((u32)Random_Next() << 2) >> 16) {
        case 0:
            Camera_MoveTo(PIXELS(0x17c7), -1, PIXELS(0x0c69), 1);
            break;
        case 1:
            Camera_MoveTo(PIXELS(0x17c9), -1, PIXELS(0x0c67), 1);
            break;
        case 2:
            Camera_MoveTo(PIXELS(0x17c9), -1, PIXELS(0x0c69), 1);
            break;
        case 3:
            Camera_MoveTo(PIXELS(0x17c7), -1, PIXELS(0x0c67), 1);
            break;
        }
    }
}

/* Drives actor 8 through a series of position/threshold setup calls and
 * advances the shared scene phase before the scene runs. */
void FieldScene_RunActorEightApproach(void)
{

    u32 i;
    s32 actor;

    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    Actor_SetAnimation(ACTOR, 2);
    Actor_SetPosition(ACTOR, 0x13080000, 0x3280000);
    actor = Actor_Get(ACTOR);
    {
        /* Write 0xa000 to the halfword at +6 of the actor record. */
        s32 value = 0xa000;

        *(u16 *)(actor + 6) = value;
    }
    Task_Wait(1);
    BattleFx_ScheduleRatioTransition(0x13333, 1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Camera_FollowActor(ACTOR, 1);
    Task_Wait(1);
    SCENE_PHASE = 0x100;
    Event_OpenScreen();
    Actor_SetSpeed(ACTOR, 0x6666, 0x3333);
    Actor_MoveToAndWait(ACTOR, 0x12d8, 0x2c8);
    Actor_MoveToAndWait(ACTOR, 0x12a8, 0x268);
    Actor_SetSpeed(ACTOR, 0x4ccc, 0x2666);
    Actor_MoveToAndWait(ACTOR, 0x12a8, 0x1d8);
    Actor_SetSpeed(ACTOR, 0x3333, 0x1999);
    Actor_MoveToAndWait(ACTOR, 0x1298, 0x1c8);
    Actor_SetSpeed(ACTOR, 0x1999, 0xccc);
    Actor_MoveToAndWait(ACTOR, 0x1298, 0x1b8);
    Actor_SetAnimation(ACTOR, 1);
    Event_Wait(40);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(110);
}

/* Totals slots 0 and 2 against slots 1 and 3 and returns the difference. */
s32 StoryScene_ComputeOpposingSlotDelta(void)
{
    s32 positive_total = StoryReward_LookupBySelection(0);
    s32 negative_total;

    positive_total += StoryReward_LookupBySelection(2);
    negative_total = StoryReward_LookupBySelection(1);
    negative_total += StoryReward_LookupBySelection(3);
    return positive_total - negative_total;
}

/* Complete reference-actor-54 selected-actor setup wrapper. */
s32 StoryReward_LookupBySelection(u32 selection)
{
    s32 flag_base = 0;
    u32 offset;

    switch (selection) {
    case 0:
        flag_base = 0x92C;
        break;
    case 1:
        flag_base = 0x935;
        break;
    case 2:
        flag_base = 0x917;
        break;
    case 3:
        flag_base = 0x990;
        break;
    }
    for (offset = 0; offset < 9; offset++) {
        if (GameFlag_IsSet(flag_base + offset) != 0) return gWorldMapRewards[offset];
    }
    return 0;
}

/* Every sixteenth frame, drop a puff beside actor 8 at a random offset. */
void WorldMap_SpawnActorEightPuff(void)
{
    u8 *leader;
    u8 *obj;
    struct Sprite371 *spr;
    u32 value;

    if ((*(s32 *)&gFrameCount & 15) != 0)
        return;
    leader = (u8 *)Object_GetById(8);
    obj = (u8 *)Engine_ObjectCreate(222, *(s32 *)(leader + 8) + -0x200000, *(s32 *)(leader + 12), *(s32 *)(leader + 16) + -0x100000);
    if (obj == 0)
        return;
    *(s32 *)(obj + 24) = 0x8000;
    *(s32 *)(obj + 28) = 0x8000;
    spr = *(struct Sprite371 **)(obj + 80);
    if ((u16)((u32)Engine_RandomNext() * 2 >> 16)) {
        s32 back = (((u32)Engine_RandomNext() * 48) >> 16) << 16;

        *(s32 *)(obj + 8) -= back >> 1;
        *(s32 *)(obj + 16) -= back;
    } else {
        value = (((u32)Engine_RandomNext() << 5) >> 16) << 16;
        *(s32 *)(obj + 8) += value;
        value = (s32)value >> 1;
        *(s32 *)(obj + 16) += value;
    }
    ((struct Flags38 *)spr)->flags = 0;
    spr->layer = ((struct Sprite371 *)*(u8 **)(leader + 80))->layer;
    ((struct Flags35 *)obj)->flags |= 2;
    ((struct Flags85 *)obj)->flags = leader[85];
    ObjectGroup_SetChildValue(obj, 9);
    Object_SetMode(obj, 2);
    Engine_ObjectSetScript(obj, (s32)gActorEightPuffScript);
}

void FieldScene_RunScene371_0200357c(void)
{
    struct FieldActor *actor;
    s32 record;

    actor = (struct FieldActor *)Object_GetById(8);
    Event_Wait(60);
    Event_Begin();
    BattleFx_ScheduleRatioTransition(0x9999, 1);
    actor->scale_x = 0x13333;
    actor->scale_y = 0x13333;
    Camera_FollowActor(8, 1);
    Task_Wait(1);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
    record = Actor_Get(ACTOR_PARTY_LEADER);
    Actor_SetSpriteFlags(record, 0);
    record = Actor_Get(8);
    Actor_SetSpriteFlags(record, 0);
    Actor_SetSpeed(8, 0x6666, 0x3333);
    actor->unknown_64 = 0;
    Engine_ActorEnableActionCallback(8, (s32)gOpeningLeaderRise);
    Engine_TaskAddCallback((s32)WorldMap_SpawnActorEightPuff, 0xc80);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    ColorBuffer_ApplyTarget(0x10003, 1);
    gEventWork->transition_frames = 32;
    Event_OpenScreen();
    Event_Wait(120);
    BattleFx_ScheduleRatioTransition(0x16666, 0x12c);
    Event_Wait(0x10e);
    gEventWork->transition_frames = 16;
    *(u16 *)0x05000000 = 0x7fff;
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(111);
}

void StoryScene_UpdateSelectedActorProgress(void)
{

    struct StorySelectionActor *actor;
    struct StoryProgressWork *scene;
    s32 progress;

    actor = Actor_Get(gGameState.selected_actor);
    scene = (struct StoryProgressWork *)gEventWork;
    actor->presentation = (u16)(*(volatile s32 *)&gFrameCount << 12);

    progress = GameFlag_GetByte(0x2f8);
    if (progress != 0) {
        if (progress == 1) {
            scene->state_one_marker = 99;
        } else if (GameFlag_IsSet(0x106) == 0) {
            progress -= 1;
        }
    }
    GameFlag_SetByte(0x2f8, progress);
}

void FieldScene_RunOpeningAuxiliarySequence(s32 a0)
{
    s32 rec2;
    struct FieldActor *actor;
    s32 record;
    u8 *p6;
    u8 *base;

    base = (u8 *)&gGameState;
    p6 = *(s32 *)(base + 500);
    actor = (struct FieldActor *)Object_GetById((s32)p6);
    rec2 = GameFlag_IsSet(0x2f0);
    if (rec2 == 0) {
        Event_Begin();
        Actor_SetAttachedEffect((s32)p6, 0x101);
        Actor_SetAnimation((s32)p6, 9);
        record = Object_GetById(a0);
        if (record != 0) {
            Actor_SetDestination((s32)p6, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Actor_WaitForMove((s32)p6);
        Audio_PlayCue(244);
        Engine_TaskAddCallback((s32)StoryScene_UpdateSelectedActorProgress, 0xc80);
        actor->motion_flags = rec2;
        Engine_ObjectSetPosition(actor, actor->x.fixed, actor->y.fixed + 0x200000, actor->z.fixed);
        Actor_WaitForMove((s32)p6);
        actor->velocity_y = rec2;
        actor->motion_flags = 4;
        *(u8 *)(base + 498) = 2;
        GameFlag_Set(0x2f0);
        GameFlag_SetByte(0x2f8, 180);
        Event_End();
        *(u16 *)((u8 *)gEventWork + 0x17c) = rec2;
    }
}

void StoryScene_SetReferenceActor(void)
{
    FieldScene_RunOpeningAuxiliarySequence(54);
}

void StoryScene_ActivateSharedState(void)
{
    gEffectWork->active = 1;
}

/* Publish the actor-98 scene state and restore its selected actor. */
void StoryScene_CompleteActor98(void)
{
    u8 *state;
    u8 *selected_actor;

    if (((struct StoryCompletionWork *)gEventWork)->scene_value == 99) {
        ((struct StoryCompletionWork *)gEventWork)->scene_value = 0;
    }
    GameFlag_Clear(0x2f0);
    GameFlag_Set(0x2f1);
    GameFlag_SetByte(0x2f8, 0);
    BattleFx_SetWeightedResult(98, 5);
    state = (u8 *)&gGameState;
    state[0x22b] = 3;
    BattleFx_SetWeightedResult(98, 7);
    selected_actor = Actor_Get(*(s32 *)(state + 500));
    selected_actor[85] = 2;
}

/* The selected actor and actor 54 rise out of sight together; the map closes
   and the party is sent to the world map's entrance 27. */
void WorldMap_RaiseActors(void)
{
    struct FieldActor *actor = Actor_Get(gGameState.selected_actor);
    struct FieldActor *other = Actor_Get(54);
    s32 frames;

    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Audio_PlayCue(219);
    Actor_SetSpriteFlags(actor, 0);
    other->motion_flags = 0;
    actor->motion_flags = 0;
    actor->velocity_y = 0;
    actor->unknown_5d[4] = 1;
    other->unknown_5d[4] = 1;
    for (frames = 59; frames >= 0; --frames) {
        actor->velocity_y += 0x3333;
        other->velocity_y += 0x3333;
        Task_Wait(1);
    }
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_End();
    GameFlag_Set(0x122);
    Event_SetPairWork1c0((s32)&SceneId_WorldMap, 27);
}

/* Restore the blend registers using the active display bank's mask. */
void SceneEffect_RestoreBlendRegisters(void)
{
    extern u16 gWorldMapBlend;

    QueueIoWriteDelay2(0x04000050, 0x3f41);
    if ((gFrameCount & 2) != 0) {
        QueueIoWriteDelay2(0x04000052, gWorldMapBlend | 0x0c);
    } else {
        QueueIoWriteDelay2(0x04000052, gWorldMapBlend | 0x10);
    }
}

void FieldScene_RunLateSequence(void)
{

    s32 record;
    s32 sx;
    s32 sy;
    u32 mode;

    record = Actor_Get(gGameState.selected_actor);
    sx = *(s16 *)(record + 10);
    sy = *(s16 *)(record + 18);
    if ((u32)(*(volatile s32 *)&gFrameCount) % 3 == 0) {
        mode = (u32)(Random_Next() << 2) >> 16;
        switch (mode) {
        case 0:
            Camera_MoveTo((sx << 16) - 0x10000, -1, (sy << 16) + 0x10000, 1);
            break;
        case 1:
            Engine_CameraMoveTo((sx << 16) + 0x10000, -1, (sy << 16) - 0x10000, 1);
            break;
        case 2:
            Camera_MoveTo((sx << 16) + 0x10000, -1, (sy << 16) + 0x10000, 1);
            break;
        case 3:
            Camera_MoveTo((sx << 16) - 0x10000, -1, (sy << 16) - 0x10000, 1);
            break;
        }
    }
}

/* Exact 1300-byte owner, resource_371:020039fc..02003f10.
 * Verified 2026-09-27: zero differing halfwords and equal topology.
 * The previous two-halfword residual was callback-address loading before
 * IME restoration. A one-pass scope around the final volatile restore keeps
 * that publication boundary intact; all queue entries and pools still match.
 * Reconstructed from our own ROM and the existing world-map queue model. */
void Scene_RunScene371SequenceA(s32 palette)
{
    struct DisplayTransferQueue *q;
    volatile u16 *ime;
    u8 *buffer = Runtime_BumpAllocateAlternatePool(0x4000);

    Engine_TaskWait(1);
    Engine_GameFlagClear(0x109);
    Map_ResumeAnimation();
    Resource_DecodeType01(gWorldMapPackedTiles, buffer);
    Resource_DecodeType01(gWorldMapPackedFrames, buffer + 0x1000);
    /* FAKEMATCH: one scope keeps the queue ahead of the IME pointer in
     * register allocation and the literal pool. */
    do {
        q = &gIoWriteQueue;
        ime = &REG_IME;
    } while (0);
    QueueTransfer(gWorldMapPalettes + palette * 32, (void *)0x050001c0, 0x80000010)
    QueueTransfer(buffer, (void *)0x06001000, 0x84000400)
    Engine_TaskAddCallback(SceneEffect_RestoreBlendRegisters, 0xc80);
    Engine_EventBegin();
    QueueFrame(buffer, 0x3a80)
    Object_GetById(gGameState.selected_actor)->active = 0;
    gEventWork->transition_frames = 16;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_AudioPlayCue(246);

    gWorldMapBlend = 0xe00;
    QueueFrame(buffer, 0x3480)
    Engine_EventWait(2);
    gWorldMapBlend = 0xd00;
    QueueFrame(buffer, 0x2e80)
    Engine_EventWait(2);
    gWorldMapBlend = 0xc00;
    QueueFrame(buffer, 0x2880)
    Engine_EventWait(2);
    gWorldMapBlend = 0xb00;
    QueueFrame(buffer, 0x2280)
    Engine_EventWait(2);
    gWorldMapBlend = 0xa00;
    QueueFrame(buffer, 0x1c80)
    Engine_EventWait(2);
    gWorldMapBlend = 0x900;
    QueueFrame(buffer, 0x1680)
    Engine_EventWait(2);
    gWorldMapBlend = 0x800;
    QueueFrame(buffer, 0x1080)
    Engine_EventWait(140);

    QueueFrame(buffer, 0x1680)
    Engine_EventWait(4);
    QueueFrame(buffer, 0x1c80)
    Engine_EventWait(4);
    QueueFrame(buffer, 0x2280)
    Engine_EventWait(4);
    gWorldMapBlend = 0x900;
    QueueFrame(buffer, 0x2880)
    Engine_EventWait(4);
    gWorldMapBlend = 0xa00;
    QueueFrame(buffer, 0x2e80)
    Engine_EventWait(4);
    gWorldMapBlend = 0xb00;
    QueueFrame(buffer, 0x3480)
    Engine_EventWait(4);
    gWorldMapBlend = 0xc00;
    QueueFrame(buffer, 0x3a80)
    Map_LoadAreaGraphics();
    Engine_TaskAddCallback(FieldScene_RunLateSequence, 0xc80);
    Engine_AudioPlayCue(141);
    gWorldMapBlend = 0xd00;
    Engine_EventWait(4);
    gWorldMapBlend = 0xe00;
    Engine_EventWait(4);
    gWorldMapBlend = 0xf00;
    Engine_EventWait(4);
    gWorldMapBlend = 0x1000;
    Engine_EventWait(45);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Sys_Free(buffer);
    Engine_GameFlagSet(0x101);
}

void SceneActor_UpdateObjectByCounterBits(u8 *obj)
{
    if ((gFrameCount & 2) != 0) {
        Object_SetPartPalettes(obj, 7);
    } else {
        Object_SetPartPalettes(obj, 0);
    }
    if ((gFrameCount & 15) == 0) {
        WorldMap_CreateLinkedEffects(obj);
    }
}

/* Both frame-parity steps read the frame counter afresh at every use. */
void OverlayObject_UpdateOnFrameParity(u8 *obj)
{
    if ((*(volatile u32 *)&gFrameCount & 1) != 0) {
        Object_SetPartPalettes(obj, __umodsi3((s32)(*(volatile u32 *)&gFrameCount >> 1), 6));
    }
    if ((*(volatile u32 *)&gFrameCount & 15) == 0) {
        WorldMap_CreateLinkedEffects(obj);
    }
}

void SceneState_RunSlotStepOnOddFrames(s32 arg0)
{
    if ((*(volatile s32 *)&gFrameCount & 1) != 0) {
        s32 slot = ((u32)*(volatile s32 *)&gFrameCount >> 1) % 6;

        Object_SetPartPalettes(arg0, slot);
    }
}

void Effect_AnimateVerticalPositive(struct StoryVerticalEffectActor *effect)
{
    struct StoryVerticalEffectActor *anchor_actor;
    s32 animation_frame;
    s32 vertical_amplitude;

    anchor_actor = effect->anchor;
    effect->frame = (u16)(effect->frame + 1);
    animation_frame = (s16)effect->frame;

    if (animation_frame > 31) {
        Engine_ObjectDispatchRelease(effect);
        return;
    }

    vertical_amplitude = Math_Sin(animation_frame << 10);
    effect->amplitude_x = vertical_amplitude;
    effect->amplitude_y = vertical_amplitude;
    effect->x = anchor_actor->x;
    effect->y += 0x10000;
    effect->z = anchor_actor->z + (0x10000 - vertical_amplitude) * 5 + 0x80000;
}

void Effect_AnimateVerticalNegative(struct StoryVerticalEffectActor_02004004 *effect)
{
    struct StoryVerticalEffectActor_02004004 *anchor_actor;
    s32 animation_frame;
    s32 vertical_amplitude;

    anchor_actor = effect->anchor;
    effect->frame = (u16)(effect->frame + 1);
    animation_frame = (s16)effect->frame;

    if (animation_frame > 31) {
        Engine_ObjectDispatchRelease(effect);
        return;
    }

    vertical_amplitude = Math_Sin(animation_frame << 10);
    effect->amplitude_x = vertical_amplitude;
    effect->amplitude_y = -vertical_amplitude;
    effect->x = anchor_actor->x;
    effect->y += 0x10000;
    effect->z = anchor_actor->z - (0x10000 - vertical_amplitude) * 5 + 0x100000;
}
