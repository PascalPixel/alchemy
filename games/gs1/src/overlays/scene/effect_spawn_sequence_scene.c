#include "types.h"

#define SetEffectRecordMode Func_02000030
#define NULL ((void *)0)
#define SceneEffect_SpawnPrimary Func_02000048
#define SceneEffect_SpawnSecondary Func_020000a0
#define SceneEffect_SpawnConfigured Func_0200013c
#define SceneEffect_RequestFixedEffect Func_02000314
#define SceneEffect_AdvanceRotatingSprite Func_02000358
#define SceneEffect_SpawnPeriodicEffect Func_02000370
#define SceneEffect_CalculatePositionDistance Func_020003cc
#define SceneEffect_GetPrimaryData Func_0200059c
#define SceneEffect_GetInitialValue Func_020005a4
#define SceneEffect_GetSecondaryData Func_020005a8
#define SceneEffect_PrepareState Func_020005b0
#define SceneEffect_ShowActorSetupMessage Func_020005d4
#define SceneEffect_ActivateNearbyActor Func_0200094c
#define SceneEffect_RunActorSceneMessage Func_02000cec
#define SceneEffect_DispatchStep Func_02000e4c
#define SceneEffect_GetTertiaryData Func_02001140
#define FieldScene_RunPrimarySequence Func_020005f4
#define FieldScene_RunScene3a0_02000968 Func_02000968
#define FieldScene_RunScene3a0_02000de8 Func_02000de8
#define FieldScene_RunScene3a0_02001060 Func_02001060

#include "scene_effect_sequence.h"

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

struct Sprite {
    u8 pad00[9];
    u8 flags9;
    u8 pad0a[20];
    u16 angle;
    u8 pad20[6];
    u8 state26;
};

struct Effect {
    u8 pad00[24];
    s32 accum18;
    s32 accum1c;
    u8 pad20[3];
    u8 flags23;
    u8 pad24[12];
    s32 rate30;
    s32 rate34;
    u8 pad38[12];
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    struct Sprite *sprite;
    u8 pad54;
    u8 mode55;
    u8 pad56[14];
    u16 step64;
    u8 pad66[6];
    u32 callback;
};

struct Options {
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

struct Descriptor {
    s32 pad00[3];
    s32 duration;
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

extern struct Descriptor *Data_020095a4[];
extern u32 Data_03001e40;
extern u8 Data_02009740[];
extern u8 Data_02009900[];
extern u8 Data_02009778[];
extern u8 Data_0200978e[];
extern u8 Data_020097a4[];
extern u8 Data_020097ba[];
extern u8 Data_020097d0[];

void *Func_020013ca(s32, s32, s32, s32);
void Func_02001410(void *, s32);
void Func_02001428(void *, s32);
void Func_020014e0(void *, s32);
void *Func_02001422(s32, s32, s32, s32);
void Func_0200146c(void *, s32);
void Func_0200153c(void *, s32);
struct Effect *Func_0200152e();
struct Effect *Func_020014f4();
void Func_020014fe();
void Func_02001518();
void Func_02001672();
s32 Func_020015ac();
s32 Func_020015c4();
s32 Func_020015d2();
void Func_02001618();
void Func_02001628();
void Func_020017be();
u8 *Func_0200172e();
u8 *Func_02001748();
void Func_020004f0();
typedef s32(*IwramIntegerSquareRoot)(s32);
s32 Func_02001956(s32 flagId);
void Func_02001998(void);
void Func_02001a3e(s32 messageId);
s32 Func_02001a5e(s32 actorId, s32 mode);
void Func_020019b2(void);
u8 *Func_02001d22(s32 actorId);
void Func_02001dea(s32 actorId, s32 mode);
void Func_02002156();
s32 Func_02002176();
void Func_02002214();
void Func_02002348();
void Func_02002212();
void Func_0200235a();
void Func_02002366();
void Func_02002372();
void Func_0200223c();
void Func_02002384();
void Func_0200224e();
void Func_02002396();
void Func_02002260();
void Func_020023a8();
void Func_02002272();
void Func_02001cf0();
void Func_020022d4();
s32 Func_020019b4();
void Func_020019c2();
s32 Func_020019cc();
u8 *Func_020019d8();
s32 Func_020019ec();
s32 Func_020019ee();
void Func_020019f6();
u8 *Func_020019fc();
s32 Func_02001a3e_a();
s32 Func_02001a48();
u8 *Func_02001a66();
void Func_02001a66_a();
s32 Func_02001a76();
void Func_02001a80();
s32 Func_02001a84();
s32 Func_02001a92();
void Func_02001aa0();
void Func_02001aa4();
void Func_02001ab0();
s32 Func_02001abe();
u8 *Func_02001ac6();
void Func_02001ad0();
s32 Func_02001ad4();
s32 Func_02001ae4();
void Func_02001aec();
s32 Func_02001afe();
s32 Func_02001b0e();
void Func_02001b10();
void Func_02001b24();
u8 *Func_02001b2a();
u8 *Func_02001b2a_a();
s32 Func_02001b36();
void Func_02001b3a();
u8 *Func_02001b3e();
void Func_02001b46();
void Func_02001b4e();
void Func_02001b58();
void Func_02001b66();
s32 Func_02001b70();
void Func_02001b74();
s32 Func_02001b78();
s32 Func_02001b98();
s32 Func_02001ba0();
void Func_02001bb0();
void Func_02001bc8();
void Func_02001be6();
void Func_02001bea();
void Func_02001bfe();
s32 Func_02001c30();
s32 Func_02001c3e();
void Func_02001c4c();
void Func_02001c5e();
s32 Func_02001c68();
void Func_02001c70();
void Func_02001c76();
void Func_02001c78();
s32 Func_02001ca2();
void Func_02001ca4();
void Func_02001ca8();
void Func_02001cac();
void Func_02001cae();
u8 *Func_02001cb0();
void Func_02001cc6();
void Func_02001cc8();
void Func_02001ccc();
u8 *Func_02001cd6();
void Func_02001cda();
void Func_02001cda_a();
void Func_02001cdc();
void Func_02001cf2();
void Func_02001cf4();
void Func_02001d00();
void Func_02001d0a();
void Func_02001d12();
void Func_02001cf8();
void Func_02001d2e();
void Func_02001d2e_a();
s32 Func_02001d44();
s32 Func_02001d54();
s32 Func_02001d60();
void Func_02001d66();
s32 Func_02001d68();
void Func_02001d92();
void Func_02001e4e();
s32 Func_020021c0();
void Func_020021dc();
void Func_02002234();
void Func_0200223a();
void Func_02002240();
void Func_020022d6();
void Func_02002424();
s32 Func_02002442();
s32 Func_0200244c();
s32 Func_0200244c_a();
s32 Func_02002458();
s32 Func_02002460();
s32 Func_02002468();
s32 Func_02002470();
s32 Func_02002478();
void Func_0200247a();
void Func_020024a8();
void Func_020024b6();
void Func_020024ea();
s32 Func_020024f2();
void Func_020024f2_a();
void Func_020024f4();
void Func_020024fc();
void Func_020024fe();
void Func_0200251c();
void Func_02002544();
void Func_02002588();
void Func_020025a4();

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
    extern u8 Data_03001ebc[];

    f();
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_02000968(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    f(a0, a1);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3_02000de8(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

void Func_020020b0();          /* scene open */

void Func_020020ca();          /* scene close */

void SetEffectRecordMode(struct EffectWork *work, s32 mode)
{
    work->record->mode = mode;
}

void *SceneEffect_SpawnPrimary(s32 x, s32 y, s32 z, s32 kind)
{
    extern u8 *Data_03001ebc;

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
        Func_02001410(effect, 0);
        Func_020014e0(effect, 14);
        Func_02001428(effect, 1);
        return effect;
    }
    return NULL;
}

void *SceneEffect_SpawnSecondary(s32 x, s32 y, s32 z, s32 kind)
{
    extern u8 *Data_03001ebc;

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
        Func_0200146c(effect, 0);
        Func_0200153c(effect, 15);
        effect[0x23] = (effect[0x23] & 0xfe) | 2;
        return effect;
    }
    return NULL;
}

void SceneEffect_SpawnConfigured(s32 x, s32 y,
                   s32 z, s32 vx, s32 vy, s32 vz, u32 flags,
                   const struct Options *options)
{
    extern u8 *Data_03001ebc;

    u32 table_offset;
    struct Effect *party;
    u32 copied_bits;
    s32 flag_mask;
    u32 block_bits;
    struct Effect *effect;
    struct Sprite *block;
    struct Sprite *mode_block;
    u32 option_bits;
    u16 *tag;
    s32 duration;
    s32 first_delta;
    s32 accumulated;
    party = Func_0200152e(0);

    if ((flags & 0x100000) != 0 && options != 0) {
        effect = Func_020014f4(options->kind, x, y, z);
    } else {
        effect = Func_020014f4(222, x, y, z);
    }
    if (effect == 0) return;

    block = effect->sprite;
    mode_block = block;

    Func_020014fe(effect, (flags + 1) & 15);
    table_offset = (flags & 15) << 2;
    Func_02001518(effect, Data_020095a4[table_offset >> 2]);

    effect->mode55 = 0;
    block->state26 = 0;
    effect->callback = 0x02008105;

    effect->velocity_x = vx;
    x = 3;
    effect->velocity_y = vy;
    effect->velocity_z = vz;

    copied_bits = party->sprite->flags9 & 12;
    block_bits = *(volatile u8 *)&block->flags9;
    flag_mask = ~12;
    block->flags9 = (u8)((block_bits & flag_mask) | copied_bits);

    effect->rate30 = 0;
    effect->rate34 = 0;
    effect->step64 = 0;
    tag = &effect->step64;

    if ((flags & 0xffff0000) == 0 || options == 0) return;

    if ((flags & 0x10000) != 0) {
        Func_02001672(effect, options->mode);
    }

    if ((flags & 0x20000) != 0) {
        effect->flags23 &= 0xfe;
        option_bits = *(const u8 *)options & x;
        block->flags9 = (u8)((*((const u8 *)mode_block + 9) & flag_mask)
                             | (option_bits << 2));
    }

    if ((flags & 0x80000) != 0) {
        effect->accum18 = options->accum18;
        effect->accum1c = options->accum1c;
    }

    if ((flags & 0x40000) != 0) {
        const struct Descriptor *descriptor =
            Data_020095a4[table_offset >> 2];
        s32 delta;

        if ((flags & 0x80000) != 0) {
            first_delta = *(volatile const s32 *)&options->target30;
            accumulated = *(volatile const s32 *)&effect->accum18;
            first_delta -= accumulated;
            effect->rate30 = Func_020015ac(first_delta,
                                           descriptor->duration);
            delta = options->target34;
            duration = descriptor->duration;
            delta -= effect->accum1c;
        } else {
            first_delta = options->target30;
            first_delta += (s32)0xffff0000;
            effect->rate30 = Func_020015c4(first_delta,
                                           descriptor->duration);
            delta = options->target34;
            duration = descriptor->duration;
            delta += (s32)0xffff0000;
        }

        effect->rate34 = Func_020015d2(delta, duration);
    }

    if ((flags & 0x200000) != 0) {
        Func_02001618(effect, 1);
        Func_02001628(effect, options->callback_arg);
    }

    if ((flags & 0x400000) != 0) {
        block->angle = options->angle;
    }

    if ((flags & 0x800000) != 0) {
        *tag = options->step;
    }

    if ((flags & 0x1000000) != 0) {
        effect->callback = options->callback;
    }
}

/*
 * The 16-byte owner at 0x02000314 loads no literal, so it carries no pool
 * word and no alignment halfword.  All three arguments are immediates, and
 * Func_020017be spells the overlay's own relocated call word rather than a
 * runtime address.
 */
void SceneEffect_RequestFixedEffect(void)
{
    extern u8 *Data_03001ebc;

    Func_020017be(22, 1, 2);
}

/* Complete entity-19 sprite-counter adjustment. */
void SceneEffect_AdvanceRotatingSprite(void)
{
    extern u8 *Data_03001ebc;

    u8 *entity = Func_0200172e(19);
    u8 *sprite = *(u8 **)(entity + 80);
    *(u16 *)(sprite + 30) += 0x1400;
}

void SceneEffect_SpawnPeriodicEffect(void)
{
    extern u8 *Data_03001ebc;

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
    extern u8 *Data_03001ebc;

    return (u8 *)0x020097e8;
}

s32 SceneEffect_GetInitialValue(void)
{
    extern u8 *Data_03001ebc;

    return 0;
}

u8 *SceneEffect_GetSecondaryData(void)
{
    extern u8 *Data_03001ebc;

    return (u8 *)0x020098d8;
}

s32 SceneEffect_PrepareState(void)
{
    extern u8 *Data_03001ebc;

    if (Func_02001956(0x895) != 0)
        Data_02009900[0xbe] = 0;
    return (s32)Data_02009900;
}

void SceneEffect_ShowActorSetupMessage(void)
{
    extern u8 *Data_03001ebc;

    Func_02001998();
    Func_02001a3e(0x17e8);
    Func_02001a5e(9, 0);
    Func_020019b2();
}

void FieldScene_RunPrimarySequence(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 rec7;
    s32 rec8;
    u8 *record;
    s32 v7;
    s32 v5;
    s32 p5;
    s32 q1;
    s32 t2;
    s32 q2;
    s32 hi;
    s32 lo;

    rec8 = Value1(Func_020019cc, 20);
    Func_020019c2();
    v7 = 0;
    record = Func_020019d8(18);
    *(volatile s32 *)((s32)record + 108) = v7;
    if (Value1(Func_020019b4, 0x200) == 0) {
        record = Value1(Func_020019ee, 18);
        if ((*(volatile s32 *)((s32)record + 8) >> 20) > 19) {
            goto L_020006a2;
        }
    }
    record = Func_020019fc(18);
    p5 = *(u16 *)((s32)record + 6);
    Func_02001a80(18, 0, 0);
    Func_020019f6(10);
    Call1(Func_02001aa4, 0x17fb);
    if (Value1(Func_020019ec, 0x200) == 0) {
        bump_step(1);
        Func_02001ad0(18, 0);
        *(volatile u16 *)(Func_02001a3e_a(18) + 100) = v7;
        record = Value1(Func_02001a48, 18);
        *(volatile u16 *)((s32)record + 6) = p5;
    } else {
        Func_02001aec(18, 0);
        Call3(Func_02001b10, 18, 0x8000, 20);
    }
    record = Func_02001a66(18);
    *(volatile s32 *)((s32)record + 108) = 0x2008501;
    Call0(Func_02001a66_a);
    goto L_02000916;
    L_020006a2:;
    record = Value1(Func_02001a76, 0);
    if ((*(volatile s32 *)((s32)record + 16) >> 19) > 27) {
        record = Value1(Func_02001a84, 0);
        if ((*(volatile s32 *)((s32)record + 16) >> 19) <= 29) {
            record = Value1(Func_02001a92, 0);
            if ((*(volatile s32 *)((s32)record + 8) >> 20) != 26) {
                Call3(Func_02001ab0, 0, 0x8000, 0x4000);
                Call3((void (*)())Func_02001b2a, 0, 18, 0);
                Func_02001aa0(5);
                rec7 = Value1(Func_02001abe, 0);
                record = Func_02001ac6(18);
                if (*(volatile s32 *)(rec7 + 8) < *(volatile s32 *)((s32)record + 8)) {
                    *(u8 *)(Func_02001ad4(0) + 90) &= 254;
                    record = Value1(Func_02001ae4, 18);
                    Func_02001b24(0, (((*(volatile s32 *)((s32)record + 8) >> 20) << 4) - 8), 232);
                    v7 = 1;
                } else {
                    *(u8 *)(Func_02001afe(0) + 90) &= 254;
                    record = Value1(Func_02001b0e, 18);
                    Func_02001b4e(0, (((*(volatile s32 *)((s32)record + 8) >> 20) << 4) + 24), 232);
                }
                Func_02001b74(0);
            }
        }
    }
    v5 = 128;
    record = Func_02001b2a_a(18);
    *(volatile s32 *)((s32)record + 56) = (v5 << 24);
    record = Value1(Func_02001b36, 18);
    *(volatile s32 *)((s32)record + 60) = (v5 << 24);
    record = Func_02001b3e(18);
    *(volatile s32 *)((s32)record + 64) = (v5 << 24);
    Func_02001b58(18, 1);
    Func_02001bb0(18, 1);
    Func_02001bc8(18, 2);
    Func_02001b46(10);
    Func_02001c4c(228);
    *(s32 *)(rec8 + 24) = 0x4ccc;
    *(s32 *)(rec8 + 28) = 0x4ccc;
    record = Value1(Func_02001b70, 18);
    q1 = *(volatile s32 *)((s32)record + 8);
    record = Value1(Func_02001b78, 18);
    t2 = *(volatile s32 *)((s32)record + 16) >> 20;
    Func_02001bea(20, (((q1 >> 20) << 20) + 0x80000), ((t2 << 20) + 0x80000));
    record = Value1(Func_02001b98, 18);
    q2 = *(volatile s32 *)((s32)record + 8);
    record = Value1(Func_02001ba0, 18);
    Call6(Func_02001b66, 16, 16, 1, 1, (q2 >> 20), (*(volatile s32 *)((s32)record + 16) >> 20));
    Func_02001c76(20, 2);
    {
        u8 *flags = (u8 *)(rec8 + 35);
        u8 value = *(volatile u8 *)flags;

        *flags = (u8)(value | 2);
    }
    do {
        Func_02001b3a(3);
        hi = *(s32 *)(rec8 + 28);
        lo = *(s32 *)(rec8 + 24);
        *(s32 *)(rec8 + 28) = hi + 0x1999;
        lo += 0x1999;
        *(s32 *)(rec8 + 24) = lo;
    } while (lo <= 0xffff);
    Call3(Func_02001cae, 18, 0x105, 70);
    Func_02001c70(18, 0, 0);
    Func_02001be6(20);
    Call3(Func_02001cc8, 18, 0x103, 0);
    Func_02001c78(18, 2);
    Func_02001bfe(70);
    Call1(Func_02001cac, 0x17fa);
    Func_02001cc6(18, 0, 20);
    Func_02001d0a();
    record = Value1(Func_02001c30, 0);
    if ((*(volatile s32 *)((s32)record + 8) >> 20) == 26) {
        record = Value1(Func_02001c3e, 0);
        if ((*(volatile s32 *)((s32)record + 16) >> 20) > 13) {
            v7 = 1;
        }
    }
    if (v7 != 0) {
        Call3(Func_02001c5e, 0, 0xcccc, 0x6666);
        Call3(Func_02001d12, 0, 0xc000, 10);
        *(u8 *)(Func_02001c68(0) + 90) &= 254;
        Func_02001cda(0, 2);
        Func_02001ccc(0, 0, 16);
        Func_02001cda_a(0);
        Func_02001cf2(0, 1);
    }
    Call3(Func_02001ca4, 18, 0xcccc, 0x6666);
    record = Value1(Func_02001ca2, 18);
    if ((*(volatile s32 *)((s32)record + 16) >> 20) != 14) {
        record = Func_02001cb0(18);
        Func_02001cf4(18, *(s16 *)((s32)record + 10), 232);
    }
    Call3(Func_02001d00, 18, 0x118, 232);
    Call1(Func_02001ca8, 0x200);
    {
        u8 *record = Func_02001cd6(0);
        u8 value = *(volatile u8 *)&record[90];

        record[90] = (u8)(value | 1);
    }
    Func_02001cdc();
    L_02000916:;
}

void SceneEffect_ActivateNearbyActor(void)
{
    extern u8 *Data_03001ebc;

    u8 *leader = Func_02001d22(0);
    if ((*(s32 *)(leader + 16) >> 20) <= 13)
        Func_02001dea(20, 1);
}

void FieldScene_RunScene3a0_02000968(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;
    s32 v5;
    u8 *p5;

    Func_02001d2e();
    *(u8 *)(Func_02001d44(20) + 35) &= 253;
    v5 = 0;
    *(u8 *)(Func_02001d54(20) + 85) = v5;
    record = Value1(Func_02001d60, 20);
    p5 = *(volatile s32 *)(record + 8);
    record = Value1(Func_02001d68, 20);
    Call6(Func_02001d2e_a, 3, 17, 1, 1, ((s32)p5 >> 20), (*(volatile s32 *)(record + 16) >> 20));
    Call2(Func_02001cf8, 0x2008325, 0xc80);
    Call1_02000968(Func_02001d66, 0x201);
    Func_02001e4e(20, 2);
    Func_02001d92();
}

/*
 * The 28-byte owner includes its one pool word: 0x17f7 is an identifier
 * passed as an argument, not an address.  The first and last calls are the
 * scene bracket and must stay in that order.
 */
void SceneEffect_RunActorSceneMessage(void)
{
    extern u8 *Data_03001ebc;

    Func_020020b0();
    Func_02002156(0x17f7);
    Func_02002176(17, 0);
    Func_020020ca();
}

void FieldScene_RunScene3a0_02000de8(s32 a0)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    *(u8 *)(Func_020021c0(0) + 85) = 0;
    Call3_02000de8(Func_020021dc, 0, 0x8000, 0x4000);
    if (a0 == 6) {
        Func_02002240(0, 2);
        Call3_02000de8(Func_02002234, 0, 0, -16);
    } else {
        Call3_02000de8(Func_0200223a, 0, 2, -16);
    }
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c8)) = 16;
    Func_020022d6(a0);
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

    Func_02002214();

    switch (scene[182]) {
    case 1:
        Func_02002348(158);
        Func_02002212(Data_02009778, 81, 18);
        break;
    case 2:
        Func_0200235a(158);
        shared0 = Data_0200978e;
        shared1 = 83;
        goto shared;
    case 3:
        Func_02002366(158);
        shared0 = Data_0200978e;
        shared1 = 86;
        goto shared;
    case 4:
        Func_02002372(158);
        Func_0200223c(Data_020097a4, 84, 24);
        break;
    case 5:
        Func_02002384(158);
        Func_0200224e(Data_020097a4, 72, 7);
        break;
    case 6:
        Func_02002396(188);
        shared0 = Data_020097ba;
        shared1 = 69;
    shared:
        Func_02002260(shared0, shared1, 11);
        break;
    case 7:
        Func_020023a8(158);
        Func_02002272(Data_020097d0, 83, 7);
        break;
    default:
        break;
    }

    Func_02001cf0(scene[182]);
    Func_020022d4();
}

void FieldScene_RunScene3a0_02001060(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Func_02002424();
    Call2((void (*)())Func_0200244c, 18, 1);
    record = Func_02002442(18);
    *(volatile s32 *)(record + 108) = 0;
    record = Func_0200244c_a(18);
    *(volatile s32 *)(record + 56) = -0x80000000;
    record = Value1(Func_02002458, 18);
    *(volatile s32 *)(record + 64) = -0x80000000;
    record = Value1(Func_02002460, 18);
    *(volatile s32 *)(record + 36) = 0;
    record = Value1(Func_02002468, 18);
    *(volatile s32 *)(record + 44) = 0;
    record = Value1(Func_02002470, 18);
    *(volatile s32 *)(record + 48) = 0;
    record = Func_02002478(18);
    *(volatile s32 *)(record + 52) = 0;
    Call3(Func_02002544, 18, 0x103, 0);
    Func_020024f4(18, 2);
    Func_0200247a(60);
    Call3(Func_020024a8, 18, 0x18000, 0xc000);
    Call3(Func_020024b6, 0, 0x18000, 0xc000);
    Call3(Func_020024ea, 18, 0x118, 232);
    Call3(Func_020024fe, 0, 0x128, 232);
    Func_0200251c(18);
    Call3(Func_02002588, 0, 0x8000, 20);
    Call3(Func_020025a4, 0, 0x102, 60);
    Call2(Func_020024fc, 18, 0x20095b0);
    record = Func_020024f2(18);
    *(volatile s32 *)(record + 108) = 0x2008501;
    Call0(Func_020024f2_a);
}

u8 *SceneEffect_GetTertiaryData(void)
{
    extern u8 *Data_03001ebc;

    return (u8 *)0x02009ac8;
}
