#include "TYPES.H"
#include "FIELD_EVENT.H"

#define NULL ((void *)0)

#include "SCENE_EFFECT_SEQUENCE.H"

/* Shared 22-byte head leaf proved identical for this overlay family. */
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

struct PeriodicEffectConfig {
    s32 kind;
    s32 variant;
    u8 pad_08[16];
    u16 id;
    u8 pad_1a[2];
    u8 *data;
    /*
     * The record is 40 bytes, not 32: the reference reserves `sub sp, #56' for
     * 16 bytes of outgoing stack arguments plus this object, and only writes
     * +0, +4, +24 and +28.  The trailing two words are never stored.
     */
    u8 pad_20[8];
};

extern u32 Data_03001e40;
extern u8 Data_02009740[];
extern u8 Data_02009900[];
extern u8 Data_02009778[];
extern u8 Data_0200978e[];
extern u8 Data_020097a4[];
extern u8 Data_020097ba[];
extern u8 Data_020097d0[];

void *Func_020013ca(s32, s32, s32, s32);
void *Func_02001422(s32, s32, s32, s32);
void Func_020017be();
u8 *Func_0200172e();
u8 *Func_02001748();
void Func_020004f0();
typedef s32(*IwramIntegerSquareRoot)(s32);
u8 *Func_02001d22(s32 actorId);
void Func_02001cf0();
s32 Func_020019cc();
u8 *Func_020019d8();
s32 Func_020019ee();
u8 *Func_020019fc();
s32 Func_02001a3e_a();
s32 Func_02001a48();
u8 *Func_02001a66();
s32 Func_02001a76();
s32 Func_02001a84();
s32 Func_02001a92();
s32 Func_02001abe();
u8 *Func_02001ac6();
s32 Func_02001ad4();
s32 Func_02001ae4();
s32 Func_02001afe();
s32 Func_02001b0e();
u8 *Func_02001b2a();
u8 *Func_02001b2a_a();
s32 Func_02001b36();
u8 *Func_02001b3e();
s32 Func_02001b70();
s32 Func_02001b78();
s32 Func_02001b98();
s32 Func_02001ba0();
s32 Func_02001c30();
s32 Func_02001c3e();
s32 Func_02001c68();
s32 Func_02001ca2();
u8 *Func_02001cb0();
void Func_02001d0a();
void Func_02001cf8();
s32 Func_02001d44();
s32 Func_02001d54();
s32 Func_02001d60();
s32 Func_02001d68();
s32 Func_020021c0();
s32 Func_02002442();
s32 Func_0200244c_a();
s32 Func_02002458();
s32 Func_02002460();
s32 Func_02002468();
s32 Func_02002470();
s32 Func_02002478();
s32 Func_020024f2();

/* Named shorthand for one fixed effect request, in overlay resource_3a0. */

/*
 * Complete conditional effect-spawn owner through return, alignment and all
 * three pool words.  The stack object passed as argument eight is reproduced
 * field-for-field from the stores in the owner.
 */

/*
 * Distance between two three-component 16.16 fixed-point positions.
 *
 * Each argument walks three consecutive 16.16 words in x, y, z order. The
 * per-axis deltas are taken in fixed point, shifted down to integers, squared,
 * and summed; the total is passed to the resident IWRAM integer square root.
 *
 * Expressions are preserved exactly as reconstructed: the walking-pointer form
 * is load-bearing for byte-identity and must not become struct field access.
 */

/* Returns a value: the reference sets r1 before r0 at this site, which
   only a value-returning callee does; the result is unused here. */

/*
 * One-shot message beat for overlay resource_3a0.  Each Func_ name spells
 * the overlay's own relocated call word, not a runtime address.
 */

/*
 * Scene step dispatcher for resource_3a0.  The 228-byte owner includes its
 * seven-entry jump table, an alignment halfword and its literal pool.  Each
 * call site is spelled with its own import name.
 */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call0(void (*f)())
{
    f();
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_02000968(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3_02000de8(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

void SetEffectRecordMode(struct EffectWork *work, s32 mode)
{
    work->record->mode = mode;
}

void *SceneEffect_SpawnPrimary(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *effect = Func_020013ca(kind, x, y, z);

    if (effect != NULL) {
        u8 *sprite = *(u8 **)(effect + 0x50);
        s32 flags;
        s32 mask = 13;

        flags = sprite[9];
        mask = -mask;
        mask &= flags;
        sprite[9] = mask;
        effect[0x55] = 0;
        effect[0x59] = 8;
        Actor_SetSpriteFlags(effect, 0);
        Object_SetPalette(effect, 14);
        Object_SetBlendMode(effect, 1);
        return effect;
    }
    return NULL;
}

void *SceneEffect_SpawnSecondary(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *effect = Func_02001422(kind, x, y, z);

    if (effect != NULL) {
        u8 *sprite = *(u8 **)(effect + 0x50);
        s32 flags;
        s32 mask = 13;

        flags = sprite[9];
        mask = -mask;
        mask &= flags;
        mask |= 4;
        sprite[9] = mask;
        effect[0x55] = 0;
        effect[0x59] = 8;
        Actor_SetSpriteFlags(effect, 0);
        Object_SetPalette(effect, 15);
        effect[0x23] = (effect[0x23] & 0xfe) | 2;
        return effect;
    }
    return NULL;
}

/*
 * The 16-byte owner at 0x02000314 loads no literal, so it carries no pool
 * word and no alignment halfword.  All three arguments are immediates, and
 * Func_020017be spells the overlay's own relocated call word rather than a
 * runtime address.
 */
void SceneEffect_RequestFixedEffect(void)
{
    Func_020017be(22, 1, 2);
}

/* Complete entity-19 sprite-counter adjustment. */
void SceneEffect_AdvanceRotatingSprite(void)
{
    u8 *entity = Func_0200172e(19);
    u8 *sprite = *(u8 **)(entity + 80);
    *(u16 *)(sprite + 30) += 0x1400;
}

void SceneEffect_SpawnPeriodicEffect(void)
{
    u8 *entity = Func_02001748(14);

    if ((Data_03001e40 & 3) == 0) {
        struct PeriodicEffectConfig config;
        config.kind = 1;
        config.variant = 9;
        config.id = 169;
        config.data = Data_02009740;
        Func_020004f0(
            *(s32 *)(entity + 8),
            *(s32 *)(entity + 12),
            *(s32 *)(entity + 16) - 0x10000,
            0,
            -0x10000,
            -0x10000,
            0x330000,
            &config);
    }
}

s32 SceneEffect_CalculatePositionDistance(
    s32 *first_position,
    s32 *second_position)
{
    extern u8 *Data_03001ebc;

    s32 delta_x = (*first_position++ - *second_position++) >> 16;
    s32 delta_y = (*first_position++ - *second_position++) >> 16;
    s32 delta_z = (*first_position - *second_position) >> 16;
    s32 delta_x_squared = delta_x *delta_x;
    s32 delta_y_squared = delta_y *delta_y;
    s32 delta_z_squared = delta_z *delta_z;

    return ((IwramIntegerSquareRoot) 0x030001D8)(delta_x_squared + delta_y_squared + delta_z_squared);
}

u8 *SceneEffect_GetPrimaryData(void)
{
    return (u8 *)0x020097e8;
}

s32 SceneEffect_GetInitialValue(void)
{
    return 0;
}

u8 *SceneEffect_GetSecondaryData(void)
{
    return (u8 *)0x020098d8;
}

s32 SceneEffect_PrepareState(void)
{
    if (GameFlag_IsSet(0x895) != 0)
        Data_02009900[0xbe] = 0;
    return (s32)Data_02009900;
}

void SceneEffect_ShowActorSetupMessage(void)
{
    Event_Begin();
    Event_SetMessage(0x17e8);
    Event_AskYesNo(9, 0);
    Event_End();
}

void FieldScene_RunPrimarySequence(void)
{
    u32 i;
    s32 rec7;
    struct FieldActor *actor;
    u8 *record;
    s32 v7;
    s32 v5;
    s32 p5;
    s32 q1;
    s32 t2;
    s32 q2;
    s32 hi;
    s32 lo;

    actor = (struct FieldActor *)Value1(Func_020019cc, 20);
    Event_Begin();
    v7 = 0;
    record = Func_020019d8(18);
    *(s32 *)((s32)record + 108) = v7;
    if (GameFlag_IsSet(0x200) == 0) {
        record = Value1(Func_020019ee, 18);
        if ((*(s32 *)((s32)record + 8) >> 20) > 19) {
            goto L_020006a2;
        }
    }
    record = Func_020019fc(18);
    p5 = *(u16 *)((s32)record + 6);
    Actor_FaceActor(18, 0, 0);
    Event_Wait(10);
    Event_SetMessage(0x17fb);
    if (GameFlag_IsSet(0x200) == 0) {
        bump_step(1);
        Event_ShowMessage(18, 0);
        *(u16 *)(Func_02001a3e_a(18) + 100) = v7;
        record = Value1(Func_02001a48, 18);
        *(u16 *)((s32)record + 6) = p5;
    } else {
        Event_ShowMessage(18, 0);
        Actor_FaceDirection(18, 0x8000, 20);
    }
    record = Func_02001a66(18);
    *(s32 *)((s32)record + 108) = 0x2008501;
    Call0(Engine_EventEnd);
    goto L_02000916;
    L_020006a2:;
    record = Value1(Func_02001a76, 0);
    if ((*(s32 *)((s32)record + 16) >> 19) > 27) {
        record = Value1(Func_02001a84, 0);
        if ((*(s32 *)((s32)record + 16) >> 19) <= 29) {
            record = Value1(Func_02001a92, 0);
            if ((*(s32 *)((s32)record + 8) >> 20) != 26) {
                Actor_SetSpeed(0, 0x8000, 0x4000);
                Call3((void (*)())Func_02001b2a, 0, 18, 0);
                Event_Wait(5);
                rec7 = Value1(Func_02001abe, 0);
                record = Func_02001ac6(18);
                if (*(s32 *)(rec7 + 8) < *(s32 *)((s32)record + 8)) {
                    *(u8 *)(Func_02001ad4(0) + 90) &= 254;
                    record = Value1(Func_02001ae4, 18);
                    Actor_WalkTo(0, (((*(s32 *)((s32)record + 8) >> 20) << 4) - 8), 232);
                    v7 = 1;
                } else {
                    *(u8 *)(Func_02001afe(0) + 90) &= 254;
                    record = Value1(Func_02001b0e, 18);
                    Actor_WalkTo(0, (((*(s32 *)((s32)record + 8) >> 20) << 4) + 24), 232);
                }
                Actor_WaitForMove(0);
            }
        }
    }
    v5 = 128;
    record = Func_02001b2a_a(18);
    *(s32 *)((s32)record + 56) = (v5 << 24);
    record = Value1(Func_02001b36, 18);
    *(s32 *)((s32)record + 60) = (v5 << 24);
    record = Func_02001b3e(18);
    *(s32 *)((s32)record + 64) = (v5 << 24);
    Actor_EnableActionCallback(18, 1);
    Actor_SetAnimation(18, 1);
    Actor_RunRepeatedMotion(18, 2);
    Event_Wait(10);
    Audio_PlayCue(228);
    actor->scale_x = 0x4ccc;
    actor->scale_y = 0x4ccc;
    record = Value1(Func_02001b70, 18);
    q1 = *(s32 *)((s32)record + 8);
    record = Value1(Func_02001b78, 18);
    t2 = *(s32 *)((s32)record + 16) >> 20;
    Actor_SetPosition(20, (((q1 >> 20) << 20) + 0x80000), ((t2 << 20) + 0x80000));
    record = Value1(Func_02001b98, 18);
    q2 = *(s32 *)((s32)record + 8);
    record = Value1(Func_02001ba0, 18);
    Map_CopyCellAttributes(16, 16, 1, 1, (q2 >> 20), (*(s32 *)((s32)record + 16) >> 20));
    Actor_SetSpritePriority(20, 2);
    actor->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
    do {
        Task_Wait(3);
        hi = actor->scale_y;
        lo = actor->scale_x;
        actor->scale_y = hi + 0x1999;
        lo += 0x1999;
        actor->scale_x = lo;
    } while (lo <= 0xffff);
    Actor_ShowEmote(18, 0x105, 70);
    Actor_FaceActor(18, 0, 0);
    Event_Wait(20);
    Actor_ShowEmote(18, 0x103, 0);
    Actor_StartRepeatedMotion(18, 2);
    Event_Wait(70);
    Event_SetMessage(0x17fa);
    Event_ShowMessageAndWait(18, 0, 20);
    Func_02001d0a();
    record = Value1(Func_02001c30, 0);
    if ((*(s32 *)((s32)record + 8) >> 20) == 26) {
        record = Value1(Func_02001c3e, 0);
        if ((*(s32 *)((s32)record + 16) >> 20) > 13) {
            v7 = 1;
        }
    }
    if (v7 != 0) {
        Actor_SetSpeed(0, 0xcccc, 0x6666);
        Actor_FaceDirection(0, 0xc000, 10);
        *(u8 *)(Func_02001c68(0) + 90) &= 254;
        Actor_SetAnimation(0, 2);
        Actor_SetDestinationOffset(0, 0, 16);
        Actor_WaitForMove(0);
        Actor_SetAnimation(0, 1);
    }
    Actor_SetSpeed(18, 0xcccc, 0x6666);
    record = Value1(Func_02001ca2, 18);
    if ((*(s32 *)((s32)record + 16) >> 20) != 14) {
        record = Func_02001cb0(18);
        Actor_WalkToAndWait(18, *(s16 *)((s32)record + 10), 232);
    }
    Actor_WalkToAndWait(18, 0x118, 232);
    GameFlag_Set(0x200);
    Actor_Get(0)->unknown_5a |= 1;
    Event_End();
    L_02000916:;
}

void SceneEffect_ActivateNearbyActor(void)
{
    u8 *leader = Func_02001d22(0);
    if ((*(s32 *)(leader + 16) >> 20) <= 13)
        Actor_SetSpritePriority(20, 1);
}

void FieldScene_RunScene3a0_02000968(void)
{
    u32 i;
    s32 record;
    s32 v5;
    s32 x;

    Event_Begin();
    *(u8 *)(Func_02001d44(20) + 35) &= 253;
    v5 = 0;
    *(u8 *)(Func_02001d54(20) + 85) = v5;
    record = Value1(Func_02001d60, 20);
    x = *(s32 *)(record + 8);
    record = Value1(Func_02001d68, 20);
    Map_CopyCellAttributes(3, 17, 1, 1, (x >> 20), (*(s32 *)(record + 16) >> 20));
    Call2(Func_02001cf8, 0x2008325, 0xc80);
    GameFlag_Set(0x201);
    Actor_SetSpritePriority(20, 2);
    Event_End();
}

/*
 * The 28-byte owner includes its one pool word: 0x17f7 is an identifier
 * passed as an argument, not an address.  The first and last calls are the
 * scene bracket and must stay in that order.
 */
void SceneEffect_RunActorSceneMessage(void)
{
    Event_Begin();
    Event_SetMessage(0x17f7);
    Event_AskYesNo(17, 0);
    Event_End();
}

void FieldScene_RunScene3a0_02000de8(s32 a0)
{
    u32 i;
    s32 record;

    *(u8 *)(Func_020021c0(0) + 85) = 0;
    Actor_SetSpeed(0, 0x8000, 0x4000);
    if (a0 == 6) {
        Actor_SetAnimation(0, 2);
        Actor_SetDestinationOffset(0, 0, -16);
    } else {
        Actor_CenterAndWalk(0, 2, -16);
    }
    gEventWork->transition_frames = 16;
    Event_RequestExit(a0);
}

/*
 * The scene table is reached through the IWRAM pointer Data_03001ebc, not
 * through the resident workspace, and the selector is its halfword [182],
 * guarded to the range 1..7.  The pointer is loaded before the first call and
 * held across all of them, so it is a function-top local.  Cases 2 and 3 set
 * the two shared arguments and jump into the middle of case 6 to share its
 * final call; the goto and the two locals are what reproduce that.
 */
void SceneEffect_DispatchStep(void)
{
    extern u8 *Data_03001ebc;

    s16 *scene = (s16 *)Data_03001ebc;
    u8 *shared0;
    s32 shared1;

    Event_Begin();

    switch (scene[182]) {
    case 1:
        Audio_PlayCue(158);
        Map_AnimateCells(Data_02009778, 81, 18);
        break;
    case 2:
        Audio_PlayCue(158);
        shared0 = Data_0200978e;
        shared1 = 83;
        goto shared;
    case 3:
        Audio_PlayCue(158);
        shared0 = Data_0200978e;
        shared1 = 86;
        goto shared;
    case 4:
        Audio_PlayCue(158);
        Map_AnimateCells(Data_020097a4, 84, 24);
        break;
    case 5:
        Audio_PlayCue(158);
        Map_AnimateCells(Data_020097a4, 72, 7);
        break;
    case 6:
        Audio_PlayCue(188);
        shared0 = Data_020097ba;
        shared1 = 69;
    shared:
        Map_AnimateCells(shared0, shared1, 11);
        break;
    case 7:
        Audio_PlayCue(158);
        Map_AnimateCells(Data_020097d0, 83, 7);
        break;
    default:
        break;
    }

    Func_02001cf0(scene[182]);
    Event_End();
}

void FieldScene_RunScene3a0_02001060(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Call2((void (*)())Engine_ActorEnableActionCallback, 18, 1);
    record = Func_02002442(18);
    *(s32 *)(record + 108) = 0;
    record = Func_0200244c_a(18);
    *(s32 *)(record + 56) = -0x80000000;
    record = Value1(Func_02002458, 18);
    *(s32 *)(record + 64) = -0x80000000;
    record = Value1(Func_02002460, 18);
    *(s32 *)(record + 36) = 0;
    record = Value1(Func_02002468, 18);
    *(s32 *)(record + 44) = 0;
    record = Value1(Func_02002470, 18);
    *(s32 *)(record + 48) = 0;
    record = Func_02002478(18);
    *(s32 *)(record + 52) = 0;
    Actor_ShowEmote(18, 0x103, 0);
    Actor_StartRepeatedMotion(18, 2);
    Event_Wait(60);
    Actor_SetSpeed(18, 0x18000, 0xc000);
    Actor_SetSpeed(0, 0x18000, 0xc000);
    Actor_WalkTo(18, 0x118, 232);
    Actor_WalkToAndWait(0, 0x128, 232);
    Actor_WaitForMove(18);
    Actor_FaceDirection(0, 0x8000, 20);
    Actor_ShowEmote(0, 0x102, 60);
    Actor_EnableActionCallback(18, 0x20095b0);
    record = Func_020024f2(18);
    *(s32 *)(record + 108) = 0x2008501;
    Call0(Engine_EventEnd);
}

u8 *SceneEffect_GetTertiaryData(void)
{
    return (u8 *)0x02009ac8;
}
