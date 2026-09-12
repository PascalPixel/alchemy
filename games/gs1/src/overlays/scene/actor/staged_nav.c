#include "types.h"
#include "scene.h"
#include "staged_actor_movement.h"
#include "run_staged_actor_movement_and_redraw_body.inc"
#include "create_configured_overlay_object.h"
#include "create_configured_overlay_object_body.inc"
#include "configured_effect_spawn.h"
#include "staged_actor.h"
#include "configured_effect_spawn_body.inc"

/* overlays/scene/actor/staged_nav/staged_nav.c */
/* overlays/scene/actor/staged_nav/actor_position.c */
u8 *Actor_unk3_4(s32 slot);
u8 *Actor_unk4_4();

/*
 * Place a staged actor at object ten's grid cell -- resource_3b3.
 */

/*
 * The Func_ aliases name the call words encoded in the overlay image. The
 * declarations are old-style because the call sites vary in arity.
 */

/*
 * resource_3b3 @ 0x020020b8 (56 bytes including trailing alignment).
 *
 * Compares an actor with slot zero.  When it is farther right, bit 1 at +35
 * is cleared and then restored only if the actor is also above slot zero.
 * The function always returns zero.
 */
s32 Actor_UpdateBit1ByPositionToSlotZero(u8 *actor)
{
    u8 *ref = Actor_unk3_4(0);

    if (*(s32 *)(actor + 16) > *(s32 *)(ref + 16)) {
        actor[35] = (u8)(actor[35] & 0xfd);
        if (*(s32 *)(actor + 12) < *(s32 *)(ref + 12))
            actor[35] = (u8)(actor[35] | 2);
    }

    return 0;
}

/*
 * Fetch object ten, shift its +8 and +16 fixed-point fields down to grid
 * coordinates, and place there. The last two literal arguments go on the
 * stack.
 */
void StagedActor_PlaceAtObjectTenCell(void)
{
    u8 *obj = Actor_unk4_4(10);
    s32 x;
    s32 z;

    Actor_unk5_4();

    x = *(s32 *)(obj + 8) >> 20;
    z = *(s32 *)(obj + 16) >> 20;

    Actor_unk11(2, x, z, 1, 1, 0);
    Actor_unk6_4();
}

/* overlays/scene/actor/staged_nav/actor_presentation.c */

/* Contiguous unnamed leaf-owner run for resource_3b3. */

/* Complete 12-byte actor-11 wrapper before 0x02002150. */
void Scene_RunActor11Step(void)
{
    Actor_unk5_2(11);
}

/* Complete 12-byte actor-12 wrapper before 0x0200215c. */
void Scene_RunActor12Step(void)
{
    Actor_unk6_2(12);
}

/* overlays/scene/actor/staged_nav/actor_search.c */
typedef struct {
    u8 pad[9];
    u8 low : 2;
    u8 mode : 2;
    u8 rest : 4;
} Handle;

u8 *Actor_unk7_4();
u8 *Actor_unk8_4();
u8 *Actor_unk9_4();

void Actor_CheckActors8To11NearSlotZero(void)
{
    u8 *hero = Actor_unk7_4(0);
    u32 selector = 8;
    u8 *actor;

loop:
    actor = Actor_unk8_4(selector);

    if (*(s32 *)(hero + 12) / 0x10000 != *(s32 *)(actor + 12) / 0x10000)
        goto mark_and_continue;

    if (*(s32 *)(hero + 16) > *(s32 *)(actor + 16) - 0x80000
        || *(s32 *)(hero + 16) <= *(s32 *)(actor + 16) - 0x180000)
        goto mark_and_continue;

    if (*(s32 *)(hero + 8) - 0x100000 > *(s32 *)(actor + 8)
        || *(s32 *)(actor + 8) >= *(s32 *)(hero + 8) + 0x100000)
        goto continue_loop;

    {
        Handle *handle = *(Handle **)(actor + 80);
        Actor_unk10_4(0, handle->mode);
    }
    goto done;

mark_and_continue:
    {
        u8 *mark = Actor_unk9_4(0) + 35;
        u8 bit = 1;
        bit |= *mark;
        *mark = bit;
    }

continue_loop:
    selector++;
    if (selector <= 11)
        goto loop;

done:
    return;
}

/* overlays/scene/actor/staged_nav/configurable_effect_spawn.c */
extern volatile s32 gIw;

void Effect_SpawnRandomizedParticle(void)
{
    u8 descriptor[40];
    u8 *d;
    s32 spread;
    s32 secondary;
    u32 draw;
    u32 mask;

    if ((gIw & 2) != 0)
        return;
    if ((gIw & 7) == 0)
        Actor_unk7_2(136);

    d = descriptor;
    *(s32 *)(d + 4) = 10;
    *(s32 *)(d + 8) = 0x8000;
    *(s32 *)(d + 12) = 0x8000;
    *(s32 *)(d + 16) = 0x19999;
    *(s32 *)(d + 20) = 0x19999;
    draw = (u32)Actor_unk12();
    mask = 0x0ffff000;
    mask &= draw;
    *(u16 *)(d + 32) = (u16)mask;
    *(s32 *)(d + 36) = 0x020093b1;

    draw = (u32)Actor_unk13();
    spread = -((s32)((draw * 5) >> 16) * 0x10000 + 0x60000);
    spread /= 2;
    draw = (u32)Actor_unk14();
    secondary = -((s32)((draw * 5) >> 16) * 0x10000 + 0x50000);

    Actor_unk12_5(0x01440000, 0x00300000, 0x00e40000, spread,
                  secondary, 0, 0x014d0000, d);
}

/* overlays/scene/actor/staged_nav/coordinate_preset.c */
void CopyAndOffsetCoordinatePreset(void)
{
    u32 *destination;
    const u32 *source;
    u16 *coordinates;

    source = (const u32 *)0x03001ad4;
    destination = (u32 *)0x0200b72c;
    *destination++ = *source++;
    *destination++ = *source++;
    *destination = *source;
    coordinates = (u16 *)0x0200b72c;
    coordinates[1] += 0xc0;
    coordinates[3] += 0xc0;
    coordinates[5] += 0xc0;
}

/* overlays/scene/actor/staged_nav/effect.c */
extern volatile s32 gIw;

s32 Effect_SpawnRandomEffectEveryEightFrames(u8 *actor)
{
    u8 desc[40];
    u8 *p;
    u32 phase = (u32)gIw & 7;
    s32 x;
    s32 y;
    s32 z;
    s32 scale;

    if (phase != 0)
        return 0;

    p = desc;
    *(s32 *)(p + 4) = 7;
    *(s32 *)(p + 8) = 0xb333;
    *(s32 *)(p + 12) = 0xb333;

    x = *(s32 *)(actor + 8) + (((s32)(((u32)Actor_unk15() * 17) >> 16) - 8) << 16);
    y = *(s32 *)(actor + 12) + ((s32)(((u32)Actor_unk16() * 17) >> 16) << 16);
    z = *(s32 *)(actor + 16) + (((s32)(((u32)Actor_unk17() * 17) >> 16) - 8) << 16);
    scale = Actor_Apply((s32)(((u32)Actor_unk18() * 5) >> 16) * 0x10000 + 0x30000, 10);

    Actor_unk13_5(x, y, z, 0, scale, (s32)phase, 0x00090001, p);
    return 0;
}

/* overlays/scene/actor/staged_nav/move_and_redraw.c */
#define Value Value3

u8 *Actor_unk11_4(void);

/*
 * Poll an overlay object until it settles, then reset it -- resource_3b3.
 */

/* Declared without a prototype; the call site passes one argument. */

void *OvObj_PrepareObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    u8 *obj = Actor_unk12_4(arg3, arg0, arg1, arg2);

    if (obj != NULL) {
        u8 *rec = *(u8 **)(obj + 0x50);
        s32 flags;
        s32 mask = 13;

        flags = rec[9];
        mask = -mask;
        mask &= flags;
        rec[9] = mask;
        obj[0x55] = 0;
        obj[0x59] = 8;
        Actor_Apply2(obj, 0);
        Actor_Apply3(obj, 14);
        Actor_Apply4(obj, 1);
        return obj;
    }
    return NULL;
}

void *OvObj_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
}

/*
 * Wait at most sixty polls for the object's +12 to reach its +20, then clear
 * +0x28, set +0x3c, and mirror +20 back into +12. Actor_unk13_4(1) is taken
 * to be a one-frame wait, and the field offsets are named by position and
 * not verified.
 */
void OvObj_WaitUntilSettledAndReset(u8 *obj)
{
    s32 cnt = 60;

    for (;;) {
        if (cnt == 0) {
            break;
        }
        Actor_unk13_4(1);
        if (*(u32 *)(obj + 12) == *(u32 *)(obj + 20)) {
            break;
        }
        cnt--;
    }

    *(u32 *)(obj + 0x28) = 0;
    *(u32 *)(obj + 0x3c) = 0x80000000;
    *(u32 *)(obj + 12) = *(u32 *)(obj + 20);
}

void OvObj_SetCallbackAndMode2(void)
{
    u8 *obj = Actor_unk11_4();
    u8 *base = obj;
    u8 zero = 0;

    obj += 0x22;
    *obj = 2;
    base[0x55] = zero;
    *(u32 *)(base + 0x6c) = 0x02009aa9;
}

/* overlays/scene/actor/staged_nav/scene_data.c */
extern s16 gCell[];
extern u8 Value_00000073;
extern u8 Value_00000074;
extern u8 Value_00000077;
extern u8 Value_00000079;
extern u8 Value_0000007a;
extern u8 gOv[];
extern u8 gOv2[];
extern u8 gOv3[];
extern u8 gOv4[];
extern u8 gOv5[];
extern u8 gOv6[];
extern u8 gOv7[];
extern u8 gOv8[];
extern u8 gOv9[];
extern u8 gOv10[];
extern u8 gOv11[];
extern u8 gOv12[];
extern u8 gOv13[];
extern u8 gOv14[];
extern u8 gOv15[];
extern u8 gOv16[];
extern u8 gOv17[];

/* Contiguous unnamed leaf-owner run for resource_3b3. */

s32 SceneData_SelectTableBySceneId(void)
{
    s16 v = gCell[224];

    if (v == (s32)&Value_00000073) {
        return (s32)gOv;
    }
    if (v == (s32)&Value_00000074) {
        return (s32)gOv2;
    }
    if (v == (s32)&Value_00000077) {
        return (s32)gOv3;
    }
    if (v == (s32)&Value_00000079) {
        return (s32)gOv4;
    }
    if (v == (s32)&Value_0000007a) {
        return (s32)gOv5;
    }
    return (s32)gOv6;
}

/* Complete four-byte leaf: movs r0,#0 followed by bx lr. */
s32 SceneData_ReturnZero(void)
{
    return 0;
}

/* Complete eight-byte literal-address getter, including its sole pool word. */
u8 *SceneData_GetTableb014(void)
{
    return (u8 *)0x0200b014;
}

s32 SceneData_SelectDataByRuntimeSelector(void)
{
    s16 v = gCell[224];

    if (v == (s32)&Value_00000073) {
        return (s32)gOv7;
    }
    if (v == (s32)&Value_00000074) {
        return (s32)gOv8;
    }
    if (v == (s32)&Value_00000077) {
        return (s32)gOv9;
    }
    if (v == (s32)&Value_0000007a) {
        return (s32)gOv10;
    }
    return (s32)gOv11;
}

s32 SceneData_SelectSecondaryTableBySceneId(void)
{
    s16 v = gCell[224];

    if (v == (s32)&Value_00000073) {
        return (s32)gOv12;
    }
    if (v == (s32)&Value_00000074) {
        return (s32)gOv13;
    }
    if (v == (s32)&Value_00000077) {
        return (s32)gOv14;
    }
    if (v == (s32)&Value_00000079) {
        return (s32)gOv15;
    }
    if (v == (s32)&Value_0000007a) {
        return (s32)gOv16;
    }
    return (s32)gOv17;
}

/* overlays/scene/actor/staged_nav/scene_primary_script.c */
#define REC_ID 16
#define QUERY_FLAG 0x200

extern u8 gWork[];
extern u8 gCell2[][2];
extern u8 gOv18[];

u8 *Actor_unk14_4();

u8 *Scene_GetRecord_2();

u8 *Scene_GetRecord_4();

u8 *Actor_unk15_4();

/* Record id used for the single record configured below in either branch. */

/* Flag/id value passed to the query call and to the two closing calls, and
 * written into the flag byte at +85 when the query call returned zero. */

void Scene_RunPrimarySequence(void)
{
    s32 Scene_RunScene3b3SequenceD();

    s32 rec;
    s32 flag;
    s32 p6;
    s32 p5;
    s32 record;
    s32 v1;
    s32 v2;
    s32 v3;
    u8 *base;
    u8 slot16[40];

    rec = Actor_unk20(0);
    flag = *(volatile s32 *)0x03001e40 & 3;
    if (flag == 0) {
        base = slot16;
        *(s32 *)(base + 4) = 10;
        *(s32 *)(base + 8) = 0xb333;
        *(s32 *)(base + 12) = 0xb333;
        v1 = Actor_unk21();
        p6 = *(volatile s32 *)(rec + 8) + ((((u32)((v1 << 4) + v1) >> 16) - 8) << 16);
        v2 = Actor_unk22();
        p5 = *(volatile s32 *)(rec + 16) + ((((u32)((v2 << 4) + v2) >> 16) - 8) << 16);
        v3 = Actor_unk23();
        record = Actor_unk24((((u32)((v3 << 2) + v3) >> 16) << 16) + 0x30000, 10);
        Actor_SetRect(p6, *(volatile s32 *)(rec + 12), p5, 0, record, flag, 0x90001, (s32)base);
    }
}

    u8 *rec;
    u8 *pflag;
    s32 saved;
    s32 mode;
    s32 *p;
    s32 buf[3];

    rec = Actor_unk14_4(0);
    pflag = rec + 85;
    saved = *pflag;
    mode = (*(volatile u16 *)(rec + 6) + 0x2000) & 0xc000;
    if (gCell2[249][0] != 0) {
        return 0;
    }
    p = buf;
    p[0] = (*(volatile s32 *)(rec + 8) & -0x100000) + 0x80000;
    p[1] = *(volatile s32 *)(rec + 12);
    p[2] = (*(volatile s32 *)(rec + 16) & -0x100000) + 0x80000;
    Actor_Place(0x100000, mode, (s32)p);
    if (Actor_Check((s32)rec, (s32)p) == 1) {
        goto reject;
    }
    if (Actor_unk2((s32)p, (s32)rec) != 0) {
        goto reject;
    }
    p[0] = (*(volatile s32 *)(rec + 8) & -0x100000) + 0x80000;
    p[1] = *(volatile s32 *)(rec + 12);
    p[2] = (*(volatile s32 *)(rec + 16) & -0x100000) + 0x80000;
    Actor_unk2_3(0x200000, mode, (s32)p);
    if (Actor_unk3((s32)p, (s32)rec) != 0) {
        goto reject;
    }
    if (Actor_unk4((s32)rec, (s32)p) != 0) {
        goto reject;
    }
    Actor_unk16_4();
    Actor_unk17_4((s32)rec, 6);
    Actor_unk18_4(6);
    Actor_unk19_4(152);
    Actor_unk20_4((s32)rec, 7);
    *(volatile s32 *)(rec + 48) = 0x30000;
    *(volatile s32 *)(rec + 52) = 0x20000;
    *(volatile s32 *)(rec + 40) = 0x40000;
    *pflag &= 126;
    Actor_unk21_4((s32)rec, 0);
    Actor_unk22_4(0, *(s16 *)((u8 *)p + 2), *(s16 *)((u8 *)p + 10));
    Actor_unk23_4((s32)rec, 6);
    Actor_unk24_4((s32)rec, 1);
    *pflag = saved;
    Actor_unk25_4();
    return 1;
reject:
    return 0;
}

    s32 v68;
    s32 v72;
    s32 v76;
    s32 step;
    s32 t;

    v68 = *(s32 *)(a0 + 68);
    t = *(s32 *)(a0 + 8);
    v72 = *(s32 *)(a0 + 72);
    *(s32 *)(a0 + 8) = t + v68;
    t = *(s32 *)(a0 + 12);
    v76 = *(s32 *)(a0 + 76);
    *(s32 *)(a0 + 12) = t + v72;
    *(s32 *)(a0 + 16) += v76;
    step = Actor_unk5(v68, 10);
    *(s32 *)(a0 + 68) = v68 - step;
    step = Actor_unk25(v72, 3);
    *(s32 *)(a0 + 72) = v72 - step;
    step = Actor_unk26(v76, 10);
    *(volatile s32 *)(a0 + 76) = v76 - step;
    *(volatile s32 *)(a0 + 24) += *(volatile s32 *)(a0 + 48);
    *(volatile s32 *)(a0 + 28) += *(s32 *)(a0 + 52);
    *(u16 *)(*(volatile s32 *)(a0 + 80) + 30) += *(u16 *)(a0 + 100);
}

void Scene_RunScene3b3SequenceA(void)
{
    s32 record;
    s32 count;

    record = Actor_unk6(0x200);
    if (record == 0) {
        Actor_unk26_4(10, 19, 16, 5, record, 10, 31);
        Actor_unk27_4(10, 51, 16, 5, 1, 10, 31);
        Actor_unk28_4(42, 51, 16, 5, 2, 10, 31);
    } else {
        Actor_unk29_4(10, 19, 16, 5, 0, 10, 31);
        Actor_unk30_4(10, 83, 16, 5, 1, 10, 31);
        Actor_unk31_4(42, 83, 16, 5, 2, 10, 31);
    }
    *(volatile s32 *)gOv18 = 0;
    Actor_unk27(0x20095cd, 0xc80);
    Actor_unk32_4(1);
    Actor_unk33_4(1, 0, 0x2009579);
    Actor_unk34_4(231);
    *(volatile s32 *)gOv18 = 0;
    do {
        Actor_unk35_4(1);
        count = *(volatile s32 *)gOv18;
        count = count + 1;
        *(volatile s32 *)gOv18 = count;
    } while (count <= 100);
    Actor_Do(0x121);
    if (Actor_unk7(0x200) == 0) {
        Actor_unk2_5(0, 32, 32, 0, 32, 32);
        Actor_unk3_5(32, 32, 64, 0, 32, 32);
    } else {
        Actor_unk4_5(0, 64, 32, 0, 32, 32);
        Actor_unk5_5(32, 64, 64, 0, 32, 32);
    }
    Actor_unk36_4(1);
    Actor_unk37_4(1, 0, 0);
    Actor_unk38_4(1);
    Actor_unk2_2(0x20095cd);
    Actor_unk39_4();
    Actor_unk40_4(30);
}

/* Runs one of two near-identical setup sequences for record REC_ID and
 * records 9-15, chosen by the query call's return value; each sequence ends
 * with its own closing call carrying QUERY_FLAG. */
void Scene_RunFlaggedDisplayScene(void)
{
    u32 i;
    u8 *queried;
    u8 *record;

    Battle_Reset_1();
    Motion_SetSpeedLim_1(0x10000, 0x2000);
    Motion_CamBounds_1(0x1190000, -1, 0x1b00000, 1);
    Object_CommitPositionThenWaitIfModeZero_1();
    UiText_DrawMessage_1(0x1528, 1);
    queried = GameFlag_IsSet_1(QUERY_FLAG);
    if (queried == 0) {
        Audio_PlayCue_1(232);
        Actor_unk3_3(0x200ada8, 84, 24);
        Battle_WaitMode0_1(30);
        Audio_PlayCue_2(240);
        Motion_SetActionVariant_1(REC_ID, 1);
        /* Flag byte at +85: cleared, since queried is zero here. */
        *(u8 *)(Scene_GetRecord_1(REC_ID) + 85) = queried;
        record = Scene_GetRecord_2(REC_ID);
        *(s32 *)(record + 12) = -0x200000;
        Motion_SetHPosTerrain_1(REC_ID, 0x1100000, 0x1a00000);
        Object_SetModeById_1(REC_ID, 1);
        Actor_unk4_3(0x200adfc, 80, 24);
        Actor_unk5_3(0x200ae50, 80, 28);
        Actor_unk6_5(65, 40, 16, 27, 2, 4);
        Actor_unk41_4();
        Actor_unk42_4(9);
        Actor_unk43_4(10);
        Actor_unk44_4(11);
        Actor_unk45_4(12);
        Actor_unk46_4(13);
        Actor_unk47_4(14);
        Actor_unk48_4(15);
        Actor_unk7_5(24, 3, 1, 1, 24, 8);
        GameFlag_Set_1(QUERY_FLAG);
    } else {
        Audio_PlayCue_3(232);
        Actor_unk6_3(0x200add2, 84, 24);
        Battle_WaitMode0_2(30);
        Audio_PlayCue_4(230);
        /* Flag byte at +85: cleared unconditionally in this branch. */
        *(u8 *)(Scene_GetRecord_3(REC_ID) + 85) = 0;
        record = Scene_GetRecord_4(REC_ID);
        *(s32 *)(record + 12) = -0x200000;
        Motion_SetHPosTerrain_2(REC_ID, 0x1100000, 0x1b40000);
        Object_SetModeById_2(REC_ID, 2);
        Actor_unk8_5(65, 45, 16, 27, 2, 4);
        Actor_unk7_3(0x200ae26, 80, 24);
        Actor_unk49_4();
        Actor_unk50_4(9);
        Actor_unk51_4(10);
        Actor_unk52_4(11);
        Actor_unk53_4(12);
        Actor_unk54_4(13);
        Actor_unk55_4(14);
        Actor_unk56_4(15);
        Actor_unk9_5(24, 4, 1, 1, 24, 8);
        GameFlag_Clear_1(QUERY_FLAG);
    }
    Battle_SchedShoulder_1();
}

void Scene_RunScene3b3(void)
{
    u32 i;
    u8 *record;

    Actor_unk57_4();
    if (Actor_Run() == 0) {
        *(u8 *)(Actor_unk28(0) + 85) &= 254;
        *(u8 *)(Actor_unk29(0) + 35) &= 254;
        Actor_unk58_4();
        Actor_unk59_4();
        *(u8 *)(Actor_unk30(0) + 85) |= 1;
        {
            u8 *record = Actor_unk15_4(0);
            u8 value = *(volatile u8 *)&record[35];

            record[35] = (u8)(value | 1);
        }
    }
    Actor_unk60_4();
}

void Scene_RunScene3b3(s32 a0)
{
    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Actor_unk2_4();
    Actor_unk61_4();
    *(s32 *)(rec7 + 108) = 0x200a0b9;
    Actor_unk10_5(20, 14, 1, 1, (*(s32 *)(rec7 + 8) >> 20), (*(s32 *)(rec7 + 16) >> 20));
    Actor_unk62_4((a0 + 0x1f5));
    Actor_unk8(a0, 0x200ad64);
    Actor_unk63_4();
}

void Scene_RunScene3b3(void)
{
    u32 i;
    s32 rec7;
    s32 record;
    u8 *p6;

    rec7 = Actor_unk9(0);
    record = Actor_unk31(13);
    p6 = *(s32 *)0x03001f30;
    if ((*(s32 *)(record + 8) >> 20) == (*(s32 *)(rec7 + 8) >> 20)) {
        if ((*(s32 *)(record + 16) >> 20) != (*(s32 *)(rec7 + 16) >> 20)) {
            goto L_02002198;
        }
        Actor_unk3_2(0x203);
        p6[53] = 1;
    } else {
        L_02002198:;
        Actor_unk4_2(0x203);
    }
}

void Scene_RunScene3b3(s32 a0)
{
    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Actor_unk32();
    if (Actor_unk10((a0 + 0x1f5)) != 0) {
        Actor_unk64_4(rec7, 5);
        *(s32 *)(rec7 + 108) = 0x200a0b9;
        Actor_unk11_5(20, 14, 1, 1, (*(s32 *)(rec7 + 8) >> 20), (*(s32 *)(rec7 + 16) >> 20));
        Actor_unk65_4(a0, 0x200ad64);
    }
}

/* overlays/scene/actor/staged_nav/scene_state_interaction.c */
extern u8 *gIw2;

u8 *Actor_unk66_4();

/* Complete scene/entity linker through return and its sole pool word. */
void State_LinkActorZeroToWork24(void)
{
    u8 *obj = Actor_unk66_4(0);
    *(u8 **)(gIw2 + 24) = obj;
    obj[98] = 1;
}

/* overlays/scene/actor/staged_nav/staged_actor_navigation.c */

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

extern s32 gOv19[];
extern s32 Actor_Far[];

struct StagedActor *GetStagedActor(s32 arg0);
struct StagedActor *FindNextStagedActor(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *FindBlockingStagedActor(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *FindElevatedBlockingStagedActor(s32 *arg0, struct StagedActor *arg1);

Ent *Actor_unk67_4(Desc *, Ent *);

u8 *Actor_unk68_4();

u8 *Actor_unk69_4();

/* The size test is unsigned, so out20 and out16 are u32. The two rec words
 * that travel on the stack are read into their own locals, which puts both
 * loads before both stores. The +0x23 flag set takes its mask from a local
 * assigned first, which fixes the register the mask lands in. */

/*
 * Apply a placement query and tag the object. Actor_unk33 fills out20 and
 * out16, a 24-byte record of which only rec[2] and rec[4] are read back, and
 * two further out-params whose written values are never read -- only the
 * pointers matter. On success the object is tagged at +0x23 and one of two
 * near-identical finishing calls runs; on failure the object is untouched.
 */

/* The callees are declared at their in-overlay entry points. Declaring them at
 * their veneer addresses in ROM space would route every call through a
 * veneer. */

/*
 * Apply a placement query to an actor. Actor_unk34 is an out-param helper:
 * it fills out20 and out16, a 24-byte record of which only rec[2] and rec[4]
 * are read back, and out12 and out8 passed on the stack. Its field semantics
 * are not established. On success the values thread unchanged into the two
 * finishing calls in that shape; on failure the object is left untouched.
 */

void SpawnConfiguredEffect(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct ConfiguredEffectOptions *options)
{
}

void AdvanceStagedActorPair(void)
{
    extern u32 Actor_Far2[];

    s32 destination[3];
    struct StagedActor *lead_actor;
    struct StagedActor *next_actor;
    struct StagedActor *blocking_actor;
    s32 facing_index;
    u32 step;
    s32 move_rate;
    s32 zero;

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

    zero = next_actor->transition_busy;
    if (zero != 0) return;

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
    next_actor->unknown_24 = zero;
    next_actor->unknown_2c = zero;
    lead_actor->unknown_38 = 0x80000000;
    lead_actor->unknown_40 = 0x80000000;
    lead_actor->unknown_24 = zero;
    lead_actor->unknown_2c = zero;
    lead_actor->x.value = lead_actor->x.parts.cell << 16;
    lead_actor->z.value = lead_actor->z.parts.cell << 16;
    SetStagedActorTransition(lead_actor, 1);
}

s32 stop_blocked_actor_motion(Ent *a)
{
    extern s32 Actor_Far2[];

    Desc d;
    u32 idx;
    s32 m;
    Ent *r;

    idx = a->unk6 >> 12;
    m = Actor_Far2[idx];
    d.unk0 = a->unk8 + (m & 0xffff0000);
    d.unk4 = a->unkC;
    m = m << 16;
    d.unk8 = a->unk10 + m;
    r = Actor_unk67_4(&d, a);
    if (r != 0) {
        u32 i = 0;
        s32 v = *r->unk50->unk28;
        s32 *p = gOv19;

        do {
            if (v == *p++) goto done;
            i++;
        } while (i <= 5);
        a->unk24 = 0;
        a->unk2C = 0;
        a->unk38 = 0x80000000;
        a->unk40 = 0x80000000;
    }
    m = Actor_Far2[idx];
    d.unk0 = a->unk8 + (m & 0xffff0000);
    d.unk4 = a->unkC;
    m = m << 16;
    d.unk8 = a->unk10 + m;
    if (Actor_EntOp(a, &d) > 0) {
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
    extern s32 Actor_Far2[];

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
    obj = (u8 *)Actor_unk8_3(&sel, a + 1, a);
    if (obj == 0)
        return 0;
    p = obj + 0x22;
    *p = 2;
    cnt = 0;
    E = (u8 *)Actor_Far;
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
        d = Actor_Far2[sel] & 0xffff0000;
        oc = obj;
        b[0] = *(s32 *)(oc + 8) + d;
        w = *(s32 *)(oc + 12);
        b[1] = w;
        b[2] = *(s32 *)(oc + 16) + (Actor_Far2[sel] << 16);
        a[3] = w;
    }
    for (;;) {
        s32 j, k;
        {
            u8 *E2 = (u8 *)Actor_Far;
            s32 o2 = a[0] << 4;
            o2 += 4;
            a[4] = b[2] + (*(s32 *)(E2 + o2) << 16);
        }
        j = 0;
        while (j < ny) {
            {
                u8 *E3 = (u8 *)Actor_Far;
                a[2] = b[0] + (*(s32 *)(E3 + (a[0] << 4)) << 16);
            }
            k = 0;
            while (k < nx) {
                s32 *w2 = a + 2;
                if (Actor_Apply5(obj, w2) == 2)
                    goto found;
                w2[0] = w2[0] + 0x100000;
                k++;
            }
            a[4] = a[4] + 0x100000;
            j++;
        }
        cnt++;
        buf[0] = buf[0] + (Actor_Far2[sel] & 0xffff0000);
        buf[2] = buf[2] + (Actor_Far2[sel] << 16);
    }
found:
    *p = 0;
    ret = 0;
    if (cnt != 0) {
        s32 d = Actor_Far2[sel];
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

s32 Actor_ApplyPlacementQueryAndTag(u8 *no)
{
    u8 *obj = Actor_unk68_4(no);
    u32 out20, out16;
    s32 out12, out8;
    s32 rec[6];
    s32 r2, r4;
    u8 mask;

    if (Actor_unk33(no, &out20, &out16, rec, &out12, &out8) == 0) {
        return 0;
    }

    r2 = rec[2];
    r4 = rec[4];
    Actor_unk72_4(2, 2, out20, out16, r2, r4);

    Actor_unk73_4(obj, 4);
    mask = 2;
    obj[0x23] = obj[0x23] | mask;

    if (out20 > out16) {
        Actor_unk74_4(70, 40, rec[2] + 32, rec[4] + 2, out20, out16);
    } else {
        Actor_unk75_4(68, 40, rec[2] + 32, rec[4] + 2, out20, out16);
    }

    return 1;
}

s32 Actor_ApplyPlacementQuery(u8 *no)
{
    u8 *obj = Actor_unk69_4(no);
    s32 out20, out16, out12, out8;
    s32 rec[6];

    if (Actor_unk34(no, &out20, &out16, rec, &out12, &out8) == 0) {
        return 0;
    }

    {
        s32 x = out12 + rec[2];
        s32 z = out8 + rec[4];

        Actor_unk76_4(x, z, out20, out16, rec[2], rec[4]);
        Actor_unk77_4(0, rec[2], rec[4], out20, out16, 255);
    }

    Actor_unk78_4(obj, 1);
    obj[0x23] &= 0xfd;

    return 1;
}

/* overlays/scene/actor/staged_nav/state_update.c */
typedef struct {
    s32 v[6];
} PlacementResult;


u8 *Actor_unk79_4();

/*
 * Scene state reset for overlay resource_3b3. The callee name refers to its
 * own call word rather than to a shared runtime address.
 */

void State_ApplyPlacementResult(void)
{
    PlacementResult out;

    Actor_unk80_4();
    if (Actor_unk35(&out) != 0)
        Actor_unk8_2(out);
    Actor_unk81_4();
}

/*
 * Clears gIw2[+24] and one flag byte on the object returned by
 * Actor_unk79_4. The 28-byte owner at 0x0200209c includes its one pool
 * word, the gIw2 pointer.
 */
void State_ClearWord24AndObjectByte62(void)
{
    u8 *obj = Actor_unk79_4(0);

    *(s32 *)(gIw2 + 24) = 0;
    obj[0x62] = 0;
}

/* overlays/scene/actor/staged_nav/wait_height_below_limit.c */
/*
 * Staged actor height wait for overlay resource_3b3. The callee name refers
 * to its own call word rather than to a shared runtime address.
 */

/*
 * Polls for up to sixty ticks until the height at +12 falls to the target
 * at +20 or to limit, then clears +0x28 and parks +0x3c. The height is not
 * mirrored back from +20 afterwards. The owner at 0x02000da8 is 52 bytes
 * and carries no pool.
 */
void Actor_WaitHeightBelowLimit(u8 *obj, s32 limit)
{
    s32 cnt = 60;

    for (;;) {
        if (cnt == 0) {
            break;
        }
        Actor_RunHeightBelowLimit(1);
        if (*(s32 *)(obj + 12) <= *(s32 *)(obj + 20)) {
            break;
        }
        if (*(s32 *)(obj + 12) <= limit) {
            break;
        }
        cnt--;
    }

    *(u32 *)(obj + 0x28) = 0;
    *(u32 *)(obj + 0x3c) = 0x80000000;
}

/* overlays/scene/actor/staged_nav/step_down_until_clamp.c */
/*
 * Steps a record down by a tenth of a unit per frame until it reaches the
 * clamp at 0x1999 -- resource_3b3.  The eighty-byte owner includes its
 * alignment halfword and four pool words: the clamp 0x1999, -0x1999,
 * -0xcccc and the loop bound 0x1998, which is one less than the clamp.
 * None of them is an address, and each is reached only through a
 * pc-relative load.
 */

u8 *Actor_RunDownUntilClamp();           /* Record fetch, returns the record. */
void Actor_unk2_4DownUntilClamp();          /* Dispatch stub table entry 0. */

/*
 * Each Func_ name labels the call word of one call site rather than a
 * runtime address.  The first call is made before r0 is disturbed, so the
 * index is passed straight through instead of being materialised again.  The
 * two exits differ: the thirty-two frame cap returns without pinning, while
 * the clamp path pins the record to exactly 0x1999.
 */
void StagedActor_StepDownUntilClamp(s32 index)
{
    u8 *obj = Actor_RunDownUntilClamp(index);
    u32 cnt;

    obj[0x55] = 0;

    cnt = 0;
    for (;;) {
        if (cnt > 31) return;
        Actor_unk2_4DownUntilClamp(1);
        *(s32 *)(obj + 28) += -0x1999;
        *(s32 *)(obj + 12) += -0xcccc;
        cnt++;
        if (*(s32 *)(obj + 28) <= 0x1998) {
            *(s32 *)(obj + 28) = 0x1999;
            return;
        }
    }
}

/* overlays/scene/actor/staged_nav/apply_counter_low_bits_as_mode.c */
/* Apply the actor's low four counter bits as its animation mode. */
#include "types.h"
#include "scene.h"

s32 Actor_ApplyCounterLowBitsAsMode(u8 *actor)
{
    Actor_ApplyBitsAsMode(actor, *(u16 *)(actor + 100) & 15);
    return 0;
}

/* overlays/scene/actor/staged_nav/run_transition_or_fallback.c */
/* Begin a scene, attempt the forward transition, and fall back to pushing the
 * obstructing actor when the transition cannot run.  Complete 28-byte owner
 * from the prologue at 0x02001528 through return/alignment at 0x02001543. */

void Scene_RunTransitionOrFallback(void)
{
    Actor_RunTransitionOrFallback();
    if (Actor_unk2_4TransitionOrFallback() == 0)
        Actor_unk3_4TransitionOrFallback();
    Actor_unk4_4TransitionOrFallback();
}

/* overlays/scene/actor/staged_nav/run_single_step.c */
void Scene_RunSingleStep(void)
{
    Actor_RunSingleStep();
}

/* Scene probe trio at 0x02000314 / 0x02000350 / 0x02000528. */
typedef s32 (*IwramIntegerSquareRoot)(s32);

s32 CalculateFixedPointPositionDistance(s32 *first_position, s32 *second_position)
{
    s32 delta_x = (*first_position++ - *second_position++) >> 16;
    s32 delta_y = (*first_position++ - *second_position++) >> 16;
    s32 delta_z = (*first_position - *second_position) >> 16;
    s32 delta_x_squared = delta_x *delta_x;
    s32 delta_y_squared = delta_y *delta_y;
    s32 delta_z_squared = delta_z *delta_z;

    return ((IwramIntegerSquareRoot) 0x030001D8)(delta_x_squared + delta_y_squared + delta_z_squared);
}

s32 *FindSceneActorAtFixedPointPosition(s32 *arg0)
{
    s32 **slots = (s32 **)(gWork + 0x14);
    u32 i;

    for (i = 8; i <= 65; i++) {
        s32 *p = slots[i];

        if ((arg0[0] >> 20) == (p[2] >> 20)
            && (arg0[1] / 0x10000) == (p[3] / 0x10000)
            && (arg0[2] >> 20) == (p[4] >> 20)) {
            return p;
        }
    }
    return 0;
}

extern u8 *gCam;
extern u8 gUnk[];

s32 FillSceneGridAttributeRectangle(u32 arg0, s32 arg1, s32 arg2, u32 arg3, u32 arg4, s32 arg5)
{
    u8 *g = gCam;
    u8 *base;
    u32 i;
    u32 j;

    if (g != 0) {
        if (arg0 <= 2) {
            u32 off = arg0 * 48 + 304;

            base = *(u8 **)(g + off);
        } else {
            base = gUnk;
        }
        base += (arg1 + (arg2 << 7)) * 4;
        for (i = 0; i < arg4; i++) {
            u8 *p = base + (i << 9);

            for (j = 0; j < arg3; j++) {
                p[2] = (u8)arg5;
                p += 4;
            }
        }
    }
    return 0;
}

/* overlays/shared/set_effect_record_mode.c */
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

void SetEffectRecordMode(struct EffectWork *work, s32 mode)
{
    work->record->mode = mode;
}
