#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/story/selector_effect/selector_effect.h"
#include "create_configured_overlay_object.h"
#include "create_configured_overlay_object_body.inc"
#include "select_overlay_data_by_runtime_selector.h"
#include "select_overlay_data_by_runtime_selector_body.inc"

/* overlays/scene/story/selector_effect/actor_motion.c */
#define F(base, type, off) (*(type *)((u8 *)(base) + (off)))

extern u8 Value;

u8 *Story_Run3(s32);

void Actor_RunActorZeroHandledMotion(s32 a)
{
    u8 *v = Story_Run3(0);
    Story_Run4();
    Story_Do5(0xe4);
    F(v, s32, 0x6c) = (s32)&Value;
    F(v, s32, 0x30) = 0x3333;
    Story_Apply(0, 2);
    Story_Place14(0, 0, -6);
    Story_Do6(0);
    Story_Apply2(0, 15);
    Story_Apply3(Story_Check10(0), 0);
    F(v, s32, 0x6c) = 0;
    Story_Do7(30);
    Story_Run5();
    Story_Run6();
    Story_Do8(a);
    Story_Run7();
}

/* overlays/scene/story/selector_effect/actor_position.c */
#define F(base, type, off) (*(type *)((u8 *)(base) + (off)))

extern u8 gOv[];
extern u8 Value_00000874;

u8 *Story_Run8(s32);
u8 *Story_Run9(s32);

u8 *Story_Run10(s32);
u8 *Story_Run11(s32);
u8 *Story_Run12(s32);
u8 *Story_Run13(s32);

s32 *Story_Run14(s32);

void Scene_RunActorElevenAtTile5And13(void)
{
    s32 x;
    s32 y;
    u8 *p;

    x = F(Story_Run8(11), s32, 8) / 0x100000;
    y = F(Story_Run9(11), s32, 16) / 0x100000;
    Story_Run15();
    if (x == 5 && y == 13) {
        F(Story_Run10(11), s32, 12) += 0xfffe0000;
        p = Story_Run11(11);
        F(p, s32, 0x3c) = F(Story_Run13(11), s32, 12);
        Story_SetRect12(5, 2, 5, 11, 1, 1);
        Story_Do9(0xd9);
        Story_Place15((s32)gOv, 9, 7);
        {
            s32 s0 = 9;
            s32 s1 = 10;
            Story_SetRect13(9, 5, 1, 1, s0, s1);
        }
        Story_Run12((s32)&Value_00000874);
    }
    Story_Run16();
}

void Actor_UseActorNinePositionWithYOffset(void)
{
    s32 *p = Story_Run14(9);
    u32 v = Story_Run17();

    s32 b = p[3] + (((v << 2) >> 16) << 16);
    s32 c = p[4];

    Story_Place16(p[2], b, c);
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

s32 *Story_Run18(s32);

void Effect_SpawnParticleRowsByMode(s32 mode)
{
    s32 buf[10];
    u32 i, j;

    Story_SetRect14(0x70, 0x39, 0x71, 0x2a, 1, 1);
    Story_SetRect15(0x75, 0x3a, 0x70, 0x2e, 1, 1);
    Story_SetRect16(0x75, 0x39, 0x74, 0x2c, 1, 1);
    Story_Do10(0x121);
    buf[1] = 5;
    buf[2] = 0x8000;
    buf[3] = 0x8000;
    for (j = 0; j <= 2; j++) {
        for (i = 1; i <= 7; i++) {
            if ((i & 1) != 0) {
                if (mode == 0) {
                    Story_SetRect17((0x319 - ((Story_Run19() * 5) >> 16)) << 16, 0,
                                  (((j << 2) + i) << 17) + 0x02b70000, 0,
                                  mode, 0x4000, 0x90000, buf);
                } else if (mode == 1) {
                    Story_SetRect18((((j << 2) + i) << 17) + 0x03120000, 0,
                                  (((((u32 (*)(void))Story_Run20)() * 5) >> 16) << 16) + 0x2e80000, 0x4000,
                                  0, 0, 0x90000, buf);
                } else {
                    Story_SetRect19(0x3380000 - (i << 17) - (j << 19), 0,
                                  (((Story_Run21() * 5) >> 16) << 16) + 0x2c80000, 0x4000,
                                  0, 0, 0x90000, buf);
                }
                Story_Do11(1);
            }
        }
        if (mode == 0)
            Story_SetRect20(0x70, 0x3a, 0x71, j + 43, 1, 1);
        else if (mode == 1)
            Story_SetRect21(0x70, 0x3a, j + 113, 0x2e, mode, mode);
        else
            Story_SetRect22(0x70, 0x3a, 115 - j, 0x2c, 1, 1);
    }
}

void Effect_SpawnParticleRowsAndDrawTiles(void)
{
    s32 buf[10];
    u32 i, j;

    Story_SetRect23(0x4a, 0x3a, 0x46, 0x22, 1, 1);
    buf[1] = 7;
    buf[2] = 0x8000;
    buf[3] = 0x8000;
    for (j = 0; j <= 1; j++) {
        for (i = 0; i <= 7; i++) {
            if ((i & 1) != 0) {
                s32 a = ((Story_Run22() << 3) >> 16) * 0x3333 + 0xffff3334;
                s32 b = ((Story_Run23() << 3) >> 16) * 0x3333 + 0xffff3334;

                Story_SetRect24(0x690000, 0, ((-i - (j << 4)) << 16) + 0x2200000,
                              a, 0, b, 0x90000, buf);
                Story_Do12(1);
            }
        }
        Story_SetRect25(0x4a, 0x3b, 0x46, 34 - j, 1, 1);
        Story_SetRect26(0x4a, 0x3a, 0x46, 33 - j, 1, 1);
    }
}

void Effect_SpawnParticleEveryFourthFrame(void)
{
    s32 buf[10];
    s32 *p = Story_Run18(0);
    s32 m = gIw & 3;

    if (m == 0) {
        buf[1] = 7;
        if ((((Story_Run24() << 1) >> 16) & 1) == 0)
            buf[1] = 5;
        buf[2] = 0xb333;
        buf[3] = 0xb333;
        {
            s32 y = p[3] + (((Story_Run25() << 2) >> 16) << 16);
            s32 a = ((Story_Run26() << 3) >> 16) * 0x3333 + 0xffff3334;
            s32 b = ((Story_Run27() << 3) >> 16) * 0x3333 + 0xffff3334;

            Story_SetRect27(p[2], y, p[4], a, b, m, 0x90000, buf);
        }
    }
}

void Effect_SpawnRandomEveryFourFrames(s32 x, s32 y, s32 z)
{
    s32 buf[10];
    s32 m = gIw & 3;

    if (m == 0) {
        buf[1] = 7;
        if ((((Story_Run28() << 1) >> 16) & 1) == 0)
            buf[1] = 5;
        buf[2] = 0xb333;
        buf[3] = 0xb333;
        {
            s32 a = ((Story_Run29() << 3) >> 16) * 0x3333 + 0xffff3334;
            s32 b = ((Story_Run30() << 3) >> 16) * 0x3333 + 0xffff3334;

            Story_SetRect28(x, y, z, a, b, m, 0x90000, buf);
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
        s32 a = x + (((Story_Run31() << 4) >> 16) << 16) + 0xfff80000;
        s32 b = z + (((Story_Run32() << 3) >> 16) << 16) + 0xfffc0000;

        Story_SetRect29(a, y, b, 0, 0, 0, 0xb0000, buf);
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

    seed = Story_Check11(step << 10);
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

struct BattleEffect *Story_Run33();
struct BattleEffect *Story_Run34();

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
    ref = Story_Run33(0);

    if ((flags & 0x100000) != 0 && options != 0) {
        effect = Story_Run34(options->kind, x, y, z);
    } else {
        effect = Story_Run34(222, x, y, z);
    }
    if (effect == 0) return;

    effect_sprite = effect->sprite;
    sprite0 = effect_sprite;

    Story_Run35(effect, (flags + 1) & 15);
    table_offset = (flags & 15) << 2;
    Story_Run36(effect, gOv2[table_offset >> 2]);

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
        Story_Run37(effect, options->battle_mode);
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
            effect->velocity_rate_x = Story_Check12(first_delta,
                                           descriptor->duration);
            delta = options->target34;
            duration = descriptor->duration;
            delta -= effect->position_y;
        } else {
            first_delta = options->target30;
            first_delta += (s32)0xffff0000;
            effect->velocity_rate_x = Story_Check13(first_delta,
                                           descriptor->duration);
            delta = options->target34;
            duration = descriptor->duration;
            delta += (s32)0xffff0000;
        }

        effect->velocity_rate_y = Story_Check14(delta, duration);
    }

    if ((flags & 0x200000) != 0) {
        Story_Run38(effect, 1);
        Story_Run39(effect, options->callback_arg);
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
extern u8 RuntimeSelectorTable[];
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
    Story_Run40();
    Story_Run41();
    Story_Run42();
    Story_Run43();
}

void Scene_RunSelSequenceA(void)
{
    extern u8 gWork[];

    u32 i;
    s32 rec7;
    s32 record;

    if (Story_Check(0x256) != 0) {
        Story_Run44();
        Story_Do(0x256);
        record = Story_Check15(0);
        *(volatile s32 *)(record + 12) += 0x20000;
        rec7 = Story_Check2(0);
        record = Story_Check3(0);
        *(volatile s32 *)(rec7 + 60) = *(volatile s32 *)(record + 12);
        Story_Run45(5);
        Story_SetRect(8, 29, 10, 23, 1, 1);
        Story_Run46(217);
        Story_Place(0x200e08e, 10, 18);
        Story_Run47();
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
    Story_Run48();
    Story_Apply4(0, 1);
    Story_Apply5(0x1576, 1);
    Story_Run49();
}

void Scene_RunScriptedSteps0And953(void)
{
    Story_Run50();
    Story_Apply6(0, 1);
    Story_Apply7(0x953, 1);
    Story_Run51();
}

void Scene_RunFlag881Dialogue(void)
{
    extern u8 *gWork;

    Story_Run52();
    Story_Apply8(0, 1);
    if (Story_Check16(0x881) == 0)
        Story_Apply9(0x1636, 1);
    else
        Story_Apply10(0x1635, 1);
    if (Story_Check17(0xb9) != -1) {
        s16 *slot = (s16 *)gWork + 185;
        s32 one = 1;

        *slot = one;
    }
    Story_Run53();
}

void Scene_RunActor184Sequence(void)
{
    Story_Run54();
    Story_Do13(0x53);
    Story_Apply11(0xb8, 3);
    Story_Apply12(0xb9, 0xb8);
    Story_Apply13(Story_Check18(0xb8), 1);
    Story_Apply14(0xb8, 2);
    Story_Apply15(0x1638, 1);
    Story_Do14(512);
    Story_Run55();
}

void Scene_RunSelSequenceB(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Story_Run56();
    if (gCell[224][0] == (s32)gVal) {
        Story_Place2(0, 0x1d8, 0x258);
        Story_Place3(0, 0x4000, 10);
        Story_Run(0x1d00000, -1, 0x2900000, 1);
        record = Story_Check19(0);
        Story_Run57(record, 0);
        record = Story_Check20(0);
        (void)Story_Check21(*(volatile s32 *)(record + 8), 0, 0x2be0000, 223);
        Story_SetRect2(92, 46, 92, 40, 3, 2);
        record = Story_Check22(0);
        *(volatile s32 *)(record + 72) = 0x8000;
        Story_Run58(0, 2);
        Story_Place4(0, 6, -1);
        *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x203;
        Story_Run59(60);
        Story_Run60(8);
    } else {
        Story_Place5(0, 6, -1);
    }
    Story_Run61();
}

void Scene_RunScene39b(void)
{
    extern u8 gWork[];

    u32 i;
    u8 *rec7;
    s32 rec8;
    s32 record;

    rec7 = Story_Check4(0);
    rec8 = Story_Check5(0x109);
    if (rec8 == 0) {
        Story_Run62();
        Story_Run2(-1, -1, -1, 0);
        rec7[85] = rec8;
        Story_Place6(0, (*(s16 *)((s32)rec7 + 10) << 16), ((*(s16 *)((s32)rec7 + 18) << 16) + -0x100000));
        Story_Run63(0, 15);
        record = Story_Check23(0);
        Story_Run64(record, 0);
        Story_Run65();
        Story_Run66();
        Story_Run67(228);
        *(volatile s32 *)((s32)rec7 + 108) = 0x2008cd1;
        Story_Place7(0, 0x6666, 0x3333);
        Story_Run68(0, 0, 8);
        Story_Run69(0, 0);
        record = Story_Check24(0);
        Story_Run70(record, 1);
        {
            u8 *rec9 = (u8 *)*(volatile s32 *)((s32)rec7 + 80);
            s32 mask = -13;
            rec9[9] = (mask & rec9[9]) | 4;
        }
        Story_Run71(0, 0, 10);
        rec7[85] = 3;
        *(volatile s32 *)((s32)rec7 + 108) = rec8;
        Story_Run72();
        Story_Run73();
    }
}

void Scene_RunSel(void)
{
    u8 *p5;
    u8 *addr;
    u8 v;

    p5 = *(volatile s32 *)0x03001f30;
    Story_Place8(11, 0x3480000, 0x2580000);
    Story_Run74(93, 1);
    Story_Run75(3, 11);
    addr = p5 + 0x71c;
    v = *addr | 8;
    *addr = v;
    Story_Run76();
    Story_Run77(1);
    Story_Run78();
}

/* overlays/scene/story/selector_effect/overlay_object.c */
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

void *AcquireOverlayObject(s32, s32, s32, s32);

u8 *Story_Run79(s32 kind, s32 arg1, s32 arg2, s32 arg3);

u8 **Story_Run80(s32 group, s32 slot);

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

    obj = Story_Run79(24, *(s32 *)(src + 8), *(s32 *)(src + 12), *(s32 *)(src + 16));
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
    u8 **slot = Story_Run80(35, 4);
    u8 *state;
    u8 *obj;

    if (slot == 0)
        return;

    state = *slot;
    obj = *(u8 **)(state + 20);
    if (obj == 0)
        return;

    Story_Do15(obj);
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
        Story_Run81(219);
    }
    i = (u32)a1;
    if (i < (u32)a2) {
        do {
            Story_SetRect3(45 - (s32)(i << 1), 32, 44 - (s32)(i << 1), 32, (s32)(i + 1), 6);
            Story_SetRect4(45 - (s32)i, 51, 45 - (s32)i, 32, 1, 6);
            v = 109 - (s32)i;
            Story_SetRect5(v, 32, 108 - (s32)i, 32, 1, 4);
            Story_SetRect6(v, 51, v, 32, 1, 4);
            if (a0 != 0) {
                Story_Place9(0x50000, 0x50000, 0x10000);
                Story_Place10(-1, -1, 0xe666);
                Story_Run82(a0);
            }
            i = i + 1;
        } while (i < (u32)a2);
    }
    Story_SetRect7(42, 52, 4, 5, 42, 33);
}

/* overlays/scene/story/selector_effect/run_scene_sequence_e.c */
extern u8 gWork[];

typedef struct {
    s32 f0;
    s32 f1;
    s32 f2;
    s32 f3;
    s32 t0;
    s32 t1;
} SceneEvent;

void Story_Run83(void)
{
    SceneEvent ev;
    s32 kind;
    s32 shape;
    s32 three;

    Story_Run84();
    if (Story_Check25(&ev) != 0) {
        kind = ev.f1;
        if (kind == 8) {
            shape = ev.f2;
            if ((shape >> 20) == 11) {
                Story_Run85(ev);
                Story_Run86(30);
                Story_Run87(211);
                Story_Run88();
                three = 3;
                Story_Run89(76, 60, 74, 38, three, 1);
                Story_Run90(77, 60, 76, 38, 2, 1);
                Story_Run91(75, 58, 86, 41, 1, three);
                Story_Run92(75, 59, 86, 43, 1, 2);
                Story_Run93(76, 59, 80, 49, 2, 1);
                Story_Run94(77, 59, 82, 49, 2, 1);
                Story_Run95(0x302);
            } else {
                ev.t1 = 0x200b949;
                Story_Run96(75, 57, 86, 41, 1, 1);
                Story_Run97(71, 59, 86, 42, 1, 1);
                Story_Run98(71, 59, 86, 43, 1, 1);
                Story_Run99(71, 59, 86, 44, 1, 1);
                Story_Run100(71, 59, 80, 49, 1, 1);
                Story_Run101(71, 59, 81, 49, 1, 1);
                Story_Run102(71, 59, 82, 49, 1, 1);
                Story_Run103(78, 58, 83, 49, 1, 1);
                Story_Run104(ev);
                Story_Run105(0x302);
            }
        } else if (kind == 10) {
            if ((ev.t0 >> 20) == 40) {
                Story_Run106(ev);
                if (Story_Check6(0x307) == 0) {
                    Story_Run107(0x18000, 0x3000);
                    Story_Run108(0x2ca0000, -1, 0x2500000, 1);
                    Story_Run109();
                    Story_Do2(0x307);
                    Story_Run110(5);
                    Story_Run111(50);
                } else {
                    Story_Run112(5);
                }
                Story_Run113(0x306);
            } else if ((ev.t0 >> 20) == 42) {
                ev.t1 = 0x20098f9;
                Story_Run114(ev);
                Story_Run115(5);
                Story_Run116(0x306);
            }
        }
    }
    Story_Run117();
}

/* overlays/scene/story/selector_effect/run_scene_sequence_g.c */
extern u8 gWork[];

void Story_Run118(s32 a0)
{
    u32 i;
    s32 x;
    s32 y;
    s32 k;
    s32 six;
    s32 pos;
    s32 four;
    Story_Run119(219);
    six = 6;
    i = 0;
    x = 41;
    y = 40;
    do {
        Story_SetRect8(y, 32, x, 32, 3 - (s32)i, six);
        k = 2;
        Story_SetRect9(39, 51, y, 32, 1, six);
        pos = (s32)i + 106;
        four = 4;
        Story_Run120(105, 51, pos, 32, k, four);
        if (a0 != 0) {
            Story_Place11(0x50000, 0x50000, 0x10000);
            Story_Place12(-1, -1, 0xe666);
            Story_Run121(a0);
        }
        i = i + 1;
        x = x + 2;
        y = y + 2;
    } while (i <= 2);
    Story_Do3(0x120);
    Story_SetRect10(106, 33, 4, 5, 42, 33);
    Story_Run122();
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

extern s16 RuntimeSelectorTable[];
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
    Story_Do16((s32)gOv8);
    return 0;
}

/* overlays/scene/story/selector_effect/scene_dialogue.c */

/* Complete scene-entry wrapper through return, alignment and its sole pool. */
void Dialogue_RunLine1637(void)
{
    Story_Run123();
    Story_Run124(0x1637, 1);
    Story_Run125();
}

void Dialogue_RunActor3TimedLine(void)
{
    Story_Run126();
    Story_Apply19(3, 4);
    Story_Do17(20);
    Story_Do18(0x157d);
    Story_Place17(3, 0, 20);
    Story_Run127();
}

/* overlays/scene/story/selector_effect/scene_effect_sequence.c */

void Effect_SpawnRandomizedBurst(s32 x, s32 y, s32 z, s32 w)
{
    s32 desc[10];
    s32 tmp[3];
    u32 i;

    Story_Do19(0xd8);
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
            Story_Place18((6 - (i >> 1)) * 0x1999, w, tmp);
            {
                s32 a = x + ((6 - ((Story_Run128() * 6) >> 16)) << 16);
                s32 b = z + ((6 - ((Story_Run129() * 6) >> 16)) << 16);

                Story_SetRect30(a, y, b, tmp[0], tmp[1], tmp[2], 0x90000, desc);
            }
        }
        Story_Do20(2);
        i++;
    } while (i <= 11);
}

/* overlays/scene/story/selector_effect/scene_setup.c */
extern u8 gUnk[];

void Scene_RunFourCallSequence(void)
{
    Story_Run130();
    Story_Run131();
    Story_Run132();
    Story_Run133();
}

void Scene_ApplyTable2b20000(void)
{
    Story_Place19((s32)gUnk, 0, 0x2480000);
}

void Scene_RunFourStepSequence(void)
{
    Story_Run134();
    Story_Run135();
    Story_Run136();
    Story_Run137();
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
    Story_Do21(0x161);

    target = (s32 *)rec[5];
    if (target != 0) {
        *(short *)((u8 *)target + 0x64) = 0;
        Story_Apply20(target, (s32)0x0200de2c);
        Story_Apply21(target, 7);
        rec[5] = 0;
    }
}

/* overlays/scene/story/selector_effect/state_update.c */
extern u8 *gWork;
extern u8 gOv9[];

s32 *Story_Run138(s32);
s32 *Story_Run139(s32);

s32 *Story_Run140(s32);
s32 *Story_Run141(s32);
s32 *Story_Run108(s32);

s32 *Story_Run142(s32);

/*
 * resource_39c owner at 0x02000cc0, 16 bytes: invoke one fixed two-value
 * setup call with 12 and 21.
 *
 * The executable owner runs from `push {lr}` at 0x02000cc0 through the
 * void interworking return `pop {r0} / bx r0` at 0x02000cca-0x02000ccc.
 * m2c_guard independently measures those 14 code bytes and finds no
 * seeding hazard.  The zero halfword at 0x02000cce is alignment before
 * the next owner's prologue at 0x02000cd0, making the whole span 16 bytes.
 * Its sole call resolves through the overlay veneer to Story_Run143.
 * The constants' game-level meanings are not established by this owner.
 */

void State_ApplyPair12And21(void)
{
    Story_Apply22(12, 21);
}

void State_ApplyWork16cMinus50A(void)
{
    Story_Do22(*(s16 *)(gWork + 0x16c) - 50);
}

void State_ApplyWork16cMinus50(void)
{
    Story_Do23(*(s16 *)(gWork + 0x16c) - 50);
}

void State_ApplyWork16cMinus50B(void)
{
    Story_Do24(*(s16 *)(gWork + 0x16c) - 50);
}

void State_RunWhenActor8AtTile10x23(void)
{
    s32 x = Story_Run138(8)[2] / 0x100000;
    s32 y = Story_Run139(8)[4] / 0x100000;

    Story_Run144();
    if (x == 10 && y == 23) {
        s32 *p;
        Story_Run140(8)[3] += 0xfffe0000;
        p = Story_Run141(8);
        p[15] = Story_Run108(8)[3];
        Story_SetRect31(6, 29, 10, 23, 1, 1);
        Story_Do25(0xd9);
        Story_Place20((s32)gOv9, 10, 18);
        Story_SetRect32(10, 16, 1, 1, x, 19);
        Story_Do26(0x878);
    }
    Story_Run145();
}

void State_ApplyRectWhenActor20AtColumn28(void)
{
    s32 col;

    Story_Run146();
    col = Story_Run142(20)[2] / 0x100000;
    if (col == 28) {
        Story_Do27(840);
        {
            s32 a = 31;
            s32 b = 20;

            Story_SetRect33(29, 20, 1, 1, a, b);
        }
    }
    Story_Run147();
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
extern u8 gWork[];

Ent *Story_Run148(Desc *, Ent *);

struct Rec_395 *Story_Run149();

s32 Story_Run150(Ent *a)
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
    r = Story_Run148(&d, a);
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

s32 Story_Run151(s32 *a)
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
    obj = (u8 *)Story_Place21(&sel, a + 1, a);
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

void Story_Run152(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    if (Story_Check7(0x256) != 0) {
        Story_Run153();
        Story_Do4(0x256);
        record = Story_Check26(0);
        *(volatile s32 *)(record + 12) += 0x20000;
        rec7 = Story_Check8(0);
        record = Story_Check9(0);
        *(volatile s32 *)(rec7 + 60) = *(volatile s32 *)(record + 12);
        Story_Run154(5);
        Story_SetRect11(7, 2, 5, 11, 1, 1);
        Story_Run155(217);
        Story_Place13(0x200e03a, 9, 7);
        Story_Run156();
    }
}

    if (slot != -1) {
        s32 index = Story_Check27(slot, key);

        if (index != -1) {
            Story_Run149(slot)->tbl[index] = value;
        }
    }
}
