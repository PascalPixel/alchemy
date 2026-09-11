#include "types.h"
#include "scene.h"

#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#define ACTOR_FIELD_108(rec) (*(s32 *)((rec) + 108))

struct EffectRec {
    u8 pad[9];
    u8 f0 : 2;
    u8 f1 : 2;
};

struct EffectWork {
    u8 pad[0x50];
    struct EffectRec *rec;
};

/*
 * resource_39e spawn/copy owner at 0x0200013c.
 *
 * Its complete 472-byte extent is the high-register body through the
 * three-word pool immediately before the independent owner at 0x02000314.
 * This is the locally witnessed configurable spawn/copy family: its only
 * 39e-specific identities are the table at 0x0200c62c, the ten in-image
 * call veneers below, and the installed callback 0x02008105.
 */
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

struct SceneRecordHeading {
    u8 pad[6];
    u16 heading;
};

struct SceneActor {
    u8 unknown_00[6];
    u16 facing;
};

struct Params {
    s32 unused0;
    s32 field1;
    s32 field2;
    s32 field3;
    u8 pad[24];
};

struct Descriptor {
    u32 field0;
    u8 unused[12];
    u32 field16;
    u32 field20;
    u16 field24;
    u16 unused26;
    u32 field28;
    u8 unused32[8];
};

struct Descriptor2 {
    u32 field0;
    u32 field4;
    u8 unused8[16];
    u16 field24;
    u8 unused26[6];
    u8 unused32[8];
};

extern struct Descriptor *gOv[];
extern s16 gCell[];
extern u8 Value_0000003c;
extern u8 gOv2[];
extern u8 gOv3[];
extern u8 gOv4[];
extern u8 gOv5[];
extern u8 gOv6[];
extern u8 gOv7[];
extern s32 gIw;
extern u8 gVal[];

void *AcquireOverlayObject(s32, s32, s32, s32);

void *Map_unk12_4(s32, s32, s32, s32);

struct Effect *Map_unk13_4();
struct Effect *Map_unk14_4();

void *Map_unk15_4(s32);

u8 *ClearSceneExitGateAtEntry(s32 slot);
u8 *GetSceneExitPendingWork(s32 slot);
u8 *ResetSceneExitPendingWork(s32 slot);
u8 *SetSceneExitCompletionMode(s32 slot);
u8 *ClearSceneExitField40(s32 slot);
u8 *SetSceneExitGate(s32 slot);
u8 *SetSceneExitHeading(s32 slot);
u8 *ClearSceneExitGateBeforeDescriptor(s32 slot);

u8 *Map_unk16_4();

u8 *Map_unk17_4(s32 id);

u8 *Map_unk18_4(s32 slot);
u8 *Map_unk19_4(s32 slot);

struct SceneActor *GetSceneRecord(s32 slot);

u8 *Map_unk20_4();

u8 *Map_unk21_4();

u8 *Map_unk22_4(s32 arg0);

u8 *Map_unk23_4(s32 id);

u8 *Map_unk24_4(s32 id);
u8 *Map_unk25_4(s32 id);
u8 *Map_unk26_4(s32 id);
u8 *Map_unk27_4(s32 id);
u8 *Map_unk28_4(s32 id);

u8 *Map_Far5();

u8 *Scene_GetRecord_1();

u8 *Scene_GetRecord_2();

u8 *Scene_GetRecord_3();

u8 *Scene_GetRecord_4();

u8 *Map_unk29_4();
u8 *Scene_GetRecord_5();
u8 *Battle_WaitMode0_88();

/*
 * resource_39e owner at 0x02001494, 2236 bytes.
 *
 * Second ensemble beat of this overlay's scene script: 233 calls into the
 * shared scene-script helper block, then a two-stage story branch.
 *
 *   - the opening call sets story flag 0x89a;
 *   - stage 1 (Map_unk30_4(0, 0) == 0) joins with id 16 and sets flag 0x898
 *     -- the flag the dispatcher at 0x020012e0 tests and 0x02001dbc clears;
 *   - stage 1 otherwise bumps the step counter at +472 of the scene work
 *     record and runs stage 2: its ==0 arm joins with id 18 and also sets
 *     0x898, its else arm bumps that counter again and sets flag 0x899.
 *
 * Both join arms share the tail at 0x02001cb4: Map_unk31_4(id, 0, 20) with
 * the id carried in r0, then Map_unk32_4(0x898).
 *
 * Midway the beat clears then sets bit 0 of byte +90 of record 16 around a
 * Map_unk33_4 reposition, plays sounds 158/159 with Map_unk34_4 text
 * calls, runs the slot-19/20 sequence (Map_unk35_4(19/20, 232 << 16,
 * 168 << 16), record +12 = 0xc0000, +60 = 0x80000000, +24 = 0xcccc, +30 of
 * the +80 sub-object = 0x8000, sound 124) that also closes the owners at
 * 0x02001160 and 0x02001dbc, and ends with Map_unk36_4(12, 0x0200c638) as
 * 0x02001dbc does.
 *
 * Complete owner: `push {r5, lr}` at 0x02001494 through `pop {r5} / pop {r0} /
 * bx r0` at 0x02001d42-0x02001d46, two inline pools plus two trailing pool
 * words ending 0x02001d4f; the next owner (0x02001d50, `push {lr}`) follows.
 * Called once from within this overlay.
 *
 * Call binding. This overlay reaches the shared helpers through its own veneer
 * bank, so every emitted call word names a veneer, not the main-image address.
 * The legacy alias a reference site produces is derived from that site's own
 * position, so one alias can name two different veneers and several aliases
 * can name one veneer. Each helper is therefore declared once, under an alias
 * whose reference sites all resolve to that helper's veneer, and is spelled in
 * the body under the main-image function it reaches. The bound veneer -- and
 * so the emitted call word -- depends only on the alias, never on where the
 * call lands in the candidate.
 *
 * Two call forms in the branch tail carry meaning beyond their arguments. The
 * three sites spelled through Value2 keep the callee's result register live
 * across the argument setup, which orders the r0 write after r1; the sites
 * spelled through Call2 discard it and order r0 first. Both forms appear in
 * the sibling beat at 0x02001dbc.
 *
 * The set of bit 0 in byte +90 reads the byte into its own local before the
 * or, as the staged-actor scene at resource_373:02003fb0 does at the same
 * offset; the paired clear needs no local because an and against 0xfe already
 * holds its mask in a word register.
 *
 * Uncertainty: helper roles and argument roles are read from call shape only;
 * ids, dialogue ids and constants are transcribed. 0x0200c77a, 0x0200c790 and
 * 0x0200c638 are overlay-image data pointers whose contents are unidentified.
 */

/* Veneer aliases this overlay binds; see Call binding above. */

/* The main-image helper each veneer above reaches. */

static __inline__ void SetScale(s32 actor, s32 horizontal, s32 vertical)
{
    Map_unk24_3(actor, horizontal, vertical);
}

static __inline__ void SetPosition(s32 actor, s32 horizontal, s32 vertical)
{
    Map_unk25_3(actor, horizontal, vertical);
}

/* Field cleared to 0 on each actor record below. */

/* Newly named raw call sites: each engine function below was reached only
 * through an unresolved veneer island until its own source was read; the
 * per-site macro keeps the site's original calling form. */

static __inline__ u8 *Record1(u8 *(*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Scene_BumpStep(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 472) = (u16)(*(u16 *)(work + 472) + amount);
}

void SetEffectMode(struct EffectWork *work, u32 mode)
{
    work->rec->f1 = mode;
}

void *NewEffectObject(s32 first, s32 second, s32 third, s32 fourth)
{
    void *overlay_object;
    void *object_record;
    s32 flags_mask;

    overlay_object = AcquireOverlayObject(fourth, first, second, third);
    if (overlay_object != NULL) {
        object_record = FIELD_AT_OFFSET(overlay_object, void *, 0x50);
        flags_mask = -0xD;
        FIELD_AT_OFFSET(object_record, u8, 9) = (u8)(flags_mask & FIELD_AT_OFFSET(object_record, u8, 9));
        FIELD_AT_OFFSET(overlay_object, u8, 0x55) = 0;
        FIELD_AT_OFFSET(overlay_object, u8, 0x59) = 8;
        RunOverlayObjectCommand0(overlay_object, 0);
        RunOverlayObjectCommand14(overlay_object, 0xE);
        RunOverlayObjectCommand1(overlay_object, 1);
        return overlay_object;
    }
    return NULL;
}

void *NewFlippedEffectObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    u8 *result = Map_unk12_4(arg3, arg0, arg1, arg2);

    if (result != NULL) {
        u8 *object = *(u8 **)(result + 0x50);
        s32 flags;
        s32 mask = 13;

        flags = object[9];
        mask = -mask;
        mask &= flags;
        mask |= 4;
        object[9] = mask;
        result[0x55] = 0;
        result[0x59] = 8;
        Map_Apply(result, 0);
        Map_Apply2(result, 15);
        result[0x23] = (result[0x23] & 0xfe) | 2;
        return result;
    }
    return NULL;
}

void SpawnEffect(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz, u32 flags,
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

    party = Map_unk13_4(0);

    if ((flags & 0x100000) != 0 && options != 0) {
        effect = Map_unk14_4(options->kind, x, y, z);
    } else {
        effect = Map_unk14_4(222, x, y, z);
    }
    if (effect == 0) return;

    block = effect->sprite;
    mode_block = block;

    Map_unk37_4(effect, (flags + 1) & 15);
    table_offset = (flags & 15) << 2;
    Map_unk38_4(effect, gOv[table_offset >> 2]);

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
        Map_unk39_4(effect, options->mode);
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
            effect->rate30 = Map_unk11(first_delta, descriptor->duration);
            delta = options->target34;
            duration = descriptor->duration;
            delta -= effect->accum1c;
        } else {
            first_delta = options->target30;
            first_delta += (s32)0xffff0000;
            effect->rate30 = Map_unk12(first_delta, descriptor->duration);
            delta = options->target34;
            duration = descriptor->duration;
            delta += (s32)0xffff0000;
        }
        effect->rate34 = Map_unk13(delta, duration);
    }

    if ((flags & 0x200000) != 0) {
        Map_unk40_4(effect, 1);
        Map_unk41_4(effect, options->callback_arg);
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

s32 StopXianActor(void *actor)
{
    Map_Apply3(actor, 0);
    return 0;
}

s32 FaceXianActorToPlayer(void *actor)
{
    void *player = Map_unk15_4(0);
    FIELD(actor, u16, 6) = Map_Apply4(FIELD(player, s32, 0x10) - FIELD(actor, s32, 0x10), FIELD(player, s32, 8) - FIELD(actor, s32, 8));
    return 0;
}

s32 GetXianScriptData(void)
{
    if (gCell[224] == (s32)&Value_0000003c) {
        return (s32)gOv2;
    }
    return (s32)gOv3;
}

s32 GetXianInitialState(void)
{
    return 0;
}

s32 GetXianMessageData(void)
{
    return (s32)gOv4;
}

void Scene_RunScene39e(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Map_unk42_4();
    Map_Do(0x178a);
    if (Map_Check(0x890) != 0) {
        bump_step(4);
    }
    Map_unk2(8, 0);
    if (Map_unk3(0, 0) == 0) {
        Map_unk2_2(0x890);
    } else {
        bump_step(1);
    }
    Map_unk43_4(8, 0);
    Map_unk44_4();
}

void Scene_RunFlag88FBranch(void)
{
    extern u8 *gWork;

    Map_unk45_4();
    if (Map_unk14(0x88F) != 0) {
        Map_unk13_2(0x17D6);
        Map_Apply5(12, 0);
        Map_unk46_4();
    } else {
        Map_unk14_2(0x1794);
        Map_Apply6(12, 0);
        if (Map_Apply7(0, 0) == 1) {
            u16 *q = (u16 *)(gWork + 0x1D8);
            q[0] = q[0] + 1;
            Map_Apply8(12, 0);
            if (Map_Apply9(0, 0) == 1) {
                u16 *r = (u16 *)(gWork + 0x1D8);
                r[0] = r[0] + 1;
            }
        }
        Map_Apply10(12, 0);
        Map_unk47_4();
    }
}

void Scene_ShowDialogue17B1(void)
{
    extern u8 *gWork;

    Map_unk48_4();
    Map_unk15_2(0x17B1);
    Map_Apply11(8, 0);
    Map_unk49_4();
}

void Scene_ShowDialogue1825(void)
{
    extern u8 *gWork;

    Map_unk50_4();
    Map_unk16_2(0x1825);
    Map_Apply12(9, 0);
    Map_unk51_4();
}

void Scene_RunRoofSceneExit(void)
{
    extern u8 *gWork;

    OpenSceneExit();

    ClearSceneExitGateAtEntry(12)[91] = 0;

    goto testPendingWork;
waitPendingWork:
        WaitSceneExitStep(1);
testPendingWork:
    if (*(s32 *)(GetSceneExitPendingWork(12) + 12) > 0) {
        goto waitPendingWork;
    }

    *(s32 *)(ResetSceneExitPendingWork(12) + 12) = 0;

    *(s32 *)(SetSceneExitCompletionMode(12) + 60) = 128 << 24;

    *(s32 *)(ClearSceneExitField40(12) + 40) = 0;

    SetSceneExitGate(12)[91] = 1;

    TransitionSceneExitSlot(12, 0, 0);

    if (IsFlag0895Set(0x895) != 0) {
        ShowSceneExitDialogue1a5b(0x1a5b);
    } else if (IsFlag089bSet(0x89b) != 0) {
        ShowSceneExitDialogue189e(0x189e);
    } else {
        ShowSceneExitDialogue182a(0x182a);
    }

    FinalizeSceneExitSlot(12, 0);

    ((struct SceneRecordHeading *)SetSceneExitHeading(12))->heading = 128 << 7;

    ClearSceneExitGateBeforeDescriptor(12)[91] = 0;

    InstallSceneExitDescriptor(12, (u8 *)0x0200c638);
    CloseSceneExit();
}

void Scene_ShowDialogue182D(void)
{
    extern u8 *gWork;

    Map_unk52_4();
    Map_unk17_2(0x182d);
    Map_Apply13(15, 0);
    Map_unk53_4();
}

void Scene_RunForwardArcBurst(void)
{
    extern u8 *gWork;

    u8 *record = Map_unk16_4(19);
    u32 index;
    s32 angle;

    for (index = 8; index > 3; index--) {
        angle = index << 12;
        *(u16 *)(*(u8 **)(record + 80) + 30) = (u16)angle;
        Map_unk54_4((index - 4) * 2);
        *(s32 *)(record + 8) += Map_unk15(angle)* 6;
        *(s32 *)(record + 16) += Map_unk16(angle)* 6;
    }

    *(s32 *)(record + 12) = 0x120000;
    *(s32 *)(record + 60) = 0x120000;

    Map_unk55_4(227);

    Map_unk56_4(*(s32 *)(record + 8) - 0xc0000,
                  *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + 0x80000,
                  0xffffcccd, 0x6666, 0, 0, 0);
    Map_unk57_4(*(s32 *)(record + 8),
                  *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + 0x80000,
                  0xffff3334, 0x4ccc, 0, 0, 0);
    Map_unk58_4(*(s32 *)(record + 8) + 0xa0000,
                  *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + 0x80000,
                  0xffff0000, 0x3333, 0, 0, 0);
}

void Scene_RunDescentBurst(void)
{
    extern u8 *gWork;

    u8 *record = Map_unk17_4(19);
    u32 i = 0;
    s32 step = 8;
    s32 zero;
    do {
        Map_unk18_2(step);
        *(s32 *)(record + 16) += 0xffff0000;
        *(u32 *)(record + 64) = 0x80000000;
        i++;
        step -= 2;
    } while (i <= 3);
    zero = 0;
    *(u16 *)(*(u8 **)(record + 80) + 30) = (u16)zero;
    Map_unk19_2(227);
    Map_SetRect(*(s32 *)(record + 8), *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + 0xfff80000, 0xffff3334,
                  0, 0xffffcccd, 0, 0);
    Map_unk2_5(*(s32 *)(record + 8), *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + 0xfff80000, 0x0000cccc,
                  0, 0xffffcccd, 0, 0);
    Map_unk3_5(*(s32 *)(record + 8) + 0xfffa0000, *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + (160 << 12), 0x00003333,
                  0, 0xffff0000, 0, 0);
    Map_unk4_5(*(s32 *)(record + 8) + (192 << 11), *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + (160 << 12), 0x00003333,
                  0, 0xffff0000, 0, 0);
}

void Scene_DispatchApproachByFacing(void)
{
    extern u8 *gWork;

    Map_unk59_4();

    if (*(u16 *)(Map_unk18_4(0) + 6) > (128 << 7)
        && *(u16 *)(Map_unk19_4(0) + 6) < (192 << 8)) {
        Map_unk60_4();
    } else {
        Map_unk61_4();
    }

    if (Map_unk17(0x898) != 0) {
        Map_unk62_4();
    } else {
        Map_unk20_2(0);
    }

    Map_unk63_4();
}

void Scene_DispatchByFacing(void)
{
    extern u8 *gWork;

    struct SceneActor *record = GetSceneRecord(0);
    u16 angle;

    OpenSceneSection();
    SetSceneActorMode(0, 8);
    AdvanceSceneStep(20);

    angle = *(u16 *)((u8 *)record + 6);

    if ((u16)(angle - 0x2000) <= 0x3fffu) {
        RunFacingVariantA();
    } else if ((u16)(angle - 0x6000) <= 0x3fffu) {
        RunFacingVariantB();
    } else if ((u16)(angle + (192 << 7)) <= 0x3fffu) {
        RunFacingVariantC();
    } else {
        RunFacingVariantD();
    }

    SetSceneActorModeAfterBranch(0, 1);
    RunSceneVariant(1);
    CloseSceneSection();
}

void Scene_DispatchByFacingAndFlags(void)
{
    extern u8 *gWork;

    u8 *record = Map_unk20_4(0);
    u16 facing;

    Map_unk64_4();

    facing = *(u16 *)(record + 6);
    if ((u16)(facing - 0x2000) <= 0x3fff) {
        Map_unk65_4();
    } else if ((u16)(facing - 0x6000) <= 0x3fff) {
        Map_unk66_4();
    } else if ((u16)(facing + 0x6000) <= 0x3fff) {
        Map_unk67_4();
    } else {
        Map_unk68_4();
    }

    Map_unk69_4(0x10000, 0x2000);
    Map_unk70_4(20, 1);
    Map_unk71_4();

    if (*(s16 *)(record + 18) <= 209) {
        if (Map_unk18(0x89a) == 0) goto scene0;
        if (Map_unk19(0x89b) != 0) goto scene0;
        goto scene1;
scene0:
        Map_unk72_4(0);
        goto firstSceneComplete;
scene1:
        Map_unk73_4();
firstSceneComplete:
        Map_unk74_4();
        return;
    }

    if (Map_unk20(0x89b) != 0) {
        Map_unk75_4(2);
    } else if (Map_unk21(0x89a) == 0) {
        Map_unk76_4();
    } else {
        Map_unk77_4();
    }
    Map_unk78_4();
}

void Scene_RunSecondEnsembleBeat(void)
{
    s32 id;
    u8 *rec;

    Map_unk3_2(0x89a);
    Map_unk4_2(30);
    Map_Place(13, 0, 0);
    Map_Place(15, 0, 0);
    Map_Place(16, 0, 0);
    Map_unk4_2(20);
    Map_unk2_3(13, 128 << 1, 0);
    Map_unk2_3(15, 128 << 1, 0);
    Map_unk2_3(16, 128 << 1, 0);
    Map_unk4_2(60);
    Map_unk5_2(0x183b);
    Map_unk3_3(13, 0, 20);
    Map_Place(0, 13, 0);
    Map_Run(15, 1);
    Map_unk4_2(20);
    Map_unk3_3(15, 0, 20);
    Map_Place(0, 15, 0);
    Map_Run(16, 2);
    Map_unk4_2(20);
    Map_Place(0, 16, 0);
    Map_unk2_4(16, 0);
    Map_unk4_2(50);
    Map_unk3_4(16, 1);
    Map_unk4_3(16, 0xcccc, 0x6666);
    Map_unk5_3(16, 176, 248);
    Map_unk5_3(16, 154 << 1, 248);
    Map_unk6_3(0, 128 << 6, 0);
    Map_unk6_3(16, 192 << 8, 20);
    Map_unk6_2(158);
    Map_unk7_3(0x0200c77a, 78, 13);
    Map_Run(16, 2);
    Map_unk4_2(20);
    Map_unk4_3(16, 192 << 9, 192 << 8);
    rec = Record1(Map_unk79_4, 16);
    rec[90] &= 0xfe;
    Map_unk5_3(16, 154 << 1, 136 << 1);
    Map_unk4_2(1);
    rec = Record1(Map_unk79_4, 16);
    {
        /*
         * A result temporary, not the compound or-assign the matching
         * &= 0xfe case above uses. The reference writes the result into
         * the mask register rather than the loaded value, and the
         * two-address ORR only does that when the merged result is its
         * own object; the compound form keeps the loaded value as
         * destination. Same technique already adopted in the sibling
         * owner resource_3bd:020013f8.
         */
        u8 merged = (u8)(rec[90] | 1);

        rec[90] = merged;
    }
    Map_unk3_3(16, 0, 50);
    Map_unk8_3(17, 152 << 17, 216 << 16);
    Map_unk5_3(17, 152 << 1, 248);
    Map_Place(9, 17, 0);
    Map_Place(10, 17, 0);
    Map_Place(11, 17, 0);
    Map_Place(12, 17, 0);
    Map_Place(13, 17, 0);
    Map_Place(14, 17, 0);
    Map_Place(15, 17, 0);
    Map_Place(16, 17, 0);
    Map_Place(0, 17, 0);
    Map_unk4_2(10);
    Map_unk4_4(9, 2);
    Map_unk4_4(10, 2);
    Map_unk4_4(11, 2);
    Map_unk4_4(12, 2);
    Map_unk4_4(13, 2);
    Map_unk4_4(14, 2);
    Map_unk4_4(15, 2);
    Map_Run(16, 2);
    Map_unk2_3(17, 0x103, 60);
    Map_unk8_3(18, 152 << 17, 216 << 16);
    Map_unk9_3(18, 152 << 1, 248);
    Map_unk9_3(17, 140 << 1, 132 << 1);
    Map_unk7_2(18);
    Map_unk6_3(18, 160 << 7, 0);
    Map_unk7_2(17);
    Map_unk6_2(159);
    Map_unk7_3(0x0200c790, 78, 13);
    Map_unk3_3(18, 0, 20);
    Map_Place(9, 17, 0);
    Map_Place(10, 17, 0);
    Map_Place(11, 17, 0);
    Map_Place(12, 17, 0);
    Map_Place(13, 17, 0);
    Map_Place(14, 17, 0);
    Map_Place(15, 17, 0);
    Map_Place(16, 17, 0);
    Map_Place(0, 17, 0);
    Map_unk4_2(10);
    Map_Run(17, 2);
    Map_unk4_2(20);
    Map_unk5_4(18, 4);
    Map_unk4_2(20);
    Map_unk3_3(18, 0, 20);
    Map_unk5_4(17, 3);
    Map_unk4_2(20);
    Map_Run(18, 1);
    Map_unk4_2(20);
    Map_unk3_3(18, 0, 20);
    Map_unk6_3(17, 208 << 8, 20);
    Map_unk3_3(17, 0, 20);
    Map_unk2_3(18, 0x102, 60);
    Map_unk3_3(18, 0, 20);
    Map_unk5_4(17, 3);
    Map_unk4_2(20);
    Map_unk3_3(17, 0, 20);
    Map_unk5_4(18, 3);
    Map_unk4_2(20);
    Map_unk3_3(18, 0, 20);
    Map_unk5_4(17, 3);
    Map_unk4_2(20);
    Map_unk5_4(18, 4);
    Map_unk4_2(20);
    Map_unk3_3(18, 0, 20);
    Map_Run(17, 2);
    Map_unk4_2(20);
    Map_unk6_3(17, 0, 20);
    Map_unk3_3(17, 0, 20);
    Map_unk6_3(16, 128 << 8, 20);
    Map_unk5_4(16, 3);
    Map_unk4_2(20);
    Map_unk5_4(17, 3);
    Map_unk4_2(20);
    Map_unk3_3(17, 0, 20);
    Map_unk5_4(16, 3);
    Map_unk4_2(20);
    Map_unk3_3(16, 0, 20);
    Map_unk6_3(17, 128 << 8, 20);
    Map_unk3_3(17, 0, 20);
    Map_unk5_4(9, 3);
    Map_unk4_2(20);
    Map_unk3_3(9, 0, 20);
    Map_unk6_3(17, 208 << 8, 20);
    Map_Run(17, 1);
    Map_unk4_2(20);
    Map_unk3_3(17, 0, 20);
    Map_unk2_3(18, 0x102, 60);
    Map_unk3_3(18, 0, 20);
    Map_unk2_3(17, 0x101, 60);
    Map_unk3_3(17, 0, 20);
    Map_unk5_4(18, 3);
    Map_unk4_2(20);
    Map_unk3_3(18, 0, 20);
    Map_unk2_3(17, 0x100, 60);
    Map_unk3_3(17, 0, 20);
    Map_unk5_4(18, 4);
    Map_unk4_2(20);
    Map_unk2_3(17, 0x103, 60);
    Map_unk3_3(17, 0, 20);
    Map_unk2_3(18, 0x100, 60);
    Map_unk3_3(18, 0, 20);
    Map_unk5_4(17, 4);
    Map_unk4_2(20);
    Map_unk3_3(17, 0, 20);
    Map_Run(18, 2);
    Map_unk4_2(20);
    Map_unk3_3(18, 0, 20);
    Map_Run(17, 2);
    Map_unk4_2(10);
    Map_unk5_3(17, 128 << 1, 140 << 1);
    Map_unk6_3(17, 128 << 7, 20);
    Map_unk8_3(17, 0, 0);
    Map_unk8_2(17);
    Map_unk4_2(30);
    Map_Run(9, 2);
    Map_unk4_2(20);
    Map_unk3_3(9, 0, 20);
    Map_Run(15, 2);
    Map_unk4_2(20);
    Map_unk3_3(15, 0, 20);
    Map_Place(16, 18, 0);
    Map_unk4_2(20);
    Map_Run(16, 2);
    Map_unk4_2(20);
    Map_unk3_3(16, 0, 20);
    Map_Place(18, 16, 0);
    Map_unk4_2(20);
    Map_unk3_3(18, 0, 20);
    Map_unk5_4(18, 4);
    Map_unk4_2(20);
    Map_unk3_3(18, 0, 20);
    Map_unk5_4(18, 3);
    Map_unk4_2(20);
    Map_unk3_3(18, 0, 20);
    Map_unk4_3(18, 0xcccc, 0x6666);
    Map_unk5_3(18, 128 << 1, 248);
    Map_unk6_3(18, 192 << 8, 20);
    Map_unk4_4(18, 1);
    Map_unk2_3(18, 0x100, 60);
    Map_unk5_3(18, 240, 184);
    Map_Run(18, 2);
    Map_unk4_2(20);
    Map_unk8_3(19, 232 << 16, 168 << 16);
    Map_unk8_3(20, 232 << 16, 168 << 16);
    rec = Record1(Map_unk79_4, 19);
    *(s32 *)(rec + 12) = 0xc0000;
    rec = Record1(Map_unk79_4, 19);
    *(s32 *)(rec + 60) = -0x80000000;
    rec = Record1(Map_unk79_4, 19);
    *(s32 *)(rec + 24) = 0xcccc;
    rec = Record1(Map_unk79_4, 19);
    {
        u8 *target = *(u8 **)(rec + 80);
        s32 shown = 0x8000;

        *(u16 *)(target + 30) = shown;
    }
    Map_unk6_2(124);
    Map_unk3_3(18, 0, 20);
    Map_unk6_3(0, 192 << 8, 20);
    Map_unk5_3(16, 128 << 1, 240);
    Map_unk6_3(16, 176 << 8, 20);
    Map_Run(16, 1);
    Map_unk3_3(16, 0, 20);
    Map_Place(9, 0, 0);
    Map_Place(10, 0, 0);
    Map_Place(11, 0, 0);
    Map_Place(12, 0, 0);
    Map_Place(13, 0, 0);
    Map_Place(14, 0, 0);
    Map_Place(15, 0, 0);
    Map_Place(16, 0, 0);
    Map_Run(18, 2);
    Map_unk4_2(20);
    Map_unk6_3(18, 160 << 7, 20);
    Map_unk5_3(18, 248, 208);
    Map_unk6_3(18, 160 << 7, 20);
    Map_unk6_4(18, 0);
    if (Map_unk7_4(0, 0) == 0) {
        Map_unk8_4(16, 1);
        Map_unk4_2(20);
        id = 16;
        goto joinBeat;
    }

    /* Skipped once: bump the workspace skip counter and offer the beat again. */
    Scene_BumpStep(1);
    Map_unk4_2(20);
    Map_unk2_3(18, 0x105, 60);
    Map_unk6_3(18, 128 << 7, 20);
    Map_Run(16, 2);
    Map_unk4_2(20);
    Map_unk6_4(16, 0);
    if (Map_unk7_4(0, 0) != 0) {
        goto skipTwice;
    }
    Map_unk5_4(16, 3);
    Map_unk4_2(20);
    Map_unk6_3(18, 176 << 8, 20);
    id = 18;

joinBeat:
    Map_unk3_3(id, 0, 20);
    Map_unk3_2(0x898);
    goto finish;

skipTwice:
    Scene_BumpStep(1);
    Map_unk4_2(20);
    Map_unk5_4(18, 4);
    Map_unk4_2(20);
    Map_unk3_3(18, 0, 20);
    Map_unk3_2(0x899);

finish:
    Map_unk6_3(10, 128 << 8, 0);
    Map_unk6_3(11, 128 << 8, 20);
    Map_unk9_4(10, 5);
    Map_unk9_4(11, 5);
    Map_unk36_4(12, 0x0200c638);
}

void Scene_RunSkippableStoryBeat(void)
{
    extern u8 *gWork;

    u8 *workspace;

    Map_unk80_4();
    Map_unk21_2(0x186e);
    Map_Apply14(18, 0);

    if (Map_Apply15(0, 0) == 0) {
        Map_unk81_4(20);
        Map_unk26_3(18, 0, 20);
        Map_unk82_3(0x898);
        Map_unk83_3();
    } else {
        workspace = gWork;
        *(u16 *)(workspace + 472) += 1;
        Map_unk27_3(18, 0, 20);
        Map_unk84_3();
    }
}

/* Looks up actors 18, 13, 14, 15 and 16 and clears their +108 field before
 * the scene runs. */
void Scene_RunEnsembleStoryBeat(void)
{
    extern u8 gWork[];

    u32 i;
    s32 actor;

    actor = Scene_GetRecord_1(18);
    ACTOR_FIELD_108(actor) = 0;
    actor = Scene_GetRecord_2(13);
    ACTOR_FIELD_108(actor) = 0;
    actor = Scene_GetRecord_3(14);
    ACTOR_FIELD_108(actor) = 0;
    actor = Scene_GetRecord_4(15);
    ACTOR_FIELD_108(actor) = 0;
    actor = Scene_GetRecord_5(16);
    ACTOR_FIELD_108(actor) = 0;
    Object_SetModeById_1(11, 1);
    Motion_SetSpeedLim_1(0x8000, 0x1000);
    Motion_CamBounds_1(0xe80000, -1, 0xc80000, 1);
    Object_CommitPositionThenWaitIfModeZero_1();
    SceneWork_SetStepValue_1(0x1883);
    Motion_SetSpeed_1(10, 0xcccc, 0x6666);
    Motion_SetSpeed_2(12, 0xcccc, 0x6666);
    Motion_ResetPosMode2_1(10, 152, 200);
    Motion_SetPosReset_1(12, 144, 248);
    Motion_CommitPos_1(10);
    Motion_SetAngleToward_1(9, 19, 0);
    Motion_SetAngleToward_2(11, 19, 0);
    Motion_SetAngleToward_3(13, 19, 0);
    Motion_SetAngleToward_4(14, 19, 0);
    Motion_SetAngleToward_5(15, 19, 0);
    Motion_SetAngleToward_6(16, 19, 0);
    Motion_SetAngleToward_7(18, 19, 0);
    Motion_SetSpeed_3(10, 0x18000, 0xc000);
    Motion_SetSpeed_4(12, 0x20000, 0x10000);
    Motion_ResetPosMode2_2(10, 152, 200);
    Motion_SetPosReset_2(12, 144, 248);
    Motion_SetAngleToward_8(12, 19, 0);
    Motion_CommitPos_2(10);
    Motion_SetAngleToward_9(10, 19, 0);
    Motion_SetVarCbObj_1(18, 2);
    Battle_WaitMode0_1(20);
    Battle_RunThenWaitIfModeZero_1(18, 0, 40);
    Motion_SetAngleToward_10(9, 18, 0);
    Motion_SetAngleToward_11(10, 18, 0);
    Motion_ArmCb_1(11, 0x3000, 0);
    Motion_SetAngleToward_12(12, 18, 0);
    Motion_ArmCb_2(13, 0x3000, 0);
    Motion_SetAngleToward_13(14, 18, 0);
    Motion_SetAngleToward_14(15, 18, 0);
    Motion_SetAngleToward_15(16, 18, 0);
    Battle_WaitMode0_2(20);
    Battle_RunThenWaitIfModeZero_2(16, 0, 20);
    Motion_CallWaitAnim_1(18, 3);
    Battle_WaitMode0_3(20);
    Motion_CallWaitAnim_2(16, 3);
    Battle_WaitMode0_4(20);
    Battle_RunThenWaitIfModeZero_3(16, 0, 20);
    BattleFx_SpawnLinked_1(18, 0x105, 60);
    BattleFx_SpawnLinked_2(16, 0x101, 60);
    Battle_RunThenWaitIfModeZero_4(16, 0, 20);
    Battle_RunThenWaitIfModeZero_5(18, 0, 20);
    BattleFx_SpawnLinked_3(16, 0x102, 60);
    BattleFx_SpawnLinked_4(15, 0x101, 60);
    Motion_SetSpeed_5(15, 0xcccc, 0x6666);
    Motion_SetPosReset_3(15, 216, 176);
    Motion_ArmCb_3(15, 0x3000, 20);
    Battle_RunThenWaitIfModeZero_6(15, 0, 20);
    Motion_ArmCb_4(18, 0xb000, 20);
    Motion_CallWaitAnim_3(18, 4);
    Battle_WaitMode0_5(20);
    Battle_RunThenWaitIfModeZero_7(18, 0, 20);
    Motion_SetVarCb_1(9, 2);
    Motion_SetVarCb_2(10, 2);
    Motion_SetVarCb_3(11, 2);
    Motion_SetVarCb_4(12, 2);
    Motion_SetVarCb_5(13, 2);
    Motion_SetVarCb_6(14, 2);
    Motion_SetVarCb_7(15, 2);
    Motion_SetVarCb_8(16, 2);
    Battle_WaitMode0_6(40);
    Motion_SetVarCbObj_2(13, 2);
    Battle_RunThenWaitIfModeZero_8(13, 0, 20);
    Motion_CallWaitAnim_4(18, 3);
    Battle_WaitMode0_7(20);
    Battle_RunThenWaitIfModeZero_9(18, 0, 20);
    Motion_ArmCb_5(0, 0xe000, 20);
    Motion_ArmCb_6(18, 0x5000, 20);
    BattleEventRuntime_ProcessAction_1(18, 0);
    BattleFx_SpawnLinked_5(9, 0x101, 0);
    Battle_WaitMode0_8(5);
    BattleFx_SpawnLinked_6(10, 0x101, 0);
    Battle_WaitMode0_9(5);
    BattleFx_SpawnLinked_7(11, 0x101, 0);
    Battle_WaitMode0_10(5);
    BattleFx_SpawnLinked_8(12, 0x101, 0);
    Battle_WaitMode0_11(5);
    BattleFx_SpawnLinked_9(13, 0x101, 0);
    Battle_WaitMode0_12(5);
    BattleFx_SpawnLinked_10(14, 0x101, 0);
    Battle_WaitMode0_13(5);
    BattleFx_SpawnLinked_11(15, 0x101, 0);
    Battle_WaitMode0_14(5);
    BattleFx_SpawnLinked_12(16, 0x101, 0);
    Battle_WaitMode0_15(60);
    Motion_SetVarCbObj_3(16, 2);
    Battle_RunThenWaitIfModeZero_10(16, 0, 20);
    Motion_CallWaitAnim_5(18, 3);
    Battle_WaitMode0_16(20);
    Battle_RunThenWaitIfModeZero_11(18, 0, 20);
    BattleFx_SpawnLinked_13(15, 0x101, 60);
    Battle_RunThenWaitIfModeZero_12(15, 0, 20);
    Motion_SetAngleToward_16(18, 15, 0);
    Battle_WaitMode0_17(20);
    Motion_CallWaitAnim_6(18, 4);
    Battle_WaitMode0_18(20);
    Battle_RunThenWaitIfModeZero_13(18, 0, 40);
    Object_LinkPair_1(11, 10, 0);
    Object_LinkPair_2(12, 14, 0);
    Object_LinkPair_3(13, 15, 0);
    Battle_WaitMode0_19(60);
    Motion_SetAngleToward_17(10, 18, 0);
    Motion_SetAngleToward_18(11, 18, 0);
    Motion_SetAngleToward_19(12, 18, 0);
    Motion_SetAngleToward_20(13, 18, 0);
    Motion_SetAngleToward_21(14, 18, 0);
    Motion_SetAngleToward_22(15, 18, 0);
    Battle_WaitMode0_20(20);
    Battle_RunThenWaitIfModeZero_14(18, 0, 20);
    Motion_SetVarCbObj_4(18, 2);
    Battle_WaitMode0_21(20);
    Object_SetModeById_2(9, 3);
    Object_SetModeById_3(10, 3);
    Object_SetModeById_4(11, 3);
    Object_SetModeById_5(12, 3);
    Object_SetModeById_6(13, 3);
    Object_SetModeById_7(14, 3);
    Object_SetModeById_8(15, 3);
    Motion_CallWaitAnim_7(16, 3);
    Battle_WaitMode0_22(20);
    Motion_ArmCb_7(18, 0x5000, 20);
    BattleEventRuntime_ProcessAction_2(18, 0);
    Motion_CallWaitAnim_8(18, 3);
    Battle_WaitMode0_23(20);
    Battle_RunThenWaitIfModeZero_15(18, 0, 20);
    Object_SetModeById_9(0, 3);
    Object_SetModeById_10(9, 3);
    Object_SetModeById_11(10, 3);
    Object_SetModeById_12(11, 3);
    Object_SetModeById_13(12, 3);
    Object_SetModeById_14(13, 3);
    Object_SetModeById_15(14, 3);
    Object_SetModeById_16(15, 3);
    Motion_CallWaitAnim_9(16, 3);
    Battle_WaitMode0_24(20);
    Motion_SetVarCbObj_5(18, 2);
    Battle_WaitMode0_25(20);
    Battle_RunThenWaitIfModeZero_16(18, 0, 20);
    Battle_RunThenWaitIfModeZero_17(18, 0, 20);
    Object_SetModeById_17(0, 3);
    Motion_CallWaitAnim_10(18, 3);
    Battle_WaitMode0_26(20);
    Motion_ArmCb_8(18, 0x8000, 20);
    Motion_SetVarCbObj_6(18, 2);
    Battle_WaitMode0_27(20);
    Battle_RunThenWaitIfModeZero_18(18, 0, 20);
    Object_SetModeById_18(9, 3);
    Object_SetModeById_19(10, 3);
    Object_SetModeById_20(11, 3);
    Object_SetModeById_21(12, 3);
    Object_SetModeById_22(13, 3);
    Object_SetModeById_23(14, 3);
    Object_SetModeById_24(15, 3);
    Motion_CallWaitAnim_11(16, 3);
    Battle_WaitMode0_28(20);
    Motion_ResetPosMode2_3(10, 120, 200);
    Motion_ResetPosMode2_4(12, 120, 248);
    Motion_CommitPos_3(10);
    Motion_ArmCb_9(11, 0x8000, 20);
    Object_SetModeById_25(10, 5);
    Object_SetModeById_26(11, 5);
    Motion_CommitPos_4(12);
    Motion_EnableActCb_1(12, 0x200c638);
    Motion_SetSpeed_6(15, 0xcccc, 0x6666);
    Motion_SetPosReset_4(15, 216, 168);
    Motion_SetPosReset_5(15, 232, 168);
    Motion_ArmCb_10(15, 0xc000, 20);
    Motion_SetVarCbObj_7(15, 3);
    Motion_SetHPosTerrain_1(19, 0xe80000, 0xa80000);
    actor = Scene_GetRecord_6(19);
    *(s32 *)(actor + 12) = 0xc0000;
    actor = Scene_GetRecord_7(19);
    *(s32 *)(actor + 60) = -0x80000000;
    actor = Scene_GetRecord_8(19);
    {
        s32 target = *(s32 *)(actor + 80);
        s32 shown = 0x8000;

        *(u16 *)(target + 30) = shown;
    }
    Audio_PlayCue_1(124);
    Battle_WaitMode0_29(40);
    Motion_SetPosReset_6(15, 216, 152);
    Motion_ArmCb_11(15, 0x4000, 30);
    GameFlag_Clear_1(0x898);
    GameFlag_Set_1(0x89b);
}

void Scene_ShowDialogue1A58(void)
{
    extern u8 *gWork;

    Map_unk85_3();
    Map_unk22_2(0x1a58);
    Map_Apply16(11, 0);
    Map_unk86_3();
}

void StartSchoolDoorEvent(void)
{
    Map_unk87_3();
    if (Map_unk22(2202) == 0 && Map_unk23(2197) == 0) {
        Map_unk88_3(6317, 1);
        Map_unk89_3();
    } else {
        Map_unk90_3(158);
        Map_unk91_3(0x0200c77a, 78, 13);
        SetScale(0, 0x8000, 0x4000);
        SetPosition(0, 306, 248);
        Map_unk92_3(0, 304, 216);
        Map_unk93_3(20);
        Map_unk94_3(4);
        Map_unk95_3();
    }
}

void Scene_DispatchByRange(void)
{
    extern u8 *gWork;

    u8 *record;
    u32 biased;

    record = Map_unk21_4(0);
    biased = *(u16 *)(record + 6);
    Map_unk96_3();

    biased = biased + 0xffff5fff;
    if (biased <= 0x3ffe) {
        Map_unk97_3(13);
    } else {
        Map_unk98_3(0x1a1c);
        Map_unk99_3(13, 0);
    }

    Map_unk100_3();
}

void Scene_ShowDialogue17DF(void)
{
    extern u8 *gWork;

    Map_unk101_3();
    Map_Apply17(8, 2);
    Map_unk23_2(0x17df);
    Map_Apply18(8, 0);
    Map_unk102_3();
}

s32 Scene_SelectData(void)
{
    extern u8 *gWork;

    if (gCell[224] == (s32)&Value_0000003c) {
        return (s32)gOv5;
    }
    if (gCell[225] == 3) {
        return (s32)gOv6;
    }
    return (s32)gOv7;
}

void Scene_SpawnRandomizedParticle(void)
{
    extern u8 *gWork;

    struct Params params;
    u8 *record;
    s32 draw;
    s32 offset;

    record = Map_unk22_4(0);

    params.field1 = 7;
    draw = (u32)(Map_unk24() * 7) >> 16;
    if ((draw & 7) == 0)
        params.field1 = 5;

    params.field2 = 0xb333;
    params.field3 = 0xcccc;

    offset = ((u32)(Map_unk25() * 8) >> 16) * 13107;

    Map_unk5_5(*(s32 *)(record + 8) + ((8 - (gIw & 15)) << 16),
                  *(s32 *)(record + 12) + (192 << 13),
                  *(s32 *)(record + 16),
                  0,
                  -offset,
                  0,
                  144 << 12,
                  (u8 *)&params);

    if ((gIw & 1) != 0)
        Map_Apply19(0, 15);
    else
        Map_Apply20(0, 1);
}

void Scene_ApplyOffset0Neg32(void)
{
    extern u8 *gWork;

    Map_Apply21(0, -32);
}

void Scene_ApplyOffset0Pos32(void)
{
    extern u8 *gWork;

    Map_Apply22(0, 32);
}

void Scene_ApplyOffsetNeg32_0(void)
{
    extern u8 *gWork;

    Map_Apply23(-32, 0);
}

void Scene_RunOpeningAuxiliarySequence(s32 a0, s32 a1)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Map_unk103_3();
    Map_unk10_3(0, 0x28000, 0x14000);
    Map_unk11_3(0, a0, a1);
    Map_unk104_3(0, 4, 0);
    Map_unk105_3(0, 7);
    Map_unk106_3(0);
    Map_unk107_3(0, 6);
    Map_unk108_3();
}

void Scene_RunScene39eSequenceA(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;
    s32 base5_200a5b9;

    Map_unk109_3();
    base5_200a5b9 = (s32)Map_unk110_3;
    Map_unk10_4(base5_200a5b9, 0xc80);
    Map_unk12_3(0, 0x3333, 0x1999);
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c8)) = 60;
    Map_unk111_3();
    Map_unk112_3(154);
    Map_unk113_3(0, 2);
    Map_unk13_3(0, 0, -6);
    Map_unk114_3(0);
    Map_unk115_3(0, 15);
    record = Map_unk26(0);
    Map_unk116_3(record, 0);
    Map_unk117_3(base5_200a5b9);
    Map_unk118_3();
    Map_unk119_3(3);
    Map_unk120_3();
}

void Scene_PlaySound123AndEnable(void)
{
    extern u8 *gWork;

    Map_unk24_2(123);
    Map_unk25_2(1);
}

void Scene_RunScene39e(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Map_unk121_3();
    Map_unk122_3(188);
    Map_unk14_3(0x200c764, 77, 8);
    *(u8 *)(Map_unk27(0) + 85) = 0;
    Map_unk15_3(0, 0xcccc, 0x6666);
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x100;
    Map_unk123_3(0, 2);
    Map_unk16_3(0, 0, -16);
    Map_unk124_3(16);
    Map_unk125_3(2);
    Map_unk126_3();
}

void Scene_RunScene39e(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Map_unk127_3();
    Map_unk17_3(0, 0x8000, 0x4000);
    Map_unk18_3(0, 168, 0x1f8);
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x100;
    Map_unk128_3();
    Map_unk129_3();
    Map_unk130_3(0);
    Map_unk131_3(20);
    Map_unk132_3(8, 2);
    Map_unk11_4(8, 0x102);
    Map_unk133_3(60);
    *(u8 *)(Map_unk28(8) + 91) = 0;
    Map_unk134_3(152);
    record = Map_unk29(8);
    *(volatile s32 *)(record + 40) = 0x80000;
    Map_unk135_3(8, 1);
    Map_unk9_2(0x17be);
    Map_unk136_3(8, 0, 20);
    Map_unk137_3(8, 0, 20);
    Map_unk138_3(0, 3);
    Map_unk139_3(20);
    Map_unk140_3(8, 3);
    Map_unk141_3(20);
    Map_unk142_3(8, 0, 20);
    Map_unk19_3(0, 0x101, 60);
    Map_unk4(8, 0);
    if (Map_unk5(0, 0) == 0) {
        Map_unk143_3(10);
        Map_unk144_3(8, 3);
        Map_unk145_3(20);
        Map_unk146_3(8, 0, 20);
        bump_step(2);
    } else {
        Map_unk147_3(10);
        Map_unk148_3(8, 2);
        Map_unk149_3(20);
        bump_step(1);
        Map_unk150_3(8, 0, 20);
        Map_unk151_3(8, 3);
        Map_unk152_3(20);
        Map_unk153_3(8, 0, 20);
    }
    Map_unk154_3(8, 0, 20);
    Map_unk155_3(8, 2);
    Map_unk156_3(20);
    Map_unk157_3(8, 0, 20);
    Map_unk20_3(0, 0x101, 60);
    Map_unk6(8, 0);
    if (Map_unk7(0, 0) == 1) {
        Map_unk158_3(10);
        Map_unk21_3(8, 0x102, 60);
        Map_unk10_2(0x17c8);
        Map_unk159_3(8, 0);
        L_0200299c:;
        if (Map_unk8(0, 0) == 1) {
            Map_unk160_3(10);
            Map_unk22_3(8, 0x102, 60);
            Map_unk161_3((s32)gVal);
            Map_unk162_3(8, 0);
            goto L_0200299c;
        }
    }
    Map_unk11_2(0x17c9);
    Map_unk163_3(10);
    Map_unk164_3(8, 3);
    Map_unk165_3(20);
    Map_unk9(8, 0);
    if (Map_unk10(0, 0) == 0) {
        Map_unk166_3(10);
        Map_unk167_3(0, 3);
        Map_unk168_3(20);
        Map_unk169_3(8, 0, 20);
        bump_step(1);
    } else {
        Map_unk170_3(10);
        Map_unk171_3(8, 2);
        bump_step(1);
        Map_unk172_3(8, 0, 20);
    }
    Map_unk173_3(8, 3);
    Map_unk174_3(20);
    Map_unk175_3(8, 0, 20);
    Map_unk176_3(8, 2);
    Map_unk177_3(20);
    Map_unk178_3(8, 0, 20);
    Map_unk179_3(0, 3);
    Map_unk180_3(20);
    Map_unk181_3(8, 3);
    Map_unk182_3(20);
    Map_unk183_3(8, 5);
    Map_unk12_2(0x893);
    Map_unk184_3();
}

/* Long scripted sequence: sets up and steps a series of actors (indices 0-3,
 * 8-10, 17, 20) through position, pose, animation, wait, and flag-bit calls,
 * copying a couple of fields between some actors' records along the way. */
void Scene_RunRoofEnsembleSequence(void)
{
    extern u8 gWork[];

    u32 unused;
    u8 *source_record;

    Battle_Reset_1();
    ObjectMotion_SetSpeedParameters_1(0, 0x6666, 0x3333);
    ObjectMotion_ResetAndSetPositionInMode2_1(0, 0x1d8, 0x218);
    BattleRuntime_WaitIfModeZero_1();
    ObjectMotion_SetSpeedParameters_2();
    ObjectMotion_CommitCurrentPositionAndActivate_1(0);
    ObjectMotion_ArmCallback_1(9, 0, 20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(9, 2);
    BattleRuntime_WaitIfModeZero_2(20);
    SceneWork_SetStepValue_1(0x1969);
    BattleRuntime_RunThenWaitIfModeZero_1(9, 0, 20);
    ObjectMotion_ArmCallback_2(0, 0x8000, 20);
    ObjectMotion_CallThenWaitForAnimationChange_1(0, 3);
    BattleRuntime_WaitIfModeZero_3(30);
    ObjectMotion_ArmCallback_3(8, 0x4000, 30);
    ObjectMotion_CallThenWaitForAnimationChange_2(8, 3);
    BattleRuntime_WaitIfModeZero_4(20);
    BattleRuntime_RunThenWaitIfModeZero_2(8, 0, 20);
    ObjectMotion_ArmCallback_4(0, 0xc000, 20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_2(0, 1);
    BattleRuntime_WaitIfModeZero_5(20);
    ObjectMotion_ArmCallback_5(9, 0xd000, 0);
    /* Copy the pair of s32 fields at +8/+16 from actor 0's record (as seen
     * through each accessor) into the matching setter for another actor. */
    source_record = Scene_GetRecord_1(0);
    if (source_record != 0) {
        ObjectMotion_SetHorizontalPositionWithTerrain_1(1, *(s32 *)(source_record + 8), *(s32 *)(source_record + 16));
    }
    source_record = Scene_GetRecord_2(0);
    if (source_record != 0) {
        Motion_SetHPosTerrain_2(2, *(s32 *)(source_record + 8), *(s32 *)(source_record + 16));
    }
    source_record = Scene_GetRecord_3(0);
    if (source_record != 0) {
        Motion_SetHPosTerrain_3(3, *(s32 *)(source_record + 8), *(s32 *)(source_record + 16));
    }
    ObjectMotion_SetSpeedParameters_3(0, 0x8000, 0x4000);
    ObjectMotion_SetSpeedParameters_4(1, 0x8000, 0x4000);
    ObjectMotion_SetSpeedParameters_5(2, 0x8000, 0x4000);
    ObjectMotion_SetSpeedParameters_6(3, 0x8000, 0x4000);
    ObjectMotion_ResetAndSetPositionInMode2_2(0, 0x1d0, 0x1f8);
    ObjectMotion_ResetAndSetPositionInMode2_3(2, 0x1e0, 0x1f8);
    ObjectMotion_ResetAndSetPositionInMode2_4(1, 0x1f0, 0x1f0);
    Motion_ResetPosMode2_5(3, 0x1c0, 0x1f0);
    ObjectMotion_CommitCurrentPositionAndActivate_2(0);
    ObjectMotion_CommitCurrentPositionAndActivate_3(2);
    ObjectMotion_CommitCurrentPositionAndActivate_4(3);
    ObjectMotion_ArmCallback_6(3, 0xe000, 0);
    Motion_CommitPos_5(1);
    ObjectMotion_ArmCallback_7(1, 0xa000, 0);
    BattleRuntime_WaitIfModeZero_6(20);
    BattleEffect_SpawnLinkedResourceObject_1(0, 0x101, 0);
    BattleEffect_SpawnLinkedResourceObject_2(1, 0x101, 0);
    BattleEffect_SpawnLinkedResourceObject_3(2, 0x101, 0);
    BattleEffect_SpawnLinkedResourceObject_4(3, 0x101, 60);
    ObjectMotion_ArmCallback_8(1, 0x5000, 20);
    BattleEventRuntime_ProcessAction_1(1, 0);
    BattleRuntime_WaitIfModeZero_7(20);
    ObjectMotion_CallThenWaitForAnimationChange_3(8, 3);
    BattleRuntime_WaitIfModeZero_8(20);
    BattleRuntime_RunThenWaitIfModeZero_3(8, 0, 20);
    ObjectMotion_ArmCallback_9(1, 0xa000, 20);
    BattleEffect_SpawnLinkedResourceObject_5(3, 0x100, 60);
    BattleRuntime_RunThenWaitIfModeZero_4(3, 0, 20);
    ObjectMotion_CallThenWaitForAnimationChange_4(8, 3);
    BattleRuntime_WaitIfModeZero_9(20);
    BattleRuntime_RunThenWaitIfModeZero_5(8, 0, 20);
    BattleEffect_SpawnLinkedResourceObject_6(2, 0x100, 60);
    BattleRuntime_RunThenWaitIfModeZero_6(2, 0, 20);
    ObjectMotion_CallThenWaitForAnimationChange_5(8, 3);
    ObjectMotion_ArmCallback_10(8, 0x3000, 20);
    BattleRuntime_RunThenWaitIfModeZero_7(8, 0, 20);
    ObjectMotion_ArmCallback_11(0, 0, 0);
    Motion_ArmCb_12(1, 0x6000, 0);
    Motion_ArmCb_13(3, 0x2000, 0);
    ObjectMotion_SetVariantCallback_1(2, 2);
    BattleRuntime_WaitIfModeZero_10(2, 0x102);
    BattleRuntime_WaitIfModeZero_11(60);
    Motion_ArmCb_14(0, 0xc000, 0);
    Motion_ArmCb_15(1, 0xa000, 0);
    Motion_ArmCb_16(3, 0xe000, 0);
    BattleRuntime_WaitIfModeZero_12(20);
    BattleRuntime_RunThenWaitIfModeZero_8(2, 0, 20);
    ObjectMotion_CallThenWaitForAnimationChange_6(8, 3);
    BattleRuntime_WaitIfModeZero_13(20);
    BattleRuntime_RunThenWaitIfModeZero_9(8, 0, 20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_3(2, 2);
    BattleRuntime_WaitIfModeZero_14(20);
    BattleRuntime_RunThenWaitIfModeZero_10(2, 0, 20);
    BattleRuntime_RunThenWaitIfModeZero_11(8, 0, 20);
    Object_LinkPair_1(0, 2, 50);
    Motion_ArmCb_17(0, 0xc000, 0);
    Motion_ArmCb_18(2, 0xc000, 30);
    ObjectMotion_CallThenWaitForAnimationChange_7(2, 3);
    BattleRuntime_WaitIfModeZero_15(20);
    BattleRuntime_RunThenWaitIfModeZero_12(8, 0, 20);
    BattleEffect_SpawnLinkedResourceObject_7(3, 0x101, 60);
    BattleRuntime_RunThenWaitIfModeZero_13(3, 0, 20);
    Motion_ArmCb_19(8, 0x5000, 20);
    BattleRuntime_RunThenWaitIfModeZero_14(8, 0, 20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_4(3, 1);
    BattleRuntime_WaitIfModeZero_16(20);
    ObjectMotion_CallThenWaitForAnimationChange_8(8, 3);
    BattleRuntime_WaitIfModeZero_17(20);
    BattleRuntime_RunThenWaitIfModeZero_15(8, 0, 20);
    Motion_ArmCb_20(0, 0xe000, 0);
    Motion_ArmCb_21(1, 0x6000, 0);
    BattleRuntime_WaitIfModeZero_18(30);
    Motion_ArmCb_22(0, 0xc000, 0);
    Motion_ArmCb_23(1, 0xa000, 20);
    BattleRuntime_RunThenWaitIfModeZero_16(1, 0, 20);
    ObjectMotion_CallThenWaitForAnimationChange_9(8, 4);
    BattleRuntime_WaitIfModeZero_19(20);
    ObjectMotion_SetAngleToward_1(8, 2, 0);
    BattleRuntime_WaitIfModeZero_20(10);
    BattleRuntime_RunThenWaitIfModeZero_17(8, 0, 20);
    ObjectMotion_SetAngleToward_2(0, 2, 0);
    ObjectMotion_SetAngleToward_3(1, 2, 0);
    ObjectMotion_SetAngleToward_4(3, 2, 0);
    BattleEffect_SpawnLinkedResourceObject_8(2, 0x102, 60);
    BattleRuntime_RunThenWaitIfModeZero_18(2, 0, 20);
    ObjectMotion_CallThenWaitForAnimationChange_10(8, 3);
    BattleRuntime_WaitIfModeZero_21(20);
    Battle_RunThenWaitIfModeZero_19(8, 0, 20);
    ObjectMotion_CallThenWaitForAnimationChange_11(2, 3);
    BattleRuntime_WaitIfModeZero_22(20);
    ObjectMotion_SetVariantCallback_2(1, 2);
    BattleRuntime_WaitIfModeZero_23(20);
    Battle_RunThenWaitIfModeZero_20(1, 0, 20);
    BattleEffect_SpawnLinkedResourceObject_9(8, 0x100, 60);
    ObjectMotion_SetAngleToward_5(8, 1, 0);
    BattleRuntime_WaitIfModeZero_24(20);
    Battle_RunThenWaitIfModeZero_21(8, 0, 20);
    BattleEffect_SpawnLinkedResourceObject_10(2, 0x100, 60);
    Battle_RunThenWaitIfModeZero_22(2, 0, 20);
    ObjectMotion_SetAngleToward_6(8, 1, 0);
    Motion_ArmCb_24(0, 0xc000, 0);
    Motion_ArmCb_25(1, 0xa000, 0);
    Motion_ArmCb_26(3, 0xe000, 0);
    BattleRuntime_WaitIfModeZero_25(20);
    Motion_CallWaitAnim_12(8, 3);
    BattleRuntime_WaitIfModeZero_26(20);
    Battle_RunThenWaitIfModeZero_23(8, 0, 20);
    ObjectMotion_SetVariantCallback_3(2, 2);
    BattleRuntime_WaitIfModeZero_27(20);
    Battle_RunThenWaitIfModeZero_24(2, 0, 20);
    Motion_CallWaitAnim_13(8, 3);
    BattleRuntime_WaitIfModeZero_28(20);
    Battle_RunThenWaitIfModeZero_25(8, 0, 20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_5(3, 2);
    BattleRuntime_WaitIfModeZero_29(20);
    Battle_RunThenWaitIfModeZero_26(3, 0, 20);
    Motion_ArmCb_27(8, 0x5000, 20);
    Motion_CallWaitAnim_14(8, 3);
    Battle_WaitMode0_30(20);
    Battle_RunThenWaitIfModeZero_27(8, 0, 20);
    ObjectMotion_SetVariantCallback_4(0, 2);
    ObjectMotion_SetVariantCallback_5(1, 2);
    ObjectMotion_SetVariantCallback_6(2, 2);
    ObjectMotion_SetVariantCallbackAndInvokeObject_6(3, 2);
    Battle_WaitMode0_31(20);
    Motion_CallWaitAnim_15(8, 4);
    Battle_WaitMode0_32(20);
    Battle_RunThenWaitIfModeZero_28(8, 0, 20);
    ObjectMotion_SetVariantCallbackAndInvokeObject_7(8, 2);
    Battle_WaitMode0_33(20);
    Battle_RunThenWaitIfModeZero_29(8, 0, 20);
    Object_SetModeById_1(0, 3);
    Object_SetModeById_2(1, 3);
    Object_SetModeById_3(2, 3);
    Motion_CallWaitAnim_16(3, 3);
    Battle_WaitMode0_34(20);
    Battle_RunThenWaitIfModeZero_30(8, 0, 20);
    BattleEffect_SpawnLinkedResourceObject_11(1, 0x103, 0);
    ObjectMotion_SetVariantCallback_7(1, 2);
    Battle_WaitMode0_35(60);
    Battle_RunThenWaitIfModeZero_31(1, 0, 20);
    Motion_CallWaitAnim_17(8, 4);
    Battle_WaitMode0_36(20);
    Battle_RunThenWaitIfModeZero_32(8, 0, 20);
    BattleEffect_SpawnLinkedResourceObject_12(0, 0x101, 0);
    BattleEffect_SpawnLinkedResourceObject_13(1, 0x101, 60);
    Battle_RunThenWaitIfModeZero_33(1, 0, 20);
    BattleFx_SpawnLinked_14(8, 0x102, 0);
    ObjectMotion_SetVariantCallback_8(8, 1);
    Battle_WaitMode0_37(60);
    Motion_CallWaitAnim_18(8, 4);
    Battle_WaitMode0_38(20);
    Battle_RunThenWaitIfModeZero_34(8, 0, 20);
    Battle_RunThenWaitIfModeZero_35(8, 0, 20);
    Motion_SetVarCb_9(0, 1);
    Motion_SetVarCb_10(1, 1);
    Motion_SetVarCb_11(2, 1);
    Motion_SetVarCbObj_8(3, 1);
    Battle_WaitMode0_39(20);
    Motion_CallWaitAnim_19(8, 4);
    Battle_WaitMode0_40(20);
    Battle_RunThenWaitIfModeZero_36(8, 0, 20);
    BattleFx_SpawnLinked_15(1, 0x101, 60);
    Motion_ArmCb_28(1, 0x5000, 20);
    BattleEventRuntime_ProcessAction_2(1, 0);
    Battle_WaitMode0_41(20);
    Motion_SetVarCbObj_9(2, 1);
    Battle_WaitMode0_42(20);
    Battle_RunThenWaitIfModeZero_37(2, 0, 20);
    Motion_ArmCb_29(1, 0xa000, 20);
    Motion_ArmCb_30(8, 0xc000, 20);
    Battle_RunThenWaitIfModeZero_38(8, 0, 20);
    Object_SetModeById_4(0, 3);
    Object_SetModeById_5(1, 3);
    Object_SetModeById_6(2, 3);
    Motion_CallWaitAnim_20(3, 3);
    Battle_WaitMode0_43(20);
    Motion_ArmCb_31(8, 0x4000, 20);
    Battle_RunThenWaitIfModeZero_39(8, 0, 20);
    Motion_SetVarCb_12(0, 2);
    Motion_SetVarCb_13(1, 2);
    Motion_SetVarCb_14(2, 2);
    Motion_SetVarCbObj_10(3, 2);
    Battle_WaitMode0_44(20);
    Motion_CallWaitAnim_21(8, 4);
    Battle_WaitMode0_45(20);
    Battle_RunThenWaitIfModeZero_40(8, 0, 20);
    BattleFx_SpawnLinked_16(2, 0x101, 60);
    Battle_RunThenWaitIfModeZero_41(2, 0, 20);
    Motion_CallWaitAnim_22(8, 3);
    Battle_WaitMode0_46(20);
    Battle_RunThenWaitIfModeZero_42(8, 0, 20);
    Battle_RunThenWaitIfModeZero_43(8, 0, 20);
    Battle_WaitMode0_47(0, 0x102);
    Battle_WaitMode0_48(1, 0x102);
    Battle_WaitMode0_49(2, 0x102);
    Battle_WaitMode0_50(3, 0x102);
    Battle_WaitMode0_51(60);
    Battle_RunThenWaitIfModeZero_44(3, 0, 20);
    Motion_ArmCb_32(8, 0x5000, 20);
    Battle_WaitMode0_52(20);
    Battle_RunThenWaitIfModeZero_45(8, 0, 20);
    BattleFx_SpawnLinked_17(0, 0x101, 0);
    BattleFx_SpawnLinked_18(1, 0x101, 0);
    BattleFx_SpawnLinked_19(2, 0x101, 0);
    BattleFx_SpawnLinked_20(3, 0x101, 60);
    Motion_SetVarCbObj_11(1, 1);
    Battle_WaitMode0_53(20);
    Battle_RunThenWaitIfModeZero_46(1, 0, 20);
    ObjectMotion_SetAngleToward_7(0, 1, 0);
    ObjectMotion_SetAngleToward_8(2, 1, 0);
    ObjectMotion_SetAngleToward_9(3, 1, 0);
    Battle_WaitMode0_54(20);
    Object_SetModeById_7(0, 3);
    Object_SetModeById_8(2, 3);
    Motion_CallWaitAnim_23(3, 3);
    Battle_WaitMode0_55(60);
    Motion_ArmCb_33(0, 0xc000, 0);
    Motion_ArmCb_34(2, 0xc000, 0);
    Motion_ArmCb_35(3, 0xd000, 0);
    Battle_WaitMode0_56(20);
    BattleEventRuntime_ProcessAction_3(8, 0);
    BattleFx_SpawnLinked_21(0, 0x102, 0);
    BattleFx_SpawnLinked_22(1, 0x102, 0);
    BattleFx_SpawnLinked_23(2, 0x102, 0);
    BattleFx_SpawnLinked_24(3, 0x102, 0);
    Battle_WaitMode0_57(60);
    Motion_ArmCb_36(1, 0x6000, 20);
    BattleEventRuntime_ProcessAction_4(1, 0);
    Battle_WaitMode0_58(20);
    Motion_ArmCb_37(3, 0x2000, 20);
    Battle_RunThenWaitIfModeZero_47(3, 0, 20);
    Motion_ArmCb_38(1, 0xa000, 0);
    Motion_ArmCb_39(3, 0xe000, 30);
    Motion_ArmCb_40(8, 0, 20);
    BattleFx_SpawnLinked_25(8, 0x105, 60);
    BattleFx_SpawnLinked_26(2, 0x101, 60);
    Battle_RunThenWaitIfModeZero_48(2, 0, 20);
    Motion_ArmCb_41(8, 0xc000, 20);
    BattleFx_SpawnLinked_27(8, 0x105, 60);
    Battle_RunThenWaitIfModeZero_49(8, 0, 20);
    BattleFx_SpawnLinked_28(0, 0x101, 0);
    BattleFx_SpawnLinked_29(1, 0x101, 0);
    BattleFx_SpawnLinked_30(2, 0x101, 0);
    BattleFx_SpawnLinked_31(3, 0x101, 60);
    Battle_RunThenWaitIfModeZero_50(8, 0, 20);
    BattleFx_SpawnLinked_32(0, 0x101, 0);
    BattleFx_SpawnLinked_33(1, 0x101, 0);
    BattleFx_SpawnLinked_34(2, 0x101, 0);
    BattleFx_SpawnLinked_35(3, 0x101, 60);
    Battle_RunThenWaitIfModeZero_51(8, 0, 20);
    Object_SetModeById_9(0, 3);
    Object_SetModeById_10(1, 3);
    Object_SetModeById_11(2, 3);
    Motion_CallWaitAnim_24(3, 3);
    Battle_WaitMode0_59(20);
    Motion_SetVarCbObj_12(8, 1);
    Battle_WaitMode0_60(20);
    Battle_RunThenWaitIfModeZero_52(8, 0, 20);
    Motion_ArmCb_42(0, 0, 0);
    Motion_ArmCb_43(1, 0x6000, 0);
    Motion_ArmCb_44(2, 0x8000, 0);
    Motion_ArmCb_45(3, 0x2000, 0);
    Battle_WaitMode0_61(60);
    Motion_ArmCb_46(0, 0xc000, 0);
    Motion_ArmCb_47(1, 0xa000, 0);
    Motion_ArmCb_48(2, 0xc000, 0);
    Motion_ArmCb_49(3, 0xe000, 0);
    Battle_WaitMode0_62(30);
    Motion_ArmCb_50(8, 0x4000, 20);
    Motion_CallWaitAnim_25(8, 4);
    Battle_WaitMode0_63(20);
    Battle_RunThenWaitIfModeZero_53(8, 0, 20);
    Motion_SetHPosTerrain_4(10, 0x1d80000, 0x2600000);
    Battle_RunThenWaitIfModeZero_54(10, 0, 20);
    BattleFx_SpawnLinked_36(0, 0x100, 0);
    BattleFx_SpawnLinked_37(1, 0x100, 0);
    BattleFx_SpawnLinked_38(2, 0x100, 0);
    BattleFx_SpawnLinked_39(3, 0x100, 0);
    BattleFx_SpawnLinked_40(8, 0x100, 0);
    BattleFx_SpawnLinked_41(9, 0x100, 0);
    Battle_WaitMode0_64(60);
    Motion_ArmCb_51(0, 0x4000, 0);
    Motion_ArmCb_52(1, 0x4000, 0);
    Motion_ArmCb_53(2, 0x4000, 0);
    Motion_ArmCb_54(3, 0x4000, 0);
    Motion_ArmCb_55(9, 0, 0);
    Battle_WaitMode0_65(30);
    Motion_SetSpeed_7(10, 0xcccc, 0x6666);
    ObjectMotion_SetPositionAndReset_1(10, 0x1d8, 0x218);
    BattleFx_SpawnLinked_42(8, 0x101, 60);
    Battle_RunThenWaitIfModeZero_55(8, 0, 20);
    Motion_ArmCb_56(2, 0xc000, 40);
    ObjectMotion_SetPositionAndReset_2(2, 0x1e8, 0x200);
    Battle_WaitMode0_66(10);
    Motion_ArmCb_57(2, 0x6000, 20);
    Motion_ArmCb_58(0, 0, 30);
    ObjectMotion_SetPositionAndReset_3(0, 0x1c8, 0x200);
    Battle_WaitMode0_67(10);
    Motion_ArmCb_59(0, 0x2000, 20);
    Motion_SetVarCbObj_13(10, 2);
    Battle_WaitMode0_68(20);
    Battle_RunThenWaitIfModeZero_56(10, 0, 20);
    BattleFx_SpawnLinked_43(8, 0x101, 60);
    Battle_RunThenWaitIfModeZero_57(8, 0, 20);
    Motion_SetVarCb_15(10, 2);
    Battle_WaitMode0_69(10, 0x102);
    Battle_WaitMode0_70(60);
    Battle_RunThenWaitIfModeZero_58(10, 0, 20);
    BattleFx_SpawnLinked_44(8, 0x101, 60);
    Battle_RunThenWaitIfModeZero_59(8, 0, 20);
    BattleFx_SpawnLinked_45(10, 0x100, 60);
    Battle_RunThenWaitIfModeZero_60(10, 0, 20);
    Motion_SetVarCb_16(8, 1);
    BattleFx_SpawnLinked_46(8, 0x100, 60);
    Battle_RunThenWaitIfModeZero_61(8, 0, 20);
    Motion_ArmCb_60(0, 0xe000, 0);
    Motion_ArmCb_61(1, 0xa000, 0);
    Motion_ArmCb_62(2, 0xa000, 0);
    Motion_ArmCb_63(3, 0xe000, 0);
    Battle_WaitMode0_71(40);
    Motion_CallWaitAnim_26(10, 3);
    Battle_WaitMode0_72(20);
    Battle_RunThenWaitIfModeZero_62(10, 0, 20);
    Motion_CallWaitAnim_27(8, 4);
    Battle_WaitMode0_73(20);
    Battle_RunThenWaitIfModeZero_63(8, 0, 20);
    BattleFx_SpawnLinked_47(8, 0x101, 60);
    Battle_RunThenWaitIfModeZero_64(8, 0, 20);
    Motion_SetVarCb_17(17, 1);
    Battle_WaitMode0_74(20);
    Battle_RunThenWaitIfModeZero_65(10, 0, 20);
    BattleFx_SpawnLinked_48(8, 0x105, 60);
    Battle_RunThenWaitIfModeZero_66(8, 0, 20);
    BattleFx_SpawnLinked_49(8, 0x100, 30);
    Battle_RunThenWaitIfModeZero_67(8, 0, 20);
    Object_SetModeById_12(0, 3);
    Battle_WaitMode0_75(2);
    Object_SetModeById_13(2, 3);
    Battle_WaitMode0_76(1);
    Object_SetModeById_14(3, 3);
    Battle_WaitMode0_77(5);
    Motion_CallWaitAnim_28(1, 3);
    ObjectMotion_SetAngleToward_10(8, 0, 0);
    Battle_WaitMode0_78(20);
    Battle_RunThenWaitIfModeZero_68(8, 0, 20);
    Motion_SetSpeed_8(10, 0xcccc, 0x6666);
    ObjectMotion_SetPositionAndReset_4(10, 0x1d8, 0x1f8);
    Battle_WaitMode0_79(20);
    Motion_SetVarCb_18(10, 1);
    Battle_WaitMode0_80(20);
    Battle_RunThenWaitIfModeZero_69(10, 0, 20);
    ObjectMotion_SetAngleToward_11(8, 10, 0);
    Motion_SetSpeed_9(8, 0xcccc, 0x6666);
    Motion_ArmCb_64(0, 0x2000, 0);
    Motion_ArmCb_65(1, 0x6000, 0);
    Motion_ArmCb_66(2, 0x6000, 0);
    Motion_ArmCb_67(3, 0x2000, 0);
    Motion_CommitPos_6(8);
    Battle_WaitMode0_81(30);
    Motion_CallWaitAnim_29(8, 3);
    Battle_WaitMode0_82(20);
    Battle_RunThenWaitIfModeZero_70(8, 0, 20);
    Motion_ResetPosMode2_6(10, 0x1d8, 0x238);
    ObjectMotion_SetPositionAndReset_5(8, 0x1d8, 0x218);
    Motion_SetVarCbObj_14(1, 2);
    Motion_CommitPos_7(10);
    Battle_RunThenWaitIfModeZero_71(1, 0, 20);
    Motion_SetVarCb_19(8, 2);
    BattleFx_SpawnLinked_50(8, 0x102, 60);
    Map_unk23_3(10, 0xd000, 0);
    Motion_ArmCb_68(8, 0xd000, 20);
    Battle_RunThenWaitIfModeZero_72(8, 0, 20);
    Motion_SetVarCb_20(0, 2);
    Motion_SetVarCb_21(1, 2);
    Motion_SetVarCb_22(2, 2);
    Motion_SetVarCbObj_15(3, 2);
    ObjectMotion_SetPositionAndReset_6(8, 0x1d8, 0x200);
    Motion_ArmCb_69(8, 0, 0);
    Motion_ArmCb_70(0, 0, 0);
    Motion_ArmCb_71(2, 0x8000, 0);
    Motion_ArmCb_72(9, 0xd000, 20);
    Motion_CallWaitAnim_30(8, 3);
    Battle_WaitMode0_83(20);
    Battle_RunThenWaitIfModeZero_73(8, 0, 20);
    Motion_CallWaitAnim_31(2, 3);
    Battle_WaitMode0_84(20);
    /* Clear bit 0 of the flag byte at +90. */
    Scene_GetRecord_4(0)[90] &= 0xfe;
    Motion_SetPosReset_7(0, 0x1c0, 0x200);
    Battle_WaitMode0_85(1);
    {
        /* Set bit 0 of the flag byte at +90. */
        u8 *record = Scene_GetRecord_5(0);
        u8 value = *(volatile u8 *)&record[90];

        record[90] = (u8)(value | 1);
    }
    Battle_WaitMode0_86(20);
    Map_unk185_3();
    Battle_WaitMode0_87(60);
    Map_unk186_3(2, 144);
    Motion_CallWaitAnim_32(8, 3);
    Battle_WaitMode0_88(20);
    Battle_RunThenWaitIfModeZero_74(8, 0, 20);
    Object_SetModeById_15(0, 3);
    Object_SetModeById_16(1, 3);
    Object_SetModeById_17(2, 3);
    Motion_CallWaitAnim_33(3, 3);
    Battle_WaitMode0_89(20);
    Motion_ArmCb_73(0, 0x2000, 0);
    Motion_ArmCb_74(1, 0x6000, 0);
    Motion_ArmCb_75(2, 0x6000, 0);
    Motion_ArmCb_76(3, 0x2000, 0);
    Motion_ArmCb_77(9, 0x3000, 0);
    Battle_WaitMode0_90(20);
    Motion_SetPosReset_8(8, 0x1d8, 0x228);
    Motion_SetVarCbObj_16(8, 2);
    Battle_WaitMode0_91(20);
    Motion_ArmCb_78(8, 0xd000, 20);
    Battle_RunThenWaitIfModeZero_75(8, 0, 20);
    Motion_CallWaitAnim_34(8, 3);
    Battle_WaitMode0_92(20);
    Battle_RunThenWaitIfModeZero_76(8, 0, 20);
    Motion_SetSpeed_10(8, 0x8000, 0x4000);
    Motion_SetPosReset_9(8, 0x1e0, 0x21c);
    Battle_RunThenWaitIfModeZero_77(8, 0, 20);
    Motion_SetSpeed_11(8, 0xcccc, 0x6666);
    Motion_SetSpeed_12(9, 0xcccc, 0x6666);
    Motion_ResetPosMode2_7(8, 0x1d8, 0x260);
    Motion_ResetPosMode2_8(9, 0x1d8, 0x220);
    Motion_SetPosReset_10(10, 0x1d8, 0x260);
    Motion_CommitPos_8(9);
    Motion_ResetPosMode2_9(9, 0x1d8, 0x260);
    Motion_SetHPosTerrain_5(10, 0, 0);
    Motion_CommitPos_9(8);
    Motion_SetHPosTerrain_6(8, 0, 0);
    Motion_CommitPos_10(9);
    Motion_SetHPosTerrain_7(9, 0, 0);
    Motion_SetPosReset_11(2, 0x1e8, 0x208);
    BattleFx_SpawnLinked_51(2, 0x101, 60);
    Motion_SetVarCbObj_17(3, 1);
    Battle_WaitMode0_93(20);
    Motion_ArmCb_79(3, 0x2000, 20);
    Battle_RunThenWaitIfModeZero_78(3, 0, 20);
    Motion_ArmCb_80(0, 0, 0);
    Motion_ArmCb_81(2, 0xa000, 20);
    Motion_CallWaitAnim_35(2, 4);
    Battle_WaitMode0_94(20);
    Battle_RunThenWaitIfModeZero_79(2, 0, 20);
    BattleFx_SpawnLinked_52(3, 0x105, 60);
    Motion_SetVarCbObj_18(1, 1);
    Motion_ArmCb_82(1, 0x6000, 20);
    BattleEventRuntime_ProcessAction_5(1, 0);
    Motion_CallWaitAnim_36(0, 3);
    Battle_WaitMode0_95(20);
    Motion_SetVarCbObj_19(3, 2);
    Battle_WaitMode0_96(20);
    Motion_ArmCb_83(0, 0xc000, 0);
    Motion_ArmCb_84(1, 0x8000, 0);
    Battle_RunThenWaitIfModeZero_80(3, 0, 20);
    BattleFx_SpawnLinked_53(0, 0x101, 0);
    BattleFx_SpawnLinked_54(1, 0x101, 60);
    Motion_CallWaitAnim_37(3, 4);
    Battle_WaitMode0_97(20);
    Battle_RunThenWaitIfModeZero_81(3, 0, 20);
    BattleFx_SpawnLinked_55(0, 0x102, 0);
    BattleFx_SpawnLinked_56(1, 0x102, 60);
    Motion_SetSpeed_13(3, 0);
    if (UiWork_WaitThenFinalizeCapacity_1(0, 0) == 0) {
        Battle_WaitMode0_98(20);
        Motion_CallWaitAnim_38(1, 3);
        Battle_WaitMode0_99(20);
        Battle_RunThenWaitIfModeZero_82(1, 0, 20);
        /* Same step counter as bump_step(), incremented inline here. */
        *(u16 *)((*(u8 **)0x03001ebc + 0x1d8)) += 1;
    } else {
        Battle_WaitMode0_100(20);
        /* Same step counter as bump_step(), incremented inline here. */
        *(u16 *)((*(u8 **)0x03001ebc + 0x1d8)) += 1;
        Motion_CallWaitAnim_39(3, 3);
        Battle_WaitMode0_101(20);
        Battle_RunThenWaitIfModeZero_83(3, 0, 20);
    }
    Motion_SetVarCbObj_20(1, 1);
    Battle_RunThenWaitIfModeZero_84(1, 0, 20);
    Object_SetModeById_18(0, 3);
    Motion_CallWaitAnim_40(3, 3);
    Battle_WaitMode0_102(20);
    Motion_ResetPosMode2_10(1, 0x1c0, 0x200);
    Motion_SetPosReset_12(3, 0x1c0, 0x200);
    Motion_SetHPosTerrain_8(3, 0, 0);
    Motion_CommitPos_11(1);
    Motion_SetHPosTerrain_9(1, 0, 0);
    Battle_WaitMode0_103(20);
    ObjectMotion_SetAngleToward_12(0, 2, 20);
    Battle_WaitMode0_104(30);
    ObjectMotion_SetAngleToward_13(2, 0, 20);
    Battle_RunThenWaitIfModeZero_85(2, 0, 20);
    BattleFx_SpawnLinked_57(0, 0x102, 60);
    Motion_SetVarCb_23(2, 2);
    Battle_WaitMode0_105(2, 0x102);
    Battle_WaitMode0_106(60);
    Motion_CallWaitAnim_41(2, 4);
    Battle_WaitMode0_107(20);
    Battle_RunThenWaitIfModeZero_86(2, 0, 20);
    Motion_SetPosReset_13(2, 0x1c0, 0x200);
    Motion_SetHPosTerrain_10(2, 0, 0);
    Battle_SchedShoulder_1();
    GameFlag_Set_1(0x895);
}

void Scene_SetFlag140AndFinishSequence(s32 arg0, s32 arg1)
{
    extern u8 *gWork;

    u8 *globalCtx;

    Map_unk187_3(160 << 1);
    Map_unk188_3(141, 1);
    globalCtx = *(u8 **)0x03001f30;
    Map_unk189_3(arg0, arg1);
    globalCtx[0x23] = 0;
    Map_unk190_3();
    Map_unk191_3(1);
    Map_unk192_3(1);
}

void Scene_FinishSequence(void)
{
    extern u8 *gWork;

    Map_Apply24(0, 1);
    Map_unk26_2(2);
    Map_unk193_3();
}

void Scene_SpawnEightShots(void)
{
    extern u8 *gWork;

    struct Descriptor descriptor;
    u8 *record;
    u32 i;

    record = Map_unk23_4(8);
    descriptor.field0 = 1;
    descriptor.field24 = 0x0119;
    descriptor.field28 = 0x0200d1d8;
    descriptor.field16 = 224 << 10;
    descriptor.field20 = 192 << 9;
    for (i = 0; i <= 7; i++) {
        Map_unk27_2(10);
        if (i & 1) {
            Map_unk28_2(0x82);
        }
        Map_unk6_5(*(s32 *)(record + 8), *(s32 *)(record + 12),
                      *(s32 *)(record + 16) + 0xffe80000, 0,
                      0x9999, 0, 0x00360001, (u8 *)&descriptor);
    }
    Map_unk29_2(60);
}

void Scene_SelectActorModeFromInputBit(s32 arg0)
{
    extern u8 *gWork;

    if ((*(u32 *)0x03001e40 >> 1) & 1) {
        Map_Apply25(arg0, 10);
    } else {
        Map_Apply26(arg0, 9);
    }
}

void Scene_RunParticleRain(void)
{
    extern u8 *gWork;

    struct Descriptor2 descriptor;
    u8 *record;
    u32 i;
    s32 x;
    s32 y;
    s32 scale;

    Map_unk30_2(0x83);
    *(u32 *)(Map_unk24_4(8) + 108) = 0x0200c1c5;
    Map_unk31_2(40);
    Map_Apply27(128 << 9, 0);
    Map_unk194_3(0x205c54, 1);
    Map_unk32_2(60);
    Map_unk33_2(40);
    Map_unk34_2(0x83);
    *(u32 *)(Map_unk25_4(2) + 108) = 0x0200c1c5;
    Map_unk35_2(120);
    record = Map_unk26_4(8);
    descriptor.field0 = 1;
    descriptor.field4 = 2;
    descriptor.field24 = 0x011d;
    for (i = 0; i <= 63; i++) {
        if ((i & 3) == 0) {
            Map_unk36_2(246);
        }
        x = *(s32 *)(record + 8)
            + ((((u32)(Map_unk30() * 3) << 4) >> 16) << 16)
            + 0xfff40000;
        y = *(s32 *)(record + 12)
            + ((((u32)Map_unk31() << 5) >> 16) << 16)
            + 0xfff00000;
        scale = (((u32)((u32)Map_unk32() << 2) >> 16) << 15) + (128 << 8);
        Map_unk7_5(x, y, *(s32 *)(record + 16), 0,
                      scale, 0, 152 << 13, (u8 *)&descriptor);
        Map_unk37_2(2);
    }
    Map_unk38_2(220);
    Map_unk39_2(30);
    Map_Apply28(128 << 9, 1);
    Map_unk40_2(60);
    Map_unk41_2(40);
    *(u32 *)(Map_unk27_4(8) + 108) = 0;
    *(u32 *)(Map_unk28_4(2) + 108) = 0;
    Map_Apply29(8, 0);
    Map_Apply30(2, 0);
}
