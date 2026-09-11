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

struct Obj *Field_GetObj(s32);

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
    if (Field_unk_0200270e(0x834) != 0) {
        return gOv;
    }
    return gOv2;
}

void *SceneData_SelectTableByFlags834And87a(void)
{
    if (Field_unk_02002732(0x834) != 0) {
        return gOv3;
    }
    if (gCell[225] == 12) {
        return gOv4;
    }
    if (Field_unk_02002754(0x87a) != 0) {
        return gOv5;
    }
    return gOv6;
}

void Scene_RunSequence111F(void)
{
    Field_unk_020027ac();
    Field_unk_0200277c(0x111f, 1);
    Field_unk_0200296a(126);
    Field_unk_02002962(0x3e7, 0);
    Field_unk_020027c0(10);
    Field_unk_02002798(0x974, 1);
    Field_unk_020027a4();
    Field_TestFlag142(322);
    Field_unk_020027e8();
}

void *SceneData_SelectTableByFlags87a_815_834(void)
{
    if (Field_unk_020027d2(0x87a) != 0) {
        return gOv7;
    }
    if (Field_unk_020027e0(0x815) != 0) {
        return gOv8;
    }
    if (gCell[225] == 12) {
        return gOv9;
    }
    if (Field_unk_02002802(0x834) != 0) {
        return gOv10;
    }
    return gOv11;
}

void Dialogue_RunActor23Line(void)
{
    Field_unk_02002864();
    Field_TestFlagf3c(0xf3c);
    Field_unk_02002924(23, 0, 2);
    Field_unk_02002964(23, 0);
    Field_unk_02002888();
}

void Dialogue_ShowLineF3F(void)
{
    Field_unk_02002890();
    Field_TestFlagf3f(0xf3f);
    Field_unk_02002950_a(24, 0, 2);
    Field_unk_02002990(24, 0);
    Field_unk_020028b4();
}

void Dialogue_ShowLineF44(void)
{
    Field_unk_020028bc();
    Field_TestFlagf44(0xf44);
    Field_unk_0200297c(15, 0, 2);
    Field_unk_020029bc(15, 0);
    Field_unk_020028e0();
}

void Scene_RunOpeningAuxiliarySequence(void)
{
    u32 i;
    s32 base5_1197;
    s32 base7_0;
    u8 *p6;

    Field_unk_020028e8();
    if (Field_Check(0x815) != 0) {
        base5_1197 = (s32)gVal;
        Field_unk_020029c2(base5_1197);
        if (Field_unk_020028d8(2) != 0) {
            bump_step(1);
        }
        if (Field_unk_020028f2(3) != 0) {
            bump_step(1);
        }
        Field_unk_02002a06(17, 0);
        if (Field_TestFlagf3f(0, 0) == 0) {
            Field_unk_02002a10((base5_1197 + 3));
        } else {
            Field_unk_02002a18((base5_1197 + 4));
        }
        Field_unk_02002a30(17, 0);
    } else {
        p6 = *(volatile s32 *)(*(volatile s32 *)0x03001e70);
        Field_TestFlagf48(0xf48);
        Field_unk_02002a20(17, 0, 0);
        Field_unk_02002a60(17, 0);
        Field_unk_02002976(20);
        Field_unk_02002a1e(17, 2);
        Field_unk_02002984(15);
        Field_unk_02002858();
        base7_0 = 0;
        for (i = 0; i < 40; i++) {
            Field_unk_020025fa(((s32 (*)())Field_unk_020029c2_a)(17));
            Field_unk_020028f6(1);
        }
        Field_unk_0200290c(0x200a591, 0xc80);
        Field_unk_02002b6a(107);
        for (i = 0; i != 180; i++) {
            if (Field_unk_0200290c_a(i, 10) == 0) {
                if ((1 & base7_0) != 0) {
                    *(volatile s32 *)p6 = *(volatile s32 *)p6 - 0x10000;
                } else {
                    *(volatile s32 *)p6 = *(volatile s32 *)p6 + 0x10000;
                }
                base7_0 = (base7_0 + 1);
            }
            Field_unk_020029e2(1);
        }
        Field_TestFlag121(0x121);
        Field_unk_0200295a(0x200a591);
        Field_unk_02002950(1);
        Field_unk_020028dc();
        Field_unk_02002ac4(17, 0);
        Field_unk_02002a0a(40);
        Field_TestFlagf4b(0xf4b);
        Field_unk_02002af8(17, 0);
    }
    Field_unk_02002a2c();
}

void Dialogue_RunFlagGatedMessageStep(void)
{
    Field_unk_02002a54();
    if (Field_TestFlag87a(0x87a) != 0) {
        Field_unk_02002b2c(0x1be8);
        Field_unk_02002b3c(15, 0);
        if (Field_unk_02002a94(0, 0) == 1) {
            Field_unk_02002b58(15, 0);
        } else {
            u8 *p = *(u8 **)0x03001ebc;
            *(u16 *)(p + 472) = *(u16 *)(p + 472) + 1;
            Field_unk_02002b96(15, 0);
        }
    } else if (Field_unk_02002a74(0x815) != 0) {
        Field_unk_02002b6e(0x1191);
        Field_unk_02002b96(11, 0);
    } else {
        Field_TestFlagea8(0xea8);
        Field_unk_02002ba6(11, 0);
    }
    Field_unk_02002aca();
}

void Scene_RunActorTwentySixEffectPresentation(void)
{
    Field_unk_02002ae4();
    Field_unk_02002b6c(26, 1);
    Field_unk_02002b9e(26, 0, 20);
    Field_unk_02002ba8(26, 21, 40);
    Field_unk_02002bce(0x11c7);
    Field_unk_0200273a(26, 20);
    Field_unk_02002c26(0x19999, 0x3333);
    Field_SetMode(0x1510000, -1, 0x1100000, 1);
    Field_unk_02002b24(20);
    Field_unk_02002bd4(26, 2);
    Field_unk_02002b32(20);
    Field_unk_02002bec(26, 0, 10);
    Field_unk_02002778(26, 40);
    Field_unk_02002b8c(26, 2);
    Field_unk_02002b60();
}

void Scene_RunMiddleAuxiliarySequence(void)
{
    u32 i;
    s32 p8;
    u8 *rec8;
    s32 record;
    s32 v2;

    Field_unk_02002b78();
    Field_unk_02002bec_b(0, 82, 0x2f8);
    Field_SetMode(15, 0, 30);
    Field_TestFlageae(0xeae);
    Field_unk_020027c8(15, 20);
    Field_unk_020027ec(15, 0xa000, 20);
    Field_unk_02002cba(15, 0x102);
    Field_unk_02002bb0(20);
    Field_unk_02002a84();
    for (i = 0; i < 40; i++) {
        Field_unk_02002824(Field_unk_02002bec_a(15));
        Field_unk_02002b20(1);
    }
    Field_unk_02002b36(0x200a581, 0xc80);
    Field_unk_02002b40(0x200a5a1, 0xc80);
    Field_unk_02002ce4(0, 0xa000, 10);
    rec8 = Field_unk_02002c22(20);
    v2 = rec8[85];
    rec8[85] = 0;
    p8 = v2;
    for (i = 0; i < 40; i++) {
        *(volatile s32 *)((s32)rec8 + 12) += 0x1800;
        Field_unk_02002b6c_a(1);
    }
    rec8[85] = p8;
    Field_unk_02002b8a(0x200a581);
    Field_unk_02002b90(0x200a5a1);
    Field_unk_02002b86(1);
    Field_unk_02002dec(161);
    Field_unk_02002cfc(15, 0);
    Field_unk_02002d04(20, 0);
    Field_unk_02002c4a(40);
    Field_unk_02002b2e();
    Field_unk_02002d10(0, 15, 30);
    Field_unk_02002d40(15, 0);
    Field_unk_02002c74();
}

void Dialogue_ShowLineEB1OrEB0(void)
{
    Field_unk_02002c88();
    Field_unk_02002d42(16, 0, 10);
    if (Field_TestFlag840(0x840) != 0) {
        Field_TestFlageb1(0xeb1);
        Field_unk_02002d84(16, 0);
    } else {
        Field_unk_02002d7c((s32)&Value_00000eb0);
        Field_unk_02002d94(16, 0);
    }
    Field_unk_02002cc8();
}

void Dialogue_RunActorElevenFlaggedDialogue(void)
{
    u32 i;
    s32 record;

    Field_unk_02002cd4();
    if (Field_TestFlag302(0x302) != 0) {
        Field_unk_02002dac(0x1be4);
    } else {
        Field_unk_02002db4(0x1be3);
        Field_TestFlag302(0x302);
    }
    Field_unk_02002dd2(11, 0);
    Field_unk_02002d06();
}

void Scene_RunActorTwentyOneSetup(void)
{
    struct Obj *p = Field_GetObj(21);
    Field_unk_02002d1c();
    p->f38 = 0x80000000;
    p->f3c = 0x80000000;
    p->f40 = 0x80000000;
    Field_unk_02002dae(21, 1);
    Field_unk_02002d7c_a(21);
    Field_unk_02002e40(21, 256, 40);
    p->f06 = 0xb000;
    Field_unk_02002d44(20);
    Field_unk_02002dec_a(21, 2);
    Field_unk_02002e22(0x1c94);
    Field_unk_02002e44(21, 0, 40);
    Field_unk_02002e16(21, 0, 20);
    Field_unk_02002e0e(21, 2);
    Field_unk_02002e56(21, 0);
    Field_TestFlag306(0x306);
    Field_unk_02002dd2_a(21);
    Field_unk_02002ce0(1);
    Field_unk_02002dd0(21, gOv12);
    Field_unk_02002da4();
}

void State_SetWork1c0AndRun(s32 no)
{
    u8 *p;
    if (Field_TestFlag834(0x834) != 0) {
        Field_unk_02002f20();
    }
    p = *(u8 **)0x03001ebc;
    *(s32 *)(p + 0x1c0) = 0x100;
    *(s32 *)(p + 0x1c8) = 16;
    Field_unk_02002f0c(no);
}

/* Runs four fixed scene-helper calls in sequence, one of them passed the
 * address of Value as its first argument. */
void Scene_RunSupplementalSequenceOne(void)
{
    Audio_PlayCue_1(158);
    Field_unk_02002d9c((s32)&Value, 44, 7);
    Motion_ResetPosMode2_1(0, 248, 0x117);
    Field_unk_02000e84(1);
}

/* Sets step 188, then runs a pair of 6-argument setup calls for indices 0
 * and 2 sharing the same trailing four values, followed by a pair of
 * 3-argument calls sharing the same leading two arguments, and a closing
 * 1-argument call. */
void Scene_RunSupplementalSequenceTwo(void)
{
    Audio_PlayCue_1(188);
    Field_SetRect(0, 63, 51, 8, 2, 2);
    Field_unk_02002d8c(10);
    Field_SetRect2(2, 63, 51, 8, 2, 2);
    Field_unk_02002da2(10);
    Motion_SetPosReset_1(0, 352, 306);
    Motion_SetActionVariant_1(0, 3);
    Motion_SetPosReset_2(0, 352, 296);
    Field_unk_02000ef4(2);
}

/* Runs four fixed scene-helper calls in sequence, one of them passed the
 * address of Value as its first argument. */
void Scene_RunSupplementalSequenceThree(void)
{
    Audio_PlayCue_1(158);
    Field_unk_02002e38((s32)&Value, 43, 15); /* main:08009178 */
    ObjectMotion_ResetAndSetPositionInMode2_1(0, 230, 0x197);
    Field_unk_02000f20(3);
}

/* Runs four scene calls in sequence: a single-argument call, a call that
 * passes the address of Value with two more values, a call that
 * passes 0, 374, and 0x1a3, and a final single-argument call. */
void Scene_RunSupplementalSequenceFour(void)
{
    Audio_PlayCue_1(158);
    Field_unk_02002e68((s32)&Value, 52, 18); /* main:08009178 */
    ObjectMotion_ResetAndSetPositionInMode2_1(0, 374, 0x1a3); /* object_id 0, x 374, z 0x1a3 */
    Field_unk_02000f52(4);
}

/* Runs a fixed sequence of four scripted calls: one keyed off Value
 * with two small numeric arguments, one with a 0x222 argument, and two plain
 * single-argument calls. */
void Scene_RunSupplementalSequenceFive(void)
{
    Audio_PlayCue_1(158);
    Field_unk_02002e98((s32)&Value, 41, 32); /* main:08009178 */
    ObjectMotion_ResetAndSetPositionInMode2_1(0, 200, 0x222);
    Field_unk_02000f80(5);
}

/* Runs four scripted calls with fixed literal arguments: a single-argument
 * call, a 3-argument call whose first argument is the address of
 * Value, another 3-argument call, and a closing single-argument
 * call. */
void Scene_RunSupplementalSequenceSix(void)
{
    Audio_PlayCue_1(158);
    Motion_SetPosReset_1((s32)&Value, 35, 36); /* main:08009178 */
    ObjectMotion_ResetAndSetPositionInMode2_1(0, 102, 0x263); /* object_id 0, x 102, z 611 */
    Field_unk_02000fb0(6);
}

/* Runs four scripted scene calls in sequence, passing a byte's address and a
 * handful of small immediate constants to each. */
void Scene_RunSupplementalSequenceSeven(void)
{
    Audio_PlayCue_1(158);
    Field_unk_02002ef8((s32)&Value, 51, 39); /* main:08009178 */
    ObjectMotion_ResetAndSetPositionInMode2_1(0, 358, 0x29e);
    Field_unk_02000fe2(7);
}

void Scene_RunStep7BThen8(void)
{
    Field_unk_0200312e(123);
    Field_unk_02000ffc(8);
}

void State_ApplyFlag815Branch(void)
{
    if (Field_TestFlag815(0x815) != 0) {
        Field_unk_0200314c(123);
        Field_unk_0200101a(10);
    }
}

void State_ApplyFlag90b(void)
{
    Field_TestFlag90b(0x90b);
}

void State_ApplyFlag90c(void)
{
    Field_TestFlag90c(0x90c);
}

void State_ApplyFlag90d(void)
{
    Field_TestFlag90d(0x90d);
}

void Scene_RunSplitPairSteps(s32 a, s32 b)
{
    Field_unk_02004a44(a, 0);
    Field_unk_0200496a(b);
}

void Actor_SetPairZeroAndValue(s32 a, s32 b, s32 c)
{
    Field_unk_02004a74(a, b, 0);
    Field_unk_02004982(c);
}

void OvObj_UpdateOnFrameBit1(s32 obj)
{
    if ((*(volatile s32 *)0x03001e40 & 2) != 0) {
        Field_unk_02004968(obj, 7);
    } else {
        Field_unk_02004972(obj, 0);
    }
    if ((*(volatile s32 *)0x03001e40 & 15) == 0) {
        Field_unk_02004768(obj);
    }
}

void Effect_UpdateByFrameBits(s32 no)
{
    volatile s32 *p = (volatile s32 *)0x03001e40;
    if ((*p & 1) != 0) {
        s32 t = Field_unk_02004930((u32)*p >> 1, 6);
        Field_unk_020049b0(no, t);
    }
    if ((*p & 15) == 0) {
        Field_unk_020047a4(no);
    }
}

void Effect_UpdateByFrameBit(s32 no)
{
    volatile s32 *p = (volatile s32 *)0x03001e40;
    if ((*p & 1) != 0) {
        s32 t = Field_unk_0200496c((u32)*p >> 1, 6);
        Field_unk_020049ec(no, t);
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
        Field_unk_020049e4(effect);
        return;
    }
    amplitude = Field_unk_020049cc(frame << 10);
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
        Field_unk_02004a34(effect);
        return;
    }
    amplitude = Field_unk_02004a1c(frame << 10);
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
    Field_unk_02004d98(140, 0);
}

void Scene_Forward(void)
{
    Field_unk_02004dac();
}

void Scene_RunStep15(void)
{
    Field_unk_02004c4a(15);
    Field_unk_020048be();
}

void Scene_RunStep17(void)
{
    Field_unk_02004c5a(17);
    Field_unk_020048ce();
}

void Scene_RunStep20(void)
{
    Field_unk_02004c6a(20);
    Field_unk_0200491a();
}

void State_SetValues352_365_2116_2117_40(void)
{
    Field_TestFlag160(352);
    Field_TestFlag16d(0x16d);
    Field_TestFlag844(0x844);
    Field_TestFlag845(0x845);
    Field_unk_02004d9c(40);
}
