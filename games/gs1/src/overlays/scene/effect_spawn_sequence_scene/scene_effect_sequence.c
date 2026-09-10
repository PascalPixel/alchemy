#include "types.h"

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

#include "scene_effect_sequence.h"

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
extern u8 *Data_03001ebc;
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

void Func_020020b0();          /* scene open */

void Func_020020ca();          /* scene close */

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
        Func_02001410(effect, 0);
        Func_020014e0(effect, 14);
        Func_02001428(effect, 1);
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
    if (Func_02001956(0x895) != 0)
        Data_02009900[0xbe] = 0;
    return (s32)Data_02009900;
}

void SceneEffect_ShowActorSetupMessage(void)
{
    Func_02001998();
    Func_02001a3e(0x17e8);
    Func_02001a5e(9, 0);
    Func_020019b2();
}

void SceneEffect_ActivateNearbyActor(void)
{
    u8 *leader = Func_02001d22(0);
    if ((*(s32 *)(leader + 16) >> 20) <= 13)
        Func_02001dea(20, 1);
}

/*
 * The 28-byte owner includes its one pool word: 0x17f7 is an identifier
 * passed as an argument, not an address.  The first and last calls are the
 * scene bracket and must stay in that order.
 */
void SceneEffect_RunActorSceneMessage(void)
{
    Func_020020b0();
    Func_02002156(0x17f7);
    Func_02002176(17, 0);
    Func_020020ca();
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

u8 *SceneEffect_GetTertiaryData(void)
{
    return (u8 *)0x02009ac8;
}
