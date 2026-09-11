#include "types.h"
#include "staged_actor_pair_scene.h"
#include "staged_actor.h"

/* overlays/scene/actor/staged_pair/actor_presentation.c */
#define ActorDraw_RunActorElevenRecoveryScene Func_02000f44
#define ActorDraw_SelectActorNineScript Func_02001040
#define ActorDraw_RunActorEightThresholdScene Func_020010fc
#define ActorDraw_RunActorNineThresholdScene Func_02001158
void Func_02002464();
void Func_0200247e();
void *Func_020024c4();
void Func_020024c6();
void Func_020024cc();
void Func_020024d0();
void Func_020024e6();
void Func_020024e8();
void Func_020024f0();
void Func_020024f6();
void Func_02002504();
void Func_02002506();
void Func_02002516();
void Func_0200251e();
s32 Func_02002544();
s32 Func_0200254e();
void Func_020025e8();
void Func_020025f0();
void Func_02002608();
void Func_02002314();
void Func_020025fc();
void Func_02002624();
void Func_02002638();
void Func_0200263a();
s32 *Func_02002642();
void Func_0200265e();
void Func_0200264c();
void Func_0200267a();
s32 *Func_02002698();
void Func_0200269a();
void Func_020026b6();
void Func_020026da();
void Func_020026f4();
void Func_02002702(s32, s32, s32);
void Func_02002712();
void Func_02002780();

static __inline__ void PlaceActor(s32 actor, s32 x, s32 y)
{
    Func_02002702(actor, x, y);
}

void ActorDraw_RunActorElevenRecoveryScene(void)
{
    Func_02002464();
    Func_020024c6(11, 0, 0);
    Func_020024d0(0, 11, 0);
    Func_020024d0_a(0, 1);
    Func_0200247e(10);
    Func_020024f0(0, 11, 0);
    Func_02002506(0x23d9);
    Func_0200251e(11, 0);
    Func_020024f6(11, 2);
    {
        s16 *position = Func_020024c4(0);

        if (position != 0)
            Func_020024e6(11, position[5], position[9]);
    }
    Func_02002504(11);
    Func_02002516(11, 0, 0);
    Func_020024cc(20);
    Func_020024c4_a(2464);
    Func_020024e8();
}

void ActorDraw_SelectActorNineScript(void)
{
    if (Func_02002544(2384) != 0 && Func_0200254e(2415) == 0)
        Func_020025e8(9173);
    else
        Func_020025f0(9170);
    Func_02002608(9, 0);
}

void ActorDraw_RunActorEightThresholdScene(void)
{
    Func_02002638(8);
    Func_02002624();
    {
        s32 *actor = Func_02002642(8);

        if ((actor[2] >> 20) <= 30) {
            Func_02002314(8);
            {
                s32 x = 27;
                s32 y = 19;

                Func_020025fc(29, 19, 1, 1, x, y);
            }
            Func_0200263a(2466);
        }
    }
    Func_0200265e();
}

void ActorDraw_RunActorNineThresholdScene(void)
{
    Func_0200267a();
    if ((Func_02002698(9)[2] >> 20) > 42) {
        s32 x = 107;
        s32 y = 17;

        Func_0200264c(108, 17, 1, 1, x, y);
        Func_0200269a(8);
        Func_020026f4(9, 0, 0);
        PlaceActor(10, 45613056, 18874368);
        Func_02002712(10, 3);
        Func_02002780(154);
        Func_020026b6(2469);
    }
    Func_020026da();
}
#undef ActorDraw_RunActorElevenRecoveryScene
#undef ActorDraw_SelectActorNineScript
#undef ActorDraw_RunActorEightThresholdScene
#undef ActorDraw_RunActorNineThresholdScene

/* overlays/scene/actor/staged_pair/scene_primary_script.c */
#define Scene_RunScene3beSequenceA Func_02000d88
#define Scene_RunScene3beSequenceB Func_02000df0
#define FieldScene_RunScene3be_02000fd0 Func_02000fd0
#define FieldScene_RunScene3be_02001080 Func_02001080
#define FieldScene_RunScene3be_02001394 Func_02001394
extern u8 Data_00000088[];
extern u8 Data_00000098[];
extern u8 Data_0000009d[];
extern u8 Data_0000009e[];
extern u8 Data_02000240[];
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
extern u8 Data_03001ebc[];
extern s16 Data_02000240_t[][1];
extern u8 Data_000023cc[];

s32 Func_02002234();
s32 Func_0200229c();
s32 Func_020022f2();
s32 Func_02002300();
void Func_0200232a();
s32 Func_02002356();
void Func_02002358();
void Func_02002368();
void Func_02002390();
void Func_02002394();
void Func_0200239e();
s32 Func_020023a0();
void Func_020023d2();
void Func_020023d6();
void Func_020023f0();
void Func_020023f2();
void Func_020023f4();
s32 Func_02002400();
void Func_0200240c();
s32 Func_02002412();
void Func_0200241a();
void Func_0200241c();
void Func_02002434();
void Func_02002444();
void Func_0200244e();
void Func_02002452();
void Func_02002464_a();
void Func_0200246c();
void Func_0200247a();
void Func_02002492();
void Func_0200239e_a();
void Func_0200246c_a();
s32 Func_020024f0_a();
s32 Func_020024fa();
s32 Func_02002514();
void Func_02002564();
s32 Func_02002574();
void Func_02002596();
void Func_020025ae();
void Func_020025c8();
s32 Func_0200258c();
void Func_020025a4();
void Func_020025a6();
void Func_020025ac();
void Func_020025b2();
void Func_020025b6();
void Func_020025b8();
void Func_020025bc();
void Func_020025f6();
void Func_02002672();
void Func_02002686();
void Func_02002692();
s32 Func_020028b4();
s32 Func_020028b4_a();
void Func_020028c4();
void Func_020028f8();
s32 Func_020028fa();
void Func_0200292a();
s32 Func_0200292e();
void Func_0200293e();
void Func_02002974();
void Func_02002984();
s32 Func_02002990();
void Func_020029a4();
void Func_020029b2();
void Func_020029c2();

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_02000fd0(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

s32 Func_02000d20(void)
{
    if (Data_02000240_t[224][0] == (s32)Data_00000098) {
        if (Value1(Func_02002234, 0x96f) != 0) {
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

s32 Scene_RunScene3beSequenceA(void)
{
    if (Data_02000240_t[224][0] == (s32)Data_00000098) {
        if (Value1(Func_0200229c, 0x96f) != 0) {
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

void Scene_RunScene3beSequenceB(void)
{
    s32 record;

    if (Value1(Func_020022f2, 0x98a) != 0) {
    } else {
        if (Value1(Func_02002300, 0x9a0) == 0) {
        } else {
            Func_0200232a();
            Call3(Func_02002358, 11, 0x10000, 0x8000);
            record = Value1(Func_02002356, 0);
            if (record != 0) {
                Func_02002394(11, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
            }
            Call3(Func_02002390, 11, -8, 16);
            Func_0200239e(11);
            Call3(Func_020023f2, 11, 0xd000, 0);
            Func_02002368(10);
            Func_020023d2(0, 11, 0);
            Call1(Func_020023f0, 0x23da);
            Value2(Func_02002400, 11, 0);
            if (Value2(Func_020023a0, 0, 0) == 0) {
                Func_0200241c(11, 0);
                Func_020023d6(11, 152, 232);
                Call1(Func_0200239e_a, 0x9a0);
                Func_020023f4(11);
                Func_0200240c(11, 1);
                Data_02000240_t[226][0] = (s32)Data_00000088;
                Data_02000240_t[227][0] = 30;
            } else {
                bump_step(1);
                Func_0200246c(11, 0);
                Func_02002444(11, 2);
                record = Value1(Func_02002412, 0);
                if (record != 0) {
                    Func_02002434(11, *(s16 *)(record + 10), *(s16 *)(record + 18));
                }
                Func_02002452(11);
                Func_02002464_a(11, 0, 0);
                Func_0200241a(30);
                Func_0200247a(0, 2);
                Func_0200246c_a(0, 0, 16);
                Func_0200247a_a(0);
                Func_02002492(0, 1);
            }
            Func_0200244e();
        }
    }
}

void FieldScene_RunScene3be_02000fd0(void)
{
    u32 i;
    s32 record;
    s32 base5_23cc;

    base5_23cc = (s32)Data_000023cc;
    Func_02002564(base5_23cc);
    Value2(Func_02002574, 8, 0);
    if (Value2(Func_02002514, 0, 0) == 0) {
        if (Value1_02000fd0(Func_020024f0_a, 0x950) != 0) {
            if (Value1_02000fd0(Func_020024fa, 0x96f) == 0) {
                Func_02002596((base5_23cc + 8));
            }
        }
        Func_020025ae(8, 0);
    } else {
        bump_step(1);
        Func_020025c8(8, 0);
    }
}

void FieldScene_RunScene3be_02001080(void)
{
    u32 i;
    s32 record;
    u8 *p5;

    p5 = *(volatile s32 *)Data_03001ebc;
    Func_020025a4();
    if (Value1(Func_0200258c, 0x204) != 0) {
        Call1(Func_020025a6, 0x9a3);
        Call1(Func_020025ac, 0x9a5);
        Call1(Func_020025b2, 0x9a4);
        Call1(Func_020025b8, 0x9a6);
        Call1(Func_020025b6, 0x9a5);
        Call1(Func_020025bc, 0x9a4);
    }
    Func_02002672(*(s16 *)(((s32)p5 + 0x16c)));
    Func_02002686();
    Func_02002692();
    Func_020025f6();
}

s32 FieldScene_RunScene3be_02001394(void)
{
    u32 i;
    s32 record;

    if (Data_02000240_t[224][0] == (s32)Data_00000098) {
        Call1(Func_020028b4, 0x144);
        if (Value1(Func_020028b4_a, 0x9a0) != 0) {
            Func_0200292a(11, 0, 0);
        }
    }
    if (Data_02000240_t[224][0] == (s32)Data_0000009e) {
        if (Data_02000240_t[225][0] == 1) {
            Call6(Func_020028c4, 108, 17, 1, 1, 107, 17);
        }
        if (Value1(Func_020028fa, 0x9a2) != 0) {
            Call3(Func_02002974, 8, 0x1b80000, 0x1340000);
            Func_02002984(8, 2);
            Call6(Func_020028f8, 29, 19, 1, 1, 27, 19);
        }
        if (Value1(Func_0200292e, 0x9a5) != 0) {
            Func_020029a4(9, 0, 0);
            Call3(Func_020029b2, 10, 0x2b80000, 0x1200000);
            Func_020029c2(10, 2);
        }
        record = Func_02002990(12);
        Func_0200293e(record, 0);
    }
    return 0;
}
#undef Scene_RunScene3beSequenceA
#undef Scene_RunScene3beSequenceB
#undef FieldScene_RunScene3be_02000fd0
#undef FieldScene_RunScene3be_02001080
#undef FieldScene_RunScene3be_02001394

/* overlays/scene/actor/staged_pair/staged_actor_pair_scene.c */
#define NULL ((void *)0)
#define StagedActorStepTable Data_020096c0
#define GetStagedActor Func_020018ec
#define FindNextStagedActor Func_0200073e
#define FindBlockingStagedActor Func_0200076a
#define FindElevatedBlockingStagedActor Func_02000796
#define CanStartStagedActorMove Func_02001958
#define SetStagedActorMode Func_02001940
#define SelectStagedActorSlot Func_02001930
#define StartStagedActorEffect Func_02001a7e
#define StartNextStagedActorMove Func_02001976
#define StartLeadStagedActorMove Func_02001986
#define FinishStagedActorMove Func_02001994
#define FinishStagedActorEffect Func_02001aa0
#define SetStagedActorTransition Func_020019ac
#define StagedActorPairScene_SpawnPrimaryEffect Func_02000048
#define StagedActorPairScene_SpawnSecondaryEffect Func_020000a0
#define StagedActorPairScene_SpawnConfiguredEffect Func_0200013c
#define StagedActorPairScene_AdvancePair Func_020003a8
#define StagedActorPairScene_StopBlockedMotion Func_0200058c
#define StagedActorPairScene_FindClearPosition Func_02000758
#define StagedActorPairScene_GetVariantData Func_02000cc0
#define StagedActorPairScene_GetInitialValue Func_02000d14
#define StagedActorPairScene_RunStep Func_020010f0
#define StagedActorPairScene_RunUpdate Func_02001148
#define StagedActorPairScene_RotateActorPart Func_020011c8
#define StagedActorPairScene_WaitForHeight Func_020011d8
#define StagedActorPairScene_RunActorTwelveCommand Func_02001384


struct EffectSprite {
    u8 pad00[9];
    u8 flags;
    u8 pad0a[20];
    u16 angle;
    u8 pad20[6];
    u8 state;
};

struct SceneEffect {
    u8 pad00[24];
    s32 accum18;
    s32 accum1c;
    u8 pad20[3];
    u8 flags;
    u8 pad24[12];
    s32 rate_x;
    s32 rate_y;
    u8 pad38[12];
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    struct EffectSprite *sprite;
    u8 pad54;
    u8 mode55;
    u8 pad56[14];
    u16 step64;
    u8 pad66[6];
    u32 callback;
};

struct EffectOptions {
    u8 mode_bits;
    u8 pad01[3];
    s32 mode;
    s32 accum18;
    s32 accum1c;
    s32 target30;
    s32 target34;
    s16 kind;
    u16 pad1a;
    s32 callback_arg;
    u16 angle;
    u16 step;
    u32 callback;
};

struct EffectDescriptor {
    s32 pad00[3];
    s32 duration;
};

typedef struct { s32 x; s32 y; s32 z; } ScenePosition;

typedef struct { u8 filler0[0x28]; s16 *kind; } ActorSpriteRef;

typedef struct {
    u8 filler0[6];
    u16 direction_and_kind;
    s32 x;
    s32 y;
    s32 z;
    u8 filler14[0x10];
    s32 move_delta_x;
    u8 filler28[4];
    s32 move_delta_z;
    u8 filler30[8];
    s32 move_target_x;
    u8 filler3C[4];
    s32 move_target_z;
    u8 filler44[0xC];
    ActorSpriteRef *sprite;
} MovingActor;

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

extern struct EffectDescriptor *Data_02009778[];
extern s32 Data_02009700[];
extern s32 Data_02009718[];
extern s16 Data_02000240[];
extern u8 Value_00000098;
extern u8 Value_0000009d;
extern u8 Value_0000009e;
extern u8 Data_020097b4[];
extern u8 Data_020097fc[];
extern u8 Data_02009874[];
extern u8 Data_02009784[];

void *Func_0200150e(s32, s32, s32, s32);
void Func_0200155c(void *, s32);
void Func_02001574(void *, s32);
void Func_0200160c(void *, s32);
void *Func_02001566(s32, s32, s32, s32);
void Func_020015b8(void *, s32);
void Func_02001668(void *, s32);
struct SceneEffect *Func_02001692();
struct SceneEffect *Func_02001638();
void Func_02001642();
void Func_0200165c();
void Func_0200179e();
s32 Func_02001700();
s32 Func_02001718();
s32 Func_02001726();
void Func_0200175c();
void Func_0200176c();
struct StagedActor *Func_020018ec(s32 arg0);
struct StagedActor *Func_0200073e(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *Func_0200076a(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *Func_02000796(s32 *arg0, struct StagedActor *arg1);
s32 Func_02001958(struct StagedActor *arg0, s32 *arg1);
void Func_02001940(struct StagedActor *arg0, s32 arg1);
void Func_02001930(s32 arg0);
void Func_02001a7e(s32 arg0);
void Func_02001976(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_02001986(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_02001994(struct StagedActor *arg0);
void Func_02001aa0(void);
void Func_020019ac(struct StagedActor *arg0, s32 arg1);
MovingActor *Func_0200090a(ScenePosition *, MovingActor *);
s32 Func_02001ada(MovingActor *, ScenePosition *);
s32 Func_02000da6(s32 *, s32 *, s32 *);
s32 Func_02001d14(u8 *, s32 *);
void Func_020026a8(void);
void Func_020014f4(void);
void Func_020022a8(void);
void Func_02002674();
void Func_020028f2(int, int, int);

void *StagedActorPairScene_SpawnPrimaryEffect(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *effect = Func_0200150e(kind, x, y, z);

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
        Func_0200155c(effect, 0);
        Func_0200160c(effect, 14);
        Func_02001574(effect, 1);
        return effect;
    }
    return NULL;
}

void *StagedActorPairScene_SpawnSecondaryEffect(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *effect = Func_02001566(kind, x, y, z);

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
        Func_020015b8(effect, 0);
        Func_02001668(effect, 15);
        effect[0x23] = (effect[0x23] & 0xfe) | 2;
        return effect;
    }
    return NULL;
}

void StagedActorPairScene_SpawnConfiguredEffect(
    s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz, u32 flags,
    const struct EffectOptions *options)
{
    u32 table_offset;
    struct SceneEffect *source_effect;
    u32 copied_bits;
    s32 flag_mask;
    u32 block_bits;
    struct SceneEffect *effect;
    struct EffectSprite *sprite;
    struct EffectSprite *mode_sprite;
    u32 option_bits;
    u16 *step_ptr;
    s32 duration;
    s32 first_delta;
    s32 accumulated;
    source_effect = Func_02001692(0);

    if ((flags & 0x100000) != 0 && options != 0) {
        effect = Func_02001638(options->kind, x, y, z);
    } else {
        effect = Func_02001638(222, x, y, z);
    }
    if (effect == 0) return;

    sprite = effect->sprite;
    mode_sprite = sprite;

    Func_02001642(effect, (flags + 1) & 15);
    table_offset = (flags & 15) << 2;
    Func_0200165c(effect, Data_02009778[table_offset >> 2]);

    effect->mode55 = 0;
    sprite->state = 0;
    effect->callback = 0x02008105;

    effect->velocity_x = vx;
    x = 3;
    effect->velocity_y = vy;
    effect->velocity_z = vz;

    copied_bits = source_effect->sprite->flags & 12;
    block_bits = *(volatile u8 *)&sprite->flags;
    flag_mask = ~12;
    sprite->flags = (u8)((block_bits & flag_mask) | copied_bits);

    effect->rate_x = 0;
    effect->rate_y = 0;
    effect->step64 = 0;
    step_ptr = &effect->step64;

    if ((flags & 0xffff0000) == 0 || options == 0) return;

    if ((flags & 0x10000) != 0) {
        Func_0200179e(effect, options->mode);
    }

    if ((flags & 0x20000) != 0) {
        effect->flags &= 0xfe;
        option_bits = *(const u8 *)options & x;
        sprite->flags = (u8)((*((const u8 *)mode_sprite + 9) & flag_mask)
                              | (option_bits << 2));
    }

    if ((flags & 0x80000) != 0) {
        effect->accum18 = options->accum18;
        effect->accum1c = options->accum1c;
    }

    if ((flags & 0x40000) != 0) {
        const struct EffectDescriptor *descriptor =
            Data_02009778[table_offset >> 2];
        s32 delta;

        if ((flags & 0x80000) != 0) {
            first_delta = *(volatile const s32 *)&options->target30;
            accumulated = *(volatile const s32 *)&effect->accum18;
            first_delta -= accumulated;
            effect->rate_x = Func_02001700(first_delta,
                                           descriptor->duration);
            delta = options->target34;
            duration = descriptor->duration;
            delta -= effect->accum1c;
        } else {
            first_delta = options->target30;
            first_delta += (s32)0xffff0000;
            effect->rate_x = Func_02001718(first_delta,
                                           descriptor->duration);
            delta = options->target34;
            duration = descriptor->duration;
            delta += (s32)0xffff0000;
        }

        effect->rate_y = Func_02001726(delta, duration);
    }

    if ((flags & 0x200000) != 0) {
        Func_0200175c(effect, 1);
        Func_0200176c(effect, options->callback_arg);
    }

    if ((flags & 0x400000) != 0) {
        sprite->angle = options->angle;
    }

    if ((flags & 0x800000) != 0) {
        *step_ptr = options->step;
    }

    if ((flags & 0x1000000) != 0) {
        effect->callback = options->callback;
    }
}

void StagedActorPairScene_AdvancePair(void)
{
    extern u32 Data_020096c0[];

    s32 destination[3];
    struct StagedActor *lead_actor;
    struct StagedActor *next_actor;
    struct StagedActor *blocking_actor;
    s32 facing_index;
    u32 step;
    s32 move_rate;
    s32 clear_value;

    lead_actor = GetStagedActor(0);
    facing_index = lead_actor->direction_and_kind >> 12;
    step = StagedActorStepTable[facing_index];
    destination[0] = lead_actor->x.value + (step & 0xffff0000);
    destination[1] = lead_actor->y;
    step <<= 16;
    destination[2] = lead_actor->z.value + step;
    next_actor = FindNextStagedActor(destination, lead_actor);
    if (next_actor == 0) return;

    step = StagedActorStepTable[facing_index];
    destination[0] = next_actor->x.value + (step & 0xffff0000);
    destination[1] = next_actor->y;
    step <<= 16;
    destination[2] = next_actor->z.value + step;
    blocking_actor = FindBlockingStagedActor(destination, next_actor);
    if (blocking_actor != 0 && (blocking_actor->collision_flags & 1) != 0) return;

    destination[0] = next_actor->x.value;
    destination[1] = next_actor->y + 0x100000;
    destination[2] = next_actor->z.value;
    blocking_actor = FindElevatedBlockingStagedActor(destination, next_actor);
    if (blocking_actor != 0 && (blocking_actor->collision_flags & 1) != 0) return;

    next_actor->transition_mode = 2;
    step = StagedActorStepTable[facing_index];
    destination[0] = next_actor->x.value + (step & 0xffff0000);
    destination[1] = next_actor->y;
    step <<= 16;
    destination[2] = next_actor->z.value + step;
    if (CanStartStagedActorMove(next_actor, destination) > 0) return;

    clear_value = next_actor->transition_busy;
    if (clear_value != 0) return;

    SetStagedActorMode(lead_actor, 8);
    move_rate = 0x3333;
    SelectStagedActorSlot(15);
    StartStagedActorEffect(185);
    next_actor->move_rate_x = move_rate;
    next_actor->move_rate_z = move_rate;
    StartNextStagedActorMove(next_actor, destination[0], destination[1], destination[2]);
    lead_actor->move_rate_x = move_rate;
    lead_actor->move_rate_z = move_rate;
    StartLeadStagedActorMove(lead_actor, destination[0], destination[1], destination[2]);
    FinishStagedActorMove(next_actor);
    FinishStagedActorEffect();
    next_actor->x.value = destination[0];
    next_actor->z.value = destination[2];
    next_actor->unknown_24 = clear_value;
    next_actor->unknown_2c = clear_value;
    lead_actor->unknown_38 = 0x80000000;
    lead_actor->unknown_40 = 0x80000000;
    lead_actor->unknown_24 = clear_value;
    lead_actor->unknown_2c = clear_value;
    lead_actor->x.value = lead_actor->x.parts.cell << 16;
    lead_actor->z.value = lead_actor->z.parts.cell << 16;
    SetStagedActorTransition(lead_actor, 1);
}

s32 StagedActorPairScene_StopBlockedMotion(MovingActor *actor)
{
    extern s32 Data_020096c0[];

    ScenePosition destination;
    u32 direction;
    s32 step;
    MovingActor *blocker;

    direction = actor->direction_and_kind >> 12;
    step = Data_020096c0[direction];
    destination.x = actor->x + (step & 0xffff0000);
    destination.y = actor->y;
    step = step << 16;
    destination.z = actor->z + step;
    blocker = Func_0200090a(&destination, actor);
    if (blocker != 0) {
        u32 allowed_index = 0;
        s32 blocker_kind = *blocker->sprite->kind;
        s32 *allowed_kinds = Data_02009700;

        do {
            if (blocker_kind == *allowed_kinds++) goto done;
            allowed_index++;
        } while (allowed_index <= 5);
        actor->move_delta_x = 0;
        actor->move_delta_z = 0;
        actor->move_target_x = 0x80000000;
        actor->move_target_z = 0x80000000;
    }
    step = Data_020096c0[direction];
    destination.x = actor->x + (step & 0xffff0000);
    destination.y = actor->y;
    step = step << 16;
    destination.z = actor->z + step;
    if (Func_02001ada(actor, &destination) > 0) {
        actor->move_delta_x = 0;
        actor->move_delta_z = 0;
        actor->move_target_x = 0x80000000;
        actor->move_target_z = 0x80000000;
    }
done:
    return 0;
}

s32 StagedActorPairScene_FindClearPosition(s32 *search)
{
    extern s32 Data_020096c0[];

    s32 direction;
    s32 origin[3];
    u8 *actor;
    s32 advance_count;
    s32 rows, columns;
    u8 *collision_mode;
    s32 bounds_offset;
    s32 extent_a, extent_b;
    u8 *bounds;
    s32 *candidate;
    s32 moved;
    search[5] = 0;
    actor = (u8 *)Func_02000da6(&direction, search + 1, search);
    if (actor == 0)
        return 0;
    collision_mode = actor + 0x22;
    *collision_mode = 2;
    advance_count = 0;
    bounds = (u8 *)Data_02009718;
    bounds_offset = search[0] << 4;
    {
        s32 entry_offset = bounds_offset + 4;
        extent_a = *(s32 *)(bounds + entry_offset);
        if (extent_a < 0)
            extent_a = -extent_a;
        entry_offset = bounds_offset;
        entry_offset += 12;
        extent_b = *(s32 *)(bounds + entry_offset);
        if (extent_b < 0)
            extent_b = -extent_b;
        rows = (extent_a + extent_b) >> 4;
        extent_a = *(s32 *)(bounds + bounds_offset);
        if (extent_a < 0)
            extent_a = -extent_a;
        entry_offset = bounds_offset;
        entry_offset += 8;
        extent_b = *(s32 *)(bounds + entry_offset);
        if (extent_b < 0)
            extent_b = -extent_b;
        columns = (extent_a + extent_b) >> 4;
    }
    {
        u8 *actor_bytes;
        s32 height;
        s32 step_x;
        candidate = origin;
        step_x = Data_020096c0[direction] & 0xffff0000;
        actor_bytes = actor;
        candidate[0] = *(s32 *)(actor_bytes + 8) + step_x;
        height = *(s32 *)(actor_bytes + 12);
        candidate[1] = height;
        candidate[2] = *(s32 *)(actor_bytes + 16) +
                       (Data_020096c0[direction] << 16);
        search[3] = height;
    }
    for (;;) {
        s32 row, column;
        {
            u8 *bounds_row = (u8 *)Data_02009718;
            s32 row_offset = search[0] << 4;
            row_offset += 4;
            search[4] = candidate[2] +
                        (*(s32 *)(bounds_row + row_offset) << 16);
        }
        row = 0;
        while (row < rows) {
            {
                u8 *bounds_column = (u8 *)Data_02009718;
                search[2] = candidate[0] +
                            (*(s32 *)(bounds_column + (search[0] << 4)) << 16);
            }
            column = 0;
            while (column < columns) {
                s32 *position = search + 2;
                if (Func_02001d14(actor, position) == 2)
                    goto found;
                position[0] = position[0] + 0x100000;
                column++;
            }
            search[4] = search[4] + 0x100000;
            row++;
        }
        advance_count++;
        origin[0] = origin[0] +
                    (Data_020096c0[direction] & 0xffff0000);
        origin[2] = origin[2] + (Data_020096c0[direction] << 16);
    }
found:
    *collision_mode = 0;
    moved = 0;
    if (advance_count != 0) {
        s32 step = Data_020096c0[direction];
        s32 step_x = step & 0xffff0000;
        s32 offset_x = step_x *advance_count;
        s32 offset_z = (step << 16) * advance_count;
        u8 *actor_bytes = actor;
        search[2] = *(s32 *)(actor_bytes + 8) + offset_x;
        search[3] = *(s32 *)(actor_bytes + 12);
        search[4] = *(s32 *)(actor_bytes + 16) + offset_z;
        moved = 1;
    }
    return moved;
}

s32 StagedActorPairScene_GetVariantData(void)
{
    s16 scene_id = Data_02000240[224];

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

void StagedActorPairScene_RunStep(void)
{
    Func_020026a8();
}

void StagedActorPairScene_RunUpdate(void)
{
    Func_020014f4();
    Func_020022a8();
}

void StagedActorPairScene_RotateActorPart(u8 *actor)
{
    u8 *sprite_part = *(u8 **)(actor + 80);

    *(u16 *)(sprite_part + 30) -= 0x400;
}

void StagedActorPairScene_WaitForHeight(struct HeightTrackedObject *object,
                                       s32 limit)
{
    s32 frames = 40;

    while (frames != 0) {
        Func_02002674(1);
        frames--;
        if (object->height <= limit) {
            break;
        }
    }
}

void StagedActorPairScene_RunActorTwelveCommand(void)
{
    Func_020028f2(12, 0, 0);
}
#undef NULL
#undef StagedActorStepTable
#undef GetStagedActor
#undef FindNextStagedActor
#undef FindBlockingStagedActor
#undef FindElevatedBlockingStagedActor
#undef CanStartStagedActorMove
#undef SetStagedActorMode
#undef SelectStagedActorSlot
#undef StartStagedActorEffect
#undef StartNextStagedActorMove
#undef StartLeadStagedActorMove
#undef FinishStagedActorMove
#undef FinishStagedActorEffect
#undef SetStagedActorTransition
#undef StagedActorPairScene_SpawnPrimaryEffect
#undef StagedActorPairScene_SpawnSecondaryEffect
#undef StagedActorPairScene_SpawnConfiguredEffect
#undef StagedActorPairScene_AdvancePair
#undef StagedActorPairScene_StopBlockedMotion
#undef StagedActorPairScene_FindClearPosition
#undef StagedActorPairScene_GetVariantData
#undef StagedActorPairScene_GetInitialValue
#undef StagedActorPairScene_RunStep
#undef StagedActorPairScene_RunUpdate
#undef StagedActorPairScene_RotateActorPart
#undef StagedActorPairScene_WaitForHeight
#undef StagedActorPairScene_RunActorTwelveCommand
