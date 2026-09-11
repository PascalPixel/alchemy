#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/dialogue/facing_target/facing_target.h"
#include "facing_object.h"

/* overlays/scene/dialogue/facing_target/actor_effect_motion.c */
/* The anchor pointer is read before the frame counter is stored: the reference
 * hoists `ldr r6,[r5,#104]` above the `strh`, and only that source order
 * reproduces it. */
struct Actor {
    u8 filler00[8];
    s32 x;                          /* 0x08 */
    s32 y;                          /* 0x0c, only ever advanced by 0x10000 */
    s32 z;                          /* 0x10 */
    u8 filler14[4];
    s32 amplitude_x;                /* 0x18 */
    s32 amplitude_y;                /* 0x1c */
    u8 filler20[0x44];
    u16 frame;                      /* 0x64 */
    u8 filler66[2];
    struct Actor *anchor;  /* 0x68 */
};

void OvObj_UpdateArcFromAnchor(struct Actor *obj)
{
    struct Actor *anchor;
    s32 frame;
    s32 amp;

    anchor = obj->anchor;
    obj->frame = (u16)(obj->frame + 1);
    frame = (s16)obj->frame;

    if (frame > 31) {
        Talk_Do7(obj);
        return;
    }

    amp = Talk_Check9(frame << 10);
    obj->amplitude_x = amp;
    obj->amplitude_y = -amp;
    obj->x = anchor->x;
    obj->y += 0x10000;
    obj->z = anchor->z - (0x10000 - amp) * 5 + 0x100000;
}

/* overlays/scene/dialogue/facing_target/actor_facing.c */

s32 Actor_UpdateFacingTowardTarget(struct FacingObject *object)
{
    s32 delta;
    u16 old;
    s32 tgt;
    struct FacingObject *target;

    target = object->facing_target;
    if (target != NULL) {
        object->facing_flags = (u8)(0xFE & object->facing_flags);
        tgt = (u16)CalculateFacingAngle(target->position_z - object->position_z, target->position_x - object->position_x);
        old = object->facing;
        delta = (s16)(tgt - old);
        if (delta != 0) {
            if (delta > 0x1000) {
                delta = 0x1000;
            }
            /* The loader relocates the stored pool word to -0x1000. */
            if (delta < -0x1000) {
                delta = -0x1000;
            }
            object->facing = (u16)(old + delta);
        }
    }
    return 1;
}

/* overlays/scene/dialogue/facing_target/actor_presentation.c */

void Dialogue_RunActorFourteenDialogue11AA(void)
{
    void *work;

    Talk_Check10();
    Talk_Place8(0xE, 0, 0xA);
    Talk_Check11(0x11AA);
    Talk_Apply(0xE, 0);
    if (Talk_Apply2(0, 0) == 0) {
        Talk_Apply3(0xE, 0);
    } else {
        work = *(void **)0x03001EBC;
        FIELD_AT_OFFSET(work, u16 *, 0x1D8) = (u16)(FIELD_AT_OFFSET(work, u16 *, 0x1D8) + 1);
        Talk_Apply4(0xE, 0);
    }
    Talk_Check12();
}

void ActorDraw_SetTwoSceneCells(void)
{
    {
        s32 extent = 2;

        Talk_SetRect(22, 85, 25, 85, extent, extent);
    }
    {
        s32 extent = 25;

        Talk_SetRect2(25, 15, 2, 2, extent, extent);
    }
}

/* overlays/scene/dialogue/facing_target/effect.c */
/* The anchor pointer is read before the frame counter is stored: the reference
 * hoists `ldr r6,[r5,#104]` above the `strh`, and only that source order
 * reproduces it. */
struct Actor2 {
    u8 filler00[8];
    s32 x;                          /* 0x08 */
    s32 y;                          /* 0x0c, only ever advanced by 0x10000 */
    s32 z;                          /* 0x10 */
    u8 filler14[4];
    s32 amplitude_x;                /* 0x18 */
    s32 amplitude_y;                /* 0x1c */
    u8 filler20[0x44];
    u16 frame;                      /* 0x64 */
    u8 filler66[2];
    struct Actor2 *anchor;  /* 0x68 */
};

extern s32 gIw;

void Effect_UpdateObjectByFrameParity(u8 *obj)
{
    if ((gIw & 2) != 0) {
        Talk_Apply5(obj, 7);
    } else {
        Talk_Apply6(obj, 0);
    }
    if ((gIw & 15) == 0) {
        Talk_Do8(obj);
    }
}

void Effect_UpdateAnchoredRiseFrame(struct Actor2 *self)
{
    struct Actor2 *anchor;
    s32 frame;
    s32 amplitude;

    anchor = self->anchor;
    self->frame = (u16)(self->frame + 1);
    frame = (s16)self->frame;

    if (frame > 31) {
        Talk_Do9(self);
        return;
    }

    amplitude = Talk_Check13(frame << 10);
    self->amplitude_x = amplitude;
    self->amplitude_y = amplitude;
    self->x = anchor->x;
    self->y += 0x10000;
    self->z = anchor->z + (0x10000 - amplitude) * 5 + 0x80000;
}

/* overlays/scene/dialogue/facing_target/facing_target_scene.c */
u8 *FacingTargetScene_GetScriptData(void)
{
    return (u8 *)0x02009fd0;
}

/* overlays/scene/dialogue/facing_target/overlay_object.c */

void OvObj_UpdateOnFrameParity(u8 *obj)
{
    extern volatile u32 gIw;

    if ((gIw & 1) != 0) {
        Talk_Apply7(obj, Talk_Apply8((s32)(gIw >> 1), 6));
    }
    if ((gIw & 15) == 0) {
        Talk_Do10(obj);
    }
}

void OvObj_ApplyRandomSlotOnOddFrames(s32 obj)
{
    extern volatile s32 gIw;

    if ((gIw & 1) != 0) {
        s32 slot = Talk_Apply9((u32)gIw >> 1, 6);

        Talk_Apply10(obj, slot);
    }
}

/* overlays/scene/dialogue/facing_target/scene_data.c */
extern s32 gOv[];
extern s32 gOv2[];
extern s32 gOv3[];
extern s32 gOv4[];

/* One import, four call sites, four distinct veneer addresses. */

s32 SceneData_SelectTableA1DCOrA198(void)
{
    if (Talk_Check14(0x834) != 0) {
        return 0x0200A1DC;
    }
    return 0x0200A198;
}

s32 SceneData_SelectTableByStoryFlags(void)
{
    extern u8 gCell[];

    u8 *b = gCell;
    s32 *tbl;

    if (*(s16 *)(b + 0x1c2) == 19)
        return (s32)gOv;
    if (Talk_Check15(0x87a) != 0)
        tbl = gOv2;
    else if (Talk_Check16(0x815) != 0)
        tbl = gOv3;
    else
        tbl = gOv4;
    Talk_Check17((s32)tbl);
    return (s32)tbl;
}

s32 SceneData_SelectTableByStoryFlagsB(void)
{
    extern s16 gCell[];

    if (gCell[225] == 19) {
        if (Talk_Check18(0x950) != 0) {
            return 0x0200AC5C;
        }
        return 0x0200AB9C;
    }

    if (Talk_Check19(0x834) != 0) {
        return 0x0200A5A8;
    }
    if (Talk_Check20(0x87A) != 0) {
        return 0x0200A980;
    }
    if (Talk_Check21(0x815) != 0) {
        return 0x0200A800;
    }
    return 0x0200A6B0;
}

/* overlays/scene/dialogue/facing_target/scene_dialogue.c */

void Dialogue_ShowLine1C13WithActor16Steps(void)
{
    Talk_Run3();
    Talk_Place9(0x10, 0, 0xA);
    Talk_Do11(0x1C13);
    Talk_Apply11(0x10, 0);
    Talk_Place10(0x10, 0xB000, 0xA);
    Talk_Do12(0x301);
    Talk_Run4();
}

void Dialogue_RunActorThirteenDialogue(void)
{
    Talk_Run5();
    Talk_Do13(0x1C1B);
    Talk_Apply12(0xD, 0);
    Talk_Do14(0x81C);
    Talk_Run6();
}

void Dialogue_RunActor16LineAndFlag81c(void)
{
    Talk_Run7();
    Talk_Do15(0x1C1A);
    Talk_Apply13(0x10, 0);
    Talk_Do16(0x81C);
    Talk_Run8();
}

/* overlays/scene/dialogue/facing_target/scene_primary_script.c */
extern u8 gWork[];

void Scene_RunScene377(void)
{
    u32 i;
    s32 record;

    Talk_Run9();
    Talk_Run10(16, 2);
    Talk_Run11(30);
    Talk_Do(0xf5b);
    Talk_Run12(0, 16, 10);
    Talk_Run13(16, 0, 6);
    Talk_Place(16, 0x102, 0);
    Talk_Run14(16, 1);
    Talk_Run15(20);
    Talk_Run16(16, 4);
    Talk_Run17(20);
    Talk_Check(16, 0);
    if (Talk_Check2(0, 0) == 1) {
        bump_step(1);
    }
    Talk_Run18(16, 1);
    Talk_Run19(20);
    Talk_Run20(16, 0, 4);
    Talk_Run21();
}

void Scene_RunStep7BThen2(void)
{
    Talk_Do17(0x7B);
    Talk_Do18(2);
}

void Scene_RunStep7BThen4(void)
{
    Talk_Do19(0x7B);
    Talk_Do20(4);
}

void Scene_RunStep80Then5(void)
{
    Talk_Do21(0x80);
    Talk_Do22(5);
}

void Scene_RunStep7BThen6(void)
{
    Talk_Do23(0x7B);
    Talk_Do24(6);
}

void Scene_RunStep80Then7(void)
{
    Talk_Do25(0x80);
    Talk_Do26(7);
}

void Scene_RunStep7BThen10(void)
{
    Talk_Do27(0x7B);
    Talk_Do28(10);
}

void Scene_RunScene377(void)
{
    u32 i;
    s32 record;

    record = Talk_Check22(0);
    if ((u32)(*(volatile u16 *)(record + 6) + -0x2000) > 0x9000) {
        Talk_Run22(0, 13);
    } else {
        Talk_Run23();
        if (Talk_Check3(0x87a) != 0) {
            Talk_Run24(13, 2);
            Talk_Run25(13, 0, 10);
            if (Talk_Check4(0x300) == 0) {
                Talk_Do2(0x1c14);
                Talk_Run26(13, 0);
                Talk_Do3(0x300);
            }
            Talk_Do4(0x1c15);
            Talk_Check5(13, 0);
            Talk_Run27(13, 0x9000, 10);
        } else {
            if (Talk_Check6(0x815) != 0) {
                Talk_Do5(0x11a9);
            } else {
                Talk_Do6(0xf58);
            }
            Talk_Run28(13, 0);
        }
        Talk_Run29();
    }
}

void Scene_RunSupplementalSequenceOne(s32 a0)
{
    u32 i;
    u8 *rec2;
    s32 rec7;
    s32 record;
    u8 *p6;

    Talk_Run30(a0);
    Talk_Run(-1, -1, -1, 0);
    Talk_Run31(1);
    Talk_Run32(0, 0, 0);
    Talk_Place2(18, 0x1e00000, 0xca0000);
    Talk_Run33(1);
    Talk_Run34(18, 1);
    rec7 = 0;
    rec2 = Talk_Check7(22, 0x1480000, 0x20000, 0xc30000);
    rec2[85] = rec7;
    p6 = *(volatile s32 *)((s32)rec2 + 80);
    *(volatile s32 *)((s32)rec2 + 12) = 0x50000;
    p6[39] = rec7;
    {
        s32 mask = -33;
        p6[5] &= mask;
    }
    p6[9] &= 15;
    rec7 = Talk_Check8(17, 0x608);
    Talk_Run35(224);
    Talk_Run36(p6[28], 128, (rec7 + 0x400));
    Talk_Run37(17);
    {
        s32 off = 0x1c0;
        u8 *base = *(u8 *volatile *)gWork;
        s32 *slot = (s32 *)(base + off);
        off += 66;
        *slot = off;
    }
    Talk_Run38();
    Talk_Place3(18, 0x10000, 0x8000);
    Talk_Place4(18, 0x1e0, 176);
    Talk_Place5(18, 0x1a4, 164);
    Talk_Place6(18, 0x146, 185);
    Talk_Place7(18, 0x4000, 10);
    Talk_Run39((s32)rec2, 0x2009eac);
    Talk_Run40((s32)rec2);
    Talk_Run2((s32)rec2, 0x2009ecc);
    Talk_Run41((s32)rec2);
    Talk_Run42(20);
    Talk_Run43((s32)rec2);
    Talk_Run44(18, 2, 20);
    Talk_Run45(18, 0, 40);
    Talk_Run46();
    Talk_Run47();
    Talk_Run48(22);
}

void Scene_RunStep17(void)
{
    Talk_Do29(17);
    Talk_Run49();
}

/* overlays/scene/dialogue/facing_target/scene_setup.c */
extern u8 Value_000022b9;

void Scene_RunActorCueBranch(s32 object)
{
    s32 cue = (s32)&Value_000022b9;

    Talk_Do30(cue);
    Talk_Apply14(object, 0);
    if (Talk_Apply15(0, 0) == 0) {
        Talk_Do31(10);
        Talk_Do32(cue + 1);
    } else {
        Talk_Do33(cue + 2);
    }
    Talk_Apply16(object, 0);
}

void Scene_RunActor8TwoStep(void)
{
    Talk_Do34(8);
    Talk_Run50();
}

void Scene_RunSixStepSequence17e4(void)
{
    Talk_Apply17(0x94, 1);
    Talk_Apply18(8, 0x11);
    Talk_Run51();
    Talk_Do35(1);
    Talk_Run52();
    Talk_Run53();
}

/* overlays/scene/dialogue/facing_target/shared.c */

void Scene_RunSingleStep(void)
{
    Talk_Run54();
}

/* overlays/scene/dialogue/facing_target/state_update.c */
extern u8 *gWork;

void State_SetWork448To521AndRun(s32 object)
{
    if (Talk_Check23(0x834) != 0) {
        Talk_Run55();
    }
    *(s32 *)(gWork + 448) = 521;
    Talk_Run56();
    Talk_Run57();
    Talk_Do36(object);
}

void State_SetValue123Mode1(void)
{
    Talk_Do37(0x7B);
    Talk_Do38(1);
}

void State_SetValue123Mode3(void)
{
    Talk_Do39(0x7B);
    Talk_Do40(3);
}

void State_SetValue129Mode8(void)
{
    Talk_Do41(0x81);
    Talk_Do42(8);
}

void State_SetValue129Mode9(void)
{
    Talk_Do43(0x81);
    Talk_Do44(9);
}

void State_ApplyValues123And11(void)
{
    Talk_Do45(0x7B);
    Talk_Do46(11);
}

void State_SetValue140Mode0(void)
{
    Talk_Apply19(0x8C, 0);
}
