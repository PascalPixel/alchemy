#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/actor/staged_nav/staged_nav.h"
#include "staged_actor_movement.h"
#include "run_staged_actor_movement_and_redraw_body.inc"
#include "create_configured_overlay_object.h"
#include "create_configured_overlay_object_body.inc"
#include "configured_effect_spawn.h"
#include "staged_actor.h"
#include "configured_effect_spawn_body.inc"

/* overlays/scene/actor/staged_nav/actor_position.c */
u8 *Actor_Run3(s32 slot);
u8 *Actor_Run4();

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
    u8 *ref = Actor_Run3(0);

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
    u8 *obj = Actor_Run4(10);
    s32 x;
    s32 z;

    Actor_Run5();

    x = *(s32 *)(obj + 8) >> 20;
    z = *(s32 *)(obj + 16) >> 20;

    Actor_Check11(2, x, z, 1, 1, 0);
    Actor_Run6();
}

/* overlays/scene/actor/staged_nav/actor_presentation.c */

/* Contiguous unnamed leaf-owner run for resource_3b3. */

/* Complete 12-byte actor-11 wrapper before 0x02002150. */
void Scene_RunActor11Step(void)
{
    Actor_Do5(11);
}

/* Complete 12-byte actor-12 wrapper before 0x0200215c. */
void Scene_RunActor12Step(void)
{
    Actor_Do6(12);
}

/* overlays/scene/actor/staged_nav/actor_search.c */
typedef struct {
    u8 pad[9];
    u8 low : 2;
    u8 mode : 2;
    u8 rest : 4;
} Handle;

u8 *Actor_Run7();
u8 *Actor_Run8();
u8 *Actor_Run9();

void Actor_CheckActors8To11NearSlotZero(void)
{
    u8 *hero = Actor_Run7(0);
    u32 selector = 8;
    u8 *actor;

loop:
    actor = Actor_Run8(selector);

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
        Actor_Run10(0, handle->mode);
    }
    goto done;

mark_and_continue:
    {
        u8 *mark = Actor_Run9(0) + 35;
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
        Actor_Do7(136);

    d = descriptor;
    *(s32 *)(d + 4) = 10;
    *(s32 *)(d + 8) = 0x8000;
    *(s32 *)(d + 12) = 0x8000;
    *(s32 *)(d + 16) = 0x19999;
    *(s32 *)(d + 20) = 0x19999;
    draw = (u32)Actor_Check12();
    mask = 0x0ffff000;
    mask &= draw;
    *(u16 *)(d + 32) = (u16)mask;
    *(s32 *)(d + 36) = 0x020093b1;

    draw = (u32)Actor_Check13();
    spread = -((s32)((draw * 5) >> 16) * 0x10000 + 0x60000);
    spread /= 2;
    draw = (u32)Actor_Check14();
    secondary = -((s32)((draw * 5) >> 16) * 0x10000 + 0x50000);

    Actor_SetRect12(0x01440000, 0x00300000, 0x00e40000, spread,
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

    x = *(s32 *)(actor + 8) + (((s32)(((u32)Actor_Check15() * 17) >> 16) - 8) << 16);
    y = *(s32 *)(actor + 12) + ((s32)(((u32)Actor_Check16() * 17) >> 16) << 16);
    z = *(s32 *)(actor + 16) + (((s32)(((u32)Actor_Check17() * 17) >> 16) - 8) << 16);
    scale = Actor_Apply((s32)(((u32)Actor_Check18() * 5) >> 16) * 0x10000 + 0x30000, 10);

    Actor_SetRect13(x, y, z, 0, scale, (s32)phase, 0x00090001, p);
    return 0;
}

/* overlays/scene/actor/staged_nav/move_and_redraw.c */
#define Value Value3

u8 *Actor_Run11(void);

/*
 * Poll an overlay object until it settles, then reset it -- resource_3b3.
 */

/* Declared without a prototype; the call site passes one argument. */

void *OvObj_PrepareObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    u8 *obj = Actor_Run12(arg3, arg0, arg1, arg2);

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
 * +0x28, set +0x3c, and mirror +20 back into +12. Actor_Run13(1) is taken
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
        Actor_Run13(1);
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
    u8 *obj = Actor_Run11();
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

extern u8 gCell[];
extern u8 gWork[];
extern u8 gCell2[][2];
extern u8 gOv18[];

u8 *Actor_Run14();

u8 *Scene_GetRecord_2();

u8 *Scene_GetRecord_4();

u8 *Actor_Run15();

/* Record id used for the single record configured below in either branch. */

/* Flag/id value passed to the query call and to the two closing calls, and
 * written into the flag byte at +85 when the query call returned zero. */

void Scene_RunPrimarySequence(void)
{
    s32 Actor_Check19();

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

    rec = Actor_Check20(0);
    flag = *(volatile s32 *)0x03001e40 & 3;
    if (flag == 0) {
        base = slot16;
        *(s32 *)(base + 4) = 10;
        *(s32 *)(base + 8) = 0xb333;
        *(s32 *)(base + 12) = 0xb333;
        v1 = Actor_Check21();
        p6 = *(volatile s32 *)(rec + 8) + ((((u32)((v1 << 4) + v1) >> 16) - 8) << 16);
        v2 = Actor_Check22();
        p5 = *(volatile s32 *)(rec + 16) + ((((u32)((v2 << 4) + v2) >> 16) - 8) << 16);
        v3 = Actor_Check23();
        record = Actor_Check24((((u32)((v3 << 2) + v3) >> 16) << 16) + 0x30000, 10);
        Actor_SetRect(p6, *(volatile s32 *)(rec + 12), p5, 0, record, flag, 0x90001, (s32)base);
    }
}

    u8 *rec;
    u8 *pflag;
    s32 saved;
    s32 mode;
    s32 *p;
    s32 buf[3];

    rec = Actor_Run14(0);
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
    if (Actor_Check2((s32)p, (s32)rec) != 0) {
        goto reject;
    }
    p[0] = (*(volatile s32 *)(rec + 8) & -0x100000) + 0x80000;
    p[1] = *(volatile s32 *)(rec + 12);
    p[2] = (*(volatile s32 *)(rec + 16) & -0x100000) + 0x80000;
    Actor_Place2(0x200000, mode, (s32)p);
    if (Actor_Check3((s32)p, (s32)rec) != 0) {
        goto reject;
    }
    if (Actor_Check4((s32)rec, (s32)p) != 0) {
        goto reject;
    }
    Actor_Run16();
    Actor_Run17((s32)rec, 6);
    Actor_Run18(6);
    Actor_Run19(152);
    Actor_Run20((s32)rec, 7);
    *(volatile s32 *)(rec + 48) = 0x30000;
    *(volatile s32 *)(rec + 52) = 0x20000;
    *(volatile s32 *)(rec + 40) = 0x40000;
    *pflag &= 126;
    Actor_Run21((s32)rec, 0);
    Actor_Run22(0, *(s16 *)((u8 *)p + 2), *(s16 *)((u8 *)p + 10));
    Actor_Run23((s32)rec, 6);
    Actor_Run24((s32)rec, 1);
    *pflag = saved;
    Actor_Run25();
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
    step = Actor_Check5(v68, 10);
    *(s32 *)(a0 + 68) = v68 - step;
    step = Actor_Check25(v72, 3);
    *(s32 *)(a0 + 72) = v72 - step;
    step = Actor_Check26(v76, 10);
    *(volatile s32 *)(a0 + 76) = v76 - step;
    *(volatile s32 *)(a0 + 24) += *(volatile s32 *)(a0 + 48);
    *(volatile s32 *)(a0 + 28) += *(s32 *)(a0 + 52);
    *(u16 *)(*(volatile s32 *)(a0 + 80) + 30) += *(u16 *)(a0 + 100);
}

void Scene_RunScene3b3SequenceA(void)
{
    s32 record;
    s32 count;

    record = Actor_Check6(0x200);
    if (record == 0) {
        Actor_Run26(10, 19, 16, 5, record, 10, 31);
        Actor_Run27(10, 51, 16, 5, 1, 10, 31);
        Actor_Run28(42, 51, 16, 5, 2, 10, 31);
    } else {
        Actor_Run29(10, 19, 16, 5, 0, 10, 31);
        Actor_Run30(10, 83, 16, 5, 1, 10, 31);
        Actor_Run31(42, 83, 16, 5, 2, 10, 31);
    }
    *(volatile s32 *)gOv18 = 0;
    Actor_Check27(0x20095cd, 0xc80);
    Actor_Run32(1);
    Actor_Run33(1, 0, 0x2009579);
    Actor_Run34(231);
    *(volatile s32 *)gOv18 = 0;
    do {
        Actor_Run35(1);
        count = *(volatile s32 *)gOv18;
        count = count + 1;
        *(volatile s32 *)gOv18 = count;
    } while (count <= 100);
    Actor_Do(0x121);
    if (Actor_Check7(0x200) == 0) {
        Actor_SetRect2(0, 32, 32, 0, 32, 32);
        Actor_SetRect3(32, 32, 64, 0, 32, 32);
    } else {
        Actor_SetRect4(0, 64, 32, 0, 32, 32);
        Actor_SetRect5(32, 64, 64, 0, 32, 32);
    }
    Actor_Run36(1);
    Actor_Run37(1, 0, 0);
    Actor_Run38(1);
    Actor_Do2(0x20095cd);
    Actor_Run39();
    Actor_Run40(30);
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
        Actor_Place3(0x200ada8, 84, 24);
        Battle_WaitMode0_1(30);
        Audio_PlayCue_2(240);
        Motion_SetActionVariant_1(REC_ID, 1);
        /* Flag byte at +85: cleared, since queried is zero here. */
        *(u8 *)(Scene_GetRecord_1(REC_ID) + 85) = queried;
        record = Scene_GetRecord_2(REC_ID);
        *(s32 *)(record + 12) = -0x200000;
        Motion_SetHPosTerrain_1(REC_ID, 0x1100000, 0x1a00000);
        Object_SetModeById_1(REC_ID, 1);
        Actor_Place4(0x200adfc, 80, 24);
        Actor_Place5(0x200ae50, 80, 28);
        Actor_SetRect6(65, 40, 16, 27, 2, 4);
        Actor_Run41();
        Actor_Run42(9);
        Actor_Run43(10);
        Actor_Run44(11);
        Actor_Run45(12);
        Actor_Run46(13);
        Actor_Run47(14);
        Actor_Run48(15);
        Actor_SetRect7(24, 3, 1, 1, 24, 8);
        GameFlag_Set_1(QUERY_FLAG);
    } else {
        Audio_PlayCue_3(232);
        Actor_Place6(0x200add2, 84, 24);
        Battle_WaitMode0_2(30);
        Audio_PlayCue_4(230);
        /* Flag byte at +85: cleared unconditionally in this branch. */
        *(u8 *)(Scene_GetRecord_3(REC_ID) + 85) = 0;
        record = Scene_GetRecord_4(REC_ID);
        *(s32 *)(record + 12) = -0x200000;
        Motion_SetHPosTerrain_2(REC_ID, 0x1100000, 0x1b40000);
        Object_SetModeById_2(REC_ID, 2);
        Actor_SetRect8(65, 45, 16, 27, 2, 4);
        Actor_Place7(0x200ae26, 80, 24);
        Actor_Run49();
        Actor_Run50(9);
        Actor_Run51(10);
        Actor_Run52(11);
        Actor_Run53(12);
        Actor_Run54(13);
        Actor_Run55(14);
        Actor_Run56(15);
        Actor_SetRect9(24, 4, 1, 1, 24, 8);
        GameFlag_Clear_1(QUERY_FLAG);
    }
    Battle_SchedShoulder_1();
}

void Scene_RunScene3b3(void)
{
    u32 i;
    u8 *record;

    Actor_Run57();
    if (Actor_Run() == 0) {
        *(u8 *)(Actor_Check28(0) + 85) &= 254;
        *(u8 *)(Actor_Check29(0) + 35) &= 254;
        Actor_Run58();
        Actor_Run59();
        *(u8 *)(Actor_Check30(0) + 85) |= 1;
        {
            u8 *record = Actor_Run15(0);
            u8 value = *(volatile u8 *)&record[35];

            record[35] = (u8)(value | 1);
        }
    }
    Actor_Run60();
}

void Scene_RunScene3b3(s32 a0)
{
    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Actor_Run2();
    Actor_Run61();
    *(s32 *)(rec7 + 108) = 0x200a0b9;
    Actor_SetRect10(20, 14, 1, 1, (*(s32 *)(rec7 + 8) >> 20), (*(s32 *)(rec7 + 16) >> 20));
    Actor_Run62((a0 + 0x1f5));
    Actor_Check8(a0, 0x200ad64);
    Actor_Run63();
}

void Scene_RunScene3b3(void)
{
    u32 i;
    s32 rec7;
    s32 record;
    u8 *p6;

    rec7 = Actor_Check9(0);
    record = Actor_Check31(13);
    p6 = *(s32 *)0x03001f30;
    if ((*(s32 *)(record + 8) >> 20) == (*(s32 *)(rec7 + 8) >> 20)) {
        if ((*(s32 *)(record + 16) >> 20) != (*(s32 *)(rec7 + 16) >> 20)) {
            goto L_02002198;
        }
        Actor_Do3(0x203);
        p6[53] = 1;
    } else {
        L_02002198:;
        Actor_Do4(0x203);
    }
}

void Scene_RunScene3b3(s32 a0)
{
    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Actor_Check32();
    if (Actor_Check10((a0 + 0x1f5)) != 0) {
        Actor_Run64(rec7, 5);
        *(s32 *)(rec7 + 108) = 0x200a0b9;
        Actor_SetRect11(20, 14, 1, 1, (*(s32 *)(rec7 + 8) >> 20), (*(s32 *)(rec7 + 16) >> 20));
        Actor_Run65(a0, 0x200ad64);
    }
}

/* overlays/scene/actor/staged_nav/scene_state_interaction.c */
extern u8 *gIw2;

u8 *Actor_Run66();

/* Complete scene/entity linker through return and its sole pool word. */
void State_LinkActorZeroToWork24(void)
{
    u8 *obj = Actor_Run66(0);
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

Ent *Actor_Run67(Desc *, Ent *);

u8 *Actor_Run68();

u8 *Actor_Run69();

/* The size test is unsigned, so out20 and out16 are u32. The two rec words
 * that travel on the stack are read into their own locals, which puts both
 * loads before both stores. The +0x23 flag set takes its mask from a local
 * assigned first, which fixes the register the mask lands in. */

/*
 * Apply a placement query and tag the object. Actor_Check33 fills out20 and
 * out16, a 24-byte record of which only rec[2] and rec[4] are read back, and
 * two further out-params whose written values are never read -- only the
 * pointers matter. On success the object is tagged at +0x23 and one of two
 * near-identical finishing calls runs; on failure the object is untouched.
 */

/* The callees are declared at their in-overlay entry points. Declaring them at
 * their veneer addresses in ROM space would route every call through a
 * veneer. */

/*
 * Apply a placement query to an actor. Actor_Check34 is an out-param helper:
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

s32 Actor_Run70(Ent *a)
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
    r = Actor_Run67(&d, a);
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

s32 Actor_Run71(s32 *a)
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
    obj = (u8 *)Actor_Place8(&sel, a + 1, a);
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
    u8 *obj = Actor_Run68(no);
    u32 out20, out16;
    s32 out12, out8;
    s32 rec[6];
    s32 r2, r4;
    u8 mask;

    if (Actor_Check33(no, &out20, &out16, rec, &out12, &out8) == 0) {
        return 0;
    }

    r2 = rec[2];
    r4 = rec[4];
    Actor_Run72(2, 2, out20, out16, r2, r4);

    Actor_Run73(obj, 4);
    mask = 2;
    obj[0x23] = obj[0x23] | mask;

    if (out20 > out16) {
        Actor_Run74(70, 40, rec[2] + 32, rec[4] + 2, out20, out16);
    } else {
        Actor_Run75(68, 40, rec[2] + 32, rec[4] + 2, out20, out16);
    }

    return 1;
}

s32 Actor_ApplyPlacementQuery(u8 *no)
{
    u8 *obj = Actor_Run69(no);
    s32 out20, out16, out12, out8;
    s32 rec[6];

    if (Actor_Check34(no, &out20, &out16, rec, &out12, &out8) == 0) {
        return 0;
    }

    {
        s32 x = out12 + rec[2];
        s32 z = out8 + rec[4];

        Actor_Run76(x, z, out20, out16, rec[2], rec[4]);
        Actor_Run77(0, rec[2], rec[4], out20, out16, 255);
    }

    Actor_Run78(obj, 1);
    obj[0x23] &= 0xfd;

    return 1;
}

/* overlays/scene/actor/staged_nav/state_update.c */
typedef struct {
    s32 v[6];
} PlacementResult;

extern u8 *gIw2;

u8 *Actor_Run79();

/*
 * Scene state reset for overlay resource_3b3. The callee name refers to its
 * own call word rather than to a shared runtime address.
 */

void State_ApplyPlacementResult(void)
{
    PlacementResult out;

    Actor_Run80();
    if (Actor_Check35(&out) != 0)
        Actor_Do8(out);
    Actor_Run81();
}

/*
 * Clears gIw2[+24] and one flag byte on the object returned by
 * Actor_Run79. The 28-byte owner at 0x0200209c includes its one pool
 * word, the gIw2 pointer.
 */
void State_ClearWord24AndObjectByte62(void)
{
    u8 *obj = Actor_Run79(0);

    *(s32 *)(gIw2 + 24) = 0;
    obj[0x62] = 0;
}
