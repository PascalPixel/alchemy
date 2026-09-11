#include "types.h"
#include "facing_object.h"

/* overlays/scene/dialogue/facing_target/actor_effect_motion.c */
#define OvObj_UpdateArcFromAnchor Func_02001638
/* The anchor pointer is read before the frame counter is stored: the reference
 * hoists `ldr r6,[r5,#104]` above the `strh`, and only that source order
 * reproduces it. */
struct Actor_02004004 {
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
    struct Actor_02004004 *anchor;  /* 0x68 */
};

void Func_02002eb8(struct Actor_02004004 *);
s32 Func_02002e80(s32);

void OvObj_UpdateArcFromAnchor(struct Actor_02004004 *obj)
{
    struct Actor_02004004 *anchor;
    s32 frame;
    s32 amp;

    anchor = obj->anchor;
    obj->frame = (u16)(obj->frame + 1);
    frame = (s16)obj->frame;

    if (frame > 31) {
        Func_02002eb8(obj);
        return;
    }

    amp = Func_02002e80(frame << 10);
    obj->amplitude_x = amp;
    obj->amplitude_y = -amp;
    obj->x = anchor->x;
    obj->y += 0x10000;
    obj->z = anchor->z - (0x10000 - amp) * 5 + 0x100000;
}
#undef OvObj_UpdateArcFromAnchor

/* overlays/scene/dialogue/facing_target/actor_facing.c */
#define NULL ((void *)0)
#define CalculateFacingAngle Func_0200186e
#define Actor_UpdateFacingTowardTarget Func_02000030


s32 Func_0200186e(s32, s32);

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
#undef NULL
#undef CalculateFacingAngle
#undef Actor_UpdateFacingTowardTarget

/* overlays/scene/dialogue/facing_target/actor_presentation.c */
#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
#define Dialogue_RunActorFourteenDialogue11AA Func_02000284
#define ActorDraw_SetTwoSceneCells Func_020013b4

s32 Func_02001b94();
s32 Func_02001bcc(s32, s32);
s32 Func_02001be6();
s32 Func_02001c36(s32, s32, s32);
s32 Func_02001c5c(s32);
s32 Func_02001c6c(s32, s32);
s32 Func_02001c88(s32, s32);
s32 Func_02001cb2(s32, s32);
void Func_02002c44(s32, s32, s32, s32, s32, s32);
void Func_02002c76(s32, s32, s32, s32, s32, s32);

void Dialogue_RunActorFourteenDialogue11AA(void)
{
    void *work;

    Func_02001b94();
    Func_02001c36(0xE, 0, 0xA);
    Func_02001c5c(0x11AA);
    Func_02001c6c(0xE, 0);
    if (Func_02001bcc(0, 0) == 0) {
        Func_02001c88(0xE, 0);
    } else {
        work = *(void **)0x03001EBC;
        FIELD_AT_OFFSET(work, u16 *, 0x1D8) = (u16)(FIELD_AT_OFFSET(work, u16 *, 0x1D8) + 1);
        Func_02001cb2(0xE, 0);
    }
    Func_02001be6();
}

void ActorDraw_SetTwoSceneCells(void)
{
    {
        s32 extent = 2;

        Func_02002c44(22, 85, 25, 85, extent, extent);
    }
    {
        s32 extent = 25;

        Func_02002c76(25, 15, 2, 2, extent, extent);
    }
}
#undef NULL
#undef FIELD_AT_OFFSET
#undef Dialogue_RunActorFourteenDialogue11AA
#undef ActorDraw_SetTwoSceneCells

/* overlays/scene/dialogue/facing_target/effect.c */
#define Effect_UpdateObjectByFrameParity Func_02001544
#define Effect_UpdateAnchoredRiseFrame Func_020015e8
/* The anchor pointer is read before the frame counter is stored: the reference
 * hoists `ldr r6,[r5,#104]` above the `strh`, and only that source order
 * reproduces it. */
struct Actor_02003fb4 {
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
    struct Actor_02003fb4 *anchor;  /* 0x68 */
};

extern s32 Data_03001e40;

void Func_02002e24(u8 *, s32);
void Func_02002e2e(u8 *, s32);
void Func_02002c00(u8 *);
void Func_02002e68(struct Actor_02003fb4 *);
s32 Func_02002e30(s32);

void Effect_UpdateObjectByFrameParity(u8 *obj)
{
    if ((Data_03001e40 & 2) != 0) {
        Func_02002e24(obj, 7);
    } else {
        Func_02002e2e(obj, 0);
    }
    if ((Data_03001e40 & 15) == 0) {
        Func_02002c00(obj);
    }
}

void Effect_UpdateAnchoredRiseFrame(struct Actor_02003fb4 *self)
{
    struct Actor_02003fb4 *anchor;
    s32 frame;
    s32 amplitude;

    anchor = self->anchor;
    self->frame = (u16)(self->frame + 1);
    frame = (s16)self->frame;

    if (frame > 31) {
        Func_02002e68(self);
        return;
    }

    amplitude = Func_02002e30(frame << 10);
    self->amplitude_x = amplitude;
    self->amplitude_y = amplitude;
    self->x = anchor->x;
    self->y += 0x10000;
    self->z = anchor->z + (0x10000 - amplitude) * 5 + 0x80000;
}
#undef Effect_UpdateObjectByFrameParity
#undef Effect_UpdateAnchoredRiseFrame

/* overlays/scene/dialogue/facing_target/facing_target_scene.c */
#define FacingTargetScene_GetScriptData Func_02000088
u8 *FacingTargetScene_GetScriptData(void)
{
    return (u8 *)0x02009fd0;
}
#undef FacingTargetScene_GetScriptData

/* overlays/scene/dialogue/facing_target/overlay_object.c */
#define OvObj_UpdateOnFrameParity Func_02001580
#define OvObj_ApplyRandomSlotOnOddFrames Func_020015bc
s32 Func_02002da4(s32, s32);
void Func_02002e6c(u8 *, s32);
void Func_02002c3c(u8 *);
s32 Func_02002de0(s32, s32);
void Func_02002ea8(s32, s32);

void OvObj_UpdateOnFrameParity(u8 *obj)
{
    extern volatile u32 Data_03001e40;

    if ((Data_03001e40 & 1) != 0) {
        Func_02002e6c(obj, Func_02002da4((s32)(Data_03001e40 >> 1), 6));
    }
    if ((Data_03001e40 & 15) == 0) {
        Func_02002c3c(obj);
    }
}

void OvObj_ApplyRandomSlotOnOddFrames(s32 obj)
{
    extern volatile s32 Data_03001e40;

    if ((Data_03001e40 & 1) != 0) {
        s32 slot = Func_02002de0((u32)Data_03001e40 >> 1, 6);

        Func_02002ea8(obj, slot);
    }
}
#undef OvObj_UpdateOnFrameParity
#undef OvObj_ApplyRandomSlotOnOddFrames

/* overlays/scene/dialogue/facing_target/scene_data.c */
#define SceneData_SelectTableA1DCOrA198 Func_02000094
#define SceneData_SelectTableByStoryFlags Func_020000b8
#define SceneData_SelectTableByStoryFlagsB Func_02000160
extern s32 Data_0200a518[];
extern s32 Data_0200a410[];
extern s32 Data_0200a338[];
extern s32 Data_0200a218[];

s32 Func_02001986(s32);
s32 Func_020019be(s32);
s32 Func_020019cc(s32);
s32 Func_02001a0c(s32);
/* One import, four call sites, four distinct veneer addresses. */
s32 Func_02001a64();
s32 Func_02001a76();
s32 Func_02001a84();
s32 Func_02001a92();

s32 SceneData_SelectTableA1DCOrA198(void)
{
    if (Func_02001986(0x834) != 0) {
        return 0x0200A1DC;
    }
    return 0x0200A198;
}

s32 SceneData_SelectTableByStoryFlags(void)
{
    extern u8 Data_02000240[];

    u8 *b = Data_02000240;
    s32 *tbl;

    if (*(s16 *)(b + 0x1c2) == 19)
        return (s32)Data_0200a518;
    if (Func_020019be(0x87a) != 0)
        tbl = Data_0200a410;
    else if (Func_020019cc(0x815) != 0)
        tbl = Data_0200a338;
    else
        tbl = Data_0200a218;
    Func_02001a0c((s32)tbl);
    return (s32)tbl;
}

s32 SceneData_SelectTableByStoryFlagsB(void)
{
    extern s16 Data_02000240[];

    if (Data_02000240[225] == 19) {
        if (Func_02001a64(0x950) != 0) {
            return 0x0200AC5C;
        }
        return 0x0200AB9C;
    }

    if (Func_02001a76(0x834) != 0) {
        return 0x0200A5A8;
    }
    if (Func_02001a84(0x87A) != 0) {
        return 0x0200A980;
    }
    if (Func_02001a92(0x815) != 0) {
        return 0x0200A800;
    }
    return 0x0200A6B0;
}
#undef SceneData_SelectTableA1DCOrA198
#undef SceneData_SelectTableByStoryFlags
#undef SceneData_SelectTableByStoryFlagsB

/* overlays/scene/dialogue/facing_target/scene_dialogue.c */
#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
#define Dialogue_ShowLine1C13WithActor16Steps Func_02000ef8
#define Dialogue_RunActorThirteenDialogue Func_02000f38
#define Dialogue_RunActor16LineAndFlag81c Func_02000f64

void Func_02002808();
void Func_0200281a(s32);
void Func_0200283e();
void Func_020028aa(s32, s32, s32);
void Func_020028d0(s32);
void Func_020028e8(s32, s32);
void Func_0200290c(s32, s32, s32);
void Func_02002848(void);
void Func_02002906(s32);
s32 Func_0200291e(s32, s32);
void Func_02002844(s32);
void Func_02002868(void);
void Func_02002874(void);
void Func_02002932(s32);
s32 Func_0200294a(s32, s32);
void Func_02002870(s32);
void Func_02002894(void);

void Dialogue_ShowLine1C13WithActor16Steps(void)
{
    Func_02002808();
    Func_020028aa(0x10, 0, 0xA);
    Func_020028d0(0x1C13);
    Func_020028e8(0x10, 0);
    Func_0200290c(0x10, 0xB000, 0xA);
    Func_0200281a(0x301);
    Func_0200283e();
}

void Dialogue_RunActorThirteenDialogue(void)
{
    Func_02002848();
    Func_02002906(0x1C1B);
    Func_0200291e(0xD, 0);
    Func_02002844(0x81C);
    Func_02002868();
}

void Dialogue_RunActor16LineAndFlag81c(void)
{
    Func_02002874();
    Func_02002932(0x1C1A);
    Func_0200294a(0x10, 0);
    Func_02002870(0x81C);
    Func_02002894();
}
#undef NULL
#undef FIELD_AT_OFFSET
#undef Dialogue_ShowLine1C13WithActor16Steps
#undef Dialogue_RunActorThirteenDialogue
#undef Dialogue_RunActor16LineAndFlag81c

/* overlays/scene/dialogue/facing_target/scene_primary_script.c */
#define FieldScene_RunScene377_020001e0 Func_020001e0
#define Scene_RunStep7BThen2 Func_02000330
#define Scene_RunStep7BThen4 Func_02000358
#define Scene_RunStep80Then5 Func_0200036c
#define Scene_RunStep7BThen6 Func_02000380
#define Scene_RunStep80Then7 Func_02000394
#define Scene_RunStep7BThen10 Func_020003d0
#define FieldScene_RunScene377_02000e34 Func_02000e34
#define Scene_RunSupplementalSequenceOne Func_020013e4
#define Scene_RunStep17 Func_020017d4
/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];

void Func_02001af0();
void Func_02001af6();
void Func_02001b2a();
void Func_02001b38();
s32 Func_02001b68();
void Func_02001b6a();
void Func_02001b80();
void Func_02001b88();
void Func_02001bae();
void Func_02001bb2();
void Func_02001bb4();
void Func_02001bbc();
void Func_02001be8();
void Func_02001bf4();
s32 Func_02001c08();
void Func_02001c1c();
void Func_02001c4c();
void Func_02001de2(s32);
void Func_0200061c(s32);
void Func_02001e0a(s32);
void Func_02000644(s32);
void Func_02001e1e(s32);
void Func_02000658(s32);
void Func_02001e32(s32);
void Func_0200066c(s32);
void Func_02001e46(s32);
void Func_02000680(s32);
void Func_02001e82(s32);
void Func_020006bc(s32);
s32 Func_02002748();
void Func_02002762();
s32 Func_02002766();
s32 Func_02002766_a();
void Func_02002788();
s32 Func_020027a0();
void Func_020027e6();
void Func_02002804();
void Func_02002816();
void Func_02002848_a();
void Func_0200285e();
void Func_02002860();
void Func_02002882();
s32 Func_02002886();
void Func_0200288a();
void Func_02002898();
void Func_020028a2();
void Func_020028f4();
void Func_02002c18();
void Func_02002c36();
s32 Func_02002c98();
s32 Func_02002c9e();
void Func_02002cc4();
void Func_02002cce();
void Func_02002cf8();
void Func_02002d3c();
void Func_02002d4a();
void Func_02002d5e();
void Func_02002d62();
void Func_02002d6c();
void Func_02002d70();
void Func_02002d82();
void Func_02002d90();
void Func_02002de4();
void Func_02002e06();
void Func_02002e22();
void Func_02002e28();
void Func_02002e34();
void Func_02002e3e();
void Func_02002e40();
void Func_02002e9e();
void Func_02002ecc();
void Func_02002efe();
void Func_02002f08();
void Func_02002f5e();
void Func_02002f84();
void Func_02002f90();
void Func_02003106(s32);
void Func_02002d9a(void);

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

void FieldScene_RunScene377_020001e0(void)
{
    u32 i;
    s32 record;

    Func_02001af0();
    Func_02001b80(16, 2);
    Func_02001af6(30);
    Call1(Func_02001bbc, 0xf5b);
    Func_02001bae(0, 16, 10);
    Func_02001be8(16, 0, 6);
    Call3(Func_02001c1c, 16, 0x102, 0);
    Func_02001bb4(16, 1);
    Func_02001b2a(20);
    Func_02001bb2(16, 4);
    Func_02001b38(20);
    Value2(Func_02001c08, 16, 0);
    if (Value2(Func_02001b68, 0, 0) == 1) {
        bump_step(1);
    }
    Func_02001bf4(16, 1);
    Func_02001b6a(20);
    Func_02001c4c(16, 0, 4);
    Func_02001b88();
}

void Scene_RunStep7BThen2(void)
{
    Func_02001de2(0x7B);
    Func_0200061c(2);
}

void Scene_RunStep7BThen4(void)
{
    Func_02001e0a(0x7B);
    Func_02000644(4);
}

void Scene_RunStep80Then5(void)
{
    Func_02001e1e(0x80);
    Func_02000658(5);
}

void Scene_RunStep7BThen6(void)
{
    Func_02001e32(0x7B);
    Func_0200066c(6);
}

void Scene_RunStep80Then7(void)
{
    Func_02001e46(0x80);
    Func_02000680(7);
}

void Scene_RunStep7BThen10(void)
{
    Func_02001e82(0x7B);
    Func_020006bc(10);
}

void FieldScene_RunScene377_02000e34(void)
{
    u32 i;
    s32 record;

    record = Func_02002766(0);
    if ((u32)(*(volatile u16 *)(record + 6) + -0x2000) > 0x9000) {
        Func_020028f4(0, 13);
    } else {
        Func_02002762();
        if (Value1(Func_02002748, 0x87a) != 0) {
            Func_02002804(13, 2);
            Func_02002816(13, 0, 10);
            if (Value1(Func_02002766_a, 0x300) == 0) {
                Call1(Func_02002848_a, 0x1c14);
                Func_02002860(13, 0);
                Call1(Func_02002788, 0x300);
            }
            Call1(Func_0200285e, 0x1c15);
            Value2(Func_02002886, 13, 0);
            Func_02002898(13, 0x9000, 10);
        } else {
            if (Value1(Func_020027a0, 0x815) != 0) {
                Call1(Func_02002882, 0x11a9);
            } else {
                Call1(Func_0200288a, 0xf58);
            }
            Func_020028a2(13, 0);
        }
        Func_020027e6();
    }
}

void Scene_RunSupplementalSequenceOne(s32 a0)
{
    u32 i;
    u8 *rec2;
    s32 rec7;
    s32 record;
    u8 *p6;

    Func_02002cf8(a0);
    Call4(Func_02002e22, -1, -1, -1, 0);
    Func_02002c18(1);
    Func_02002d82(0, 0, 0);
    Call3(Func_02002d90, 18, 0x1e00000, 0xca0000);
    Func_02002c36(1);
    Func_02002e3e(18, 1);
    rec7 = 0;
    rec2 = Value4(Func_02002c98, 22, 0x1480000, 0x20000, 0xc30000);
    rec2[85] = rec7;
    p6 = *(volatile s32 *)((s32)rec2 + 80);
    *(volatile s32 *)((s32)rec2 + 12) = 0x50000;
    p6[39] = rec7;
    {
        s32 mask = -33;
        p6[5] &= mask;
    }
    p6[9] &= 15;
    rec7 = Value2(Func_02002c9e, 17, 0x608);
    Func_02002d5e(224);
    Func_02002cce(p6[28], 128, (rec7 + 0x400));
    Func_02002cc4(17);
    {
        s32 off = 0x1c0;
        u8 *base = *(u8 *volatile *)Data_03001ebc;
        s32 *slot = (s32 *)(base + off);
        off += 66;
        *slot = off;
    }
    Func_02002efe();
    Call3(Func_02002de4, 18, 0x10000, 0x8000);
    Call3(Func_02002e28, 18, 0x1e0, 176);
    Call3(Func_02002e34, 18, 0x1a4, 164);
    Call3(Func_02002e40, 18, 0x146, 185);
    Call3(Func_02002ecc, 18, 0x4000, 10);
    Func_02002d3c((s32)rec2, 0x2009eac);
    Func_02002d62((s32)rec2);
    Call2(Func_02002d4a, (s32)rec2, 0x2009ecc);
    Func_02002d70((s32)rec2);
    Func_02002e06(20);
    Func_02002d6c((s32)rec2);
    Func_02002e9e(18, 2, 20);
    Func_02002f08(18, 0, 40);
    Func_02002f84();
    Func_02002f90();
    Func_02002f5e(22);
}

void Scene_RunStep17(void)
{
    Func_02003106(17);
    Func_02002d9a();
}
#undef FieldScene_RunScene377_020001e0
#undef Scene_RunStep7BThen2
#undef Scene_RunStep7BThen4
#undef Scene_RunStep80Then5
#undef Scene_RunStep7BThen6
#undef Scene_RunStep80Then7
#undef Scene_RunStep7BThen10
#undef FieldScene_RunScene377_02000e34
#undef Scene_RunSupplementalSequenceOne
#undef Scene_RunStep17

/* overlays/scene/dialogue/facing_target/scene_setup.c */
#define Scene_RunActorCueBranch Func_02000118
#define Scene_RunActor8TwoStep Func_020017c4
#define Scene_RunSixStepSequence17e4 Func_020017e4
extern u8 Value_000022b9;

void Func_02001ae6(s32);
s32 Func_02001af6_a(s32, s32);
s32 Func_02001a56(s32, s32);
void Func_02001a40(s32);
void Func_02001b06(s32);
void Func_02001b0e(s32);
void Func_02001b26(s32, s32);
void Func_020030f6(s32);
void Func_02002d4e(void);
void Func_02003268(s32, s32);
s32 Func_02003278(s32, s32);
void Func_02003294(void);
void Func_02003272(s32);
void Func_0200328e(void);
void Func_0200329a(void);

void Scene_RunActorCueBranch(s32 object)
{
    s32 cue = (s32)&Value_000022b9;

    Func_02001ae6(cue);
    Func_02001af6_a(object, 0);
    if (Func_02001a56(0, 0) == 0) {
        Func_02001a40(10);
        Func_02001b06(cue + 1);
    } else {
        Func_02001b0e(cue + 2);
    }
    Func_02001b26(object, 0);
}

void Scene_RunActor8TwoStep(void)
{
    Func_020030f6(8);
    Func_02002d4e();
}

void Scene_RunSixStepSequence17e4(void)
{
    Func_02003268(0x94, 1);
    Func_02003278(8, 0x11);
    Func_02003294();
    Func_02003272(1);
    Func_0200328e();
    Func_0200329a();
}
#undef Scene_RunActorCueBranch
#undef Scene_RunActor8TwoStep
#undef Scene_RunSixStepSequence17e4

/* overlays/scene/dialogue/facing_target/shared.c */
#define Scene_RunSingleStep Func_020017b8
void Func_02003250(void);

void Scene_RunSingleStep(void)
{
    Func_02003250();
}
#undef Scene_RunSingleStep

/* overlays/scene/dialogue/facing_target/state_update.c */
#define State_SetWork448To521AndRun Func_020002e0
#define State_SetValue123Mode1 Func_0200031c
#define State_SetValue123Mode3 Func_02000344
#define State_SetValue129Mode8 Func_020003a8
#define State_SetValue129Mode9 Func_020003bc
#define State_ApplyValues123And11 Func_020003e4
#define State_SetValue140Mode0 Func_020017a8
extern u8 *Data_03001ebc;

s32 Func_02001bd4(s32);
void Func_02001d34(void);
void Func_02001d66(void);
void Func_02001d72(void);
void Func_02001d40(s32);
void Func_02001dce(s32);
void Func_02000608(s32);
void Func_02001df6(s32);
void Func_02000630(s32);
void Func_02001e5a(s32);
void Func_02000694(s32);
void Func_02001e6e(s32);
void Func_020006a8(s32);
void Func_02001e96(s32);
void Func_020006d0(s32);
void Func_0200322c(s32, s32);

void State_SetWork448To521AndRun(s32 object)
{
    if (Func_02001bd4(0x834) != 0) {
        Func_02001d34();
    }
    *(s32 *)(Data_03001ebc + 448) = 521;
    Func_02001d66();
    Func_02001d72();
    Func_02001d40(object);
}

void State_SetValue123Mode1(void)
{
    Func_02001dce(0x7B);
    Func_02000608(1);
}

void State_SetValue123Mode3(void)
{
    Func_02001df6(0x7B);
    Func_02000630(3);
}

void State_SetValue129Mode8(void)
{
    Func_02001e5a(0x81);
    Func_02000694(8);
}

void State_SetValue129Mode9(void)
{
    Func_02001e6e(0x81);
    Func_020006a8(9);
}

void State_ApplyValues123And11(void)
{
    Func_02001e96(0x7B);
    Func_020006d0(11);
}

void State_SetValue140Mode0(void)
{
    Func_0200322c(0x8C, 0);
}
#undef State_SetWork448To521AndRun
#undef State_SetValue123Mode1
#undef State_SetValue123Mode3
#undef State_SetValue129Mode8
#undef State_SetValue129Mode9
#undef State_ApplyValues123And11
#undef State_SetValue140Mode0
