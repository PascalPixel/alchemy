#include "types.h"
#include "scene.h"

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

struct Obj *Field_unk2_4(s32);

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

s32 get_tertiary_default_result(void)
{
    return 0;
}

void *SceneData_SelectTableByFlag834(void)
{
    if (Field_unk12(0x834) != 0) {
        return gOv;
    }
    return gOv2;
}

void *SceneData_SelectTableByFlags834And87a(void)
{
    if (Field_unk13(0x834) != 0) {
        return gOv3;
    }
    if (gCell[225] == 12) {
        return gOv4;
    }
    if (Field_unk14(0x87a) != 0) {
        return gOv5;
    }
    return gOv6;
}

void Scene_RunSequence111F(void)
{
    Field_unk4_4();
    Field_Apply(0x111f, 1);
    Field_unk20_2(126);
    Field_Apply2(0x3e7, 0);
    Field_unk21_2(10);
    Field_Apply3(0x974, 1);
    Field_unk5_4();
    Field_unk22_2(322);
    Field_unk6_4();
}

void *SceneData_SelectTableByFlags87a_815_834(void)
{
    if (Field_unk15(0x87a) != 0) {
        return gOv7;
    }
    if (Field_unk16(0x815) != 0) {
        return gOv8;
    }
    if (gCell[225] == 12) {
        return gOv9;
    }
    if (Field_unk17(0x834) != 0) {
        return gOv10;
    }
    return gOv11;
}

void Dialogue_RunActor23Line(void)
{
    Field_unk7_4();
    Field_unk23_2(0xf3c);
    Field_unk10_3(23, 0, 2);
    Field_Apply4(23, 0);
    Field_unk8_4();
}

void Dialogue_ShowLineF3F(void)
{
    Field_unk9_4();
    Field_unk24_2(0xf3f);
    Field_unk11_3(24, 0, 2);
    Field_Apply5(24, 0);
    Field_unk10_4();
}

void Dialogue_ShowLineF44(void)
{
    Field_unk11_4();
    Field_unk25_2(0xf44);
    Field_unk12_3(15, 0, 2);
    Field_Apply6(15, 0);
    Field_unk12_4();
}

void Scene_RunOpeningAuxiliarySequence(void)
{
    u32 i;
    s32 base5_1197;
    s32 base7_0;
    u8 *p6;

    Field_unk13_4();
    if (Field_Check(0x815) != 0) {
        base5_1197 = (s32)gVal;
        Field_unk14_4(base5_1197);
        if (Field_unk2(2) != 0) {
            bump_step(1);
        }
        if (Field_unk3(3) != 0) {
            bump_step(1);
        }
        Field_unk4(17, 0);
        if (Field_unk5(0, 0) == 0) {
            Field_unk15_4((base5_1197 + 3));
        } else {
            Field_unk16_4((base5_1197 + 4));
        }
        Field_unk17_4(17, 0);
    } else {
        p6 = *(volatile s32 *)(*(volatile s32 *)0x03001e70);
        Field_Do(0xf48);
        Field_unk18_4(17, 0, 0);
        Field_unk19_4(17, 0);
        Field_unk20_4(20);
        Field_unk21_4(17, 2);
        Field_unk22_4(15);
        Field_unk23_4();
        base7_0 = 0;
        for (i = 0; i < 40; i++) {
            Field_unk24_4(((s32 (*)())Field_unk25_4)(17));
            Field_unk26_4(1);
        }
        Field_unk6(0x200a591, 0xc80);
        Field_unk27_4(107);
        for (i = 0; i != 180; i++) {
            if (Field_unk7(i, 10) == 0) {
                if ((1 & base7_0) != 0) {
                    *(volatile s32 *)p6 = *(volatile s32 *)p6 - 0x10000;
                } else {
                    *(volatile s32 *)p6 = *(volatile s32 *)p6 + 0x10000;
                }
                base7_0 = (base7_0 + 1);
            }
            Field_unk28_4(1);
        }
        Field_unk2_2(0x121);
        Field_unk3_2(0x200a591);
        Field_unk29_4(1);
        Field_unk30_4();
        Field_unk31_4(17, 0);
        Field_unk32_4(40);
        Field_unk4_2(0xf4b);
        Field_unk33_4(17, 0);
    }
    Field_unk34_4();
}

void Dialogue_RunFlagGatedMessageStep(void)
{
    Field_unk35_4();
    if (Field_unk18(0x87a) != 0) {
        Field_unk26_2(0x1be8);
        Field_Apply7(15, 0);
        if (Field_Apply8(0, 0) == 1) {
            Field_Apply9(15, 0);
        } else {
            u8 *p = *(u8 **)0x03001ebc;
            *(u16 *)(p + 472) = *(u16 *)(p + 472) + 1;
            Field_Apply10(15, 0);
        }
    } else if (Field_unk19(0x815) != 0) {
        Field_unk27_2(0x1191);
        Field_Apply10(11, 0);
    } else {
        Field_unk28_2(0xea8);
        Field_Apply11(11, 0);
    }
    Field_unk36_4();
}

void Scene_RunActorTwentySixEffectPresentation(void)
{
    Field_unk37_4();
    Field_Apply12(26, 1);
    Field_unk13_3(26, 0, 20);
    Field_unk14_3(26, 21, 40);
    Field_unk29_2(0x11c7);
    Field_Apply13(26, 20);
    Field_Apply14(0x19999, 0x3333);
    Field_SetMode(0x1510000, -1, 0x1100000, 1);
    Field_unk30_2(20);
    Field_Apply15(26, 2);
    Field_unk31_2(20);
    Field_unk15_3(26, 0, 10);
    Field_Apply16(26, 40);
    Field_Apply17(26, 2);
    Field_unk38_4();
}

void Scene_RunMiddleAuxiliarySequence(void)
{
    u32 i;
    s32 p8;
    u8 *rec8;
    s32 record;
    s32 v2;

    Field_unk39_4();
    Field_Place(0, 82, 0x2f8);
    Field_unk40_4(15, 0, 30);
    Field_unk5_2(0xeae);
    Field_unk41_4(15, 20);
    Field_unk2_3(15, 0xa000, 20);
    Field_Run(15, 0x102);
    Field_unk42_4(20);
    Field_unk43_4();
    for (i = 0; i < 40; i++) {
        Field_unk44_4(Field_unk20(15));
        Field_unk45_4(1);
    }
    Field_unk8(0x200a581, 0xc80);
    Field_unk9(0x200a5a1, 0xc80);
    Field_unk3_3(0, 0xa000, 10);
    rec8 = Field_unk10(20);
    v2 = rec8[85];
    rec8[85] = 0;
    p8 = v2;
    for (i = 0; i < 40; i++) {
        *(volatile s32 *)((s32)rec8 + 12) += 0x1800;
        Field_unk46_4(1);
    }
    rec8[85] = p8;
    Field_unk6_2(0x200a581);
    Field_unk7_2(0x200a5a1);
    Field_unk47_4(1);
    Field_unk48_4(161);
    Field_unk49_4(15, 0);
    Field_unk50_4(20, 0);
    Field_unk51_4(40);
    Field_unk52_4();
    Field_unk53_4(0, 15, 30);
    Field_unk54_4(15, 0);
    Field_unk55_4();
}

void Dialogue_ShowLineEB1OrEB0(void)
{
    Field_unk56_4();
    Field_unk16_3(16, 0, 10);
    if (Field_unk21(0x840) != 0) {
        Field_unk32_2(0xeb1);
        Field_Apply18(16, 0);
    } else {
        Field_unk33_2((s32)&Value_00000eb0);
        Field_Apply19(16, 0);
    }
    Field_unk57_4();
}

void Dialogue_RunActorElevenFlaggedDialogue(void)
{
    u32 i;
    s32 record;

    Field_unk58_4();
    if (Field_unk11(0x302) != 0) {
        Field_unk8_2(0x1be4);
    } else {
        Field_unk9_2(0x1be3);
        Field_unk10_2(0x302);
    }
    Field_unk59_4(11, 0);
    Field_unk60_4();
}

void Scene_RunActorTwentyOneSetup(void)
{
    struct Obj *p = Field_unk2_4(21);
    Field_unk61_4();
    p->f38 = 0x80000000;
    p->f3c = 0x80000000;
    p->f40 = 0x80000000;
    Field_Apply20(21, 1);
    Field_unk34_2(21);
    Field_unk17_3(21, 256, 40);
    p->f06 = 0xb000;
    Field_unk35_2(20);
    Field_Apply21(21, 2);
    Field_unk36_2(0x1c94);
    Field_unk18_3(21, 0, 40);
    Field_unk19_3(21, 0, 20);
    Field_Apply22(21, 2);
    Field_Apply23(21, 0);
    Field_unk37_2(0x306);
    Field_unk38_2(21);
    Field_unk39_2(1);
    Field_Apply24(21, gOv12);
    Field_unk62_4();
}

void State_SetWork1c0AndRun(s32 no)
{
    u8 *p;
    if (Field_unk22(0x834) != 0) {
        Field_unk63_4();
    }
    p = *(u8 **)0x03001ebc;
    *(s32 *)(p + 0x1c0) = 0x100;
    *(s32 *)(p + 0x1c8) = 16;
    Field_unk40_2(no);
}

/* Runs four fixed scene-helper calls in sequence, one of them passed the
 * address of Value as its first argument. */
void Scene_RunSupplementalSequenceOne(void)
{
    Audio_PlayCue_1(158);
    Field_unk4_3((s32)&Value, 44, 7);
    Motion_ResetPosMode2_1(0, 248, 0x117);
    Field_unk11_2(1);
}

/* Sets step 188, then runs a pair of 6-argument setup calls for indices 0
 * and 2 sharing the same trailing four values, followed by a pair of
 * 3-argument calls sharing the same leading two arguments, and a closing
 * 1-argument call. */
void Scene_RunSupplementalSequenceTwo(void)
{
    Audio_PlayCue_1(188);
    Field_SetRect(0, 63, 51, 8, 2, 2);
    Field_unk12_2(10);
    Field_unk2_6(2, 63, 51, 8, 2, 2);
    Field_unk13_2(10);
    Motion_SetPosReset_1(0, 352, 306);
    Motion_SetActionVariant_1(0, 3);
    Motion_SetPosReset_2(0, 352, 296);
    Field_unk14_2(2);
}

/* Runs four fixed scene-helper calls in sequence, one of them passed the
 * address of Value as its first argument. */
void Scene_RunSupplementalSequenceThree(void)
{
    Audio_PlayCue_1(158);
    Field_unk5_3((s32)&Value, 43, 15); /* main:08009178 */
    ObjectMotion_ResetAndSetPositionInMode2_1(0, 230, 0x197);
    Field_unk15_2(3);
}

/* Runs four scene calls in sequence: a single-argument call, a call that
 * passes the address of Value with two more values, a call that
 * passes 0, 374, and 0x1a3, and a final single-argument call. */
void Scene_RunSupplementalSequenceFour(void)
{
    Audio_PlayCue_1(158);
    Field_unk6_3((s32)&Value, 52, 18); /* main:08009178 */
    ObjectMotion_ResetAndSetPositionInMode2_1(0, 374, 0x1a3); /* object_id 0, x 374, z 0x1a3 */
    Field_unk16_2(4);
}

/* Runs a fixed sequence of four scripted calls: one keyed off Value
 * with two small numeric arguments, one with a 0x222 argument, and two plain
 * single-argument calls. */
void Scene_RunSupplementalSequenceFive(void)
{
    Audio_PlayCue_1(158);
    Field_unk7_3((s32)&Value, 41, 32); /* main:08009178 */
    ObjectMotion_ResetAndSetPositionInMode2_1(0, 200, 0x222);
    Field_unk17_2(5);
}

/* Runs four scripted calls with fixed literal arguments: a single-argument
 * call, a 3-argument call whose first argument is the address of
 * Value, another 3-argument call, and a closing single-argument
 * call. */
void Scene_RunSupplementalSequenceSix(void)
{
    Audio_PlayCue_1(158);
    Field_unk8_3((s32)&Value, 35, 36); /* main:08009178 */
    ObjectMotion_ResetAndSetPositionInMode2_1(0, 102, 0x263); /* object_id 0, x 102, z 611 */
    Field_unk18_2(6);
}

/* Runs four scripted scene calls in sequence, passing a byte's address and a
 * handful of small immediate constants to each. */
void Scene_RunSupplementalSequenceSeven(void)
{
    Audio_PlayCue_1(158);
    Field_unk9_3((s32)&Value, 51, 39); /* main:08009178 */
    ObjectMotion_ResetAndSetPositionInMode2_1(0, 358, 0x29e);
    Field_unk19_2(7);
}

void Scene_RunStep7BThen8(void)
{
    Field_unk41_2(123);
    Field_unk42_2(8);
}

void State_ApplyFlag815Branch(void)
{
    if (Field_unk23(0x815) != 0) {
        Field_unk43_2(123);
        Field_unk44_2(10);
    }
}

void State_ApplyFlag90b(void)
{
    Field_unk45_2(0x90b);
}

void State_ApplyFlag90c(void)
{
    Field_unk46_2(0x90c);
}

void State_ApplyFlag90d(void)
{
    Field_unk47_2(0x90d);
}

void Scene_RunSplitPairSteps(s32 a, s32 b)
{
    Field_Apply25(a, 0);
    Field_unk48_2(b);
}

void Actor_SetPairZeroAndValue(s32 a, s32 b, s32 c)
{
    Field_unk20_3(a, b, 0);
    Field_unk49_2(c);
}

void OvObj_UpdateOnFrameBit1(s32 obj)
{
    if ((*(volatile s32 *)0x03001e40 & 2) != 0) {
        Field_Apply26(obj, 7);
    } else {
        Field_Apply27(obj, 0);
    }
    if ((*(volatile s32 *)0x03001e40 & 15) == 0) {
        Field_unk50_2(obj);
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
        Field_unk51_2(no);
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
        Field_unk52_2(effect);
        return;
    }
    amplitude = Field_unk24(frame << 10);
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
        Field_unk53_2(effect);
        return;
    }
    amplitude = Field_unk25(frame << 10);
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
    Field_unk64_4();
}

void Scene_RunStep15(void)
{
    Field_unk54_2(15);
    Field_unk65_4();
}

void Scene_RunStep17(void)
{
    Field_unk55_2(17);
    Field_unk66_4();
}

void Scene_RunStep20(void)
{
    Field_unk56_2(20);
    Field_unk67_4();
}

void State_SetValues352_365_2116_2117_40(void)
{
    Field_unk57_2(352);
    Field_unk58_2(0x16d);
    Field_unk59_2(0x844);
    Field_unk60_2(0x845);
    Field_unk61_2(40);
}
