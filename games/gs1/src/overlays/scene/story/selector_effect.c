#include "types.h"
#include "scene.h"
#include "create_configured_overlay_object.h"
#include "create_configured_overlay_object_body.inc"
#include "select_overlay_data_by_runtime_selector.h"
#include "select_overlay_data_by_runtime_selector_body.inc"

/* overlays/scene/story/selector_effect/selector_effect.c */
/* overlays/scene/story/selector_effect/selector_effect.c */
/* overlays/scene/story/selector_effect/actor_motion.c */
#define F(base, type, off) (*(type *)((u8 *)(base) + (off)))

extern u8 Value;

u8 *Story_unk3_4(s32);

void Actor_RunActorZeroHandledMotion(s32 a)
{
    u8 *v = Story_unk3_4(0);
    Story_unk4_4();
    Story_unk5_2(0xe4);
    F(v, s32, 0x6c) = (s32)&Value;
    F(v, s32, 0x30) = 0x3333;
    Story_Apply(0, 2);
    Story_unk14_3(0, 0, -6);
    Story_unk6_2(0);
    Story_Apply2(0, 15);
    Story_Apply3(Story_unk10(0), 0);
    F(v, s32, 0x6c) = 0;
    Story_unk7_2(30);
    Story_unk5_4();
    Story_unk6_4();
    Story_unk8_2(a);
    Story_unk7_4();
}

/* overlays/scene/story/selector_effect/actor_position.c */
#define F(base, type, off) (*(type *)((u8 *)(base) + (off)))

extern u8 gOv[];
extern u8 Value_00000874;

u8 *Story_unk8_4(s32);
u8 *Story_unk9_4(s32);

u8 *Story_unk10_4(s32);
u8 *Story_unk11_4(s32);
u8 *Story_unk12_4(s32);
u8 *Story_unk13_4(s32);

s32 *Story_unk14_4(s32);

void Scene_RunActorElevenAtTile5And13(void)
{
    s32 x;
    s32 y;
    u8 *p;

    x = F(Story_unk8_4(11), s32, 8) / 0x100000;
    y = F(Story_unk9_4(11), s32, 16) / 0x100000;
    Story_unk15_4();
    if (x == 5 && y == 13) {
        F(Story_unk10_4(11), s32, 12) += 0xfffe0000;
        p = Story_unk11_4(11);
        F(p, s32, 0x3c) = F(Story_unk13_4(11), s32, 12);
        Story_unk12_5(5, 2, 5, 11, 1, 1);
        Story_unk9_2(0xd9);
        Story_unk15_3((s32)gOv, 9, 7);
        {
            s32 s0 = 9;
            s32 s1 = 10;
            Story_unk13_5(9, 5, 1, 1, s0, s1);
        }
        Story_unk12_4((s32)&Value_00000874);
    }
    Story_unk16_4();
}

void Actor_UseActorNinePositionWithYOffset(void)
{
    s32 *p = Story_unk14_4(9);
    u32 v = Story_unk17_4();

    s32 b = p[3] + (((v << 2) >> 16) << 16);
    s32 c = p[4];

    Story_unk16_3(p[2], b, c);
}

/* overlays/scene/story/selector_effect/effect.c */
/*
 * resource_39c owner at 0x020051b0, 64 bytes.
 * Advances a frame counter and, while it is still within 31, reseeds the two
 * accumulators from the counter and re-anchors the position on the src
 * record. Returns whether the step ran.
 */
struct Src_39c {
    u8 pad00[8];
    s32 f8;                     /* +8  */
    u8 pad0c[4];
    s32 f16;                    /* +16 */
};

struct Work_39c {
    u8 pad00[8];
    s32 f8;                     /* +8  */
    s32 f12;                    /* +12 */
    s32 f16;                    /* +16 */
    u8 pad14[4];
    s32 f24;                    /* +24 */
    s32 f28;                    /* +28 */
    u8 pad20[68];
    s16 f100;                   /* +100 */
    u8 pad66[2];
    struct Src_39c *f104;       /* +104 */
};

extern u32 gIw;

s32 *Story_unk18_4(s32);

void Effect_SpawnParticleRowsByMode(s32 mode)
{
    s32 buf[10];
    u32 i, j;

    Story_unk14_5(0x70, 0x39, 0x71, 0x2a, 1, 1);
    Story_unk15_5(0x75, 0x3a, 0x70, 0x2e, 1, 1);
    Story_unk16_5(0x75, 0x39, 0x74, 0x2c, 1, 1);
    Story_unk10_2(0x121);
    buf[1] = 5;
    buf[2] = 0x8000;
    buf[3] = 0x8000;
    for (j = 0; j <= 2; j++) {
        for (i = 1; i <= 7; i++) {
            if ((i & 1) != 0) {
                if (mode == 0) {
                    Story_unk17_5((0x319 - ((Story_unk19_4() * 5) >> 16)) << 16, 0,
                                  (((j << 2) + i) << 17) + 0x02b70000, 0,
                                  mode, 0x4000, 0x90000, buf);
                } else if (mode == 1) {
                    Story_unk18_5((((j << 2) + i) << 17) + 0x03120000, 0,
                                  (((((u32 (*)(void))Story_unk20_4)() * 5) >> 16) << 16) + 0x2e80000, 0x4000,
                                  0, 0, 0x90000, buf);
                } else {
                    Story_unk19_5(0x3380000 - (i << 17) - (j << 19), 0,
                                  (((Story_unk21_4() * 5) >> 16) << 16) + 0x2c80000, 0x4000,
                                  0, 0, 0x90000, buf);
                }
                Story_unk11_2(1);
            }
        }
        if (mode == 0)
            Story_unk20_5(0x70, 0x3a, 0x71, j + 43, 1, 1);
        else if (mode == 1)
            Story_unk21_5(0x70, 0x3a, j + 113, 0x2e, mode, mode);
        else
            Story_unk22_5(0x70, 0x3a, 115 - j, 0x2c, 1, 1);
    }
}

void Effect_SpawnParticleRowsAndDrawTiles(void)
{
    s32 buf[10];
    u32 i, j;

    Story_unk23_5(0x4a, 0x3a, 0x46, 0x22, 1, 1);
    buf[1] = 7;
    buf[2] = 0x8000;
    buf[3] = 0x8000;
    for (j = 0; j <= 1; j++) {
        for (i = 0; i <= 7; i++) {
            if ((i & 1) != 0) {
                s32 a = ((Story_unk22_4() << 3) >> 16) * 0x3333 + 0xffff3334;
                s32 b = ((Story_unk23_4() << 3) >> 16) * 0x3333 + 0xffff3334;

                Story_unk24_5(0x690000, 0, ((-i - (j << 4)) << 16) + 0x2200000,
                              a, 0, b, 0x90000, buf);
                Story_unk12_2(1);
            }
        }
        Story_unk25_5(0x4a, 0x3b, 0x46, 34 - j, 1, 1);
        Story_unk26_5(0x4a, 0x3a, 0x46, 33 - j, 1, 1);
    }
}

void Effect_SpawnParticleEveryFourthFrame(void)
{
    s32 buf[10];
    s32 *p = Story_unk18_4(0);
    s32 m = gIw & 3;

    if (m == 0) {
        buf[1] = 7;
        if ((((Story_unk24_4() << 1) >> 16) & 1) == 0)
            buf[1] = 5;
        buf[2] = 0xb333;
        buf[3] = 0xb333;
        {
            s32 y = p[3] + (((Story_unk25_4() << 2) >> 16) << 16);
            s32 a = ((Story_unk26_4() << 3) >> 16) * 0x3333 + 0xffff3334;
            s32 b = ((Story_unk27_4() << 3) >> 16) * 0x3333 + 0xffff3334;

            Story_unk27_5(p[2], y, p[4], a, b, m, 0x90000, buf);
        }
    }
}

void Effect_SpawnRandomEveryFourFrames(s32 x, s32 y, s32 z)
{
    s32 buf[10];
    s32 m = gIw & 3;

    if (m == 0) {
        buf[1] = 7;
        if ((((Story_unk28_4() << 1) >> 16) & 1) == 0)
            buf[1] = 5;
        buf[2] = 0xb333;
        buf[3] = 0xb333;
        {
            s32 a = ((Story_unk29_4() << 3) >> 16) * 0x3333 + 0xffff3334;
            s32 b = ((Story_unk30_4() << 3) >> 16) * 0x3333 + 0xffff3334;

            Story_unk28_5(x, y, z, a, b, m, 0x90000, buf);
        }
    }
}

void Effect_SpawnWithRandomOffset(s32 x, s32 y, s32 z)
{
    s32 buf[10];

    buf[1] = 7;
    buf[0] = 1;
    buf[2] = 0xb333;
    buf[3] = 0xb333;
    {
        s32 a = x + (((Story_unk31_4() << 4) >> 16) << 16) + 0xfff80000;
        s32 b = z + (((Story_unk32_4() << 3) >> 16) << 16) + 0xfffc0000;

        Story_unk29_5(a, y, b, 0, 0, 0, 0xb0000, buf);
    }
}

s32 Effect_AdvanceAnchoredRiseFrame(struct Work_39c *work)
{
    struct Src_39c *src = work->f104;
    s16 step = ++work->f100;
    s32 seed;

    if (step > 31) {
        return 0;
    }

    seed = Story_unk11(step << 10);
    work->f24 = seed;
    work->f28 = seed;
    work->f8 = src->f8;
    work->f12 += 0x10000;
    work->f16 = src->f16;
    return 1;
}

/* overlays/scene/story/selector_effect/effects.c */

struct BattleEffectSprite {
    u8 reserved00[9];
    u8 flags9;
    u8 reserved0a[20];
    u16 angle;
    u8 reserved20[6];
    u8 state26;
};

struct BattleEffect {
    u8 reserved00[24];
    s32 position_x;
    s32 position_y;
    u8 reserved20[3];
    u8 effect_flags;
    u8 reserved24[12];
    s32 velocity_rate_x;
    s32 velocity_rate_y;
    u8 reserved38[12];
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    struct BattleEffectSprite *sprite;
    u8 reserved54;
    u8 motion_mode;
    u8 reserved56[14];
    u16 frame_step;
    u8 reserved66[6];
    u32 callback;
};

struct EffectSpawnOptions {
    u8 mode_bits;
    u8 reserved01[3];
    s32 battle_mode;
    s32 position_x;
    s32 position_y;
    s32 target30;
    s32 target34;
    s16 kind;
    u16 reserved1a;
    s32 callback_arg;
    u16 angle;
    u16 step;
    u32 callback;
};

struct EffectDescriptor {
    s32 reserved00[3];
    s32 duration;
};

extern struct EffectDescriptor *gOv2[];

struct BattleEffect *Story_unk33_4();
struct BattleEffect *Story_unk34_4();

void BattleFx_SpawnConfigured(s32 x, s32 y,
                   s32 z, s32 vx, s32 vy, s32 vz, u32 flags,
                   const struct EffectSpawnOptions *options)
{
    u32 table_offset;
    struct BattleEffect *ref;
    u32 copied_bits;
    s32 flag_mask;
    u32 block_bits;
    struct BattleEffect *effect;
    struct BattleEffectSprite *effect_sprite;
    struct BattleEffectSprite *sprite0;
    u32 option_bits;
    u16 *tag;
    s32 duration;
    s32 first_delta;
    s32 accumulated;
    ref = Story_unk33_4(0);

    if ((flags & 0x100000) != 0 && options != 0) {
        effect = Story_unk34_4(options->kind, x, y, z);
    } else {
        effect = Story_unk34_4(222, x, y, z);
    }
    if (effect == 0) return;

    effect_sprite = effect->sprite;
    sprite0 = effect_sprite;

    Story_unk35_4(effect, (flags + 1) & 15);
    table_offset = (flags & 15) << 2;
    Story_unk36_4(effect, gOv2[table_offset >> 2]);

    effect->motion_mode = 0;
    effect_sprite->state26 = 0;
    effect->callback = 0x02008105;

    effect->velocity_x = vx;
    x = 3;
    effect->velocity_y = vy;
    effect->velocity_z = vz;

    copied_bits = ref->sprite->flags9 & 12;
    block_bits = *(volatile u8 *)&effect_sprite->flags9;
    flag_mask = ~12;
    effect_sprite->flags9 = (u8)((block_bits & flag_mask) | copied_bits);

    effect->velocity_rate_x = 0;
    effect->velocity_rate_y = 0;
    effect->frame_step = 0;
    tag = &effect->frame_step;

    if ((flags & 0xffff0000) == 0 || options == 0) return;

    if ((flags & 0x10000) != 0) {
        Story_unk37_4(effect, options->battle_mode);
    }

    if ((flags & 0x20000) != 0) {
        effect->effect_flags &= 0xfe;
        option_bits = *(const u8 *)options & x;
        effect_sprite->flags9 = (u8)((*((const u8 *)sprite0 + 9) & flag_mask)
                             | (option_bits << 2));
    }

    if ((flags & 0x80000) != 0) {
        effect->position_x = options->position_x;
        effect->position_y = options->position_y;
    }

    if ((flags & 0x40000) != 0) {
        const struct EffectDescriptor *descriptor =
            gOv2[table_offset >> 2];
        s32 delta;

        if ((flags & 0x80000) != 0) {
            first_delta = *(volatile const s32 *)&options->target30;
            accumulated = *(volatile const s32 *)&effect->position_x;
            first_delta -= accumulated;
            effect->velocity_rate_x = Story_unk12(first_delta,
                                           descriptor->duration);
            delta = options->target34;
            duration = descriptor->duration;
            delta -= effect->position_y;
        } else {
            first_delta = options->target30;
            first_delta += (s32)0xffff0000;
            effect->velocity_rate_x = Story_unk13(first_delta,
                                           descriptor->duration);
            delta = options->target34;
            duration = descriptor->duration;
            delta += (s32)0xffff0000;
        }

        effect->velocity_rate_y = Story_unk14(delta, duration);
    }

    if ((flags & 0x200000) != 0) {
        Story_unk38_4(effect, 1);
        Story_unk39_4(effect, options->callback_arg);
    }

    if ((flags & 0x400000) != 0) {
        effect_sprite->angle = options->angle;
    }

    if ((flags & 0x800000) != 0) {
        *tag = options->step;
    }

    if ((flags & 0x1000000) != 0) {
        effect->callback = options->callback;
    }
}

/* overlays/scene/story/selector_effect/field_scene_tail.c */
#define SCENE_WORK (*(u8 **)0x03001ebc)
#define FX16_0_1 0x1999
#define FX16_0_25 0x4000
#define FX16_0_4 0x6666
#define FX16_0_5 0x8000
#define FX16_0_75 0xc000
#define FX16_0_8 0xcccc
#define FX16_1_0 0x10000

extern u8 gVal[];
extern s16 RuntimeSelectorTable[];
extern s16 gCell[][1];

/* Pointer to the shared scene work record. */

/* Values below that look like 16.16 fixed-point fractions, named by that
 * fraction's decimal value. */

static __inline__ void bump_step(s32 amount)
{
    u8 *work = SCENE_WORK;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void Scene_RunFourSteps(void)
{
    Story_unk40_4();
    Story_unk41_4();
    Story_unk42_4();
    Story_unk43_4();
}

void Scene_RunSelSequenceA(void)
{
    extern u8 gWork[];

    u32 i;
    s32 rec7;
    s32 record;

    if (Story_Check(0x256) != 0) {
        Story_unk44_4();
        Story_Do(0x256);
        record = Story_unk15(0);
        *(volatile s32 *)(record + 12) += 0x20000;
        rec7 = Story_unk2(0);
        record = Story_unk3(0);
        *(volatile s32 *)(rec7 + 60) = *(volatile s32 *)(record + 12);
        Story_unk45_4(5);
        Story_SetRect(8, 29, 10, 23, 1, 1);
        Story_unk46_4(217);
        Story_Place(0x200e08e, 10, 18);
        Story_unk47_4();
    }
}

/* Sets up actor 3 and actor 20, branches on the result of a query call
 * against actor 0, then runs a shared closing sequence over both actors. */
void Scene_RunActorThreeBranchSequence(void)
{
    u32 i;
    s32 record;

    Battle_Reset_1();
    Motion_SetSpeed_1(3, FX16_0_8, FX16_0_4);
    Motion_SetSpeed_2(0, FX16_0_8, FX16_0_4);
    SceneWork_SetStepValue_1(0x1577);
    Battle_RunThenWaitIfModeZero_1(3, 0, 20);
    Motion_SetPosReset_1(3, 0x348, 0x288);
    BattleFx_SpawnLinked_1(3, 0x100, 60);
    Motion_ArmCb_1(3, FX16_0_5, 20);
    Object_SetModeById_1(3, 16);
    record = Scene_GetRecord_1(3);
    /* Set the +24 field of actor 3's record to -1.0 in 16.16 fixed point. */
    *(s32 *)(record + 24) = -FX16_1_0;
    Battle_WaitMode0_1(20);
    Battle_RunThenWaitIfModeZero_2(3, 0, 20);
    Object_SetModeById_2(3, 1);
    record = Scene_GetRecord_2(3);
    /* Set the +24 field of actor 3's record to 1.0 in 16.16 fixed point. */
    *(s32 *)(record + 24) = FX16_1_0;
    Battle_WaitMode0_2(20);
    Motion_ArmCb_2(3, FX16_0_25, 20);
    Motion_SetSpeed_3(3, 0);
    if (UiWork_WaitThenFinalizeCapacity_1(0, 0) == 0) {
        Battle_WaitMode0_3(20);
        Motion_CallWaitAnim_1(3, 3);
        Battle_RunThenWaitIfModeZero_3(3, 0, 20);
        *(u16 *)((SCENE_WORK + 0x1d8)) += 1;
    } else {
        *(u16 *)((SCENE_WORK + 0x1d8)) += 1;
        Battle_WaitMode0_4(20);
        Motion_CallWaitAnim_2(3, 4);
        Battle_RunThenWaitIfModeZero_4(3, 0, 20);
    }
    Battle_WaitMode0_5(20);
    Motion_ArmCb_3(3, FX16_0_75, 20);
    Motion_SetSpeedLim_1(FX16_0_8, FX16_0_1);
    Motion_CamBounds_1(0x3480000, -1, 0x2780000, 1);
    Motion_SetPosReset_2(3, 0x348, 0x278);
    Object_CommitPositionThenWaitIfModeZero_1();
    Battle_WaitMode0_6(20);
    Motion_SetVarCbObj_1(3, 2);
    Battle_WaitMode0_7(10);
    Motion_CallWaitAnim_3(3, 4);
    Battle_WaitMode0_8(20);
    Battle_RunThenWaitIfModeZero_5(3, 0, 20);
    GameFlag_Set_1(0x870);
    Battle_SchedShoulder_1();
}

void Scene_RunScriptedSteps0And1576(void)
{
    Story_unk48_4();
    Story_Apply4(0, 1);
    Story_Apply5(0x1576, 1);
    Story_unk49_4();
}

void Scene_RunScriptedSteps0And953(void)
{
    Story_unk50_4();
    Story_Apply6(0, 1);
    Story_Apply7(0x953, 1);
    Story_unk51_4();
}

void Scene_RunFlag881Dialogue(void)
{
    extern u8 *gWork;

    Story_unk52_3();
    Story_Apply8(0, 1);
    if (Story_unk16(0x881) == 0)
        Story_Apply9(0x1636, 1);
    else
        Story_Apply10(0x1635, 1);
    if (Story_unk17(0xb9) != -1) {
        s16 *slot = (s16 *)gWork + 185;
        s32 one = 1;

        *slot = one;
    }
    Story_unk53_3();
}

void Scene_RunActor184Sequence(void)
{
    Story_unk54_3();
    Story_unk13_2(0x53);
    Story_Apply11(0xb8, 3);
    Story_Apply12(0xb9, 0xb8);
    Story_Apply13(Story_unk18(0xb8), 1);
    Story_Apply14(0xb8, 2);
    Story_Apply15(0x1638, 1);
    Story_unk14_2(512);
    Story_unk55_3();
}

void Scene_RunSelSequenceB(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Story_unk56_3();
    if (gCell[224][0] == (s32)gVal) {
        Story_unk2_3(0, 0x1d8, 0x258);
        Story_unk3_3(0, 0x4000, 10);
        Story_Run(0x1d00000, -1, 0x2900000, 1);
        record = Story_unk19(0);
        Story_unk57_3(record, 0);
        record = Story_unk20(0);
        (void)Story_unk21(*(volatile s32 *)(record + 8), 0, 0x2be0000, 223);
        Story_unk2_5(92, 46, 92, 40, 3, 2);
        record = Story_unk22(0);
        *(volatile s32 *)(record + 72) = 0x8000;
        Story_unk58_3(0, 2);
        Story_unk4_3(0, 6, -1);
        *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x203;
        Story_unk59_3(60);
        Story_unk60_3(8);
    } else {
        Story_unk5_3(0, 6, -1);
    }
    Story_unk61_3();
}

void Scene_RunScene39b(void)
{
    extern u8 gWork[];

    u32 i;
    u8 *rec7;
    s32 rec8;
    s32 record;

    rec7 = Story_unk4(0);
    rec8 = Story_unk5(0x109);
    if (rec8 == 0) {
        Story_unk62_3();
        Story_unk2_4(-1, -1, -1, 0);
        rec7[85] = rec8;
        Story_unk6_3(0, (*(s16 *)((s32)rec7 + 10) << 16), ((*(s16 *)((s32)rec7 + 18) << 16) + -0x100000));
        Story_unk63_3(0, 15);
        record = Story_unk23(0);
        Story_unk64_3(record, 0);
        Story_unk65_3();
        Story_unk66_3();
        Story_unk67_3(228);
        *(volatile s32 *)((s32)rec7 + 108) = 0x2008cd1;
        Story_unk7_3(0, 0x6666, 0x3333);
        Story_unk68_3(0, 0, 8);
        Story_unk69_3(0, 0);
        record = Story_unk24(0);
        Story_unk70_3(record, 1);
        {
            u8 *rec9 = (u8 *)*(volatile s32 *)((s32)rec7 + 80);
            s32 mask = -13;
            rec9[9] = (mask & rec9[9]) | 4;
        }
        Story_unk71_3(0, 0, 10);
        rec7[85] = 3;
        *(volatile s32 *)((s32)rec7 + 108) = rec8;
        Story_unk72_3();
        Story_unk73_3();
    }
}

void Scene_RunSel(void)
{
    u8 *p5;
    u8 *addr;
    u8 v;

    p5 = *(volatile s32 *)0x03001f30;
    Story_unk8_3(11, 0x3480000, 0x2580000);
    Story_unk74_3(93, 1);
    Story_unk75_3(3, 11);
    addr = p5 + 0x71c;
    v = *addr | 8;
    *addr = v;
    Story_unk76_3();
    Story_unk77_3(1);
    Story_unk78_3();
}

/* overlays/scene/story/selector_effect/overlay_object.c */
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

void *AcquireOverlayObject(s32, s32, s32, s32);

u8 *Story_unk79_3(s32 kind, s32 arg1, s32 arg2, s32 arg3);

u8 **Story_unk80_3(s32 group, s32 slot);

/*
 * Spawns a kind-24 object from three fields of the caller's object, sets
 * three of its byte fields, then ORs two low bits into its owner record's
 * flag byte -- a sibling routine in this overlay clears those bits instead,
 * and the two are deliberately not unified. The 88-byte owner includes its
 * one pool word, 0x0200de08, which is never dereferenced here and so stays
 * a raw literal. The field offsets are named by position only.
 */

void *OvObj_PrepareSpawnedObject(s32 first, s32 second, s32 third, s32 fourth)
{
    void *obj;
    void *rec;
    s32 mask;

    obj = AcquireOverlayObject(fourth, first, second, third);
    if (obj != NULL) {
        rec = FIELD_AT_OFFSET(obj, void *, 0x50);
        mask = -0xD;
        FIELD_AT_OFFSET(rec, u8, 9) = (u8)(mask & FIELD_AT_OFFSET(rec, u8, 9));
        FIELD_AT_OFFSET(obj, u8, 0x55) = 0;
        FIELD_AT_OFFSET(obj, u8, 0x59) = 8;
        RunOverlayObjectCommand0(obj, 0);
        RunOverlayObjectCommand14(obj, 0xE);
        RunOverlayObjectCommand1(obj, 1);
        return obj;
    }
    return NULL;
}

void *OvObj_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
}

void OvObj_SpawnKind24AtObject(u8 *src)
{
    u8 *obj;
    u8 *rec;

    obj = Story_unk79_3(24, *(s32 *)(src + 8), *(s32 *)(src + 12), *(s32 *)(src + 16));
    if (obj == 0) {
        return;
    }

    rec = *(u8 **)(obj + 0x50);
    Story_Apply16(obj, (void *)0x0200de08);

    obj[0x55] = 0;
    obj[0x22] = 1;
    obj[0x23] = 2;

    if (rec != 0) {
        Story_Apply17(rec, 2);
        rec[0x26] = 0;
        rec[9] |= 0x0C;
    }
}

s32 OvObj_StepScaleUpSixteenFrames(u8 *o)
{
    u16 *c = (u16 *)(o + 100);
    s16 n;

    *c = *c + 1;
    n = (s16)*c;
    if (n > 16) {
        return 0;
    }
    *(s32 *)(o + 24) = (n << 11) + 0x10000;
    *(s32 *)(o + 28) = (n << 11) + 0x10000;
    return 1;
}

/* Release the scene object's optional attachment and clear its slot. */
void OvObj_ReleasePublishedAttachmentB(void)
{
    u8 **slot = Story_unk80_3(35, 4);
    u8 *state;
    u8 *obj;

    if (slot == 0)
        return;

    state = *slot;
    obj = *(u8 **)(state + 20);
    if (obj == 0)
        return;

    Story_unk15_2(obj);
    *(u8 **)(state + 20) = 0;
}

/* overlays/scene/story/selector_effect/palette_scene.c */
/*
 * resource_39c palette-rotation owner at 0x02002648, complete 60-byte span
 * through its four-word pool. Once every eight frames, rotate the eight
 * BGR555 entries at palette indices 40-47 one place toward the front.
 */
void Effect_RotatePaletteEntries40To47(void)
{
    unsigned int index;
    u16 *dst;
    u16 *src;
    u32 front;

    if ((*(volatile u32 *)0x03001e40 & 7) != 0) {
        return;
    }

    dst = (u16 *)0x05000050;
    front = *dst;
    index = 0;
    *(u16 *)0x0500005e = front;

    src = (u16 *)0x05000052;
    while (index <= 6) {
        *dst++ = *src++;
        index++;
    }
}

/* Rotate seven adjacent palette entries once every eight frames. */
void Effect_RotatePaletteEntries97To103(void)
{
    unsigned int i;
    u16 *dst;
    u16 *src;
    u32 first;

    if ((*(volatile u32 *)0x03001e40 & 7) != 0) {
        return;
    }

    dst = (u16 *)0x050000c2;
    first = *dst;
    i = 0;
    *(u16 *)0x050000ce = first;

    src = (u16 *)0x050000c4;
    while (i <= 5) {
        *dst++ = *src++;
        i++;
    }
}

/* overlays/scene/story/selector_effect/run_scene_opening_auxiliary_sequence.c */
extern u8 gWork[];

void Scene_RunOpeningAuxiliarySequence(s32 a0, s32 a1, s32 a2)
{
    u32 i;
    s32 v;

    if (a0 != 0) {
        Story_unk81_3(219);
    }
    i = (u32)a1;
    if (i < (u32)a2) {
        do {
            Story_unk3_5(45 - (s32)(i << 1), 32, 44 - (s32)(i << 1), 32, (s32)(i + 1), 6);
            Story_unk4_5(45 - (s32)i, 51, 45 - (s32)i, 32, 1, 6);
            v = 109 - (s32)i;
            Story_unk5_5(v, 32, 108 - (s32)i, 32, 1, 4);
            Story_unk6_5(v, 51, v, 32, 1, 4);
            if (a0 != 0) {
                Story_unk9_3(0x50000, 0x50000, 0x10000);
                Story_unk10_3(-1, -1, 0xe666);
                Story_unk82_3(a0);
            }
            i = i + 1;
        } while (i < (u32)a2);
    }
    Story_unk7_5(42, 52, 4, 5, 42, 33);
}

/* overlays/scene/story/selector_effect/run_scene_sequence_e.c */

typedef struct {
    s32 f0;
    s32 f1;
    s32 f2;
    s32 f3;
    s32 t0;
    s32 t1;
} SceneEvent;

void Scene_RunSelSequenceE(void)
{
    SceneEvent ev;
    s32 kind;
    s32 shape;
    s32 three;

    Story_unk84_3();
    if (Story_unk25(&ev) != 0) {
        kind = ev.f1;
        if (kind == 8) {
            shape = ev.f2;
            if ((shape >> 20) == 11) {
                Story_unk85_3(ev);
                Story_unk86_3(30);
                Story_unk87_3(211);
                Story_unk88_3();
                three = 3;
                Story_unk89_3(76, 60, 74, 38, three, 1);
                Story_unk90_3(77, 60, 76, 38, 2, 1);
                Story_unk91_3(75, 58, 86, 41, 1, three);
                Story_unk92_3(75, 59, 86, 43, 1, 2);
                Story_unk93_3(76, 59, 80, 49, 2, 1);
                Story_unk94_3(77, 59, 82, 49, 2, 1);
                Story_unk95_3(0x302);
            } else {
                ev.t1 = 0x200b949;
                Story_unk96_3(75, 57, 86, 41, 1, 1);
                Story_unk97_3(71, 59, 86, 42, 1, 1);
                Story_unk98_3(71, 59, 86, 43, 1, 1);
                Story_unk99_3(71, 59, 86, 44, 1, 1);
                Story_unk100_3(71, 59, 80, 49, 1, 1);
                Story_unk101_3(71, 59, 81, 49, 1, 1);
                Story_unk102_3(71, 59, 82, 49, 1, 1);
                Story_unk103_3(78, 58, 83, 49, 1, 1);
                Story_unk104_3(ev);
                Story_unk105_3(0x302);
            }
        } else if (kind == 10) {
            if ((ev.t0 >> 20) == 40) {
                Story_unk106_3(ev);
                if (Story_unk6(0x307) == 0) {
                    Story_unk107_3(0x18000, 0x3000);
                    Story_unk108_3(0x2ca0000, -1, 0x2500000, 1);
                    Story_unk109_3();
                    Story_unk2_2(0x307);
                    Story_unk110_3(5);
                    Story_unk111_3(50);
                } else {
                    Story_unk112_3(5);
                }
                Story_unk113_3(0x306);
            } else if ((ev.t0 >> 20) == 42) {
                ev.t1 = 0x20098f9;
                Story_unk114_3(ev);
                Story_unk115_3(5);
                Story_unk116_3(0x306);
            }
        }
    }
    Story_unk117_3();
}

/* overlays/scene/story/selector_effect/run_scene_sequence_g.c */

void Scene_RunSelSequenceG(s32 a0)
{
    u32 i;
    s32 x;
    s32 y;
    s32 k;
    s32 six;
    s32 pos;
    s32 four;
    Story_unk119_3(219);
    six = 6;
    i = 0;
    x = 41;
    y = 40;
    do {
        Story_unk8_5(y, 32, x, 32, 3 - (s32)i, six);
        k = 2;
        Story_unk9_5(39, 51, y, 32, 1, six);
        pos = (s32)i + 106;
        four = 4;
        Story_unk120_3(105, 51, pos, 32, k, four);
        if (a0 != 0) {
            Story_unk11_3(0x50000, 0x50000, 0x10000);
            Story_unk12_3(-1, -1, 0xe666);
            Story_unk121_3(a0);
        }
        i = i + 1;
        x = x + 2;
        y = y + 2;
    } while (i <= 2);
    Story_unk3_2(0x120);
    Story_unk10_5(106, 33, 4, 5, 42, 33);
    Story_unk122_3();
}

/* overlays/scene/story/selector_effect/scaled_motion.c */
s32 Effect_AdvanceScaleOverSixteenFrames(u8 *o)
{
    u16 *c = (u16 *)(o + 100);
    s16 n;

    *c = *c + 1;
    n = (s16)*c;
    if (n > 16) {
        return 0;
    }
    *(s32 *)(o + 24) = (n * 3) << 10;
    *(s32 *)(o + 28) = (n * 3) << 10;
    return 1;
}

/* overlays/scene/story/selector_effect/scene_data_head.c */
#define PrimaryRuntimeSelector Value_00000039
#define SecondaryRuntimeSelector Value_00000038
#define TertiaryRuntimeSelector Value_00000037

extern u8 Value_00000036;
extern u8 Value_00000037;
extern u8 Value_00000038;
extern u8 Value_00000039;
extern u8 gOv3[];
extern u8 gOv4[];
extern u8 gOv5[];
extern u8 gOv6[];
extern u8 gOv7[];

s32 SceneData_SelectOverlayDataByRuntimeSelector(void)
{
}

/* Complete four-byte leaf: movs r0,#0 followed by bx lr. */
s32 SceneData_ReturnZero(void)
{
    return 0;
}

/* Complete eight-byte literal-address getter, including its sole pool word. */
u8 *SceneData_GetTableE614(void)
{
    return (u8 *)0x0200e614;
}

s32 SceneData_SelectTableByWord224(void)
{
    s16 v = RuntimeSelectorTable[224];

    if (v == (s32)&Value_00000036) {
        return (s32)gOv3;
    }
    if (v == (s32)&Value_00000037) {
        return (s32)gOv4;
    }
    if (v == (s32)&Value_00000038) {
        return (s32)gOv5;
    }
    if (v == (s32)&Value_00000039) {
        return (s32)gOv6;
    }
    return (s32)gOv7;
}

/* overlays/scene/story/selector_effect/scene_data_tail.c */
#define PrimaryRuntimeSelector Value_00000036
#define SecondaryRuntimeSelector Value_00000037
#define TertiaryRuntimeSelector Value_00000038

extern u8 gOv8[];

s32 SceneData_SelectDataByRuntimeSelector(void)
{
}

s32 SceneData_LoadBlockA2c5(void)
{
    s32 n = 0xc80;

    Story_Apply18((s32)gOv8, n);
    return 0;
}

s32 SceneData_ApplyTableA2c5AndReturnZero(void)
{
    Story_unk16_2((s32)gOv8);
    return 0;
}

/* overlays/scene/story/selector_effect/scene_dialogue.c */

/* Complete scene-entry wrapper through return, alignment and its sole pool. */
void Dialogue_RunLine1637(void)
{
    Story_unk123_3();
    Story_unk124_3(0x1637, 1);
    Story_unk125_3();
}

void Dialogue_RunActor3TimedLine(void)
{
    Story_unk126_3();
    Story_Apply19(3, 4);
    Story_unk17_2(20);
    Story_unk18_2(0x157d);
    Story_unk17_3(3, 0, 20);
    Story_unk127_3();
}

/* overlays/scene/story/selector_effect/scene_effect_sequence.c */

void Effect_SpawnRandomizedBurst(s32 x, s32 y, s32 z, s32 w)
{
    s32 desc[10];
    s32 tmp[3];
    u32 i;

    Story_unk19_2(0xd8);
    i = 0;
    do {
        if ((i & 1) != 0) {
            desc[1] = 7;
            if ((i & 2) != 0)
                desc[1] = 5;
            desc[2] = 0x9999;
            desc[3] = 0x9999;
            tmp[0] = 0;
            tmp[1] = 0;
            tmp[2] = 0;
            Story_unk18_3((6 - (i >> 1)) * 0x1999, w, tmp);
            {
                s32 a = x + ((6 - ((Story_unk128_3() * 6) >> 16)) << 16);
                s32 b = z + ((6 - ((Story_unk129_3() * 6) >> 16)) << 16);

                Story_unk30_5(a, y, b, tmp[0], tmp[1], tmp[2], 0x90000, desc);
            }
        }
        Story_unk20_2(2);
        i++;
    } while (i <= 11);
}

/* overlays/scene/story/selector_effect/scene_setup.c */
extern u8 gUnk[];

void Scene_RunFourCallSequence(void)
{
    Story_unk130_3();
    Story_unk131_3();
    Story_unk132_3();
    Story_unk133_3();
}

void Scene_ApplyTable2b20000(void)
{
    Story_unk19_3((s32)gUnk, 0, 0x2480000);
}

void Scene_RunFourStepSequence(void)
{
    Story_unk134_3();
    Story_unk135_3();
    Story_unk136_3();
    Story_unk137_3();
}

/* overlays/scene/story/selector_effect/scene_state_interaction.c */
extern s32 **gIw2;

/*
 * Scene state interaction for resource_39c.  gIw2 is the
 * current-object cell: a pointer read once, then dereferenced to reach the
 * live record.
 */

/*
 * Clear the current record's flag word and, if it has a linked object, reset
 * that object's halfword at +0x64, notify twice and drop the link.  The
 * 72-byte owner includes its three pool words.  Story_Apply20 and
 * Story_Apply21 are one import called twice with very different second
 * arguments; its parameter meaning is unverified, so each call is left as
 * compiled rather than unified.
 */
void State_ClearCurrentRecordAndReleaseTarget(void)
{
    s32 *rec = *gIw2;
    s32 *target;

    if (rec[0] == 0) {
        return;
    }

    rec[0] = 0;
    Story_unk21_2(0x161);

    target = (s32 *)rec[5];
    if (target != 0) {
        *(short *)((u8 *)target + 0x64) = 0;
        Story_Apply20(target, (s32)0x0200de2c);
        Story_Apply21(target, 7);
        rec[5] = 0;
    }
}

/* overlays/scene/story/selector_effect/state_update.c */
extern u8 gOv9[];

s32 *Story_unk138_3(s32);
s32 *Story_unk139_3(s32);

s32 *Story_unk140_3(s32);
s32 *Story_unk141_3(s32);
s32 *Story_unk108_3(s32);

s32 *Story_unk142_3(s32);

/*
 * resource_39c owner at 0x02000cc0, 16 bytes: invoke one fixed two-value
 * setup call with 12 and 21.
 *
 * The executable owner runs from `push {lr}` at 0x02000cc0 through the
 * void interworking return `pop {r0} / bx r0` at 0x02000cca-0x02000ccc.
 * m2c_guard independently measures those 14 code bytes and finds no
 * seeding hazard.  The zero halfword at 0x02000cce is alignment before
 * the next owner's prologue at 0x02000cd0, making the whole span 16 bytes.
 * Its sole call resolves through the overlay veneer to BattleFx_SetPhaseRequest.
 * The constants' game-level meanings are not established by this owner.
 */

void State_ApplyPair12And21(void)
{
    Story_Apply22(12, 21);
}

void State_ApplyWork16cMinus50A(void)
{
    Story_unk22_2(*(s16 *)(gWork + 0x16c) - 50);
}

void State_ApplyWork16cMinus50(void)
{
    Story_unk23_2(*(s16 *)(gWork + 0x16c) - 50);
}

void State_ApplyWork16cMinus50B(void)
{
    Story_unk24_2(*(s16 *)(gWork + 0x16c) - 50);
}

void State_RunWhenActor8AtTile10x23(void)
{
    s32 x = Story_unk138_3(8)[2] / 0x100000;
    s32 y = Story_unk139_3(8)[4] / 0x100000;

    Story_unk144_3();
    if (x == 10 && y == 23) {
        s32 *p;
        Story_unk140_3(8)[3] += 0xfffe0000;
        p = Story_unk141_3(8);
        p[15] = Story_unk108_3(8)[3];
        Story_unk31_5(6, 29, 10, 23, 1, 1);
        Story_unk25_2(0xd9);
        Story_unk20_3((s32)gOv9, 10, 18);
        Story_unk32_5(10, 16, 1, 1, x, 19);
        Story_unk26_2(0x878);
    }
    Story_unk145_3();
}

void State_ApplyRectWhenActor20AtColumn28(void)
{
    s32 col;

    Story_unk146_3();
    col = Story_unk142_3(20)[2] / 0x100000;
    if (col == 28) {
        Story_unk27_2(840);
        {
            s32 a = 31;
            s32 b = 20;

            Story_unk33_5(29, 20, 1, 1, a, b);
        }
    }
    Story_unk147_3();
}

/* overlays/scene/story/selector_effect/story_effect_sequence.c */
typedef struct { s32 unk0; s32 unk4; s32 unk8; } Desc;

typedef struct { u8 filler0[0x28]; s16 *unk28; } Sub;

typedef struct {
    u8 filler0[6];
    u16 unk6;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    u8 filler14[0x10];
    s32 unk24;
    u8 filler28[4];
    s32 unk2C;
    u8 filler30[8];
    s32 unk38;
    u8 filler3C[4];
    s32 unk40;
    u8 filler44[0xC];
    Sub *unk50;
} Ent;

/*
 * resource_39c owner at 0x02004f90, 60 bytes.
 * Two lookups, each of which can fail with -1; on success stores the caller's
 * halfword into the table at +216 of the record the first index names.
 */
struct Rec_395 {
    u8 pad00[216];
    u16 tbl[1];                 /* +216 */
};

extern s32 gOv10[];
extern s32 gOv11[];
extern s32 gOv12[];

Ent *Story_unk148_3(Desc *, Ent *);

struct Rec_395 *Story_unk149_3();

s32 stop_blocked_actor_motion(Ent *a)
{
    Desc d;
    u32 idx;
    s32 m;
    Ent *r;

    idx = a->unk6 >> 12;
    m = gOv10[idx];
    d.unk0 = a->unk8 + (m & 0xffff0000);
    d.unk4 = a->unkC;
    m = m << 16;
    d.unk8 = a->unk10 + m;
    r = Story_unk148_3(&d, a);
    if (r != 0) {
        u32 i = 0;
        s32 v = *r->unk50->unk28;
        s32 *p = gOv11;

        do {
            if (v == *p++) goto done;
            i++;
        } while (i <= 5);
        a->unk24 = 0;
        a->unk2C = 0;
        a->unk38 = 0x80000000;
        a->unk40 = 0x80000000;
    }
    m = gOv10[idx];
    d.unk0 = a->unk8 + (m & 0xffff0000);
    d.unk4 = a->unkC;
    m = m << 16;
    d.unk8 = a->unk10 + m;
    if (Story_EntOp(a, &d) > 0) {
        a->unk24 = 0;
        a->unk2C = 0;
        a->unk38 = 0x80000000;
        a->unk40 = 0x80000000;
    }
done:
    return 0;
}

s32 find_clear_actor_position(s32 *a)
{
    s32 sel;
    s32 buf[3];
    u8 *obj;
    s32 cnt;
    s32 ny, nx;
    u8 *p;
    s32 off;
    s32 t, u;
    u8 *E;
    s32 *b;
    s32 ret;
    a[5] = 0;
    obj = (u8 *)Story_unk21_3(&sel, a + 1, a);
    if (obj == 0)
        return 0;
    p = obj + 0x22;
    *p = 2;
    cnt = 0;
    E = (u8 *)gOv12;
    off = a[0] << 4;
    {
        s32 o = off + 4;
        t = *(s32 *)(E + o);
        if (t < 0)
            t = -t;
        o = off;
        o += 12;
        u = *(s32 *)(E + o);
        if (u < 0)
            u = -u;
        ny = (t + u) >> 4;
        t = *(s32 *)(E + off);
        if (t < 0)
            t = -t;
        o = off;
        o += 8;
        u = *(s32 *)(E + o);
        if (u < 0)
            u = -u;
        nx = (t + u) >> 4;
    }
    {
        u8 *oc;
        s32 w;
        s32 d;
        b = buf;
        d = gOv10[sel] & 0xffff0000;
        oc = obj;
        b[0] = *(s32 *)(oc + 8) + d;
        w = *(s32 *)(oc + 12);
        b[1] = w;
        b[2] = *(s32 *)(oc + 16) + (gOv10[sel] << 16);
        a[3] = w;
    }
    for (;;) {
        s32 j, k;
        {
            u8 *E2 = (u8 *)gOv12;
            s32 o2 = a[0] << 4;
            o2 += 4;
            a[4] = b[2] + (*(s32 *)(E2 + o2) << 16);
        }
        j = 0;
        while (j < ny) {
            {
                u8 *E3 = (u8 *)gOv12;
                a[2] = b[0] + (*(s32 *)(E3 + (a[0] << 4)) << 16);
            }
            k = 0;
            while (k < nx) {
                s32 *w2 = a + 2;
                if (Story_Apply23(obj, w2) == 2)
                    goto found;
                w2[0] = w2[0] + 0x100000;
                k++;
            }
            a[4] = a[4] + 0x100000;
            j++;
        }
        cnt++;
        buf[0] = buf[0] + (gOv10[sel] & 0xffff0000);
        buf[2] = buf[2] + (gOv10[sel] << 16);
    }
found:
    *p = 0;
    ret = 0;
    if (cnt != 0) {
        s32 d = gOv10[sel];
        s32 m = d & 0xffff0000;
        s32 v1 = m *cnt;
        s32 v2 = (d << 16) * cnt;
        u8 *oc = obj;
        a[2] = *(s32 *)(oc + 8) + v1;
        a[3] = *(s32 *)(oc + 12);
        a[4] = *(s32 *)(oc + 16) + v2;
        ret = 1;
    }
    return ret;
}

void Scene_AdvanceSelectedRecord(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    if (Story_unk7(0x256) != 0) {
        Story_unk153_3();
        Story_unk4_2(0x256);
        record = Story_unk26(0);
        *(volatile s32 *)(record + 12) += 0x20000;
        rec7 = Story_unk8(0);
        record = Story_unk9(0);
        *(volatile s32 *)(rec7 + 60) = *(volatile s32 *)(record + 12);
        Story_unk154_3(5);
        Story_unk11_5(7, 2, 5, 11, 1, 1);
        Story_unk155_3(217);
        Story_unk13_3(0x200e03a, 9, 7);
        Story_unk156_3();
    }
}

    if (slot != -1) {
        s32 index = Story_unk27(slot, key);

        if (index != -1) {
            Story_unk149_3(slot)->tbl[index] = value;
        }
    }
}

/* overlays/scene/story/selector_effect/move_and_redraw.c */
#define ValueMoveAndRedraw Value2

#include "staged_actor_movement.h"

void Story_RunMoveAndRedraw(StagedActorMovementRequest request)
{
#include "run_staged_actor_movement_and_redraw_body.inc"
}

/* overlays/scene/story/selector_effect/run_scene_sequence_c.c */
#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

s32 Scene_RunScene39bSequenceC(s32 a0)
{
    s32 value;
    s32 magic;
    s32 base5_3001e40;
    s32 none;
    u8 storage[40];
    u8 *rec = storage;

    FIELD(rec, s32, 4) = 7;
    base5_3001e40 = 0x3001e40;
    if ((*(volatile s32 *)base5_3001e40 & 1) == 0) {
        FIELD(rec, s32, 4) = 5;
    }
    FIELD(rec, s32, 8) = 0xcccc;
    FIELD(rec, s32, 12) = 0xcccc;
    none = 0;
    FIELD(rec, s32, 0) = none;
    value = Story_Check();
    magic = -((((((u32)(value << 3) >> 16) << 1) + ((u32)(value << 3) >> 16)) + (((((u32)(value << 3) >> 16) << 1) + ((u32)(value << 3) >> 16)) << 4)) + ((((((u32)(value << 3) >> 16) << 1) + ((u32)(value << 3) >> 16)) + (((((u32)(value << 3) >> 16) << 1) + ((u32)(value << 3) >> 16)) << 4)) << 8));
    Story_RunSequenceC((*(volatile s32 *)(a0 + 8) + ((8 - (*(volatile s32 *)base5_3001e40 & 15)) << 16)), (*(volatile s32 *)(a0 + 12) + 0x1a0000), *(volatile s32 *)(a0 + 16), none, magic, 0, 0xb0000, rec);
    return 0;
}

/* overlays/scene/story/selector_effect/scene_call_helper_b.c */
/* overlays/scene/story/selector_effect/helper_b.c */
void Scene_CallHelper(void)
{
    FunctionHead_020067e8();
}

/* overlays/scene/story/selector_effect/forward.c */
void Scene_Forward(void)
{
    FunctionHead_0200646c();
}

/* overlays/scene/story/selector_effect/run_single_step.c */
void Scene_RunSingleStep(void)
{
    FunctionHead_020064c0();
}

/* overlays/scene/story/selector_effect/helper.c */
void Scene_CallHelper(void)
{
    FunctionHead_02006364();
}

/* overlays/scene/script/sel_open_step.c */

void Scene_RunSelSelOpenStep(s32 a0)
{
    u32 i;
    s32 record;

    Script_RunSelOpenStep();
    Script_unk2_4SelOpenStep(228);
    Script_Place(0, 0x6666, 0x3333);
    Script_unk3_4SelOpenStep(0, 2);
    Script_unk2_3(0, 0, -8);
    record = Script_Check(0);
    Script_unk4_4SelOpenStep(record, 0);
    Script_unk5_3SelOpenStep(8);
    Script_unk6_3SelOpenStep(0, ((a0 << 19) + 0x80000), 0);
    Script_unk7_3SelOpenStep(30);
}

/* overlays/scene/script/sel_place_step.c */

void Scene_RunSelSelPlaceStep(void)
{
    u32 i;
    s32 record;

    if (Script_Check(0x310) != 0) {
        record = Script_unk2(0x311);
        if (record == 0) {
            goto L_02001140;
        }
        record = Script_unk3(0x312);
        if (record == 0) {
            goto L_02001140;
        }
        Script_Do(0x876);
        Script_RunSelPlaceStep(30);
        Script_Place(0x10000, 0x10000, 0x10000);
        Script_unk2_4SelPlaceStep(141);
        Script_unk3_4SelPlaceStep(60);
        *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x100;
        Script_unk4_4SelPlaceStep();
        Script_unk5_3SelPlaceStep();
        Script_unk2_2(0x121);
        Script_unk2_3(-1, -1, 0xe666);
        Script_unk6_3SelPlaceStep();
        Script_unk7_3SelPlaceStep(13);
    } else {
        L_02001140:;
        Script_unk3_2(0x876);
    }
}

/* overlays/scene/script/sel_cam_step.c */

u8 *Script_RunSelCamStep();

void Scene_RunSelSelCamStep(void)
{
    u8 *p5;

    p5 = *(volatile s32 *)gWork;
    Script_unk2_4SelCamStep();
    Script_Check(0x200ba65, 0xc80);
    Script_Place(0, 0x28000, 0x14000);
    Script_unk3_4SelCamStep(0, 1);
    *(u8 *)(Script_unk2(0) + 90) &= 254;
    Script_unk4_4SelCamStep(228);
    if (*(s16 *)((s32)p5 + 0x16c) == 2) {
        Script_unk2_3(0, 232, (154 << 2));
    } else if (*(s16 *)((s32)p5 + 0x16c) == 3) {
        Script_unk2_3(0, (180 << 1), (182 << 2));
    } else if (*(s16 *)((s32)p5 + 0x16c) == 4) {
        Script_unk2_3(0, 248, (198 << 2));
    } else {
        Script_unk3_3(0, 696, 592);
        Script_unk4_3(0, 696, 600);
        Script_Do(30);
    }
    Script_unk5_3SelCamStep(0);
    {
        u8 *record = Script_RunSelCamStep(0);
        u8 value = *(volatile u8 *)&record[90];

        record[90] = (u8)(value | 1);
    }
    Script_unk2_2(0x200ba65);
    Script_unk6_3SelCamStep();
}

/* overlays/scene/script/sel_close_step.c */
void Scene_RunSelSelCloseStep(void)
{
    s32 rec7;
    s32 record;
    s32 record2;
    s16 v10;
    s16 v18;

    if (Script_Check(0x256) == 0) {
        record = Script_unk2(0);
        v10 = *(s16 *)(record + 10);
        record2 = Script_unk3(0);
        v18 = *(s16 *)(record2 + 18);
        if ((u32)(v10 - 84) <= 7) {
            if (v18 > 211) {
                if (v18 <= 219) {
                    Script_unk6(record2);
                    Script_Do(0x256);
                    Script_RunSelCloseStep(5);
                    record = Script_unk7(0);
                    *(volatile s32 *)(record + 12) += -0x20000;
                    rec7 = Script_unk4(0);
                    record = Script_unk5(0);
                    *(volatile s32 *)(rec7 + 60) = *(volatile s32 *)(record + 12);
                    Script_SetRect(5, 2, 5, 11, 1, 1);
                    Script_unk2_4SelCloseStep(217);
                    Script_Place(0x200e010, 9, 7);
                    Script_unk3_4SelCloseStep();
                }
            }
        }
    }
}

/* overlays/scene/story/selector_effect/run_scene_supplemental_sequence_one.c */
/* overlays/scene/story/selector_effect/run_scene_supplemental_sequence_one.c */

void Scene_RunSupplementalSequenceOne(s32 a0)
{
    s32 *rec;
    s32 outer;
    s32 shift4;
    s32 v8;
    s32 inner;
    s32 neg;
    s32 nsh;
    s32 va0;
    s32 vb0;
    s32 va;
    s32 vb;
    s32 next;
    s32 slot20[10];

    Story_SetRect(78, 59, 110, 36, 1, 1);
    Story_unk2_5(76, 59, 109, 36, 1, 1);
    rec = slot20;
    rec[1] = 7;
    rec[2] = 0x8000;
    rec[3] = 0x8000;
    outer = 0;
    do {
        shift4 = (outer << 4);
        inner = 0;
        nsh = -(outer << 20);
        v8 = (0x2d80000 + nsh);
        do {
            if ((inner & 1) != 0) {
                va0 = Story_unk2();
                va = (((((((u32)(va0 << 3) >> 16) << 1) + ((u32)(va0 << 3) >> 16)) + (((((u32)(va0 << 3) >> 16) << 1) + ((u32)(va0 << 3) >> 16)) << 4)) + ((((((u32)(va0 << 3) >> 16) << 1) + ((u32)(va0 << 3) >> 16)) + (((((u32)(va0 << 3) >> 16) << 1) + ((u32)(va0 << 3) >> 16)) << 4)) << 8)) + -0xcccc);
                neg = -inner;
                vb0 = Story_unk3();
                vb = (((((((u32)(vb0 << 3) >> 16) << 1) + ((u32)(vb0 << 3) >> 16)) + (((((u32)(vb0 << 3) >> 16) << 1) + ((u32)(vb0 << 3) >> 16)) << 4)) + ((((((u32)(vb0 << 3) >> 16) << 1) + ((u32)(vb0 << 3) >> 16)) + (((((u32)(vb0 << 3) >> 16) << 1) + ((u32)(vb0 << 3) >> 16)) << 4)) << 8)) + -0xcccc);
                Story_Run(v8, 0, 0x2480000, va, 0, vb, 0x90000, (s32)rec);
                Story_unk2_4(1);
            } else {
                neg = -inner;
            }
            Story_unk3_4SupplementalSequenceOne(((s32)((s32)(neg - shift4) << 16) + 0x2d80000), 0, 0x2480000);
            inner = (inner + 1);
            v8 = (v8 + -0x10000);
        } while ((u32)inner <= 7);
        Story_unk3_5(76, 59, (108 - outer), 36, 2, 1);
        next = outer + 1;
        Story_unk4_4SupplementalSequenceOne(a0, outer, next);
        outer = next;
    } while ((u32)next <= 1);
    Story_unk5_4SupplementalSequenceOne(a0);
    Story_unk6_4SupplementalSequenceOne(0, next, (next + 1));
    Story_unk7_4SupplementalSequenceOne(211);
    Story_Check(0x2009791, 0xc80);
    Story_unk8_4SupplementalSequenceOne();
}

typedef struct {
    unsigned lo : 24;
    unsigned mid : 24;
    unsigned hi : 16;
} SceneTail;

typedef struct {
    s32 f0;
    s32 f1;
    s32 f2;
    s32 f3;
    SceneTail tail;
} SceneEvent;

/* overlays/scene/story/selector_effect/run_scene_sequence_f.c */

void Scene_RunSelSequenceF(void)
{
    s32 *rec;
    s32 outer;
    s32 inner;
    s32 base;
    s32 raw;
    s32 pos;
    s32 shown;
    s32 arr[10];

    Story_SetRect(78, 58, 110, 36, 1, 1);
    rec = arr;
    rec[1] = 5;
    rec[2] = 0x8000;
    rec[3] = 0x8000;
    outer = 0;
    do {
        base = -0x20000;
        inner = 1;
        do {
            if ((inner & 1) != 0) {
                raw = Story_Run();
                shown = ((0x248 - (s32)((u32)((raw << 2) + raw) >> 16)) & 0xffff) << 16;
                pos = (base - (outer << 19)) + 0x2d80000;
                Story_unk3_4SequenceF(pos, 0, shown, -0x4000, 0, 0, 0x90000, (s32)rec);
                Story_unk4_4SequenceF(1);
            }
            inner = inner + 1;
            base = base + -0x20000;
        } while ((u32)inner <= 7);
        Story_unk2_5(111, 35, (109 - outer), 36, 1, 1);
        outer = outer + 1;
    } while ((u32)outer <= 2);
    Story_Do(0x2009791);
}

typedef struct {
    unsigned lo : 24;
    unsigned mid : 24;
    unsigned hi : 16;
} SceneTail;

typedef struct {
    s32 f0;
    s32 f1;
    s32 f2;
    s32 f3;
    SceneTail tail;
} SceneEvent;
