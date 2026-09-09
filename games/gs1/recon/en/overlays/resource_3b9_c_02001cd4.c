#include "types.h"

/* FieldScene_BuildActorPresentationSequence - resource_3b9:02001cd4, 1804 bytes.
 *
 * One straight-line field-scene presentation script for actor records 0-3 and
 * 8-11. It resets the battle runtime, places the three lead actors, raises the
 * scene phase word, then walks a long ordered list of spawn, motion, callback
 * and wait calls. Two thirds of the way through it asks the UI work helper
 * whether the player already finished the pending step: the answer selects a
 * short path (one paired call) or a long one (an animation-change wait, a
 * masked-halfword forward and the same paired call), and both paths bump the
 * scene step counter exactly once. Near the end it arms the action callbacks
 * of actors 1, 2 and 3 with one shared 0x0200adac word, then walks actors 11
 * and 0 to their closing positions. It ends by lowering the scene phase word
 * back to 256, storing 40 at field 0x1c8 and running the three closing
 * helpers.
 *
 * Uncertain: the argument roles of the unnamed veneers below, whether the
 * 0x0200adac word passed to the three callback helpers is a function pointer
 * or a work record, and whether the second scene-step bump is a separate
 * source statement or a shared tail the compiler duplicated.
 *
 * Residual: candidate=1804 reference=1804 differing_halfwords=11,
 * topology=equal, wrong_instructions=0, class=scheduling-floor. The candidate
 * emits exactly the reference's instructions; two short runs sit in a
 * different order.
 *
 *   - at owner offset 0x42c the two constant arguments of the Func_02004da2
 *     call are set r0 before r1 where the reference sets r1 before r0;
 *   - the pc-relative load of 0x0200adac that the three action-callback calls
 *     share lands at owner offset 0x5f8, before the third
 *     ObjectMotion_SetSpeedParameters call; the reference issues it at 0x604,
 *     after that call and before its first use, which also
 *     swaps the r0/r1 setup of the first of those three calls: the reference
 *     schedule fills the load-use gap with `movs r0, #1`, ours has no gap to
 *     fill.
 *
 * Neither survived respelling. Wrapping or prototyping the Func_02004da2 site
 * leaves it unchanged. Spelling the first action-callback site through the
 * constant-passing wrapper below, or through a shared local, moves the load
 * but does not place it after the call: the wrapper form measures 8 differing
 * halfwords, and is not used here because it buys that number by emitting a
 * second `ldr r1, [pc]` the reference does not contain and dropping an
 * `adds r1, r5, #0` it does (wrong_instructions=2, class=missing-extra-code).
 */

#define FieldScene_BuildActorPresentationSequence Func_02001cd4

/* A call site spelled through one of these wrappers passes its constants
 * straight into the argument registers; a direct call lets the compiler
 * precompute a costly constant into a pseudo and share it with later uses in
 * the block. Most sites here want the first shape; the three action-callback
 * sites near the end want the second, because the reference reaches all three
 * of them through one shared pointer register. The adopted sibling
 * run_scene_3b9_conditional_scene_setup.c uses the same device for the same
 * reason. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

/* Shared scene work record, reached through the pointer at 0x03001ebc. The
 * three field offsets are the ones the adopted sibling
 * games/gs1/src/overlays/scene_primary_script/run_scene_3b9_conditional_scene_setup.c
 * already spells. */

#define SCENE_WORK (*(u8 **)0x03001ebc)
#define SCENE_PHASE (*(s32 *)(SCENE_WORK + 0x1c0))
#define SCENE_FIELD_1C8 (*(s32 *)(SCENE_WORK + 0x1c8))
#define SCENE_STEP (*(u16 *)(SCENE_WORK + 0x1d8))

/* Overlay call bindings.
 *
 * The loader rewrites every BL-shaped halfword pair, so the word an overlay
 * image stores for a call site depends on the site as well as on the callee.
 * The legacy Func_<address> alias a disassembler prints is therefore per site,
 * and the 180 call sites in this owner reach only 31 distinct runtime targets.
 * Each declaration below is one runtime target, spelled with an alias that the
 * reference binds to that target and to no other; its comment gives the
 * overlay veneer the call enters and the main-image entry that veneer reaches.
 *
 * The alias a site produces folds in that site's own position: it is
 * RESOURCE_BASE + (owner offset + site + 4) + the stored displacement, while
 * the runtime target the loader reaches is RUNTIME_BASE + that displacement
 * + 2. Two sites therefore share one alias whenever they are as far apart as
 * their two targets. That is the case for the veneer at 0x0200ad0c: it is
 * reached only from 0x02001d28 and 0x02001d64, and each of those sits exactly
 * eight bytes before a site that calls the neighbouring veneer at 0x0200ad04,
 * so both aliases the reference offers for it - Func_02004a36 and
 * Func_02004a72 - name two targets and neither can bind. Every other target
 * in this owner has at least one alias of its own, and is declared under it
 * exactly once.
 *
 * The one unbindable target is spelled Data_0200ad0c: a name carrying the
 * runtime address directly, which is the only spelling that reaches an
 * overlay-space address without going through the per-site alias table
 * (a Func_02xxxxxx name is always resolved as a legacy alias). The emitted
 * call word matches the reference at both sites. If this owner is ever given
 * a translation-unit entry, that entry should declare the runtime symbol
 * instead and the name can go back to the ordinary Func_ form.
 *
 * Names come from the established spelling of the same main-image entry in
 * already adopted overlay sources. Unnamed veneers keep their alias.
 */

/* 0x0200abf4 -> 0x0808a018 */
void Func_020048d4();
#define BattleRuntime_Reset Func_020048d4

/* 0x0200abec -> 0x0808a010 */
void Func_02004966();
#define BattleRuntime_WaitIfModeZero(a0) Call1(Func_02004966, a0)

/* 0x0200ac04 -> 0x0808a070 */
s32 Func_02004d3c();
#define UiWork_WaitThenFinalizeCapacity(a0, a1) Value2(Func_02004d3c, a0, a1)

/* 0x0200ac54 -> 0x0808a0f0 */
void Func_02004942();
#define ObjectMotion_SetHorizontalPositionWithTerrain(a0, a1, a2) Call3(Func_02004942, a0, a1, a2)

/* 0x0200ac14 -> 0x0808a090 */
void Func_02004f2e();
#define ObjectMotion_SetSpeedParameters(a0, a1, a2) Call3(Func_02004f2e, a0, a1, a2)

/* 0x0200ac1c -> 0x0808a098. Two sites reach this veneer, at 0x020022de and
 * 0x020022e6; each keeps its own unambiguous alias. */
void Func_02004efc();
#define ObjectMotion_EnableActionAndSetCallback_1 Func_02004efc
void Func_02004f04();
#define ObjectMotion_EnableActionAndSetCallback_2 Func_02004f04

/* 0x0200ac2c -> 0x0808a0b0 */
void Func_02004f1c();
#define ObjectMotion_MarkActiveAndSetActionCallback Func_02004f1c

/* 0x0200ac34 -> 0x0808a0b8 */
void Func_02004f86();
#define ObjectMotion_ResetAndSetPosition(a0, a1, a2) Call3(Func_02004f86, a0, a1, a2)

/* 0x0200ac3c -> 0x0808a0c0 */
void Func_02004f74();
#define ObjectMotion_SetPositionAndCommit(a0, a1, a2) Call3(Func_02004f74, a0, a1, a2)

/* 0x0200ac44 -> 0x0808a0c8 */
void Func_02004fd0();
#define ObjectMotion_ResetAndSetPositionInMode2(a0, a1, a2) Call3(Func_02004fd0, a0, a1, a2)

/* 0x0200ac4c -> 0x0808a0d0 */
void Func_02004fca();
#define ObjectMotion_SetPositionAndReset(a0, a1, a2) Call3(Func_02004fca, a0, a1, a2)

/* 0x0200ac5c -> 0x0808a100 */
void Func_02004b5e();
#define Object_SetModeById(a0, a1) Call2(Func_02004b5e, a0, a1)

/* 0x0200ac6c -> 0x0808a110 */
void Func_02004a62();
#define ObjectMotion_CallThenWaitForAnimationChange(a0, a1) Call2(Func_02004a62, a0, a1)

/* 0x0200ac74 -> 0x0808a130 */
void Func_02004ace();
#define ObjectMotion_SetVariantCallback(a0, a1) Call2(Func_02004ace, a0, a1)

/* 0x0200ac7c -> 0x0808a138 */
void Func_020049fe();
#define ObjectMotion_SetVariantCallbackAndInvokeObject(a0, a1) Call2(Func_020049fe, a0, a1)

/* 0x0200ac94 -> 0x0808a170 */
void Func_02004a7c();
#define SceneWork_SetStepValue(a0) Call1(Func_02004a7c, a0)

/* 0x0200acbc -> 0x0808a1b8 */
void Func_02004ae0();
#define ObjectMotion_ArmCallback(a0, a1, a2) Call3(Func_02004ae0, a0, a1, a2)

/* 0x0200acc4 -> 0x0808a1e8 */
void Func_02004a98();
#define BattleEffect_SpawnLinkedResourceObject(a0, a1, a2) Call3(Func_02004a98, a0, a1, a2)

/* 0x0200acdc -> 0x0808a208 */
void Func_0200503c();
#define ObjectMotion_SetSpeedLimitAndAcceleration(a0, a1) Call2(Func_0200503c, a0, a1)

/* 0x0200ace4 -> 0x0808a210 */
void Func_02004a06();
#define ObjectMotion_PlaceWithinCameraBounds(a0, a1, a2, a3) Call4(Func_02004a06, a0, a1, a2, a3)

/* 0x02009c48, the owner registered as FieldScene_CallPairWith10. Its readable
 * name is bound to Func_02001c48, which is not one of this owner's call words,
 * so the call keeps its alias. Takes one record id. */
void Func_02003a36();

/* 0x02009c5c, the owner registered as SceneState_ForwardMaskedHalfwordWith10;
 * same reason. Takes a record id and the masked halfword. */
void Func_02003a00();

/* Unnamed veneers: no adopted source spells these main-image entries, so the
 * alias stands and the comment records the entry and the observed arity. */
/* 0x0200ab8c -> 0x080000c0, (arg0) */
void Func_0200489c();
/* 0x0200ac9c -> 0x0808a178, (id, arg1) */
void Func_02004da2();
/* 0x0200acfc -> 0x0808a248, (arg0) */
void Func_020050b2();
/* 0x0200ad04 -> 0x0808a330, (arg0, arg1) */
void Func_02004ac0();
/* 0x0200ad0c -> 0x0808a338, (arg0, arg1). Named by runtime address rather
 * than by a legacy alias; see the binding note above. */
void Data_0200ad0c();
/* 0x0200ad14 -> 0x0808a348, (arg0) */
void Func_02004a4c();
/* 0x0200ad1c -> 0x0808a360, () */
void Func_02004a76();
/* 0x0200ad24 -> 0x0808a368, () */
void Func_020050d0();
/* 0x0200ad2c -> 0x0808a370, () */
void Func_02004a8a();

void FieldScene_BuildActorPresentationSequence(void)
{
    s32 flag;

    BattleRuntime_Reset();
    ObjectMotion_SetHorizontalPositionWithTerrain(1, 0x3180000, 0x880000);
    ObjectMotion_SetHorizontalPositionWithTerrain(2, 0x3380000, 0x880000);
    ObjectMotion_SetHorizontalPositionWithTerrain(3, 0x3280000, 0x980000);
    Func_0200489c(1);
    ObjectMotion_PlaceWithinCameraBounds(-1, -1, -1, 0);
    Data_0200ad0c(0, 0);
    Func_02004ac0(0, 0);
    Func_02004a4c(1);
    Func_0200489c(1);
    SCENE_PHASE = 515;
    SCENE_FIELD_1C8 = 1;
    Func_02004a76();
    Func_02004a8a();
    Data_0200ad0c(0, 0);
    Func_02004ac0(0x10002, 0);
    Func_02004a4c(40);
    BattleRuntime_WaitIfModeZero(80);
    ObjectMotion_SetVariantCallbackAndInvokeObject(8, 1);
    BattleRuntime_WaitIfModeZero(20);
    ObjectMotion_SetVariantCallbackAndInvokeObject(2, 2);
    BattleRuntime_WaitIfModeZero(40);
    Func_02003a00(8, 0x3000);
    ObjectMotion_SetVariantCallbackAndInvokeObject(8, 2);
    BattleRuntime_WaitIfModeZero(20);
    Func_02004ac0(0x10000, 0);
    Func_02004a4c(40);
    BattleRuntime_WaitIfModeZero(80);
    BattleEffect_SpawnLinkedResourceObject(2, 0x100, 0);
    ObjectMotion_SetVariantCallbackAndInvokeObject(2, 1);
    BattleRuntime_WaitIfModeZero(20);
    SceneWork_SetStepValue(0x20f8);
    Func_02003a36(2);
    ObjectMotion_CallThenWaitForAnimationChange(8, 3);
    Func_02003a36(8);
    BattleEffect_SpawnLinkedResourceObject(3, 0x102, 40);
    Func_02003a36(3);
    ObjectMotion_ArmCallback(8, 0x8000, 0);
    ObjectMotion_ArmCallback(9, 0, 40);
    ObjectMotion_ArmCallback(8, 0x5000, 0);
    Func_02003a00(9, 0x3000);
    BattleEffect_SpawnLinkedResourceObject(9, 0x105, 20);
    Func_02003a36(9);
    BattleEffect_SpawnLinkedResourceObject(1, 0x103, 20);
    ObjectMotion_SetVariantCallback(1, 2);
    Func_02003a36(1);
    BattleEffect_SpawnLinkedResourceObject(10, 0x102, 40);
    Func_02003a36(10);
    ObjectMotion_CallThenWaitForAnimationChange(11, 3);
    Func_02003a36(11);
    Func_02003a00(2, 0xa000);
    ObjectMotion_CallThenWaitForAnimationChange(2, 4);
    Func_02003a36(2);
    ObjectMotion_SetVariantCallbackAndInvokeObject(3, 1);
    Func_02003a36(3);
    BattleEffect_SpawnLinkedResourceObject(1, 0x103, 20);
    Func_02003a00(1, 0);
    ObjectMotion_SetVariantCallback(1, 2);
    Func_02003a36(1);
    Func_02003a00(0, 0x6000);
    BattleEffect_SpawnLinkedResourceObject(0, 0x102, 60);
    ObjectMotion_SetVariantCallbackAndInvokeObject(2, 1);
    Func_02003a00(2, 0x8000);
    Func_02003a36(2);
    Object_SetModeById(3, 4);
    BattleRuntime_WaitIfModeZero(20);
    Func_02003a36(3);
    ObjectMotion_SetVariantCallbackAndInvokeObject(1, 1);
    Func_02003a00(1, 0x2000);
    Func_02003a36(1);
    BattleEffect_SpawnLinkedResourceObject(2, 0x105, 60);
    ObjectMotion_ArmCallback(2, 0x2000, 0);
    ObjectMotion_ArmCallback(1, 0, 0);
    ObjectMotion_ArmCallback(3, 0xe000, 20);
    BattleEffect_SpawnLinkedResourceObject(0, 0x102, 0);
    BattleEffect_SpawnLinkedResourceObject(1, 0x102, 0);
    BattleEffect_SpawnLinkedResourceObject(3, 0x102, 80);
    BattleEffect_SpawnLinkedResourceObject(2, 0x106, 0);
    ObjectMotion_SetVariantCallbackAndInvokeObject(2, 1);
    Func_02003a00(2, 0xc000);
    Func_02003a36(2);
    ObjectMotion_ArmCallback(0, 0xc000, 0);
    ObjectMotion_ArmCallback(1, 0xc000, 0);
    ObjectMotion_ArmCallback(3, 0xc000, 20);
    Func_02003a00(8, 0x3000);
    ObjectMotion_CallThenWaitForAnimationChange(8, 3);
    Func_02003a36(8);
    BattleEffect_SpawnLinkedResourceObject(0, 0x100, 0);
    BattleEffect_SpawnLinkedResourceObject(1, 0x100, 0);
    BattleEffect_SpawnLinkedResourceObject(3, 0x100, 40);
    ObjectMotion_SetVariantCallbackAndInvokeObject(9, 1);
    Func_02003a36(9);
    ObjectMotion_ArmCallback(0, 0x6000, 0);
    ObjectMotion_ArmCallback(1, 0xe000, 0);
    ObjectMotion_ArmCallback(2, 0x6000, 0);
    ObjectMotion_ArmCallback(3, 0xe000, 40);
    ObjectMotion_SetVariantCallbackAndInvokeObject(10, 1);
    Func_02003a36(10);
    ObjectMotion_ArmCallback(0, 0xc000, 0);
    ObjectMotion_ArmCallback(1, 0xc000, 0);
    ObjectMotion_ArmCallback(2, 0xc000, 0);
    Func_02003a00(3, 0xc000);
    ObjectMotion_CallThenWaitForAnimationChange(11, 3);
    Func_02003a36(11);
    ObjectMotion_ArmCallback(0, 0x6000, 0);
    ObjectMotion_ArmCallback(1, 0xe000, 0);
    ObjectMotion_ArmCallback(2, 0x6000, 0);
    ObjectMotion_ArmCallback(3, 0xe000, 20);
    BattleEffect_SpawnLinkedResourceObject(1, 0x105, 0);
    BattleEffect_SpawnLinkedResourceObject(2, 0x105, 0);
    BattleEffect_SpawnLinkedResourceObject(3, 0x105, 80);
    BattleEffect_SpawnLinkedResourceObject(2, 0x106, 0);
    ObjectMotion_SetVariantCallbackAndInvokeObject(2, 1);
    Func_02003a00(2, 0xe000);
    Func_02003a36(2);
    ObjectMotion_ArmCallback(0, 0, 0);
    ObjectMotion_ArmCallback(1, 0, 20);
    BattleEffect_SpawnLinkedResourceObject(11, 0x101, 60);
    Func_02004da2(2, 0);
    ObjectMotion_ArmCallback(0, 0x2000, 0);
    ObjectMotion_ArmCallback(1, 0xe000, 0);
    ObjectMotion_ArmCallback(2, 0xa000, 0);
    ObjectMotion_ArmCallback(3, 0xc000, 0);
    flag = 0;
    if (UiWork_WaitThenFinalizeCapacity(0, 0) == 1) {
        Func_02003a36(2);
        flag = 1;
    } else {
        SCENE_STEP++;
        ObjectMotion_CallThenWaitForAnimationChange(2, 3);
        Func_02003a00(2, 0xc000);
        Func_02003a36(2);
    }
    if (flag != 0) {
        SCENE_STEP++;
    }
    ObjectMotion_ArmCallback(8, 0x8000, 0);
    ObjectMotion_ArmCallback(9, 0, 0);
    ObjectMotion_ArmCallback(10, 0x3000, 0);
    ObjectMotion_ArmCallback(11, 0xb000, 20);
    BattleEffect_SpawnLinkedResourceObject(8, 0x105, 0);
    BattleEffect_SpawnLinkedResourceObject(9, 0x105, 0);
    BattleEffect_SpawnLinkedResourceObject(10, 0x105, 0);
    BattleEffect_SpawnLinkedResourceObject(11, 0x105, 60);
    ObjectMotion_SetVariantCallbackAndInvokeObject(8, 1);
    Func_02003a00(8, 0x3000);
    Func_02003a36(8);
    ObjectMotion_ArmCallback(0, 0xc000, 0);
    ObjectMotion_ArmCallback(1, 0xc000, 0);
    ObjectMotion_ArmCallback(2, 0xc000, 0);
    ObjectMotion_ArmCallback(3, 0xc000, 20);
    ObjectMotion_SetVariantCallbackAndInvokeObject(9, 1);
    Func_02003a00(9, 0x3000);
    Func_02003a36(9);
    ObjectMotion_SetVariantCallbackAndInvokeObject(10, 1);
    Func_02003a00(10, 0x5000);
    Func_02003a36(10);
    Func_02003a00(11, 0x8000);
    ObjectMotion_CallThenWaitForAnimationChange(11, 3);
    Func_02003a36(11);
    ObjectMotion_ArmCallback(1, 0xe000, 0);
    ObjectMotion_ArmCallback(2, 0xa000, 20);
    Object_SetModeById(1, 3);
    Object_SetModeById(2, 3);
    ObjectMotion_CallThenWaitForAnimationChange(3, 3);
    ObjectMotion_SetSpeedParameters(1, 0x10000, 0x8000);
    ObjectMotion_SetSpeedParameters(2, 0x10000, 0x8000);
    ObjectMotion_SetSpeedParameters(3, 0x10000, 0x8000);
    ObjectMotion_EnableActionAndSetCallback_1(1, 0x200adac);
    ObjectMotion_EnableActionAndSetCallback_2(2, 0x200adac);
    ObjectMotion_MarkActiveAndSetActionCallback(3, 0x200adac);
    BattleRuntime_WaitIfModeZero(20);
    Func_02003a00(0, 0);
    ObjectMotion_CallThenWaitForAnimationChange(0, 3);
    ObjectMotion_CallThenWaitForAnimationChange(11, 3);
    ObjectMotion_SetSpeedParameters(11, 0x10000, 0x8000);
    ObjectMotion_SetSpeedParameters(0, 0x10000, 0x8000);
    Object_SetModeById(11, 2);
    ObjectMotion_SetPositionAndCommit(11, 830, 152);
    ObjectMotion_SetPositionAndCommit(11, 808, 164);
    ObjectMotion_ResetAndSetPosition(11, 808, 312);
    BattleRuntime_WaitIfModeZero(20);
    ObjectMotion_SetSpeedLimitAndAcceleration(0x6666, 0xccc);
    ObjectMotion_PlaceWithinCameraBounds(0x3280000, -1, 0x1380000, 1);
    ObjectMotion_SetPositionAndReset(0, 808, 164);
    ObjectMotion_ResetAndSetPositionInMode2(0, 808, 312);
    BattleRuntime_WaitIfModeZero(60);
    SCENE_PHASE = 256;
    SCENE_FIELD_1C8 = 40;
    Func_020050d0();
    Func_02004a8a();
    Func_020050b2(64);
}

/* Integration note. This owner has no translation-unit entry, so it scores
 * standalone with `alchemy score <this file> --owner resource_3b9:02001cd4`.
 * The only symbol that cannot come from the reference alias table is
 * Data_0200ad0c; if an entry is ever added, declare
 * {"address": "0x0200ad0c", "kind": "thumb"} for it and the call can be
 * spelled with an ordinary Func_ name again. Nothing else in this file
 * depends on that entry existing.
 */
