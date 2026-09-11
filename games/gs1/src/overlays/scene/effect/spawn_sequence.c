#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/effect/spawn_sequence.h"

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

void *Effect_Run6(s32, s32, s32, s32);

void *Effect_Run7(s32, s32, s32, s32);

struct Effect *Effect_Run8();
struct Effect *Effect_Run9();

u8 *Effect_Run10();
u8 *Effect_Run11();

typedef s32(*IwramIntegerSquareRoot)(s32);

u8 *Effect_Run12(s32 actorId);

u8 *Effect_Run13();

u8 *Effect_Run14();

u8 *Effect_Run15();

u8 *Effect_Run16();

u8 *Effect_Run17();
u8 *Effect_Run18();

u8 *Effect_Run19();

u8 *Effect_Run20();

u8 *Effect_Run21();

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

void Effect_Run22();          /* scene open */

void Effect_Run23();          /* scene close */

void SetEffectRecordMode(struct EffectWork *work, s32 mode)
{
    work->record->mode = mode;
}

void *Effect_SpawnPrimary(s32 x, s32 y, s32 z, s32 kind)
{
    extern u8 *gWork;

    u8 *effect = Effect_Run6(kind, x, y, z);

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

    u8 *effect = Effect_Run7(kind, x, y, z);

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
    party = Effect_Run8(0);

    if ((flags & 0x100000) != 0 && options != 0) {
        effect = Effect_Run9(options->kind, x, y, z);
    } else {
        effect = Effect_Run9(222, x, y, z);
    }
    if (effect == 0) return;

    block = effect->sprite;
    mode_block = block;

    Effect_Run24(effect, (flags + 1) & 15);
    table_offset = (flags & 15) << 2;
    Effect_Run25(effect, gOv[table_offset >> 2]);

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
        Effect_Run26(effect, options->mode);
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
            effect->rate30 = Effect_Check26(first_delta,
                                           descriptor->duration);
            delta = options->target34;
            duration = descriptor->duration;
            delta -= effect->accum1c;
        } else {
            first_delta = options->target30;
            first_delta += (s32)0xffff0000;
            effect->rate30 = Effect_Check27(first_delta,
                                           descriptor->duration);
            delta = options->target34;
            duration = descriptor->duration;
            delta += (s32)0xffff0000;
        }

        effect->rate34 = Effect_Check28(delta, duration);
    }

    if ((flags & 0x200000) != 0) {
        Effect_Run27(effect, 1);
        Effect_Run28(effect, options->callback_arg);
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
 * Effect_Run29 spells the overlay's own relocated call word rather than a
 * runtime address.
 */
void Effect_RequestFixedEffect(void)
{
    extern u8 *gWork;

    Effect_Run29(22, 1, 2);
}

/* Complete entity-19 sprite-counter adjustment. */
void Effect_AdvanceRotatingSprite(void)
{
    extern u8 *gWork;

    u8 *entity = Effect_Run10(19);
    u8 *sprite = *(u8 **)(entity + 80);
    *(u16 *)(sprite + 30) += 0x1400;
}

void Effect_SpawnPeriodicEffect(void)
{
    extern u8 *gWork;

    u8 *entity = Effect_Run11(14);

    if ((gIw & 3) == 0) {
        struct PeriodicEffectConfig config;
        config.kind = 1;
        config.variant = 9;
        config.id = 169;
        config.data = gOv2;
        Effect_Run30(
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

    if (Effect_Check29(0x895) != 0)
        gOv3[0xbe] = 0;
    return (s32)gOv3;
}

void Effect_ShowActorSetupMessage(void)
{
    extern u8 *gWork;

    Effect_Run31();
    Effect_Do5(0x17e8);
    Effect_Apply6(9, 0);
    Effect_Run32();
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
    Effect_Run33();
    v7 = 0;
    record = Effect_Run13(18);
    *(volatile s32 *)((s32)record + 108) = v7;
    if (Effect_Check2(0x200) == 0) {
        record = Effect_Check3(18);
        if ((*(volatile s32 *)((s32)record + 8) >> 20) > 19) {
            goto L_020006a2;
        }
    }
    record = Effect_Run14(18);
    p5 = *(u16 *)((s32)record + 6);
    Effect_Run34(18, 0, 0);
    Effect_Run35(10);
    Effect_Do(0x17fb);
    if (Effect_Check4(0x200) == 0) {
        bump_step(1);
        Effect_Run36(18, 0);
        *(volatile u16 *)(Effect_Check30(18) + 100) = v7;
        record = Effect_Check5(18);
        *(volatile u16 *)((s32)record + 6) = p5;
    } else {
        Effect_Run37(18, 0);
        Effect_Place(18, 0x8000, 20);
    }
    record = Effect_Run15(18);
    *(volatile s32 *)((s32)record + 108) = 0x2008501;
    Effect_Run();
    goto L_02000916;
    L_020006a2:;
    record = Effect_Check6(0);
    if ((*(volatile s32 *)((s32)record + 16) >> 19) > 27) {
        record = Effect_Check7(0);
        if ((*(volatile s32 *)((s32)record + 16) >> 19) <= 29) {
            record = Effect_Check8(0);
            if ((*(volatile s32 *)((s32)record + 8) >> 20) != 26) {
                Effect_Place2(0, 0x8000, 0x4000);
                Effect_Place3(0, 18, 0);
                Effect_Run38(5);
                rec7 = Effect_Check9(0);
                record = Effect_Run16(18);
                if (*(volatile s32 *)(rec7 + 8) < *(volatile s32 *)((s32)record + 8)) {
                    *(u8 *)(Effect_Check31(0) + 90) &= 254;
                    record = Effect_Check10(18);
                    Effect_Run39(0, (((*(volatile s32 *)((s32)record + 8) >> 20) << 4) - 8), 232);
                    v7 = 1;
                } else {
                    *(u8 *)(Effect_Check32(0) + 90) &= 254;
                    record = Effect_Check11(18);
                    Effect_Run40(0, (((*(volatile s32 *)((s32)record + 8) >> 20) << 4) + 24), 232);
                }
                Effect_Run41(0);
            }
        }
    }
    v5 = 128;
    record = Effect_Run18(18);
    *(volatile s32 *)((s32)record + 56) = (v5 << 24);
    record = Effect_Check12(18);
    *(volatile s32 *)((s32)record + 60) = (v5 << 24);
    record = Effect_Run19(18);
    *(volatile s32 *)((s32)record + 64) = (v5 << 24);
    Effect_Run42(18, 1);
    Effect_Run43(18, 1);
    Effect_Run44(18, 2);
    Effect_Run45(10);
    Effect_Run46(228);
    *(s32 *)(rec8 + 24) = 0x4ccc;
    *(s32 *)(rec8 + 28) = 0x4ccc;
    record = Effect_Check13(18);
    q1 = *(volatile s32 *)((s32)record + 8);
    record = Effect_Check14(18);
    t2 = *(volatile s32 *)((s32)record + 16) >> 20;
    Effect_Run47(20, (((q1 >> 20) << 20) + 0x80000), ((t2 << 20) + 0x80000));
    record = Effect_Check15(18);
    q2 = *(volatile s32 *)((s32)record + 8);
    record = Effect_Check16(18);
    Effect_SetRect(16, 16, 1, 1, (q2 >> 20), (*(volatile s32 *)((s32)record + 16) >> 20));
    Effect_Run48(20, 2);
    {
        u8 *flags = (u8 *)(rec8 + 35);
        u8 value = *(volatile u8 *)flags;

        *flags = (u8)(value | 2);
    }
    do {
        Effect_Run49(3);
        hi = *(s32 *)(rec8 + 28);
        lo = *(s32 *)(rec8 + 24);
        *(s32 *)(rec8 + 28) = hi + 0x1999;
        lo += 0x1999;
        *(s32 *)(rec8 + 24) = lo;
    } while (lo <= 0xffff);
    Effect_Place4(18, 0x105, 70);
    Effect_Run50(18, 0, 0);
    Effect_Run51(20);
    Effect_Place5(18, 0x103, 0);
    Effect_Run52(18, 2);
    Effect_Run53(70);
    Effect_Do2(0x17fa);
    Effect_Run54(18, 0, 20);
    Effect_Run55();
    record = Effect_Check17(0);
    if ((*(volatile s32 *)((s32)record + 8) >> 20) == 26) {
        record = Effect_Check18(0);
        if ((*(volatile s32 *)((s32)record + 16) >> 20) > 13) {
            v7 = 1;
        }
    }
    if (v7 != 0) {
        Effect_Place6(0, 0xcccc, 0x6666);
        Effect_Place7(0, 0xc000, 10);
        *(u8 *)(Effect_Check33(0) + 90) &= 254;
        Effect_Run56(0, 2);
        Effect_Run57(0, 0, 16);
        Effect_Run58(0);
        Effect_Run59(0, 1);
    }
    Effect_Place8(18, 0xcccc, 0x6666);
    record = Effect_Check19(18);
    if ((*(volatile s32 *)((s32)record + 16) >> 20) != 14) {
        record = Effect_Run20(18);
        Effect_Run60(18, *(s16 *)((s32)record + 10), 232);
    }
    Effect_Place9(18, 0x118, 232);
    Effect_Do3(0x200);
    {
        u8 *record = Effect_Run21(0);
        u8 value = *(volatile u8 *)&record[90];

        record[90] = (u8)(value | 1);
    }
    Effect_Run61();
    L_02000916:;
}

void Effect_ActivateNearbyActor(void)
{
    extern u8 *gWork;

    u8 *leader = Effect_Run12(0);
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

    Effect_Run62();
    *(u8 *)(Effect_Check34(20) + 35) &= 253;
    v5 = 0;
    *(u8 *)(Effect_Check35(20) + 85) = v5;
    record = Effect_Check20(20);
    p5 = *(volatile s32 *)(record + 8);
    record = Effect_Check21(20);
    Effect_SetRect2(3, 17, 1, 1, ((s32)p5 >> 20), (*(volatile s32 *)(record + 16) >> 20));
    Effect_Run2(0x2008325, 0xc80);
    Effect_Do4(0x201);
    Effect_Run63(20, 2);
    Effect_Run64();
}

/*
 * The 28-byte owner includes its one pool word: 0x17f7 is an identifier
 * passed as an argument, not an address.  The first and last calls are the
 * scene bracket and must stay in that order.
 */
void Effect_RunActorSceneMessage(void)
{
    extern u8 *gWork;

    Effect_Run22();
    Effect_Run65(0x17f7);
    Effect_Check36(17, 0);
    Effect_Run23();
}

void Scene_RunScene3a0(s32 a0)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    *(u8 *)(Effect_Check37(0) + 85) = 0;
    Effect_Place10(0, 0x8000, 0x4000);
    if (a0 == 6) {
        Effect_Run66(0, 2);
        Effect_Place11(0, 0, -16);
    } else {
        Effect_Place12(0, 2, -16);
    }
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c8)) = 16;
    Effect_Run67(a0);
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

    Effect_Run68();

    switch (scene[182]) {
    case 1:
        Effect_Run69(158);
        Effect_Run70(gOv4, 81, 18);
        break;
    case 2:
        Effect_Run71(158);
        shared0 = gOv5;
        shared1 = 83;
        goto shared;
    case 3:
        Effect_Run72(158);
        shared0 = gOv5;
        shared1 = 86;
        goto shared;
    case 4:
        Effect_Run73(158);
        Effect_Run74(gOv6, 84, 24);
        break;
    case 5:
        Effect_Run75(158);
        Effect_Run76(gOv6, 72, 7);
        break;
    case 6:
        Effect_Run77(188);
        shared0 = gOv7;
        shared1 = 69;
    shared:
        Effect_Run78(shared0, shared1, 11);
        break;
    case 7:
        Effect_Run79(158);
        Effect_Run80(gOv8, 83, 7);
        break;
    default:
        break;
    }

    Effect_Run81(scene[182]);
    Effect_Run82();
}

void Scene_RunScene3a0(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Effect_Run83();
    Effect_Run3(18, 1);
    record = Effect_Check38(18);
    *(volatile s32 *)(record + 108) = 0;
    record = Effect_Check39(18);
    *(volatile s32 *)(record + 56) = -0x80000000;
    record = Effect_Check22(18);
    *(volatile s32 *)(record + 64) = -0x80000000;
    record = Effect_Check23(18);
    *(volatile s32 *)(record + 36) = 0;
    record = Effect_Check24(18);
    *(volatile s32 *)(record + 44) = 0;
    record = Effect_Check25(18);
    *(volatile s32 *)(record + 48) = 0;
    record = Effect_Check40(18);
    *(volatile s32 *)(record + 52) = 0;
    Effect_Place13(18, 0x103, 0);
    Effect_Run84(18, 2);
    Effect_Run85(60);
    Effect_Place14(18, 0x18000, 0xc000);
    Effect_Place15(0, 0x18000, 0xc000);
    Effect_Place16(18, 0x118, 232);
    Effect_Place17(0, 0x128, 232);
    Effect_Run86(18);
    Effect_Place18(0, 0x8000, 20);
    Effect_Place19(0, 0x102, 60);
    Effect_Run4(18, 0x20095b0);
    record = Effect_Check41(18);
    *(volatile s32 *)(record + 108) = 0x2008501;
    Effect_Run5();
}

u8 *Effect_GetTertiaryData(void)
{
    extern u8 *gWork;

    return (u8 *)0x02009ac8;
}
