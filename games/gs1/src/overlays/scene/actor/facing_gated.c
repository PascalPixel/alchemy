#include "types.h"
#include "scene.h"
#include "facing_object.h"

/* overlays/scene/actor/facing_gated/actor_facing.c */

typedef struct {
    u8 pad_to_angle[6];
    u16 angle;
} ActorState;

ActorState *GetActorState(s32 actor_id);

s32 IsActorFacingInward(void)
{
    ActorState *actor = GetActorState(0);

    if ((u32)((actor->angle + 0x5fff) << 16) <= 0x3ffe0000) {
        return 1;
    }
    return 0;
}

/* overlays/scene/actor/facing_gated/facing_target_scene.c */
/* Close through scene 8 when facing inward; otherwise select the story line. */

void Scene_RunActorEightFacingDialogue(void)
{
    if (Actor_unk10() != 0) {
        Actor_unk9_2(8);
        return;
    }

    Actor_unk2_4();
    if (Actor_unk11(0x87a) != 0)
        Actor_unk10_2(0x1bfc);
    else if (Actor_unk12(0x815) != 0)
        Actor_unk11_2(0x119d);
    else
        Actor_unk12_2(0x1035);
    Actor_Apply(8, 0);
    Actor_unk3_4();
}

/* overlays/scene/actor/facing_gated/orbiting_effect.c */
/*
 * Ellipse orbit step for resource_378.  The object is offset from its anchor
 * along two axes and its angle advanced once per call.
 */
s32 Actor_unk4_4();           /* Relocated call word, not a runtime address. */

s32 Actor_unk5_4();           /* Relocated call word, not a runtime address. */

/*
 * The anchor at +104 supplies the centre; the result is published to the
 * object's +8/+16 and to its +56/+64 shadow pair, with +56 taken from a fresh
 * read of +8 rather than from x.  The two imports take the same angle and form
 * a cosine/sine pair; which is which is not settled.  The radii 14 and 10 and
 * the +100/+102 displacements are built from immediates.  The angle at +100 and
 * its step at +102 are separate halfwords, not one 32-bit field.
 */
void Effect_StepEllipseOrbit(u8 *obj)
{
    u8 *anchor = *(u8 **)(obj + 104);
    u16 *angle = (u16 *)(obj + 100);
    u16 theta = *angle;
    s32 x;
    s32 z;
    s32 tmp;

    x = *(s32 *)(anchor + 8) + Actor_unk4_4(theta)* 14;
    *(s32 *)(obj + 8) = x;

    z = *(s32 *)(anchor + 16) + Actor_unk5_4(theta)* 10;
    tmp = *(s32 *)(obj + 8);
    *(s32 *)(obj + 16) = z;
    *(s32 *)(obj + 64) = z;
    *(s32 *)(obj + 56) = tmp;

    *angle = (u16)(*angle + *(u16 *)(obj + 102));
}

/* overlays/scene/actor/facing_gated/overlay_object.c */

struct FacingObject *ResolveFacingObject(s16);

s32 UpdateFacingFromResolvedObject(struct FacingObject *object)
{
    struct FacingObject *target;

    target = ResolveFacingObject(object->unknown_64);
    object->facing = CalculateFacingAngle(
        target->position_z - object->position_z,
        target->position_x - object->position_x
    );
    return 0;
}

/* overlays/scene/actor/facing_gated/scene_data.c */

extern u8 gOv;
extern u8 gOv2;
extern u8 gOv3[];   /* Empty table: place nothing. */
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

/* One symbol per call site, named at the site's decoded address. */

void *SceneData_GetTableBaa8(void)
{
    return &gOv;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

void *SceneData_GetTableBbc8(void)
{
    return &gOv2;
}

/*
 * Select this scene's placement script from its stored sub-state.  The
 * 220-byte owner at 0x02000070 includes the 35-entry jump table at
 * 0x0200008c-0x02000117 and the literal pool at 0x02000130-0x0200014b.  The
 * selector is the signed halfword at offset 450 of the scene-record block, and
 * its address is built rather than folded: the `s32 off = 450;` local in its
 * own block is what forces that.  The out-of-range arm is also the arm for
 * most of the in-range entries, so it is a genuine default.
 */

/*
 * In-image script tables: runs of 24-byte records terminated by 0x0000ffff, in
 * the shape {0xffff0000 | selector, count, x, 0, z, value} with coordinates in
 * 16.16 fixed point.  The layout is read off the data, so the fields at +12
 * and +20 are named by position only, and the selector's return type stays an
 * opaque pointer.  The tables live in EWRAM, which is writable and used as
 * save state, so they are deliberately not const.
 */

/* Shared cross-overlay scene-record block; +450 is the scene sub-state. */

/*
 * Select a table from the scene id and two flags.  The 304-byte owner at
 * 0x0200014c decomposes as dispatcher, the 41-entry jump table at
 * 0x02000168-0x0200020b, the case bodies, an alignment halfword at 0x02000246
 * and the literal pool.  Case-arm order follows the table -- entries grouped
 * by value, distinct values ascending -- so the 20/21/50 arm comes third and
 * the 29 arm after the 32 arm, not in selector order.
 */
void *SceneData_SelectPlacementTableBySubstate(void)
{
    extern u8 gCell[];

    s16 sub;

    {
        s32 off = 450;

        sub = *(s16 *)(gCell + off);
    }
    switch ((s32)sub) {
    case 1:
    case 2:
        return gOv4;

    case 10:
    case 11:
    case 12:
    case 35:
        return gOv5;

    case 20:
    case 21:
        return gOv6;

    case 29:
    case 32:
        return gOv7;

    default:
        return gOv3;
    }
}

u8 *SceneData_SelectTableBySceneIdAndFlags(void)
{
    extern s16 gCell[];

    s16 *tbl = gCell;
    s32 no = tbl[225];

    switch (no) {
    case 10:
    case 12:
        return gOv9;
    case 11:
        return gOv10;
    case 20:
    case 21:
    case 50:
        return gOv11;
    case 32:
        return gOv16;
    case 29:
        return gOv14;
    case 35:
        return gOv15;
    default:
        break;
    }

    if (Actor_unk13(0x87a) != 0) {
        return gOv13;
    }
    if (Actor_unk14(0x815) != 0) {
        return gOv12;
    }
    return gOv8;
}

/* overlays/scene/actor/facing_gated/scene_dialogue.c */
/* Close scene 8 when facing inward; otherwise choose its story line. */

/* Close scene 8 when facing inward; otherwise emit its conditional follow-up. */

/* Close scene 8 when facing inward; otherwise emit its fixed story line. */

void Dialogue_RunActorEightFlaggedDialogue(void)
{
    if (Actor_unk15() != 0) {
        Actor_unk13_2(8);
        return;
    }

    Actor_unk6_4();
    if (Actor_unk16(0x845) != 0)
        Actor_unk14_2(0x171c);
    else
        Actor_unk15_2(0x1408);
    Actor_Apply2(8, 0);
    Actor_unk7_4();
}

void Dialogue_RunActorEightFollowupDialogue(void)
{
    if (Actor_unk17() != 0) {
        Actor_unk16_2(8);
        return;
    }

    Actor_unk8_4();
    Actor_unk17_2(0x190a);
    if (Actor_unk18(0x909) != 0)
        Actor_unk18_2(0x1951);
    Actor_Apply3(8, 0);
    Actor_unk9_4();
}

void Dialogue_RunActorEightDialogue(void)
{
    if (Actor_unk19() != 0) {
        Actor_unk19_2(8);
        return;
    }

    Actor_unk10_4();
    Actor_unk20_2(0x1823);
    Actor_Apply4(8, 0);
    Actor_unk11_4();
}

/* overlays/scene/actor/facing_gated/scene_primary_script.c */

extern s16 gCell[];

u8 *Actor_unk12_4();

u8 *Actor_unk13_4();

u8 *Actor_unk14_4();

u8 *Scene_GetRecord_1();

u8 *Scene_GetRecord_1();
u8 *Scene_GetRecord_2();

u8 *Scene_GetRecord_6();

u8 *Scene_GetRecord_7();
u8 *Scene_GetRecord_8();

u8 *Scene_GetRecord_9();
u8 *Scene_GetRecord_10();

u8 *ObjectMotion_SetVariantCallback_5();
u8 *Motion_SetPosReset_10();
u8 *Scene_GetRecord_5();
u8 *Actor_unk15_4();
u8 *Actor_unk16_4();
u8 *Scene_GetRecord_11();

/*
 * Each Func_ symbol names the pre-relocation call word the image holds, not
 * a runtime address; a single word can serve two sites with different
 * targets. Where a macro names an engine function, that is the function the
 * site reaches through the overlay veneer and the main-image veneer island,
 * keeping the site's own calling form. Names without a binding in the
 * repository are provisional.
 */

static __inline__ void bump_step(s32 amount)
{
    void Motion_SetVarCb_1();
    u8 *Scene_GetRecord_2();

    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ void bump_step2(s32 amount)
{
    extern u8 gWork[];
    u8 *Motion_SetVarCb_1();
    s32 Scene_GetRecord_2();

    u8 *work = *(u8 **)gWork;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/*
 * Runs actor nine's flag-branched dialogue. The 112-byte owner includes its
 * five pool words. The scene selector is the signed halfword at
 * gCell + 450, reached as index 225. The last two calls must stay
 * after the selector test: the epilogue pops the return address into r0, so
 * the second call's result is discarded there.
 */
void Scene_RunActorNineFlagDialogueA(void)
{
    Actor_unk17_4();

    if (Actor_unk20(0x855) != 0) {
        Actor_unk21_2(0x1377);
    } else {
        Actor_unk22_2(0x1289);
    }

    if (gCell[225] == 11) {
        Actor_unk23_2(0x1ce9);
    }

    Actor_Apply5(9, 1);
    Actor_unk4_3(9, 0, 0);
    Actor_unk24_2(2);
    Actor_Apply6(9, 0);
    Actor_unk18_4();
}

/*
 * The sibling path to the dialogue above, reading the same selector
 * halfword. The 116-byte owner includes its five pool words.
 */
void Scene_RunActorNineFlagDialogueB(void)
{
    void Actor_unk19_4(s32 id);
    void Actor_unk20_4(s32 id, s32 arg1);

    Actor_unk21_4();

    if (Actor_unk21(0x855) == 0) {
        Actor_unk25_2(0x128b);
    } else {
        Actor_unk26_2(0x1379);
    }

    if (gCell[225] == 11) {
        Actor_unk27_2(0x1ceb);
    }

    Actor_unk19_4(9);
    Actor_Apply7(9, 1);
    Actor_unk28_2(2);
    Actor_unk20_4(9, 0);
    Actor_Apply8(9, 2);
    Actor_unk22_4();
}

void Scene_RunSupplementalSequenceOne(void)
{
    extern u8 gWork[];

    u32 i;
    u8 *record;

    Actor_unk23_4();
    Actor_Do(0x1164);
    Actor_Check(8, 0);
    if (Actor_unk2(0, 0) == 0) {
        Actor_unk24_4(8, 3);
        Actor_unk25_4(20);
    } else {
        Actor_unk26_4(20);
        Actor_unk3(8, 0);
        if (Actor_unk4(0, 0) != 0) {
        } else {
            Actor_unk27_4(20);
            Actor_unk5(8, 0);
            if (Actor_unk6(0, 0) != 0) {
            } else {
                Actor_unk28_4(20);
                record = Actor_unk12_4(8);
                {
                    u32 shown = (u32)*(u16 *)((s32)record + 6);
                    u32 low = 0xa000;

                    if (shown < low) {
                        goto L_02000488;
                    }
                }
                record = Actor_unk13_4(8);
                {
                    u32 shown = (u32)*(u16 *)((s32)record + 6);
                    u32 high = 0xe000;

                    if (shown > high) {
                        goto L_02000488;
                    }
                }
                Actor_Place(8, 0x8000, 0x4000);
                Actor_unk29_4(8, 0, 0);
                Actor_unk2_2(10);
                *(u8 *)(Actor_unk22(8) + 90) &= 254;
                Actor_unk30_4(8, 152, 120);
                Actor_unk31_4(1);
                {
                    u8 *record = Actor_unk14_4(8);
                    u8 value = *(volatile u8 *)&record[90];

                    record[90] = (u8)(value | 1);
                }
                Actor_unk32_4(20);
                Actor_unk33_4(8, 3);
                Actor_unk34_4(20);
                Actor_unk35_4(0, 168, 120);
                Actor_unk36_4(0, 192, 168);
                Actor_unk37_4(20);
                Actor_unk38_4(8, 168, 120);
                Actor_unk2_3(8, 0x3000, 0);
                Actor_unk39_4(0);
                goto L_020004aa;
                L_02000488:
                Actor_unk40_4(0, 192, 168);
                Actor_unk41_4(20);
                Actor_unk3_3(8, 0x3000, 0);
                Actor_unk42_4(0);
                L_020004aa:
                Actor_unk43_4();
                Actor_unk44_4(0, 0);
                Actor_unk45_4(120);
                Actor_unk46_4(120);
                Actor_unk47_4(86);
                Actor_unk48_4();
                Actor_unk3_2(0x9f0);
                Actor_unk49_4(30);
            }
        }
    }
    Actor_unk50_4();
}

void Scene_RunScene378SequenceB(void)
{
    extern u8 gWork[];
    void Actor_unk19_4();
    u8 *Actor_unk20_4();

    u32 i;
    s32 record;

    Actor_unk51_4();
    Actor_unk4_2(0x200bc9c);
    Actor_unk5_2(1);
    Actor_unk6_2(0x1bfd);
    Actor_unk7(9, 0);
    if (Actor_unk8(0, 0) == 0) {
        Actor_unk52_4(9, 0);
    } else {
        bump_step();
        Actor_SetRect(2, 16, 1, 24, 1, 3, 7, 16, 1, 14, 0);
        Actor_unk53_4(9, 0);
    }
    Actor_unk54_4();
}

/*
 * Steps actor ten and, when the check passes, increments the same workspace
 * +472 halfword the preceding owner writes. The 88-byte owner includes its
 * two pool words.
 */
void Scene_RunActorTenCountStep(void)
{
    extern u8 *gWork;

    Actor_unk55_4();
    Actor_unk56_4(10, 0, 0);
    Actor_unk57_4(10);
    Actor_unk58_4(0x119f);
    Actor_unk23(10, 0);

    if (Actor_unk24(0, 0) == 1) {
        (*(u16 *)(gWork + 472))++;
    }

    Actor_unk59_4(10, 0);
    Actor_unk60_4();
}

/*
 * Steps a fixed sequence of actor position, pose and timing calls over
 * slots 0, 1, 8, 9, 10, 11, 12 and 13, including one loop that nudges a
 * pair of per-actor record fields down 32 times.
 */
void Scene_RunActorUpdateSequence(void)
{
    void Motion_SetVarCb_1();
    u8 *Scene_GetRecord_2();

    u32 i;
    u8 *record;

    Battle_WaitMode0_1(1, 0x102);
    Motion_SetVarCbObj_1(1, 2);
    Battle_WaitMode0_2(20);
    Motion_SetSpeed_1(0, 0xcccc, 0x6666);
    Motion_SetSpeed_2(1, 0xcccc, 0x6666);
    Motion_SetSpeed_3(11, 0xcccc, 0x6666);
    Motion_SetSpeed_4(12, 0xcccc, 0x6666);
    Motion_SetSpeed_5(9, 0xcccc, 0x6666);
    Motion_SetSpeed_6(10, 0xcccc, 0x6666);
    Motion_SetSpeed_7(8, 0xcccc, 0x6666);
    Motion_CamBounds_1(0xc00000, -1, 0xa00000, 1);
    Object_CommitPositionThenWaitIfModeZero_1();
    Motion_SetAngleToward_1(1, 0, 0);
    Motion_SetAngleToward_2(12, 0, 0);
    Motion_SetAngleToward_3(9, 0, 0);
    Motion_SetAngleToward_4(10, 0, 0);
    Object_LinkPair_1(0, 11, 0);
    Battle_WaitMode0_3(10);
    Object_SetModeById_1(0, 3);
    Motion_CallWaitAnim_1(11, 3);
    Battle_WaitMode0_4(60);
    Object_SetModeById_2(8, 3);
    Object_SetModeById_3(12, 3);
    Object_SetModeById_4(9, 3);
    Motion_CallWaitAnim_2(10, 3);
    Battle_WaitMode0_5(50);
    Motion_SetAngleToward_5(0, 8, 0);
    Motion_SetAngleToward_6(11, 8, 0);
    Motion_SetAngleToward_7(12, 8, 0);
    Motion_SetAngleToward_8(9, 8, 0);
    Motion_SetAngleToward_9(10, 8, 0);
    Battle_WaitMode0_6(30);
    Motion_CallWaitAnim_3(1, 4);
    Battle_WaitMode0_7(20);
    SceneWork_SetStepValue_1(0x1171);
    BattleEv_RunWait_1(1, 0);
    Battle_WaitMode0_8(20);
    Motion_SetAngleToward_10(12, 1, 0);
    Motion_SetVarCb_1(12, 2);
    BattleFx_SpawnLinked_1(12, 0x103, 0);
    Battle_WaitMode0_9(60);
    BattleEv_RunWait_2(12, 0);
    Battle_WaitMode0_10(20);
    Motion_SetVarCbObj_2(1, 2);
    Battle_WaitMode0_11(20);
    Motion_SetAngleToward_11(1, 12, 0);
    Battle_WaitMode0_12(20);
    Motion_CallWaitAnim_4(1, 3);
    Battle_WaitMode0_13(40);
    Motion_SetAngleToward_12(1, 8, 0);
    Motion_SetAngleToward_13(12, 8, 0);
    Battle_WaitMode0_14(20);
    Motion_CallWaitAnim_5(8, 3);
    Battle_WaitMode0_15(20);
    Motion_ArmCb_1(8, 0xd000, 0);
    Battle_WaitMode0_16(30);
    Motion_CallWaitAnim_6(8, 3);
    Battle_WaitMode0_17(30);
    BattleEv_RunWait_3(8, 0);
    Battle_WaitMode0_18(80);
    Audio_PlayCue_1(17);
    Battle_WaitMode0_19(0x10005, 1);
    Battle_WaitMode0_20(60);
    Battle_WaitMode0_21(40);
    Motion_SetSpeedLim_1(0x6666, 0xccc);
    Motion_CamBounds_2(0xc00000, -1, 0x680000, 1);
    Battle_WaitMode0_22(120);
    Audio_PlayCue_2(21);
    Audio_PlayCue_3(0x134);
    Motion_SetHPosTerrain_1(13, 0xc80000, 0x80000);
    Motion_SetSpeed_8(13, 0x6666, 0x3333);
    Motion_SetPositionAndCommit_1(13, 200, 72);
    Audio_PlayCue_4(0x120);
    Battle_WaitMode0_23(30);
    Motion_SetVarCbObj_3(8, 2);
    Object_SetModeById_5(8, 0);
    Actor_unk61_4();
    Motion_SetAngleToward_14(0, 13, 0);
    Motion_SetAngleToward_15(1, 13, 0);
    Motion_SetAngleToward_16(11, 13, 0);
    Motion_SetAngleToward_17(12, 13, 0);
    Motion_SetAngleToward_18(9, 13, 0);
    Motion_SetAngleToward_19(10, 13, 0);
    Motion_SetVarCb_2(0, 2);
    Motion_SetVarCb_3(1, 2);
    Motion_SetVarCb_4(11, 2);
    Motion_SetVarCb_5(12, 2);
    Motion_SetVarCb_6(9, 2);
    Motion_SetVarCb_7(10, 2);
    Battle_WaitMode0_24(40);
    BattleEv_RunWait_4(13, 0);
    Battle_WaitMode0_25(20);
    Battle_WaitMode0_26(40);
    BattleEv_RunWait_5(13, 0);
    Battle_WaitMode0_27(60);
    ObjectGroup_ConfigureChildValue_1(13, 0x100);
    Audio_PlayCue_5(17);
    Audio_PlayCue_6(0x134);
    /*
     * 32 repeats: step actor 13's animation, then subtract a fixed amount
     * from the record fields at +24 and +28.
     */
    for (i = 0; i < 32; i++) {
        Actor_unk62_4(13);
        Battle_WaitMode0_28(4);
        record = Scene_GetRecord_1(13);
        *(s32 *)(record + 24) += -0x28f;
        record = Scene_GetRecord_2(13);
        *(s32 *)(record + 28) += -0x28f;
    }
    Audio_PlayCue_7(0x120);
    ObjectGroup_ConfigureChildValue_2(13, 0);
    Motion_SetHPosTerrain_2(13, 0, 0);
    Battle_WaitMode0_29(40);
    Motion_CamBounds_3(0xc00000, -1, 0xa00000, 1);
    Object_CommitPositionThenWaitIfModeZero_2();
    Battle_WaitMode0_30(0x10000, 0);
    Battle_WaitMode0_31(60);
    Battle_WaitMode0_32(120);
    Actor_unk63_4();
    Object_SetModeById_6(8, 1);
    Audio_PlayCue_8(2);
    Battle_WaitMode0_33(60);
    Motion_SetVarCbObj_4(8, 2);
    Battle_WaitMode0_34(20);
    Motion_SetVarCb_8(0, 1);
    Motion_SetVarCb_9(1, 1);
    Motion_SetVarCb_10(11, 1);
    Motion_SetVarCb_11(12, 1);
    Motion_SetVarCb_12(9, 1);
    Motion_SetVarCbObj_5(10, 1);
    Battle_WaitMode0_35(30);
    BattleEv_RunWait_6(11, 0);
    Battle_WaitMode0_36(30);
    Motion_SetAngleToward_20(8, 0, 0);
    Battle_WaitMode0_37(30);
    Motion_SetAngleToward_21(12, 8, 0);
    Battle_WaitMode0_38(20);
    Motion_SetAngleToward_22(0, 8, 0);
    Motion_SetAngleToward_23(1, 8, 0);
    Motion_SetAngleToward_24(11, 8, 0);
    Motion_SetAngleToward_25(9, 8, 0);
    Motion_SetAngleToward_26(10, 8, 0);
    Battle_WaitMode0_39(20);
    BattleEv_RunWait_7(12, 0);
    Battle_WaitMode0_40(20);
    Motion_SetAngleToward_27(8, 12, 0);
    Battle_WaitMode0_41(20);
    Motion_CallWaitAnim_7(8, 3);
    Battle_WaitMode0_42(20);
    Object_SetModeById_7(0, 3);
    Motion_CallWaitAnim_8(1, 3);
    Battle_WaitMode0_43(20);
    Motion_SetAngleToward_28(8, 0, 0);
    Battle_WaitMode0_44(20);
    Motion_CallWaitAnim_9(8, 3);
    Battle_WaitMode0_45(30);
    BattleEv_RunWait_8(8, 0);
    Battle_WaitMode0_46(20);
    BattleFx_SpawnLinked_2(0, 0x102, 0);
    BattleFx_SpawnLinked_3(1, 0x102, 0);
    BattleFx_SpawnLinked_4(11, 0x102, 0);
    BattleFx_SpawnLinked_5(12, 0x102, 0);
    BattleFx_SpawnLinked_6(9, 0x102, 0);
    BattleFx_SpawnLinked_7(10, 0x102, 0);
    Battle_WaitMode0_47(30);
    BattleFx_SpawnLinked_8(8, 0x102, 0);
    Battle_WaitMode0_48(30);
    BattleEv_RunWait_9(8, 0);
    Battle_WaitMode0_49(20);
    Object_SetModeById_8(0, 3);
    Object_SetModeById_9(1, 3);
    Object_SetModeById_10(11, 3);
    Object_SetModeById_11(12, 3);
    Object_SetModeById_12(9, 3);
    Motion_CallWaitAnim_10(10, 3);
    Battle_WaitMode0_50(20);
    Motion_CallWaitAnim_11(8, 3);
    Battle_WaitMode0_51(180);
    BattleFx_SpawnLinked_9(12, 0x105, 0);
    Battle_WaitMode0_52(60);
    Object_LinkPair_2(0, 1, 0);
    Object_LinkPair_3(9, 10, 0);
    Battle_WaitMode0_53(40);
    Motion_SetAngleToward_29(0, 8, 0);
    Motion_SetAngleToward_30(1, 8, 0);
    Motion_SetAngleToward_31(9, 8, 0);
    Motion_SetAngleToward_32(10, 8, 0);
    Battle_WaitMode0_54(40);
    Motion_SetVarCbObj_6(1, 2);
    Battle_WaitMode0_55(20);
    BattleEv_RunWait_10(1, 0);
    Battle_WaitMode0_56(30);
    BattleFx_SpawnLinked_10(8, 0x101, 0);
    Battle_WaitMode0_57(60);
    Motion_SetVarCb_13(0, 1);
    Motion_SetVarCb_14(1, 1);
    Motion_SetVarCb_15(11, 1);
    Motion_SetVarCb_16(12, 1);
    Motion_SetVarCb_17(9, 1);
    Motion_SetVarCbObj_7(10, 1);
    Battle_WaitMode0_58(30);
    BattleEv_RunWait_11(11, 0);
    Battle_WaitMode0_59(20);
    BattleFx_SpawnLinked_11(8, 0x101, 0);
    Battle_WaitMode0_60(80);
    BattleFx_SpawnLinked_12(12, 0x102, 0);
    Battle_WaitMode0_61(60);
    BattleEv_RunWait_12(12, 0);
    Battle_WaitMode0_62(30);
    BattleFx_SpawnLinked_13(8, 0x105, 0);
    Battle_WaitMode0_63(60);
    Motion_SetVarCbObj_8(8, 1);
    BattleFx_SpawnLinked_14(8, 0x106, 0);
    Battle_WaitMode0_64(20);
    Motion_SetVarCb_18(0, 1);
    Motion_SetVarCb_19(1, 1);
    Motion_SetVarCb_20(11, 1);
    Motion_SetVarCb_21(12, 1);
    Motion_SetVarCb_22(9, 1);
    Motion_SetVarCbObj_9(10, 1);
    Battle_WaitMode0_65(40);
    BattleEv_RunWait_13(8, 0);
    Battle_WaitMode0_66(20);
    Object_SetModeById_13(0, 3);
    Object_SetModeById_14(1, 3);
    Object_SetModeById_15(11, 3);
    Object_SetModeById_16(12, 3);
    Object_SetModeById_17(9, 3);
    Motion_CallWaitAnim_12(10, 3);
    Battle_WaitMode0_67(30);
    BattleEv_RunWait_14(8, 0);
    Battle_WaitMode0_68(30);
    Object_SetModeById_18(0, 3);
    Object_SetModeById_19(1, 3);
    Object_SetModeById_20(11, 3);
    Object_SetModeById_21(12, 3);
    Object_SetModeById_22(9, 3);
    Motion_CallWaitAnim_13(10, 3);
    Battle_WaitMode0_69(120);
    Motion_SetVarCbObj_10(1, 1);
    Battle_WaitMode0_70(20);
    BattleEv_RunWait_15(1, 0);
    Battle_WaitMode0_71(30);
    Motion_CallWaitAnim_14(8, 3);
    Battle_WaitMode0_72(20);
    BattleEv_RunWait_16(8, 0);
    Battle_WaitMode0_73(20);
    BattleFx_SpawnLinked_15(0, 0x105, 0);
    BattleFx_SpawnLinked_16(1, 0x105, 0);
    BattleFx_SpawnLinked_17(11, 0x105, 0);
    BattleFx_SpawnLinked_18(12, 0x100, 0);
    BattleFx_SpawnLinked_19(9, 0x105, 0);
    BattleFx_SpawnLinked_20(10, 0x105, 0);
    Battle_WaitMode0_74(60);
    Motion_SetVarCbObj_11(12, 1);
    BattleEv_RunWait_17(12, 0);
    Battle_WaitMode0_75(20);
    Motion_SetAngleToward_33(8, 12, 0);
    Battle_WaitMode0_76(20);
    Motion_CallWaitAnim_15(8, 4);
    Battle_WaitMode0_77(20);
    BattleEv_RunWait_18(8, 0);
    Battle_WaitMode0_78(20);
    BattleFx_SpawnLinked_21(0, 0x100, 0);
    BattleFx_SpawnLinked_22(1, 0x100, 0);
    BattleFx_SpawnLinked_23(11, 0x100, 0);
    BattleFx_SpawnLinked_24(12, 0x100, 0);
    BattleFx_SpawnLinked_25(9, 0x100, 0);
    BattleFx_SpawnLinked_26(10, 0x100, 0);
    Battle_WaitMode0_79(60);
    Motion_SetAngleToward_34(8, 0, 0);
    Battle_WaitMode0_80(20);
    Motion_CallWaitAnim_16(8, 3);
    BattleEv_RunWait_19(8, 0);
    Battle_WaitMode0_81(40);
    Motion_SetPosReset_1(8, 168, 176);
    Motion_SetAngleToward_35(0, 8, 0);
    Motion_SetAngleToward_36(1, 8, 0);
    Motion_SetAngleToward_37(11, 8, 0);
    Motion_SetAngleToward_38(12, 8, 0);
    Motion_SetAngleToward_39(9, 8, 0);
    Motion_SetAngleToward_40(10, 8, 0);
    Motion_SetPosReset_2(8, 200, 200);
    Motion_SetAngleToward_41(0, 8, 0);
    Motion_SetAngleToward_42(1, 8, 0);
    Motion_SetAngleToward_43(12, 8, 0);
    Motion_ArmCb_2(11, 0, 0);
    Motion_ArmCb_3(9, 0x8000, 0);
    Motion_ArmCb_4(10, 0x8000, 0);
    Motion_ResetPosMode2_1(8, 200, 0x110);
    Battle_WaitMode0_82(40);
    Motion_ArmCb_5(11, 0x3000, 0);
    Motion_ArmCb_6(9, 0x5000, 0);
    Motion_ArmCb_7(10, 0x5000, 0);
    Motion_CommitPos_1(8);
    Motion_SetHPosTerrain_3(8, 0, 0);
    Battle_WaitMode0_83(60);
    BattleFx_SpawnLinked_27(0, 0x102, 0);
    BattleFx_SpawnLinked_28(1, 0x102, 0);
    BattleFx_SpawnLinked_29(11, 0x102, 0);
    BattleFx_SpawnLinked_30(12, 0x102, 0);
    BattleFx_SpawnLinked_31(9, 0x102, 0);
    BattleFx_SpawnLinked_32(10, 0x102, 0);
    Battle_WaitMode0_84(60);
    Battle_WaitMode0_85(120);
    Motion_SetAngleToward_44(0, 9, 0);
    Motion_SetAngleToward_45(1, 10, 0);
    Motion_SetAngleToward_46(11, 9, 0);
    Motion_SetAngleToward_47(12, 9, 0);
    Battle_WaitMode0_86(120);
    BattleFx_SpawnLinked_33(9, 0x105, 0);
    Battle_WaitMode0_87(60);
    Motion_SetAngleToward_48(9, 0, 0);
    Battle_WaitMode0_88(10);
    Motion_SetVarCbObj_12(9, 2);
    Battle_WaitMode0_89(10);
    Motion_ArmCb_8(9, 0x5000, 0);
    Battle_WaitMode0_90(20);
    BattleFx_SpawnLinked_34(9, 0x102, 0);
    Battle_WaitMode0_91(80);
    Motion_SetAngleToward_49(9, 10, 0);
    Motion_SetVarCbObj_13(9, 1);
    Battle_WaitMode0_92(20);
    BattleEv_RunWait_20(9, 0);
    Battle_WaitMode0_93(20);
    Motion_ArmCb_9(10, 0x5000, 0);
    Battle_WaitMode0_94(20);
    BattleFx_SpawnLinked_35(10, 0x101, 0);
    Battle_WaitMode0_95(60);
    Motion_SetVarCbObj_14(9, 2);
    Battle_WaitMode0_96(20);
    Motion_SetAngleToward_50(10, 0, 0);
    Battle_WaitMode0_97(30);
    Motion_SetVarCbObj_15(10, 2);
    Battle_WaitMode0_98(30);
    Motion_ArmCb_10(10, 0x5000, 0);
    Battle_WaitMode0_99(20);
    BattleEv_RunWait_21(10, 0);
    Battle_WaitMode0_100(20);
    Object_SetModeById_23(9, 3);
    Motion_CallWaitAnim_17(10, 3);
    Battle_WaitMode0_101(20);
    Motion_ResetPosMode2_2(9, 200, 0x110);
    Motion_SetPosReset_3(10, 200, 0x110);
    Motion_SetHPosTerrain_4(9, 0, 0);
    Motion_SetHPosTerrain_5(10, 0, 0);
    BattleFx_SpawnLinked_36(12, 0x105, 0);
    Battle_WaitMode0_102(60);
    Motion_SetPosReset_4(12, 200, 136);
    Object_LinkPair_4(0, 12, 0);
    Motion_SetAngleToward_51(1, 12, 0);
    Motion_SetAngleToward_52(11, 12, 0);
    Battle_WaitMode0_103(30);
    Motion_CallWaitAnim_18(12, 4);
    Battle_WaitMode0_104(20);
    BattleEv_RunWait_22(12, 0);
    Battle_WaitMode0_105(30);
    Object_SetModeById_24(0, 3);
    Motion_CallWaitAnim_19(1, 3);
    Battle_WaitMode0_106(20);
    BattleEv_RunWait_23(11, 0);
    Battle_WaitMode0_107(20);
    Motion_SetPosReset_5(11, 168, 168);
    Motion_SetAngleToward_53(11, 12, 0);
    Battle_WaitMode0_108(20);
    Motion_SetAngleToward_54(0, 11, 0);
    Motion_SetAngleToward_55(1, 11, 0);
    Battle_WaitMode0_109(20);
    Motion_CallWaitAnim_20(11, 4);
    Battle_WaitMode0_110(20);
    BattleEv_RunWait_24(11, 0);
    Battle_WaitMode0_111(20);
    BattleFx_SpawnLinked_37(0, 0x102, 0);
    BattleFx_SpawnLinked_38(1, 0x102, 0);
    Battle_WaitMode0_112(60);
    Motion_CallWaitAnim_21(12, 4);
    Battle_WaitMode0_113(20);
    Motion_SetAngleToward_56(0, 12, 0);
    Motion_SetAngleToward_57(1, 12, 0);
    Battle_WaitMode0_114(20);
    BattleEv_RunWait_25(12, 0);
    Battle_WaitMode0_115(20);
    Motion_SetVarCbObj_16(11, 2);
    Battle_WaitMode0_116(20);
    BattleEv_RunWait_26(11, 0);
    Battle_WaitMode0_117(20);
    BattleFx_SpawnLinked_39(0, 0x102, 0);
    BattleFx_SpawnLinked_40(1, 0x102, 0);
    Battle_WaitMode0_118(60);
    BattleFx_SpawnLinked_41(12, 0x102, 0);
    Battle_WaitMode0_119(60);
    BattleEv_RunWait_27(12, 0);
    Battle_WaitMode0_120(20);
    Motion_SetVarCb_23(0, 2);
    Motion_SetVarCb_24(1, 2);
    BattleFx_SpawnLinked_42(0, 0x100, 0);
    BattleFx_SpawnLinked_43(1, 0x100, 0);
    Battle_WaitMode0_121(60);
    Motion_CallWaitAnim_22(11, 3);
    Battle_WaitMode0_122(20);
    BattleEv_RunWait_28(11, 0);
    Battle_WaitMode0_123(20);
    Motion_SetAngleToward_58(0, 11, 0);
    Battle_WaitMode0_124(30);
    Object_SetModeById_25(11, 3);
    Battle_WaitMode0_125(30);
    Object_SetModeById_26(12, 3);
}

void Scene_RunPairedActorChoreography(void)
{
    extern u8 gWork[];
    u8 *Motion_SetVarCb_1();
    s32 Scene_GetRecord_2();

    u32 i;
    u8 *record;
    s32 none;

    ObjectMotion_SetSpeedParameters_1(0, 0x18000, 0xc000);
    ObjectMotion_SetSpeedParameters_2(1, 0x18000, 0xc000);
    Actor_Run(12, 2); /* main:0808a138 */
    BattleRuntime_WaitIfModeZero_1(10);
    ObjectMotion_CallThenWaitForAnimationChange_1(12, 3); /* main:0808a110 */
    BattleRuntime_WaitIfModeZero_2(10);
    ObjectMotion_CallThenWaitForAnimationChange_2(0, 3); /* main:0808a110 */
    BattleRuntime_WaitIfModeZero_3(15);
    ObjectMotion_SetAngleToward_1(0, 1, 0);
    ObjectMotion_SetVariantCallback_5(0, 1); /* object 0, variant 1 */
    {
        u8 *record = Scene_GetRecord_1(0); /* main:0808a080 */
        u8 value = *(volatile u8 *)&record[90];
        s32 masked = value & 254;

        none = 0;
        record[90] = (u8)masked;
    }
    ObjectMotion_ResetAndSetPositionInMode2_1(0, 184, 168);
    *(u8 *)(Scene_GetRecord_2(1) + 90) &= 254;
    Motion_SetPosReset_10(1, 200, 168); /* object 1, x 200, z 168 */
    BattleRuntime_WaitIfModeZero_4(1);
    *(u8 *)(Scene_GetRecord_3(1) + 90) |= 1;
    ObjectMotion_CommitCurrentPositionAndActivate_1(0);
    Object_SetModeById_1(0, 1);
    *(u8 *)(Scene_GetRecord_4(0) + 90) |= 1;
    {
        u8 *record = Scene_GetRecord_5(1);
        u8 value = *(volatile u8 *)&record[90];

        record[90] = (u8)(value | 1);
    }
    Motion_Launch_1(1, 2, 0);
    BattleRuntime_WaitIfModeZero_5(15);
    ObjectMotion_SetAngleToward_2(1, 8, 0);
    Actor_unk7_2(5); /* main:0808a080 */
    Motion_Launch_2(1, 2, 0);
    BattleRuntime_WaitIfModeZero_6(25);
    ObjectMotion_SetVariantCallbackAndInvokeObject_1(1, 2);
    ObjectMotion_SetAngleToward_3(1, 12, 0);
    BattleRuntime_WaitIfModeZero_7(5);
    ObjectMotion_CallThenWaitForAnimationChange_3(1, 3); /* main:0808a110 */
    BattleRuntime_WaitIfModeZero_8(10);
    ObjectMotion_CallThenWaitForAnimationChange_4(0, 3); /* main:0808a110 */
    BattleRuntime_WaitIfModeZero_9(5);
    ObjectMotion_CallThenWaitForAnimationChange_5(0, 3); /* main:0808a110 */
    BattleRuntime_WaitIfModeZero_10(10);
    ObjectMotion_SetAngleToward_4(1, 0, 0);
    BattleRuntime_WaitIfModeZero_11(10);
    ObjectMotion_CallThenWaitForAnimationChange_6(1, 3); /* main:0808a110 */
    BattleRuntime_WaitIfModeZero_12(15);
    Object_SetModeById_2(11, 3);
    Object_SetModeById_3(12, 3);
    Object_SetModeById_4(8, 3);
    Object_SetModeById_5(9, 3);
    ObjectMotion_CallThenWaitForAnimationChange_7(10, 3); /* main:0808a110 */
    BattleRuntime_WaitIfModeZero_13(20);
    ObjectMotion_SetAngleToward_5(0, 12, 0);
    ObjectMotion_SetAngleToward_6(1, 12, 0);
    BattleRuntime_WaitIfModeZero_14(10); /* main:0808a138 */
    Object_SetModeById_6(0, 3);
    ObjectMotion_CallThenWaitForAnimationChange_8(1, 3); /* main:0808a110 */
    BattleRuntime_WaitIfModeZero_15(20);
    ObjectMotion_SetAngleToward_7(0, 11, 0);
    ObjectMotion_SetAngleToward_8(1, 11, 0);
    BattleRuntime_WaitIfModeZero_16(10);
    Object_SetModeById_7(0, 3);
    ObjectMotion_CallThenWaitForAnimationChange_9(1, 3); /* main:0808a110 */
    BattleRuntime_WaitIfModeZero_17(20);
    ObjectMotion_ArmCallback_1(0, 0, 0);
    BattleRuntime_WaitIfModeZero_18(15);
    ObjectMotion_SetVariantCallbackAndInvokeObject_2(0, 2); /* main:0808a138 */
    BattleRuntime_WaitIfModeZero_19(10);
    Actor_unk9(222, 0xb80000, 0x1b0000, 0xa80000);
    ObjectMotion_SetAngleToward_9(1, 0, 0);
    BattleRuntime_WaitIfModeZero_20(10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_3(1, 1);
    BattleRuntime_WaitIfModeZero_21(10);
    Motion_Launch_3(1, 4, 0); /* main:0808a138 */
    ObjectMotion_ArmCallback_2(1, 0xd000, 0);
    BattleRuntime_WaitIfModeZero_22(15);
    ObjectMotion_ArmCallback_3(1, 0xb000, 0);
    BattleRuntime_WaitIfModeZero_23(10);
    ObjectMotion_ArmCallback_4(1, 0xd000, 0);
    BattleRuntime_WaitIfModeZero_24(10);
    ObjectMotion_ArmCallback_5(1, 0xb000, 0);
    BattleRuntime_WaitIfModeZero_25(10);
    ObjectMotion_ArmCallback_6(1, 0xd000, 0);
    BattleRuntime_WaitIfModeZero_26(30);
    Motion_Launch_4(1, 4, 0);
    ObjectMotion_ArmCallback_7(1, 0x3000, 0);
    BattleRuntime_WaitIfModeZero_27(15);
    ObjectMotion_ArmCallback_8(1, 0x5000, 0);
    BattleRuntime_WaitIfModeZero_28(10);
    ObjectMotion_ArmCallback_9(1, 0x3000, 0);
    BattleRuntime_WaitIfModeZero_29(10);
    ObjectMotion_ArmCallback_10(1, 0x5000, 0);
    BattleRuntime_WaitIfModeZero_30(10);
    Motion_ArmCb_11(1, 0x3000, 0);
    BattleRuntime_WaitIfModeZero_31(30);
    Motion_ArmCb_12(0, 0xc000, 0);
    Motion_ArmCb_13(1, 0xc000, 0);
    BattleRuntime_WaitIfModeZero_32(10);
    BattleEffect_SpawnLinkedResourceObject_1(0, 0x102, 0);
    BattleEffect_SpawnLinkedResourceObject_2(1, 0x102, 0);
    BattleRuntime_WaitIfModeZero_33(60);
    ObjectMotion_SetAngleToward_10(1, 12, 0);
    BattleRuntime_WaitIfModeZero_34(10);
    ObjectMotion_CallThenWaitForAnimationChange_10(1, 3); /* main:0808a110 */
    BattleRuntime_WaitIfModeZero_35(10);
    {
        u8 *record = Scene_GetRecord_6(8); /* main:0808a080 */
        s32 shown = 1;

        *(volatile u16 *)(record + 100) = shown;
    }
    record = Scene_GetRecord_7(8); /* main:0808a080 */
    *(volatile s32 *)(record + 108) = 0x2008031;
    {
        u8 *record = Scene_GetRecord_8(12); /* main:0808a080 */
        s32 shown = 1;

        *(volatile u16 *)(record + 100) = shown;
    }
    record = Scene_GetRecord_9(12); /* main:0808a080 */
    *(volatile s32 *)(record + 108) = 0x2008031;
    ObjectMotion_SetPositionAndReset_1(1, 196, 180);
    ObjectMotion_SetPositionAndReset_2(1, 184, 184);
    ObjectMotion_SetPositionAndReset_3(1, 180, 180);
    ObjectMotion_SetPositionAndReset_4(1, 168, 168);
    ObjectMotion_SetPositionAndReset_5(1, 180, 156);
    ObjectMotion_ResetAndSetPositionInMode2_2(1, 200, 104);
    Motion_SetPosReset_6(0, 192, 168);
    Motion_ArmCb_14(0, 0xc000, 0);
    Motion_CommitPos_2(1);
    BattleRuntime_WaitIfModeZero_36(30);
    ObjectMotion_SetVariantCallbackAndInvokeObject_4(1, 1); /* main:0808a138 */
    BattleRuntime_WaitIfModeZero_37(10);
    Motion_ArmCb_15(1, 0x5000, 0);
    BattleRuntime_WaitIfModeZero_38(15);
    record = Scene_GetRecord_10(12);
    *(volatile s32 *)(record + 108) = none;
    record = Scene_GetRecord_11(8); /* main:0808a080 */
    *(volatile s32 *)(record + 108) = none;
    ObjectMotion_SetVariantCallback_1(8, 2);
    BattleEffect_SpawnLinkedResourceObject_3(8, 0x100, 0);
    Actor_unk8_2(60); /* main:0808a080 */
    Object_SetModeById_8(8, 0);
    BattleEffect_SpawnLinkedResourceObject_4(0, 0x102, 0);
    BattleRuntime_WaitIfModeZero_39(60);
    Object_LinkPair_1(0, 11, 0);
    BattleRuntime_WaitIfModeZero_40(10);
    ObjectMotion_CallThenWaitForAnimationChange_11(0, 3); /* main:0808a110 */
    BattleRuntime_WaitIfModeZero_41(10);
    ObjectMotion_CallThenWaitForAnimationChange_12(11, 3); /* main:0808a110 */
    BattleRuntime_WaitIfModeZero_42(10);
    Motion_ArmCb_16(0, 0xc000, 0);
    BattleRuntime_WaitIfModeZero_43(10);
    ObjectMotion_SetVariantCallbackAndInvokeObject_5(0, 2);
    BattleRuntime_WaitIfModeZero_44(10);
    Motion_Launch_5(0, 2, 0); /* main:0808a138 */
    BattleRuntime_WaitIfModeZero_45(20);
    Motion_Launch_6(0, 2, 0);
    BattleRuntime_WaitIfModeZero_46(20);
    BattleRuntime_WaitIfModeZero_47(15);
    Object_LinkPair_2(0, 12, 0);
    BattleRuntime_WaitIfModeZero_48(10);
    ObjectMotion_CallThenWaitForAnimationChange_13(0, 3); /* main:0808a110 */
    BattleRuntime_WaitIfModeZero_49(10);
    ObjectMotion_CallThenWaitForAnimationChange_14(12, 3); /* main:0808a110 */
    BattleRuntime_WaitIfModeZero_50(60);
    Motion_SetPosReset_7(1, 208, 168);
    ObjectMotion_SetAngleToward_11(0, 11, 0);
    ObjectMotion_SetAngleToward_12(1, 12, 0);
    BattleRuntime_WaitIfModeZero_51(10);
    Object_SetModeById_9(0, 4);
    ObjectMotion_CallThenWaitForAnimationChange_15(1, 4); /* main:0808a110 */
    BattleRuntime_WaitIfModeZero_52(10);
    ObjectMotion_SetVariantCallback_2(0, 1);
    ObjectMotion_SetVariantCallbackAndInvokeObject_6(1, 1); /* main:0808a138 */
    BattleRuntime_WaitIfModeZero_53(10);
    Motion_ArmCb_17(1, 0, 0);
    BattleRuntime_WaitIfModeZero_54(10);
    ObjectMotion_CallThenWaitForAnimationChange_16(1, 3); /* main:0808a110 */
    BattleRuntime_WaitIfModeZero_55(10);
    ObjectMotion_SetAngleToward_13(1, 12, 0);
    BattleRuntime_WaitIfModeZero_56(10);
    ObjectMotion_CallThenWaitForAnimationChange_17(12, 3); /* main:0808a110 */
    BattleRuntime_WaitIfModeZero_57(10);
    Motion_ArmCb_18(0, 0, 0);
    Motion_ArmCb_19(1, 0, 0);
    BattleRuntime_WaitIfModeZero_58(10);
    ObjectMotion_SetVariantCallback_3(0, 2);
    ObjectMotion_SetVariantCallbackAndInvokeObject_7(1, 2);
    BattleRuntime_WaitIfModeZero_59(10);
    Motion_EnableActCb_1(0, 0x200b740);
    Motion_EnableActCb_2(1, 0x200b81c);
    Object_LookupAndStep_1(0); /* main:0808a0a0 */
    Object_LookupAndStep_2(1); /* main:0808a0a0 */
    ObjectMotion_SetSpeedParameters_3(0, 0x18000, 0xc000);
    ObjectMotion_SetSpeedParameters_4(1, 0x18000, 0xc000);
    Motion_Launch_7(0, 6, 0);
    Motion_Launch_8(1, 6, 0);
    ObjectMotion_SetAngleToward_14(0, 9, 0);
    ObjectMotion_SetAngleToward_15(1, 8, 0);
    BattleRuntime_WaitIfModeZero_60(1);
    ObjectMotion_SetAngleToward_16(0, 12, 0);
    ObjectMotion_SetAngleToward_17(1, 11, 0);
    BattleRuntime_WaitIfModeZero_61(1);
    ObjectMotion_SetAngleToward_18(0, 8, 0);
    ObjectMotion_SetAngleToward_19(1, 9, 0);
    BattleRuntime_WaitIfModeZero_62(1); /* main:0808a138 */
    Motion_ResetPosMode2_3(0, 192, 168);
    Motion_SetPosReset_8(1, 208, 168);
    Motion_CommitPos_3(0);
    BattleRuntime_WaitIfModeZero_63(10);
    Motion_ArmCb_20(0, 0x3000, 0);
    Motion_ArmCb_21(1, 0xd000, 0);
    BattleRuntime_WaitIfModeZero_64(10);
    Motion_ArmCb_22(0, 0x5000, 0);
    Motion_ArmCb_23(1, 0xb000, 0);
    BattleRuntime_WaitIfModeZero_65(10);
    Motion_ArmCb_24(0, 0x3000, 0);
    Motion_ArmCb_25(1, 0xd000, 0);
    BattleRuntime_WaitIfModeZero_66(10);
    Motion_ArmCb_26(0, 0x5000, 0);
    Motion_ArmCb_27(1, 0xb000, 0);
    BattleRuntime_WaitIfModeZero_67(20);
    ObjectMotion_SetAngleToward_20(0, 11, 0);
    ObjectMotion_SetAngleToward_21(1, 12, 0);
    BattleRuntime_WaitIfModeZero_68(20);
    Object_SetModeById_10(0, 3);
    ObjectMotion_CallThenWaitForAnimationChange_18(1, 3); /* main:0808a110 */
    BattleRuntime_WaitIfModeZero_69(10);
    Object_SetModeById_11(11, 3);
    ObjectMotion_CallThenWaitForAnimationChange_19(12, 3); /* main:0808a110 */
    BattleRuntime_WaitIfModeZero_70(30);
    Object_LinkPair_3(0, 1, 0);
    BattleRuntime_WaitIfModeZero_71(20);
    Object_SetModeById_12(0, 3);
    ObjectMotion_CallThenWaitForAnimationChange_20(1, 3); /* main:0808a110 */
    BattleRuntime_WaitIfModeZero_72(10);
    Motion_ArmCb_28(1, 0, 0);
    BattleRuntime_WaitIfModeZero_73(10);
    Object_SetModeById_13(0, 2);
    Object_SetModeById_14(1, 2);
    BattleRuntime_WaitIfModeZero_74(60);
}

/*
 * Dispatches on the scene selector gCell[225] over the range 10 to
 * 50, through a 41-entry jump table. The epilogue pops the return address
 * into r0, so no result survives it and the owner is void; the 296-byte
 * owner covers dispatcher, table, case bodies and literal pool. The default
 * arm doubles as the shared tail, so the arms that fall into it break while
 * the 20/21/50 arm returns instead.
 */
void Scene_DispatchBySceneId(void)
{
    s16 *tbl;
    s32 no;

    if (Actor_unk25() != 0) {
        Actor_unk64_4(8);
        return;
    }

    Actor_unk65_4();

    tbl = gCell;
    no = tbl[225];

    switch (no) {
    case 10:
    case 12:
        if (Actor_unk26(0x855) != 0) {
            Actor_unk66_4(0x1376);
        } else {
            Actor_unk67_4(0x1288);
        }
        break;
    case 11:
        Actor_unk68_4(0x1ce8);
        break;
    case 20:
    case 21:
    case 50:
        Actor_unk69_4();
        Actor_unk70_4();
        return;
    default:
        break;
    }

    Actor_unk71_4(8, 0);
    Actor_unk72_4();
}

/* overlays/scene/actor/facing_gated/scene_setup.c */
/*
 * Resource 378 scene reset at 0x020006e8(100 bytes including its literal).
 * The prologue and the pop-{r0}/bx-r0 epilogue are unambiguous.  The literal
 * 0x116c is loaded as a value (not an in-image pointer), so it stays an
 * integer argument here.  All calls are retained in the ROM order.
 */

void Scene_RunActorEightResetSequence(void)
{
    Actor_unk73_4();
    Actor_unk74_4(0x10000, 0x2000);
    Actor_unk75_4(1, 1);
    Actor_unk76_4();
    Actor_unk77_4(20);
    Actor_unk78_4(8, 0, 0);
    Actor_unk79_4(10);
    Actor_unk80_4(8, 4);
    Actor_unk81_4(20);
    Actor_unk82_4(0x116c);
    Actor_unk83_4(8, 0);
    Actor_unk84_4(0x200);
    Actor_unk85_4();
}

/* overlays/scene/actor/facing_gated/scripted_scene_sequence.c */

/* Calls use this overlay's loader veneers. The early long branch shares
 * the dialogue tail and epilogue; the two timing loops each run six times. */

extern u8 *gWork;

u8 *Scene_GetRecord();

/* The sibling actor-update script passes repeated large constants through
 * these inline call forms, keeping each call's argument evaluation local. */

void Scene_RunScriptedSceneSequence(void)
{
    u32 i;

    Actor_unk87_4();
    if (GameFlag_IsSet(0x201) != 0) {
        Actor_unk88_4(0, 8, 0);
        Actor_unk89_4(1, 0, 0);
        Actor_unk89_4(0xb, 0, 0);
        Actor_unk89_4(0xc, 0, 0);
        Actor_unk89_4(9, 0, 0);
        Actor_unk89_4(0xa, 0, 0);
        Actor_unk90_4(0xc00000, -1, 0xa00000, 1);
        Actor_unk91_4();
        SCENE_REQUEST = 0x100;
        SCENE_SETUP_WORD = 0x40;
        Actor_unk92_4();
        Actor_unk93_4();
        Actor_unk94_4(0x78);
        goto dialogue;
    }

    Actor_unk95_4(0x10002, 0);
    Actor_unk96_4(1);
    Actor_unk94_4(1);
    Actor_unk90_4(0xc00000, -1, 0xa00000, 1);
    Actor_unk91_4();
    SCENE_REQUEST = 0x209;
    Actor_unk92_4();
    Actor_unk93_4();
    Scene_RunPairedActorChoreography();
    Actor_unk95_4(0x10000, 0);
    Actor_unk96_4(0x3c);
    Actor_unk94_4(0x64);
    Actor_unk97_4(0, 1);
    Actor_unk97_4(1, 1);
    Actor_unk94_4(0x1e);
    Actor_unk89_4(0, 0xc, 0);
    Actor_unk89_4(1, 0xc, 0);
    Actor_unk94_4(0x14);
    Actor_unk98_4(8, 1);
    Actor_unk98_4(0xc, 1);
    Actor_Far(0, 0xcccc, 0x6666);
    Actor_Far(1, 0xcccc, 0x6666);
    Actor_Far(0xb, 0xcccc, 0x6666);
    Actor_Far(0xc, 0xcccc, 0x6666);
    Actor_Far(9, 0xcccc, 0x6666);
    Actor_Far(0xa, 0xcccc, 0x6666);
    Actor_Far(8, 0xcccc, 0x6666);
    Actor_unk94_4(0x1e);
    Actor_unk99_4(0xc, 2);
    Actor_unk100_4(0x1138);
    Actor_unk101_4(0xc, 0);
    Actor_unk94_4(0xa);
    Actor_unk97_4(0, 3);
    Actor_unk102_4(1, 3);
    Actor_unk94_4(0x1e);
    Actor_unk102_4(0xc, 3);
    Actor_unk94_4(0x14);
    Actor_unk99_4(0xb, 2);
    Actor_unk94_4(0x14);
    Actor_unk89_4(0, 0xb, 0);
    Actor_unk89_4(1, 0xb, 0);
    Actor_unk94_4(0x14);
    Actor_unk28(0xb, 0);
    /* Each arm advances once, on its own side of the object-state call. */
    if (Actor_unk29(0, 0) == 0) {
        Actor_unk94_4(0x14);
        Actor_unk102_4(0xb, 3);
        Actor_unk94_4(0x14);
        Actor_unk101_4(0xb, 0);
        SKIP_BEATS++;
    } else {
        Actor_unk94_4(0x14);
        Actor_unk102_4(0xb, 4);
        Actor_unk94_4(0x14);
        SKIP_BEATS++;
        Actor_unk101_4(0xb, 0);
    }
    Actor_unk94_4(0x14);
    Actor_unk99_4(9, 1);
    Actor_unk94_4(0x14);
    Actor_unk89_4(0, 9, 0);
    Actor_unk89_4(1, 9, 0);
    Actor_unk89_4(0xb, 9, 0);
    Actor_unk94_4(0x14);
    Actor_unk28(9, 0);
    if (Actor_unk29(0, 0) == 0) {
        Actor_unk94_4(0x14);
        Actor_unk102_4(9, 3);
        Actor_unk94_4(0x14);
        Actor_unk101_4(9, 0);
        SKIP_BEATS++;
    } else {
        Actor_unk94_4(0x14);
        Actor_unk102_4(9, 4);
        Actor_unk94_4(0x14);
        SKIP_BEATS++;
        Actor_unk101_4(9, 0);
    }
    Actor_unk94_4(0x14);
    Actor_unk97_4(0, 3);
    Actor_unk97_4(1, 3);
    Actor_unk97_4(0xb, 3);
    Actor_unk97_4(0xc, 3);
    Actor_unk97_4(9, 3);
    Actor_unk102_4(0xa, 3);
    Actor_unk94_4(0x1e);
    Actor_Far3(0xc, 0x101, 0);
    Actor_unk94_4(0x3c);
    Actor_unk89_4(0xc, 8, 0);
    Actor_unk94_4(0x14);
    Actor_unk103_4(0xc, 0xe0, 0x78);
    Actor_unk94_4(0xa);
    Actor_unk101_4(0xc, 0);
    Actor_unk94_4(0x14);
    Actor_unk89_4(0, 8, 0);
    Actor_unk89_4(1, 8, 0);
    Actor_unk89_4(0xb, 8, 0);
    Actor_unk89_4(9, 8, 0);
    Actor_unk89_4(0xa, 8, 0);
    Actor_unk94_4(0xa);
    Actor_unk99_4(0xb, 1);
    Actor_unk94_4(0xa);
    Actor_unk101_4(0xb, 0);
    Actor_unk94_4(0x1e);
    Actor_unk99_4(8, 3);
    Actor_Far3(8, 0x100, 0);
    Actor_unk94_4(0x3c);
    Actor_unk89_4(8, 0xc, 0);
    Actor_unk94_4(0x14);
    Actor_unk102_4(8, 3);
    Actor_unk94_4(0xa);
    Actor_unk101_4(8, 0);
    Actor_unk94_4(0x14);
    Actor_Far2(0xc, 0x5000, 0);
    Actor_unk94_4(0x3c);
    Actor_unk89_4(0xc, 8, 0);
    Actor_unk94_4(0x14);
    Actor_unk102_4(0xc, 3);
    Actor_unk94_4(0xa);
    Actor_unk101_4(0xc, 0);
    Actor_unk94_4(0xa);
    Actor_unk102_4(8, 3);
    Actor_unk94_4(0x14);
    Actor_Far2(8, 0x3000, 0);
    Actor_unk94_4(0xa);
    Actor_unk101_4(8, 0);
    Actor_unk94_4(0x14);
    Actor_Far2(0xc, 0x5000, 0);
    Actor_unk94_4(0x3c);
    Actor_unk89_4(0xc, 8, 0);
    Actor_unk94_4(0x32);
    Actor_Far3(0xc, 0x101, 0);
    Actor_unk94_4(0x28);
    Actor_Far3(0, 0x101, 0);
    Actor_Far3(1, 0x101, 0);
    Actor_Far3(0xb, 0x101, 0);
    Actor_Far3(9, 0x101, 0);
    Actor_Far3(0xa, 0x101, 0);
    Actor_unk94_4(0x3c);
    Actor_unk99_4(8, 1);
    Actor_unk94_4(0xa);
    Actor_unk101_4(8, 0);
    Actor_unk94_4(0xa);
    Actor_Far3(0, 0x100, 0);
    Actor_Far3(1, 0x100, 0);
    Actor_Far3(0xb, 0x100, 0);
    Actor_Far3(0xc, 0x100, 0);
    Actor_Far3(9, 0x100, 0);
    Actor_Far3(0xa, 0x100, 0);
    Actor_unk94_4(0x3c);
    Actor_unk99_4(1, 2);
    Actor_unk94_4(0xa);
    Actor_unk101_4(1, 0);
    Actor_unk94_4(0x14);
    Actor_unk102_4(8, 3);
    Actor_unk94_4(0xa);
    Actor_unk101_4(8, 0);
    Actor_unk94_4(0xa);
    Actor_Far3(0xc, 0x102, 0);
    Actor_unk104_4(0, 1);
    Actor_unk104_4(1, 1);
    Actor_unk104_4(0xb, 1);
    Actor_unk104_4(9, 1);
    Actor_unk99_4(0xa, 1);
    Actor_unk94_4(0xa);
    Actor_unk102_4(8, 4);
    Actor_unk94_4(0xa);
    Actor_unk101_4(8, 0);
    Actor_unk94_4(0x1e);
    Actor_unk89_4(0xc, 0, 0);
    Actor_unk89_4(0xb, 0, 0);
    Actor_unk94_4(0x14);
    Actor_unk99_4(0, 2);
    Actor_unk94_4(0x3c);
    Actor_unk99_4(0xc, 2);
    Actor_unk94_4(0x14);
    Actor_unk89_4(0xc, 8, 0);
    Actor_unk94_4(0x14);
    Actor_unk101_4(0xc, 0);
    Actor_unk94_4(0x1e);
    Actor_unk99_4(0xb, 2);
    Actor_unk94_4(0x14);
    Actor_unk89_4(8, 0, 0);
    Actor_unk94_4(0x14);
    Actor_unk101_4(0xb, 0);
    Actor_unk94_4(0x14);
    Actor_unk94_4(0x28);
    Actor_unk99_4(8, 2);
    Actor_unk97_4(8, 0);
    Actor_unk105_4(8, 0x100);
    Actor_unk106_4();
    Audio_PlayCue(0xc4);
    Actor_unk108_4(8, 0x1200);
    Actor_unk94_4(0x20);
    Actor_unk108_4(8, 0x1200);
    Actor_unk94_4(0x10);
    for (i = 0; i < 6; i++) {
        Actor_unk108_4(8, 0x1200);
        Actor_unk94_4(8);
    }
    Actor_unk94_4(8);
    Actor_unk108_4(8, 0x1200);
    Actor_unk94_4(0x20);
    Actor_unk108_4(8, 0x1200);
    Actor_unk94_4(0x60);
    Actor_unk94_4(0x20);
    Actor_unk105_4(8, 0);
    Actor_unk94_4(0x1e);
    State_ResetObject14Word108();
    Actor_unk97_4(8, 1);
    Actor_unk99_4(8, 2);
    Actor_unk94_4(0x14);
    Actor_unk101_4(8, 0);
    Actor_unk94_4(0x14);
    Actor_Far4(0xc, 0x102);
    Actor_Far4(0, 0x102);
    Actor_Far4(1, 0x102);
    Actor_Far4(0xb, 0x102);
    Actor_Far4(9, 0x102);
    Actor_Far4(0xa, 0x102);
    Actor_unk94_4(0x3c);
    Actor_unk99_4(0xa, 1);
    Actor_unk94_4(0x14);
    Actor_unk101_4(0xa, 0);
    Actor_unk94_4(0x14);
    Actor_unk88_4(0, 1, 0);
    Actor_unk94_4(0x3c);
    Actor_unk102_4(8, 4);
    Actor_unk94_4(0x14);
    Actor_unk89_4(0, 8, 0);
    Actor_unk89_4(1, 8, 0);
    Actor_unk94_4(0x14);
    Actor_unk101_4(8, 0);
    Actor_unk94_4(0x1e);
    Actor_unk88_4(0, 1, 0);
    Actor_unk94_4(0x14);
    Actor_unk97_4(0, 3);
    Actor_unk97_4(1, 3);
    Actor_unk88_4(9, 0xa, 0);
    Actor_unk94_4(0x14);
    Actor_unk97_4(9, 3);
    Actor_unk97_4(0xa, 3);
    Actor_unk109_4(0xa);
    Actor_unk94_4(0x1e);
    Actor_unk88_4(0, 0xb, 0);
    Actor_unk88_4(1, 0xc, 0);
    Actor_unk94_4(0x14);
    Actor_unk97_4(0, 3);
    Actor_unk97_4(0xb, 3);
    Actor_unk97_4(1, 3);
    Actor_unk97_4(0xc, 3);
    Actor_unk109_4(0xc);
    Actor_unk94_4(0x3c);
    Actor_unk99_4(8, 2);
    Actor_unk97_4(8, 0);
    Actor_unk105_4(8, 0x100);
    Actor_unk106_4();
    Audio_PlayCue(0xc4);
    Actor_unk108_4(8, 0x1200);
    Actor_unk94_4(0x20);
    Actor_unk108_4(8, 0x1200);
    Actor_unk89_4(0, 8, 0);
    Actor_unk89_4(1, 8, 0);
    Actor_unk89_4(0xb, 8, 0);
    Actor_unk89_4(0xc, 8, 0);
    Actor_unk89_4(9, 8, 0);
    Actor_unk89_4(0xa, 8, 0);
    Actor_unk94_4(0x10);
    Actor_unk104_4(0, 1);
    Actor_unk104_4(1, 1);
    Actor_unk104_4(0xb, 1);
    Actor_unk104_4(0xc, 1);
    Actor_unk104_4(9, 1);
    Actor_unk104_4(0xa, 1);
    for (i = 0; i < 6; i++) {
        Actor_unk108_4(8, 0x1200);
        Actor_unk94_4(8);
    }
    Actor_unk94_4(8);
    Actor_unk108_4(8, 0x1200);
    Actor_unk94_4(0x20);
    Actor_unk108_4(8, 0x1200);
    Actor_unk94_4(0x80);
    Actor_unk105_4(8, 0);
    Actor_unk94_4(0x1e);
    State_ResetObject14Word108();
    Actor_unk97_4(8, 1);
    Actor_unk99_4(8, 2);
    Actor_unk94_4(0x1e);
    Actor_Far3(0xc, 0x105, 0);
    Actor_unk94_4(0x14);
    Actor_unk88_4(9, 0xa, 0);
    Actor_unk94_4(0x28);
    Actor_unk89_4(9, 8, 0);
    Actor_unk89_4(0xa, 8, 0);
    Actor_unk101_4(0xc, 0);
    Actor_unk94_4(0x14);
    Actor_unk99_4(0xb, 2);
    Actor_unk94_4(0x14);
    Actor_unk101_4(0xb, 0);
    Actor_unk94_4(0x14);
    Actor_unk104_4(0, 1);
    Actor_unk104_4(1, 1);
    Actor_unk88_4(0, 1, 0);
    Actor_unk104_4(9, 1);
    Actor_unk104_4(0xa, 1);
    Actor_unk88_4(9, 0xa, 0);
    Actor_Far4(0xc, 0x102);
    Actor_unk94_4(0x3c);
    Actor_unk101_4(8, 0);
    Actor_unk94_4(0x14);
    Actor_unk89_4(0, 8, 0);
    Actor_unk89_4(1, 8, 0);
    Actor_unk89_4(9, 8, 0);
    Actor_unk89_4(0xa, 8, 0);
    Actor_unk94_4(0x14);
    Actor_unk101_4(1, 0);
    Actor_unk94_4(0x14);
    Actor_unk102_4(8, 3);
    Actor_unk94_4(0x14);
    Actor_unk101_4(8, 0);
    Actor_unk94_4(0x14);
    Actor_Far3(0, 0x101, 0);
    Actor_Far3(1, 0x101, 0);
    Actor_Far3(0xb, 0x101, 0);
    Actor_Far3(0xc, 0x101, 0);
    Actor_Far3(9, 0x101, 0);
    Actor_Far3(0xa, 0x101, 0);
    Actor_unk94_4(0x3c);
    Actor_unk102_4(8, 4);
    Actor_unk94_4(0x14);
    Actor_unk101_4(8, 0);
    Actor_unk94_4(0x14);
    Actor_unk97_4(0, 3);
    Actor_unk97_4(1, 3);
    Actor_unk97_4(0xb, 3);
    Actor_unk97_4(0xc, 3);
    Actor_unk97_4(9, 3);
    Actor_unk97_4(0xa, 3);
    Actor_unk109_4(0xa);
    Actor_unk94_4(0x14);
    Actor_unk99_4(0xc, 1);
    Actor_unk94_4(0x14);
    Actor_unk101_4(0xc, 0);
    Actor_unk94_4(0x14);
    Actor_unk89_4(8, 0xc, 0);
    Actor_unk94_4(0x14);
    Actor_unk102_4(8, 3);
    Actor_unk94_4(0x14);
    Actor_unk101_4(8, 0);
    Actor_unk94_4(0x32);
    Actor_unk89_4(8, 0, 0);
    Actor_unk94_4(0x14);
    Actor_unk101_4(8, 0);
    Actor_unk94_4(0x1e);
    Actor_Far3(0, 0x102, 0);
    Actor_Far3(1, 0x102, 0);
    Actor_Far3(0xb, 0x102, 0);
    Actor_Far3(0xc, 0x102, 0);
    Actor_Far3(9, 0x102, 0);
    Actor_Far3(0xa, 0x102, 0);
    Actor_unk94_4(0x50);
    Actor_unk99_4(0xb, 1);
    Actor_unk94_4(0x14);
    Actor_unk101_4(0xb, 0);
    Actor_unk94_4(0x14);
    Actor_unk102_4(8, 4);
    Actor_unk94_4(0x14);
    Actor_Far3(0, 0x101, 0);
    Actor_Far3(1, 0x101, 0);
    Actor_Far3(0xb, 0x101, 0);
    Actor_Far3(0xc, 0x101, 0);
    Actor_Far3(9, 0x101, 0);
    Actor_Far3(0xa, 0x101, 0);
    Actor_unk94_4(0x50);
    Actor_unk99_4(8, 1);
    Actor_unk94_4(0x14);
    Actor_unk101_4(8, 0);
    Actor_unk94_4(0x14);
    Actor_Far4(0, 0x102);
    Actor_Far4(1, 0x102);
    Actor_Far4(0xb, 0x102);
    Actor_Far4(0xc, 0x102);
    Actor_Far4(9, 0x102);
    Actor_Far4(0xa, 0x102);
    Actor_unk94_4(0x3c);
    Actor_unk102_4(0xc, 4);
    Actor_unk94_4(0x14);
    Actor_unk101_4(0xc, 0);
    Actor_unk94_4(0x14);
    Actor_unk99_4(8, 1);
    Actor_unk94_4(0x14);
    Actor_unk102_4(8, 4);
    Actor_unk94_4(0x14);
    Actor_unk101_4(8, 0);
    Actor_unk94_4(0x28);
    Actor_unk88_4(0, 1, 0);
    Actor_unk88_4(9, 0xa, 0);
    Actor_unk89_4(0xc, 0, 0);
    Actor_unk94_4(0x3c);
    Actor_unk89_4(0, 8, 0);
    Actor_unk89_4(1, 8, 0);
    Actor_unk89_4(0xb, 8, 0);
    Actor_unk89_4(0xc, 8, 0);
    Actor_unk89_4(9, 8, 0);
    Actor_unk89_4(0xa, 8, 0);
    Actor_unk94_4(0x14);
    Actor_unk99_4(0xb, 1);
    Actor_unk94_4(0x14);
    Actor_unk101_4(0xb, 0);
    Actor_unk94_4(0x3c);
    Actor_unk103_4(8, 0xc8, 0x88);
    Actor_unk89_4(0, 8, 0);
    Actor_unk89_4(0xc, 0, 0);
    Actor_unk88_4(8, 1, 0);
    Actor_unk94_4(0x28);
    Actor_unk102_4(8, 3);
    Actor_unk94_4(0x32);
    Actor_unk89_4(8, 0, 0);
    Actor_unk94_4(0x28);
    Actor_unk102_4(8, 3);
    Actor_unk94_4(0x32);
    Actor_unk99_4(8, 1);
    Actor_unk94_4(0x14);
    Actor_unk101_4(8, 0);
    Actor_unk94_4(0x1e);
    Actor_Far3(0, 0x102, 0);
    Actor_Far3(1, 0x102, 0);
    Actor_unk94_4(0x3c);
    Actor_Far4(0xb, 0x102);
    Actor_unk99_4(0xb, 1);
    Actor_unk101_4(0xb, 0);
    Actor_unk94_4(0x1e);
    Actor_unk102_4(8, 3);
    Actor_unk94_4(0x1e);
    Actor_Far3(0xc, 0x102, 0);
    Actor_unk104_4(0xc, 1);
    Actor_unk94_4(0x14);
    Actor_unk101_4(0xc, 0);
    Actor_unk94_4(0x1e);
    Actor_unk103_4(8, 0xa8, 0x78);
    Actor_unk89_4(0, 8, 0);
    Actor_unk89_4(1, 8, 0);
    Actor_unk89_4(0xc, 8, 0);
    Actor_Far2(8, 0xd000, 0);
    Actor_unk94_4(0x14);
    Actor_unk102_4(8, 4);
    Actor_unk94_4(0x14);
    Actor_unk101_4(8, 0);
    Actor_unk94_4(0x1e);
    Actor_Far3(0, 0x102, 0);
    Actor_Far3(1, 0x102, 0);
    Actor_unk94_4(0x3c);
    Actor_unk102_4(8, 3);
    Actor_unk94_4(0xa);
    Actor_unk101_4(8, 0);
    Actor_unk94_4(0xa);
    Actor_unk89_4(8, 0, 0);
    Actor_unk94_4(0x14);
    Actor_unk101_4(8, 0);
    Actor_unk94_4(0x14);
    Actor_unk89_4(0xc, 0, 0);
    Actor_Far4(1, 0x102);
    Actor_unk99_4(1, 2);
    Actor_unk89_4(1, 0, 0);
    Actor_unk94_4(0x14);
    Actor_unk101_4(1, 0);
    Actor_unk94_4(0x1e);
dialogue:
    Actor_unk100_4(0x1162);
    Actor_unk99_4(8, 1);
    Actor_unk94_4(0x14);
    Actor_unk28(8, 0);
    if (Actor_unk29(0, 0) == 0) {
        Scene_RunActorUpdateSequence();
        Actor_unk95_4(0, 0);
        Actor_unk96_4(0x78);
        Actor_unk94_4(0x78);
        Audio_PlayCue(0x56);
        Actor_unk110_4();
        GameFlag_Set(0x9f0);
        Actor_unk112_4(0x1e);
    } else {
        u8 *actor;
        s32 flags;

        Actor_unk94_4(0x1e);
        Actor_unk99_4(0xc, 1);
        Actor_unk102_4(0xc, 4);
        Actor_unk94_4(0x14);
        Actor_unk101_4(0xc, 0);
        Actor_unk94_4(0x14);
        actor = Scene_GetRecord(0xa);
        actor += 0x23;
        flags = 254;
        flags &= *actor;
        *actor = flags;
        actor = Scene_GetRecord(0xa);
        *(u32 *)(actor + 0x6c) = 0x02008849;
    }
    Actor_unk113_4();
}

/* overlays/scene/actor/facing_gated/state_update.c */
/* Resource 378 object reset at 0x02002660(28 bytes including alignment). */
extern u8 *Actor_unk114_4();

void State_ResetObject14Word108(void)
{
    u8 *state = Actor_unk114_4(14);
    *(s32 *)(state + 108) = 0;
    Actor_unk115_4(14, 0, 0);
}

/* Publish the scene's upper prompt and lower dialogue panel. */
void State_ApplyTwoRects(void)
{
    {
        s32 a5 = 3, a6 = 2;
        Actor_unk2_5(0, 64, 11, 68, a5, a6);
    }
    {
        s32 a5 = 11, a6 = 8;
        Actor_unk3_5(11, 10, 3, 2, a5, a6);
    }
    Actor_unk29_2(1);
}
