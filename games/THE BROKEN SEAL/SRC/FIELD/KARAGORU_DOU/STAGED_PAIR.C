#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#define NULL ((void *)0)

#include "STAGED_ACTOR_PAIR_SCENE.H"
#include "STAGED_ACTOR.H"

enum StagedPairMessage {
    MSG_WARRIORS_HAVE_BEEN_FIGHTING_WHILE = 0x23d2,
    MSG_WE_MISSED_COLOSSO_BECAUSE_WE = 0x23d5,
    MSG_IVE_BEEN_WAITING_FOR_ROBIN = 0x23d9,
    MSG_WHY_GOING_BACK_ROBIN_DO = 0x23da
};


struct EffectRecord {
    u8 pad[9];
    u8 flags_lo : 2;
    u8 mode : 2;
    u8 flags_hi : 4;
};

struct EffectWork {
    u8 pad[80];
    struct EffectRecord *record;
};

/*
 * resource_3be owner at 0x020011d8, 32 bytes.
 *
 * Runs a step at most 40 times, stopping early once the caller's +12 field has
 * come down to the limit. Both the counter and the field test guard the loop.
 */
struct HeightTrackedObject {
    u8 pad00[12];
    s32 height;                 /* +12 */
};

extern u8 Value_00000098;
extern u8 Value_0000009d;
extern u8 Value_0000009e;
extern u8 Data_020097b4[];
extern u8 Data_020097fc[];
extern u8 Data_02009874[];
extern u8 Data_02009784[];
extern u8 Data_00000088[];
extern u8 Data_00000098[];
extern u8 Data_0000009d[];
extern u8 Data_0000009e[];
extern u8 Data_0200995c[];
extern u8 Data_02009974[];
extern u8 Data_020099d4[];
extern u8 Data_02009a4c[];
extern u8 Data_02009aac[];
extern u8 Data_02009b3c[];
extern u8 Data_02009b48[];
extern u8 Data_02009bcc[];
extern u8 Data_02009c80[];
extern u8 Data_02009ce0[];
extern s16 Data_02000240_t[][1];
extern u8 Data_000023cc[];

s32 *Func_02002698();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}

static __inline__ s32 Value1_02000fd0(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void PlaceActor(s32 actor, s32 x, s32 y)
{
    Actor_SetPosition(actor, x, y);
}

void SetEffectRecordMode(struct EffectWork *work, s32 mode)
{
    work->record->mode = mode;
}

void *StagedActorPairScene_SpawnPrimaryEffect(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *effect = Object_Create(kind, x, y, z);

    if (effect != NULL) {
        u8 *sprite = *(u8 **)(effect + 0x50);
        s32 sprite_flags;
        s32 flag_mask = 13;

        sprite_flags = sprite[9];
        flag_mask = -flag_mask;
        flag_mask &= sprite_flags;
        sprite[9] = flag_mask;
        effect[0x55] = 0;
        effect[0x59] = 8;
        Actor_SetSpriteFlags(effect, 0);
        Object_SetPalette(effect, 14);
        Object_SetBlendMode(effect, 1);
        return effect;
    }
    return NULL;
}

void *StagedActorPairScene_SpawnSecondaryEffect(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *effect = Object_Create(kind, x, y, z);

    if (effect != NULL) {
        u8 *sprite = *(u8 **)(effect + 0x50);
        s32 sprite_flags;
        s32 flag_mask = 13;

        sprite_flags = sprite[9];
        flag_mask = -flag_mask;
        flag_mask &= sprite_flags;
        flag_mask |= 4;
        sprite[9] = flag_mask;
        effect[0x55] = 0;
        effect[0x59] = 8;
        Actor_SetSpriteFlags(effect, 0);
        Object_SetPalette(effect, 15);
        effect[0x23] = (effect[0x23] & 0xfe) | 2;
        return effect;
    }
    return NULL;
}

s32 StagedActorPairScene_GetVariantData(void)
{
    s16 scene_id = gGameState.scene;

    if (scene_id == (s32)&Value_00000098) {
        return (s32)Data_020097b4;
    }
    if (scene_id == (s32)&Value_0000009d) {
        return (s32)Data_020097fc;
    }
    if (scene_id == (s32)&Value_0000009e) {
        return (s32)Data_02009874;
    }
    return (s32)Data_02009784;
}

s32 StagedActorPairScene_GetInitialValue(void) { return 0; }

u8 *StagedActorPairScene_GetMessageData(void)
{
    return (u8 *)0x0200991c;
}

s32 Func_02000d20(void)
{
    if (Data_02000240_t[224][0] == (s32)Data_00000098) {
        if (GameFlag_IsSet(0x96f) != 0) {
            return (s32)Data_020099d4;
        }
        return (s32)Data_02009974;
    } else {
        if (Data_02000240_t[224][0] == (s32)Data_0000009d) {
            return (s32)Data_02009a4c;
        } else {
            if (Data_02000240_t[224][0] == (s32)Data_0000009e) {
                return (s32)Data_02009aac;
            } else {
            }
        }
    }
    return (s32)Data_0200995c;
}

s32 FieldScene_RunScene3beSequenceA(void)
{
    if (Data_02000240_t[224][0] == (s32)Data_00000098) {
        if (GameFlag_IsSet(0x96f) != 0) {
            return (s32)Data_02009bcc;
        }
        return (s32)Data_02009b48;
    } else {
        if (Data_02000240_t[224][0] == (s32)Data_0000009d) {
            return (s32)Data_02009c80;
        } else {
            if (Data_02000240_t[224][0] == (s32)Data_0000009e) {
                return (s32)Data_02009ce0;
            } else {
            }
        }
    }
    return (s32)Data_02009b3c;
}

void FieldScene_RunScene3beSequenceB(void)
{
    s32 record;

    if (GameFlag_IsSet(0x98a) == 0 && GameFlag_IsSet(0x9a0) != 0) {
        Event_Begin();
        Actor_SetSpeed(11, 0x10000, 0x8000);
        record = Value1(Engine_ActorGet, ACTOR_PARTY_LEADER);
        if (record != 0) {
            Actor_SetPosition(11, *(s32 *)(record + 8), *(s32 *)(record + 16));
        }
        Actor_SetDestinationOffset(11, -8, 16);
        Actor_WaitForMove(11);
        Actor_FaceDirection(11, 0xd000, 0);
        Event_Wait(10);
        Actor_FaceActor(ACTOR_PARTY_LEADER, 11, 0);
        Event_SetMessage(MSG_WHY_GOING_BACK_ROBIN_DO);
        Event_OpenMessage(11, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            Event_ShowMessage(11, 0);
            Actor_WalkTo(11, 152, 232);
            GameFlag_Clear(0x9a0);
            Actor_WaitForMove(11);
            Actor_SetAnimation(11, 1);
            Data_02000240_t[226][0] = (s32)Data_00000088;
            Data_02000240_t[227][0] = 30;
        } else {
            bump_step(1);
            Event_ShowMessage(11, 0);
            Actor_SetAnimation(11, 2);
            record = Value1(Engine_ActorGet, ACTOR_PARTY_LEADER);
            if (record != 0) {
                Actor_SetDestination(11, *(s16 *)(record + 10), *(s16 *)(record + 18));
            }
            Actor_WaitForMove(11);
            Actor_SetPosition(11, 0, 0);
            Event_Wait(30);
            Actor_SetAnimation(ACTOR_PARTY_LEADER, 2);
            Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 16);
            Actor_WaitForMove(ACTOR_PARTY_LEADER);
            Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
        }
        Event_End();
    }
}

void ActorPresentation_RunActorElevenRecoveryScene(void)
{
    Event_Begin();
    Actor_FaceActor(11, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 11, 0);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Event_Wait(10);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, 11, 0);
    Event_SetMessage(MSG_IVE_BEEN_WAITING_FOR_ROBIN);
    Event_ShowMessage(11, 0);
    Actor_SetAnimation(11, 2);
    {
        s16 *position = Actor_Get(ACTOR_PARTY_LEADER);

        if (position != 0)
            Actor_SetDestination(11, position[5], position[9]);
    }
    Actor_WaitForMove(11);
    Actor_SetPosition(11, 0, 0);
    Event_Wait(20);
    GameFlag_Set(2464);
    Event_End();
}

void FieldScene_RunScene3be_02000fd0(void)
{
    u32 i;
    s32 record;
    s32 base5_23cc;

    base5_23cc = (s32)Data_000023cc;
    Event_SetMessage(base5_23cc);
    Event_OpenMessage(8, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        if (GameFlag_IsSet(0x950) != 0) {
            if (GameFlag_IsSet(0x96f) == 0) {
                Event_SetMessage((base5_23cc + 8));
            }
        }
        Event_ShowMessage(8, 0);
    } else {
        bump_step(1);
        Event_ShowMessage(8, 0);
    }
}

void ActorPresentation_SelectActorNineScript(void)
{
    if (GameFlag_IsSet(2384) != 0 && GameFlag_IsSet(2415) == 0)
        Event_SetMessage(MSG_WE_MISSED_COLOSSO_BECAUSE_WE);
    else
        Event_SetMessage(MSG_WARRIORS_HAVE_BEEN_FIGHTING_WHILE);
    Event_ShowMessage(9, 0);
}

void FieldScene_RunScene3be_02001080(void)
{
    extern u8 *Data_03001ebc;

    u8 *work;

    work = Data_03001ebc;
    Event_Begin();
    if (GameFlag_IsSet(0x204) != 0) {
        GameFlag_Clear(0x9a3);
        GameFlag_Clear(0x9a5);
        GameFlag_Clear(0x9a4);
        GameFlag_Clear(0x9a6);
        GameFlag_Set(0x9a5);
        GameFlag_Set(0x9a4);
    }
    Event_RequestExit(*(s16 *)(work + 0x16c));
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_End();
}

void StagedActorPairScene_RunStep(void)
{
    Leader_CheckAhead();
}

void ActorPresentation_RunActorEightThresholdScene(void)
{
    Actor_Get(8);
    Event_Begin();
    {
        s32 *actor = Actor_Get(8);

        if ((actor[2] >> 20) <= 30) {
            StagedActorPairScene_RunSpinningLeap(8);
            {
                s32 x = 27;
                s32 y = 19;

                Map_CopyCellAttributes(29, 19, 1, 1, x, y);
            }
            GameFlag_Set(2466);
        }
    }
    Event_End();
}

void StagedActorPairScene_RunUpdate(void)
{
    StagedActor_AdvancePair();
    ActorPresentation_RunActorNineThresholdScene();
}

void ActorPresentation_RunActorNineThresholdScene(void)
{
    Event_Begin();
    if ((Func_02002698(9)[2] >> 20) > 42) {
        s32 x = 107;
        s32 y = 17;

        Map_CopyCellAttributes(108, 17, 1, 1, x, y);
        Event_Wait(8);
        Actor_SetPosition(9, 0, 0);
        PlaceActor(10, 45613056, 18874368);
        Actor_SetAnimation(10, 3);
        Audio_PlayCue(154);
        GameFlag_Set(2469);
    }
    Event_End();
}

void StagedActorPairScene_NoopActorCallback(void){}

void StagedActorPairScene_RotateActorPart(u8 *actor)
{
    u8 *sprite_part = *(u8 **)(actor + 80);

    *(u16 *)(sprite_part + 30) -= 0x400;
}

void StagedActorPairScene_WaitForHeight(struct HeightTrackedObject *object,
                                       s32 limit)
{
    extern u8 Data_03001ebc[];

    s32 frames = 40;

    while (frames != 0) {
        Task_Wait(1);
        frames--;
        if (object->height <= limit) {
            break;
        }
    }
}

void StagedActorPairScene_NoopSceneCallback(void){}

void StagedActorPairScene_RunActorTwelveCommand(void)
{
    Actor_SetPosition(12, 0, 0);
}

s32 FieldScene_RunScene3be_02001394(void)
{
    u32 i;
    s32 record;

    if (Data_02000240_t[224][0] == (s32)Data_00000098) {
        GameFlag_Set(0x144);
        if (GameFlag_IsSet(0x9a0) != 0) {
            Actor_SetPosition(11, 0, 0);
        }
    }
    if (Data_02000240_t[224][0] == (s32)Data_0000009e) {
        if (Data_02000240_t[225][0] == 1) {
            Map_CopyCellAttributes(108, 17, 1, 1, 107, 17);
        }
        if (GameFlag_IsSet(0x9a2) != 0) {
            Actor_SetPosition(8, 0x1b80000, 0x1340000);
            Actor_SetAnimation(8, 2);
            Map_CopyCellAttributes(29, 19, 1, 1, 27, 19);
        }
        if (GameFlag_IsSet(0x9a5) != 0) {
            Actor_SetPosition(9, 0, 0);
            Actor_SetPosition(10, 0x2b80000, 0x1200000);
            Actor_SetAnimation(10, 2);
        }
        record = Actor_Get(12);
        Actor_SetSpriteFlags(record, 0);
    }
    return 0;
}
