#include "types.h"
#include "scene.h"

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

extern struct Descriptor *gOv[];
extern u32 gIw;
extern u8 gOv2[];
extern u8 gOv3[];
extern u8 gOv4[];
extern u8 gOv5[];
extern u8 gOv6[];
extern u8 gOv7[];
extern u8 gOv8[];

void *Effect_unk6_4(s32, s32, s32, s32);

void *Effect_unk7_4(s32, s32, s32, s32);

struct Effect *Effect_unk8_4();
struct Effect *Effect_unk9_4();

u8 *Effect_unk10_4();
u8 *Effect_unk11_4();

typedef s32(*IwramIntegerSquareRoot)(s32);

u8 *Effect_unk12_4(s32 actorId);

u8 *Effect_unk13_4();

u8 *Effect_unk14_4();

u8 *Effect_unk15_4();

u8 *Effect_unk16_4();

u8 *Effect_unk17_4();
u8 *Effect_unk18_4();

u8 *Effect_unk19_4();

u8 *Effect_unk20_4();

u8 *Effect_unk21_4();

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

void Effect_unk22_4();          /* scene open */

void Effect_unk23_4();          /* scene close */

void SetEffectRecordMode(struct EffectWork *work, s32 mode)
{
    work->record->mode = mode;
}

void *Effect_SpawnPrimary(s32 x, s32 y, s32 z, s32 kind)
{
    extern u8 *gWork;

    u8 *effect = Effect_unk6_4(kind, x, y, z);

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
        Effect_Apply(effect, 0);
        Effect_Apply2(effect, 14);
        Effect_Apply3(effect, 1);
        return effect;
    }
    return NULL;
}

void *Effect_SpawnSecondary(s32 x, s32 y, s32 z, s32 kind)
{
    extern u8 *gWork;

    u8 *effect = Effect_unk7_4(kind, x, y, z);

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
        Effect_Apply4(effect, 0);
        Effect_Apply5(effect, 15);
        effect[0x23] = (effect[0x23] & 0xfe) | 2;
        return effect;
    }
    return NULL;
}

void Effect_SpawnConfigured(s32 x, s32 y,
                   s32 z, s32 vx, s32 vy, s32 vz, u32 flags,
                   const struct Options *options)
{
    extern u8 *gWork;

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
    party = Effect_unk8_4(0);

    if ((flags & 0x100000) != 0 && options != 0) {
        effect = Effect_unk9_4(options->kind, x, y, z);
    } else {
        effect = Effect_unk9_4(222, x, y, z);
    }
    if (effect == 0) return;

    block = effect->sprite;
    mode_block = block;

    Effect_unk24_4(effect, (flags + 1) & 15);
    table_offset = (flags & 15) << 2;
    Effect_unk25_3(effect, gOv[table_offset >> 2]);

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
        Effect_unk26_3(effect, options->mode);
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
            gOv[table_offset >> 2];
        s32 delta;

        if ((flags & 0x80000) != 0) {
            first_delta = *(volatile const s32 *)&options->target30;
            accumulated = *(volatile const s32 *)&effect->accum18;
            first_delta -= accumulated;
            effect->rate30 = Effect_unk26(first_delta,
                                           descriptor->duration);
            delta = options->target34;
            duration = descriptor->duration;
            delta -= effect->accum1c;
        } else {
            first_delta = options->target30;
            first_delta += (s32)0xffff0000;
            effect->rate30 = Effect_unk27(first_delta,
                                           descriptor->duration);
            delta = options->target34;
            duration = descriptor->duration;
            delta += (s32)0xffff0000;
        }

        effect->rate34 = Effect_unk28(delta, duration);
    }

    if ((flags & 0x200000) != 0) {
        Effect_unk27_3(effect, 1);
        Effect_unk28_3(effect, options->callback_arg);
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
 * Effect_unk29_3 spells the overlay's own relocated call word rather than a
 * runtime address.
 */
void Effect_RequestFixedEffect(void)
{
    extern u8 *gWork;

    Effect_unk29_3(22, 1, 2);
}

/* Complete entity-19 sprite-counter adjustment. */
void Effect_AdvanceRotatingSprite(void)
{
    extern u8 *gWork;

    u8 *entity = Effect_unk10_4(19);
    u8 *sprite = *(u8 **)(entity + 80);
    *(u16 *)(sprite + 30) += 0x1400;
}

void Effect_SpawnPeriodicEffect(void)
{
    extern u8 *gWork;

    u8 *entity = Effect_unk11_4(14);

    if ((gIw & 3) == 0) {
        struct PeriodicEffectConfig config;
        config.kind = 1;
        config.variant = 9;
        config.id = 169;
        config.data = gOv2;
        Effect_unk30_2(
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

s32 Effect_CalculatePositionDistance(
    s32 *first_position,
    s32 *second_position)
{
    extern u8 *gWork;

    s32 delta_x = (*first_position++ - *second_position++) >> 16;
    s32 delta_y = (*first_position++ - *second_position++) >> 16;
    s32 delta_z = (*first_position - *second_position) >> 16;
    s32 delta_x_squared = delta_x *delta_x;
    s32 delta_y_squared = delta_y *delta_y;
    s32 delta_z_squared = delta_z *delta_z;

    return ((IwramIntegerSquareRoot) 0x030001D8)(delta_x_squared + delta_y_squared + delta_z_squared);
}

u8 *Effect_GetPrimaryData(void)
{
    extern u8 *gWork;

    return (u8 *)0x020097e8;
}

s32 Effect_GetInitialValue(void)
{
    extern u8 *gWork;

    return 0;
}

u8 *Effect_GetSecondaryData(void)
{
    extern u8 *gWork;

    return (u8 *)0x020098d8;
}

s32 Effect_PrepareState(void)
{
    extern u8 *gWork;

    if (Effect_unk29(0x895) != 0)
        gOv3[0xbe] = 0;
    return (s32)gOv3;
}

void Effect_ShowActorSetupMessage(void)
{
    extern u8 *gWork;

    Effect_unk31_2();
    Effect_unk5_2(0x17e8);
    Effect_Apply6(9, 0);
    Effect_unk32_2();
}

void Scene_RunPrimarySequence(void)
{
    extern u8 gWork[];

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

    rec8 = Effect_Check(20);
    Effect_unk33_2();
    v7 = 0;
    record = Effect_unk13_4(18);
    *(volatile s32 *)((s32)record + 108) = v7;
    if (Effect_unk2(0x200) == 0) {
        record = Effect_unk3(18);
        if ((*(volatile s32 *)((s32)record + 8) >> 20) > 19) {
            goto L_020006a2;
        }
    }
    record = Effect_unk14_4(18);
    p5 = *(u16 *)((s32)record + 6);
    Effect_unk34_2(18, 0, 0);
    Effect_unk35_2(10);
    Effect_Do(0x17fb);
    if (Effect_unk4(0x200) == 0) {
        bump_step(1);
        Effect_unk36_2(18, 0);
        *(volatile u16 *)(Effect_unk30(18) + 100) = v7;
        record = Effect_unk5(18);
        *(volatile u16 *)((s32)record + 6) = p5;
    } else {
        Effect_unk37_2(18, 0);
        Effect_Place(18, 0x8000, 20);
    }
    record = Effect_unk15_4(18);
    *(volatile s32 *)((s32)record + 108) = 0x2008501;
    Effect_Run();
    goto L_02000916;
    L_020006a2:;
    record = Effect_unk6(0);
    if ((*(volatile s32 *)((s32)record + 16) >> 19) > 27) {
        record = Effect_unk7(0);
        if ((*(volatile s32 *)((s32)record + 16) >> 19) <= 29) {
            record = Effect_unk8(0);
            if ((*(volatile s32 *)((s32)record + 8) >> 20) != 26) {
                Effect_unk2_3(0, 0x8000, 0x4000);
                Effect_unk3_3(0, 18, 0);
                Effect_unk38_2(5);
                rec7 = Effect_unk9(0);
                record = Effect_unk16_4(18);
                if (*(volatile s32 *)(rec7 + 8) < *(volatile s32 *)((s32)record + 8)) {
                    *(u8 *)(Effect_unk31(0) + 90) &= 254;
                    record = Effect_unk10(18);
                    Effect_unk39_2(0, (((*(volatile s32 *)((s32)record + 8) >> 20) << 4) - 8), 232);
                    v7 = 1;
                } else {
                    *(u8 *)(Effect_unk32(0) + 90) &= 254;
                    record = Effect_unk11(18);
                    Effect_unk40_2(0, (((*(volatile s32 *)((s32)record + 8) >> 20) << 4) + 24), 232);
                }
                Effect_unk41_2(0);
            }
        }
    }
    v5 = 128;
    record = Effect_unk18_4(18);
    *(volatile s32 *)((s32)record + 56) = (v5 << 24);
    record = Effect_unk12(18);
    *(volatile s32 *)((s32)record + 60) = (v5 << 24);
    record = Effect_unk19_4(18);
    *(volatile s32 *)((s32)record + 64) = (v5 << 24);
    Effect_unk42(18, 1);
    Effect_unk43(18, 1);
    Effect_unk44(18, 2);
    Effect_unk45(10);
    Effect_unk46(228);
    *(s32 *)(rec8 + 24) = 0x4ccc;
    *(s32 *)(rec8 + 28) = 0x4ccc;
    record = Effect_unk13(18);
    q1 = *(volatile s32 *)((s32)record + 8);
    record = Effect_unk14(18);
    t2 = *(volatile s32 *)((s32)record + 16) >> 20;
    Effect_unk47(20, (((q1 >> 20) << 20) + 0x80000), ((t2 << 20) + 0x80000));
    record = Effect_unk15(18);
    q2 = *(volatile s32 *)((s32)record + 8);
    record = Effect_unk16(18);
    Effect_SetRect(16, 16, 1, 1, (q2 >> 20), (*(volatile s32 *)((s32)record + 16) >> 20));
    Effect_unk48(20, 2);
    {
        u8 *flags = (u8 *)(rec8 + 35);
        u8 value = *(volatile u8 *)flags;

        *flags = (u8)(value | 2);
    }
    do {
        Effect_unk49(3);
        hi = *(s32 *)(rec8 + 28);
        lo = *(s32 *)(rec8 + 24);
        *(s32 *)(rec8 + 28) = hi + 0x1999;
        lo += 0x1999;
        *(s32 *)(rec8 + 24) = lo;
    } while (lo <= 0xffff);
    Effect_unk4_3(18, 0x105, 70);
    Effect_unk50(18, 0, 0);
    Effect_unk51(20);
    Effect_unk5_3(18, 0x103, 0);
    Effect_unk52(18, 2);
    Effect_unk53(70);
    Effect_unk2_2(0x17fa);
    Effect_unk54(18, 0, 20);
    Effect_unk55();
    record = Effect_unk17(0);
    if ((*(volatile s32 *)((s32)record + 8) >> 20) == 26) {
        record = Effect_unk18(0);
        if ((*(volatile s32 *)((s32)record + 16) >> 20) > 13) {
            v7 = 1;
        }
    }
    if (v7 != 0) {
        Effect_unk6_3(0, 0xcccc, 0x6666);
        Effect_unk7_3(0, 0xc000, 10);
        *(u8 *)(Effect_unk33(0) + 90) &= 254;
        Effect_unk56(0, 2);
        Effect_unk57(0, 0, 16);
        Effect_unk58(0);
        Effect_unk59(0, 1);
    }
    Effect_unk8_3(18, 0xcccc, 0x6666);
    record = Effect_unk19(18);
    if ((*(volatile s32 *)((s32)record + 16) >> 20) != 14) {
        record = Effect_unk20_4(18);
        Effect_unk60(18, *(s16 *)((s32)record + 10), 232);
    }
    Effect_unk9_3(18, 0x118, 232);
    Effect_unk3_2(0x200);
    {
        u8 *record = Effect_unk21_4(0);
        u8 value = *(volatile u8 *)&record[90];

        record[90] = (u8)(value | 1);
    }
    Effect_unk61();
    L_02000916:;
}

void Effect_ActivateNearbyActor(void)
{
    extern u8 *gWork;

    u8 *leader = Effect_unk12_4(0);
    if ((*(s32 *)(leader + 16) >> 20) <= 13)
        Effect_Apply7(20, 1);
}

void Scene_RunScene3a0(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;
    s32 v5;
    u8 *p5;

    Effect_unk62();
    *(u8 *)(Effect_unk34(20) + 35) &= 253;
    v5 = 0;
    *(u8 *)(Effect_unk35(20) + 85) = v5;
    record = Effect_unk20(20);
    p5 = *(volatile s32 *)(record + 8);
    record = Effect_unk21(20);
    Effect_unk2_5(3, 17, 1, 1, ((s32)p5 >> 20), (*(volatile s32 *)(record + 16) >> 20));
    Effect_unk2_4(0x2008325, 0xc80);
    Effect_unk4_2(0x201);
    Effect_unk63(20, 2);
    Effect_unk64();
}

/*
 * The 28-byte owner includes its one pool word: 0x17f7 is an identifier
 * passed as an argument, not an address.  The first and last calls are the
 * scene bracket and must stay in that order.
 */
void Effect_RunActorSceneMessage(void)
{
    extern u8 *gWork;

    Effect_unk22_4();
    Effect_unk65(0x17f7);
    Effect_unk36(17, 0);
    Effect_unk23_4();
}

void Scene_RunScene3a0(s32 a0)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    *(u8 *)(Effect_unk37(0) + 85) = 0;
    Effect_unk10_3(0, 0x8000, 0x4000);
    if (a0 == 6) {
        Effect_unk66(0, 2);
        Effect_unk11_3(0, 0, -16);
    } else {
        Effect_unk12_3(0, 2, -16);
    }
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c8)) = 16;
    Effect_unk67(a0);
}

/*
 * The scene table is reached through the IWRAM pointer gWork, not
 * through the resident workspace, and the selector is its halfword [182],
 * guarded to the range 1..7.  The pointer is loaded before the first call and
 * held across all of them, so it is a function-top local.  Cases 2 and 3 set
 * the two shared arguments and jump into the middle of case 6 to share its
 * final call; the goto and the two locals are what reproduce that.
 */
void Effect_DispatchStep(void)
{
    extern u8 *gWork;

    s16 *scene = (s16 *)gWork;
    u8 *shared0;
    s32 shared1;

    Effect_unk68();

    switch (scene[182]) {
    case 1:
        Effect_unk69(158);
        Effect_unk70(gOv4, 81, 18);
        break;
    case 2:
        Effect_unk71(158);
        shared0 = gOv5;
        shared1 = 83;
        goto shared;
    case 3:
        Effect_unk72(158);
        shared0 = gOv5;
        shared1 = 86;
        goto shared;
    case 4:
        Effect_unk73(158);
        Effect_unk74(gOv6, 84, 24);
        break;
    case 5:
        Effect_unk75(158);
        Effect_unk76(gOv6, 72, 7);
        break;
    case 6:
        Effect_unk77(188);
        shared0 = gOv7;
        shared1 = 69;
    shared:
        Effect_unk78(shared0, shared1, 11);
        break;
    case 7:
        Effect_unk79(158);
        Effect_unk80(gOv8, 83, 7);
        break;
    default:
        break;
    }

    Effect_unk81(scene[182]);
    Effect_unk82();
}

void Scene_RunScene3a0(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Effect_unk83();
    Effect_unk3_4(18, 1);
    record = Effect_unk38(18);
    *(volatile s32 *)(record + 108) = 0;
    record = Effect_unk39(18);
    *(volatile s32 *)(record + 56) = -0x80000000;
    record = Effect_unk22(18);
    *(volatile s32 *)(record + 64) = -0x80000000;
    record = Effect_unk23(18);
    *(volatile s32 *)(record + 36) = 0;
    record = Effect_unk24(18);
    *(volatile s32 *)(record + 44) = 0;
    record = Effect_unk25(18);
    *(volatile s32 *)(record + 48) = 0;
    record = Effect_unk40(18);
    *(volatile s32 *)(record + 52) = 0;
    Effect_unk13_3(18, 0x103, 0);
    Effect_unk84(18, 2);
    Effect_unk85(60);
    Effect_unk14_3(18, 0x18000, 0xc000);
    Effect_unk15_3(0, 0x18000, 0xc000);
    Effect_unk16_3(18, 0x118, 232);
    Effect_unk17_3(0, 0x128, 232);
    Effect_unk86(18);
    Effect_unk18_3(0, 0x8000, 20);
    Effect_unk19_3(0, 0x102, 60);
    Effect_unk4_4(18, 0x20095b0);
    record = Effect_unk41(18);
    *(volatile s32 *)(record + 108) = 0x2008501;
    Effect_unk5_4();
}

u8 *Effect_GetTertiaryData(void)
{
    extern u8 *gWork;

    return (u8 *)0x02009ac8;
}
