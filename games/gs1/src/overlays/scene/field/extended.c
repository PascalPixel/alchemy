#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/field/extended.h"

#include "facing_object.h"

struct Obj {
    u8 pad00[6];
    u16 f06;
    u8 pad08[0x30];
    s32 f38;
    s32 f3c;
    s32 f40;
};

struct VerticalEffectAnchor {
    u8 pad00[8];
    s32 x;
    u8 pad0c[4];
    s32 z;
};

struct SceneVerticalEffect {
    u8 pad00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 pad14[4];
    s32 amplitude_x;
    s32 amplitude_y;
    u8 pad20[0x44];
    u16 frame;
    u8 pad66[2];
    struct VerticalEffectAnchor *anchor;
};

extern u8 gOv[];
extern u8 gOv2[];
extern s16 gCell[];
extern u8 gOv3[];
extern u8 gOv4[];
extern u8 gOv5[];
extern u8 gOv6[];
extern u8 gOv7[];
extern u8 gOv8[];
extern u8 gOv9[];
extern u8 gOv10[];
extern u8 gOv11[];
extern u8 gVal[];
extern u8 gWork[];
extern u8 Value;
extern u8 Value_00000eb0;
extern u8 gOv12[];

struct Obj *Field_Run2(s32);

s32 Object_UpdateFacingTowardTarget(struct FacingObject *object)
{
    s32 facing_delta;
    u16 old_facing;
    s32 target_facing;
    struct FacingObject *target;

    target = object->facing_target;
    if (target != NULL) {
        object->facing_flags = (u8)(0xFE & object->facing_flags);
        target_facing = (u16)CalculateFacingAngle(target->position_z - object->position_z, target->position_x - object->position_x);
        old_facing = object->facing;
        facing_delta = (s16)(target_facing - old_facing);
        if (facing_delta != 0) {
            if (facing_delta > 0x1000) {
                facing_delta = 0x1000;
            }
            /* The loader relocates the stored pool word to -0x1000. */
            if (facing_delta < -0x1000) {
                facing_delta = -0x1000;
            }
            object->facing = (u16)(old_facing + facing_delta);
        }
    }
    return 1;
}

/*
 * Table getter for resource_374. The owner at 0x02000088 is eight bytes and
 * includes its one pool word at 0x0200008c: the pc-relative load reads that
 * word, so the word belongs to this owner. The word is an address returned
 * without being dereferenced. Many getters share this body, but each returns
 * a different address.
 */
u8 *SceneData_GetTableAfa0(void)
{
    return (u8 *)0x0200afa0;
}

s32 Field_Run3(void)
{
    return 0;
}

void *SceneData_SelectTableByFlag834(void)
{
    if (Field_Check12(0x834) != 0) {
        return gOv;
    }
    return gOv2;
}

void *SceneData_SelectTableByFlags834And87a(void)
{
    if (Field_Check13(0x834) != 0) {
        return gOv3;
    }
    if (gCell[225] == 12) {
        return gOv4;
    }
    if (Field_Check14(0x87a) != 0) {
        return gOv5;
    }
    return gOv6;
}

void Scene_RunSequence111F(void)
{
    Field_Run4();
    Field_Apply(0x111f, 1);
    Field_Do20(126);
    Field_Apply2(0x3e7, 0);
    Field_Do21(10);
    Field_Apply3(0x974, 1);
    Field_Run5();
    Field_Do22(322);
    Field_Run6();
}

void *SceneData_SelectTableByFlags87a_815_834(void)
{
    if (Field_Check15(0x87a) != 0) {
        return gOv7;
    }
    if (Field_Check16(0x815) != 0) {
        return gOv8;
    }
    if (gCell[225] == 12) {
        return gOv9;
    }
    if (Field_Check17(0x834) != 0) {
        return gOv10;
    }
    return gOv11;
}

void Dialogue_RunActor23Line(void)
{
    Field_Run7();
    Field_Do23(0xf3c);
    Field_Place10(23, 0, 2);
    Field_Apply4(23, 0);
    Field_Run8();
}

void Dialogue_ShowLineF3F(void)
{
    Field_Run9();
    Field_Do24(0xf3f);
    Field_Place11(24, 0, 2);
    Field_Apply5(24, 0);
    Field_Run10();
}

void Dialogue_ShowLineF44(void)
{
    Field_Run11();
    Field_Do25(0xf44);
    Field_Place12(15, 0, 2);
    Field_Apply6(15, 0);
    Field_Run12();
}

void Scene_RunOpeningAuxiliarySequence(void)
{
    u32 i;
    s32 base5_1197;
    s32 base7_0;
    u8 *p6;

    Field_Run13();
    if (Field_Check(0x815) != 0) {
        base5_1197 = (s32)gVal;
        Field_Run14(base5_1197);
        if (Field_Check2(2) != 0) {
            bump_step(1);
        }
        if (Field_Check3(3) != 0) {
            bump_step(1);
        }
        Field_Check4(17, 0);
        if (Field_Check5(0, 0) == 0) {
            Field_Run15((base5_1197 + 3));
        } else {
            Field_Run16((base5_1197 + 4));
        }
        Field_Run17(17, 0);
    } else {
        p6 = *(volatile s32 *)(*(volatile s32 *)0x03001e70);
        Field_Do(0xf48);
        Field_Run18(17, 0, 0);
        Field_Run19(17, 0);
        Field_Run20(20);
        Field_Run21(17, 2);
        Field_Run22(15);
        Field_Run23();
        base7_0 = 0;
        for (i = 0; i < 40; i++) {
            Field_Run24(((s32 (*)())Field_Run25)(17));
            Field_Run26(1);
        }
        Field_Check6(0x200a591, 0xc80);
        Field_Run27(107);
        for (i = 0; i != 180; i++) {
            if (Field_Check7(i, 10) == 0) {
                if ((1 & base7_0) != 0) {
                    *(volatile s32 *)p6 = *(volatile s32 *)p6 - 0x10000;
                } else {
                    *(volatile s32 *)p6 = *(volatile s32 *)p6 + 0x10000;
                }
                base7_0 = (base7_0 + 1);
            }
            Field_Run28(1);
        }
        Field_Do2(0x121);
        Field_Do3(0x200a591);
        Field_Run29(1);
        Field_Run30();
        Field_Run31(17, 0);
        Field_Run32(40);
        Field_Do4(0xf4b);
        Field_Run33(17, 0);
    }
    Field_Run34();
}

void Dialogue_RunFlagGatedMessageStep(void)
{
    Field_Run35();
    if (Field_Check18(0x87a) != 0) {
        Field_Do26(0x1be8);
        Field_Apply7(15, 0);
        if (Field_Apply8(0, 0) == 1) {
            Field_Apply9(15, 0);
        } else {
            u8 *p = *(u8 **)0x03001ebc;
            *(u16 *)(p + 472) = *(u16 *)(p + 472) + 1;
            Field_Apply10(15, 0);
        }
    } else if (Field_Check19(0x815) != 0) {
        Field_Do27(0x1191);
        Field_Apply10(11, 0);
    } else {
        Field_Do28(0xea8);
        Field_Apply11(11, 0);
    }
    Field_Run36();
}

void Scene_RunActorTwentySixEffectPresentation(void)
{
    Field_Run37();
    Field_Apply12(26, 1);
    Field_Place13(26, 0, 20);
    Field_Place14(26, 21, 40);
    Field_Do29(0x11c7);
    Field_Apply13(26, 20);
    Field_Apply14(0x19999, 0x3333);
    Field_SetMode(0x1510000, -1, 0x1100000, 1);
    Field_Do30(20);
    Field_Apply15(26, 2);
    Field_Do31(20);
    Field_Place15(26, 0, 10);
    Field_Apply16(26, 40);
    Field_Apply17(26, 2);
    Field_Run38();
}

void Scene_RunMiddleAuxiliarySequence(void)
{
    u32 i;
    s32 p8;
    u8 *rec8;
    s32 record;
    s32 v2;

    Field_Run39();
    Field_Place(0, 82, 0x2f8);
    Field_Run40(15, 0, 30);
    Field_Do5(0xeae);
    Field_Run41(15, 20);
    Field_Place2(15, 0xa000, 20);
    Field_Run(15, 0x102);
    Field_Run42(20);
    Field_Run43();
    for (i = 0; i < 40; i++) {
        Field_Run44(Field_Check20(15));
        Field_Run45(1);
    }
    Field_Check8(0x200a581, 0xc80);
    Field_Check9(0x200a5a1, 0xc80);
    Field_Place3(0, 0xa000, 10);
    rec8 = Field_Check10(20);
    v2 = rec8[85];
    rec8[85] = 0;
    p8 = v2;
    for (i = 0; i < 40; i++) {
        *(volatile s32 *)((s32)rec8 + 12) += 0x1800;
        Field_Run46(1);
    }
    rec8[85] = p8;
    Field_Do6(0x200a581);
    Field_Do7(0x200a5a1);
    Field_Run47(1);
    Field_Run48(161);
    Field_Run49(15, 0);
    Field_Run50(20, 0);
    Field_Run51(40);
    Field_Run52();
    Field_Run53(0, 15, 30);
    Field_Run54(15, 0);
    Field_Run55();
}

void Dialogue_ShowLineEB1OrEB0(void)
{
    Field_Run56();
    Field_Place16(16, 0, 10);
    if (Field_Check21(0x840) != 0) {
        Field_Do32(0xeb1);
        Field_Apply18(16, 0);
    } else {
        Field_Do33((s32)&Value_00000eb0);
        Field_Apply19(16, 0);
    }
    Field_Run57();
}

void Dialogue_RunActorElevenFlaggedDialogue(void)
{
    u32 i;
    s32 record;

    Field_Run58();
    if (Field_Check11(0x302) != 0) {
        Field_Do8(0x1be4);
    } else {
        Field_Do9(0x1be3);
        Field_Do10(0x302);
    }
    Field_Run59(11, 0);
    Field_Run60();
}

void Scene_RunActorTwentyOneSetup(void)
{
    struct Obj *p = Field_Run2(21);
    Field_Run61();
    p->f38 = 0x80000000;
    p->f3c = 0x80000000;
    p->f40 = 0x80000000;
    Field_Apply20(21, 1);
    Field_Do34(21);
    Field_Place17(21, 256, 40);
    p->f06 = 0xb000;
    Field_Do35(20);
    Field_Apply21(21, 2);
    Field_Do36(0x1c94);
    Field_Place18(21, 0, 40);
    Field_Place19(21, 0, 20);
    Field_Apply22(21, 2);
    Field_Apply23(21, 0);
    Field_Do37(0x306);
    Field_Do38(21);
    Field_Do39(1);
    Field_Apply24(21, gOv12);
    Field_Run62();
}

void State_SetWork1c0AndRun(s32 no)
{
    u8 *p;
    if (Field_Check22(0x834) != 0) {
        Field_Run63();
    }
    p = *(u8 **)0x03001ebc;
    *(s32 *)(p + 0x1c0) = 0x100;
    *(s32 *)(p + 0x1c8) = 16;
    Field_Do40(no);
}

/* Runs four fixed scene-helper calls in sequence, one of them passed the
 * address of Value as its first argument. */
void Scene_RunSupplementalSequenceOne(void)
{
    Audio_PlayCue_1(158);
    Field_Place4((s32)&Value, 44, 7);
    Motion_ResetPosMode2_1(0, 248, 0x117);
    Field_Do11(1);
}

/* Sets step 188, then runs a pair of 6-argument setup calls for indices 0
 * and 2 sharing the same trailing four values, followed by a pair of
 * 3-argument calls sharing the same leading two arguments, and a closing
 * 1-argument call. */
void Scene_RunSupplementalSequenceTwo(void)
{
    Audio_PlayCue_1(188);
    Field_SetRect(0, 63, 51, 8, 2, 2);
    Field_Do12(10);
    Field_SetRect2(2, 63, 51, 8, 2, 2);
    Field_Do13(10);
    Motion_SetPosReset_1(0, 352, 306);
    Motion_SetActionVariant_1(0, 3);
    Motion_SetPosReset_2(0, 352, 296);
    Field_Do14(2);
}

/* Runs four fixed scene-helper calls in sequence, one of them passed the
 * address of Value as its first argument. */
void Scene_RunSupplementalSequenceThree(void)
{
    Audio_PlayCue_1(158);
    Field_Place5((s32)&Value, 43, 15); /* main:08009178 */
    ObjectMotion_ResetAndSetPositionInMode2_1(0, 230, 0x197);
    Field_Do15(3);
}

/* Runs four scene calls in sequence: a single-argument call, a call that
 * passes the address of Value with two more values, a call that
 * passes 0, 374, and 0x1a3, and a final single-argument call. */
void Scene_RunSupplementalSequenceFour(void)
{
    Audio_PlayCue_1(158);
    Field_Place6((s32)&Value, 52, 18); /* main:08009178 */
    ObjectMotion_ResetAndSetPositionInMode2_1(0, 374, 0x1a3); /* object_id 0, x 374, z 0x1a3 */
    Field_Do16(4);
}

/* Runs a fixed sequence of four scripted calls: one keyed off Value
 * with two small numeric arguments, one with a 0x222 argument, and two plain
 * single-argument calls. */
void Scene_RunSupplementalSequenceFive(void)
{
    Audio_PlayCue_1(158);
    Field_Place7((s32)&Value, 41, 32); /* main:08009178 */
    ObjectMotion_ResetAndSetPositionInMode2_1(0, 200, 0x222);
    Field_Do17(5);
}

/* Runs four scripted calls with fixed literal arguments: a single-argument
 * call, a 3-argument call whose first argument is the address of
 * Value, another 3-argument call, and a closing single-argument
 * call. */
void Scene_RunSupplementalSequenceSix(void)
{
    Audio_PlayCue_1(158);
    Field_Place8((s32)&Value, 35, 36); /* main:08009178 */
    ObjectMotion_ResetAndSetPositionInMode2_1(0, 102, 0x263); /* object_id 0, x 102, z 611 */
    Field_Do18(6);
}

/* Runs four scripted scene calls in sequence, passing a byte's address and a
 * handful of small immediate constants to each. */
void Scene_RunSupplementalSequenceSeven(void)
{
    Audio_PlayCue_1(158);
    Field_Place9((s32)&Value, 51, 39); /* main:08009178 */
    ObjectMotion_ResetAndSetPositionInMode2_1(0, 358, 0x29e);
    Field_Do19(7);
}

void Scene_RunStep7BThen8(void)
{
    Field_Do41(123);
    Field_Do42(8);
}

void State_ApplyFlag815Branch(void)
{
    if (Field_Check23(0x815) != 0) {
        Field_Do43(123);
        Field_Do44(10);
    }
}

void State_ApplyFlag90b(void)
{
    Field_Do45(0x90b);
}

void State_ApplyFlag90c(void)
{
    Field_Do46(0x90c);
}

void State_ApplyFlag90d(void)
{
    Field_Do47(0x90d);
}

void Scene_RunSplitPairSteps(s32 a, s32 b)
{
    Field_Apply25(a, 0);
    Field_Do48(b);
}

void Actor_SetPairZeroAndValue(s32 a, s32 b, s32 c)
{
    Field_Place20(a, b, 0);
    Field_Do49(c);
}

void OvObj_UpdateOnFrameBit1(s32 obj)
{
    if ((*(volatile s32 *)0x03001e40 & 2) != 0) {
        Field_Apply26(obj, 7);
    } else {
        Field_Apply27(obj, 0);
    }
    if ((*(volatile s32 *)0x03001e40 & 15) == 0) {
        Field_Do50(obj);
    }
}

void Effect_UpdateByFrameBits(s32 no)
{
    volatile s32 *p = (volatile s32 *)0x03001e40;
    if ((*p & 1) != 0) {
        s32 t = Field_Apply28((u32)*p >> 1, 6);
        Field_Apply29(no, t);
    }
    if ((*p & 15) == 0) {
        Field_Do51(no);
    }
}

void Effect_UpdateByFrameBit(s32 no)
{
    volatile s32 *p = (volatile s32 *)0x03001e40;
    if ((*p & 1) != 0) {
        s32 t = Field_Apply30((u32)*p >> 1, 6);
        Field_Apply31(no, t);
    }
}

void Effect_AnimateVerticalPositive(struct SceneVerticalEffect *effect)
{
    struct VerticalEffectAnchor *anchor;
    s32 frame;
    s32 amplitude;
    s32 offset;

    anchor = effect->anchor;
    effect->frame = effect->frame + 1;
    frame = (s16)effect->frame;
    if (frame > 31) {
        Field_Do52(effect);
        return;
    }
    amplitude = Field_Check24(frame << 10);
    effect->amplitude_x = amplitude;
    effect->amplitude_y = amplitude;
    effect->x = anchor->x;
    offset = 0x10000;
    effect->y = effect->y + offset;
    offset = offset - amplitude;
    effect->z = anchor->z + (offset * 4 + offset) + 0x80000;
}

void Effect_AnimateVerticalNegative(struct SceneVerticalEffect *effect)
{
    struct VerticalEffectAnchor *anchor;
    s32 frame;
    s32 amplitude;
    s32 offset;

    anchor = effect->anchor;
    effect->frame = effect->frame + 1;
    frame = (s16)effect->frame;
    if (frame > 31) {
        Field_Do53(effect);
        return;
    }
    amplitude = Field_Check25(frame << 10);
    effect->amplitude_x = amplitude;
    effect->amplitude_y = -amplitude;
    effect->x = anchor->x;
    offset = 0x10000;
    effect->y = effect->y + offset;
    offset = offset - amplitude;
    effect->z = anchor->z - (offset * 4 + offset) + 0x100000;
}

void State_ApplyPair140And0(void)
{
    Field_Apply32(140, 0);
}

void Scene_Forward(void)
{
    Field_Run64();
}

void Scene_RunStep15(void)
{
    Field_Do54(15);
    Field_Run65();
}

void Scene_RunStep17(void)
{
    Field_Do55(17);
    Field_Run66();
}

void Scene_RunStep20(void)
{
    Field_Do56(20);
    Field_Run67();
}

void State_SetValues352_365_2116_2117_40(void)
{
    Field_Do57(352);
    Field_Do58(0x16d);
    Field_Do59(0x844);
    Field_Do60(0x845);
    Field_Do61(40);
}
