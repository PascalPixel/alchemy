#include "objcmd.h"
#include "types.h"
#include "edition.h"

//--------------------------------------------------------------------------
// Object motion scripts and the small tables between them. Most are The
// Broken Seal's object scripts with The Lost Age's command numbers; its
// names for them are in the comments. The names the listings use stay.
//--------------------------------------------------------------------------

/// Each callback takes the object and returns nonzero to go on with the
/// script this frame. They take different views of the object, so the
/// declarations leave the parameters unspecified.
s32 ObjectMotion_StepAngle();
s32 Object_ClearChildAndCollisionFlags();
s32 Object_ClearChildFlags();
s32 Object_SetPriorityBit5();
s32 ObjectMotion_MoveTowardTarget();
s32 SceneActor_UpdateFacingTowardTarget();
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
s32 Func_080da908();
#else
s32 Func_080d4ab4();
#endif

/// DisplayTransition_ShapesTail
// clang-format off
const u8 gData_080f321c[12] = {
    0, 2, 4, 2,
    0, 2, 4, 2,
    0, 2, 4, 2,
};

/// {key, value} rows ended by a key of -1; this list has only the end.
const s32 gData_080f3228[2] = {
    -1, -1,
};

/// The cells of an 8x8 grid, one diagonal at a time from the top left
/// corner, each diagonal ended by -1; -4 follows the longest diagonal and
/// -9 the last.
const s16 gData_080f3230[80] = {
    0, -1,
    1, 8, -1,
    2, 9, 16, -1,
    3, 10, 17, 24, -1,
    4, 11, 18, 25, 32, -1,
    5, 12, 19, 26, 33, 40, -1,
    6, 13, 20, 27, 34, 41, 48, -1,
    7, 14, 21, 28, 35, 42, 49, 56, -4,
    15, 22, 29, 36, 43, 50, 57, -1,
    23, 30, 37, 44, 51, 58, -1,
    31, 38, 45, 52, 59, -1,
    39, 46, 53, 60, -1,
    47, 54, 61, -1,
    55, 62, -1,
    63, -9,
    0,
};
// clang-format on

const s32 gObject_OffsetMotionScript[] = {
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

const s32 gData_080f32e0[] = {
    SC_SYNC, SC_WAIT(1), SC_ARG(1), SC_CALL(ObjectMotion_StepAngle), SC_END,
};

const s32 gObjectMotion_StepAngleScript[] = {
    SC_CALL(ObjectMotion_StepAngle),
    SC_END,
};

/// gUi_RenderResultValues
const u8 gData_080f330c[4] = {
    1,
    3,
    10,
    0,
};

/// gBattleFx_LinkedObjectScript: squash and stretch, then release.
const s32 gData_080f3310[] = {
    SC_ADD(SW_SCALE_X, -0x8000),
    SC_ADD(SW_SCALE_Y, 0x4000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x4000),
    SC_ADD(SW_SCALE_Y, -0x2000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x2000),
    SC_ADD(SW_SCALE_Y, -0x1000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x2000),
    SC_ADD(SW_SCALE_Y, -0x1000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -0x800),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, -0x1000),
    SC_ADD(SW_SCALE_Y, 0x800),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, -0x800),
    SC_ADD(SW_SCALE_Y, 0x400),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x800),
    SC_ADD(SW_SCALE_Y, -0x400),
    SC_WAIT(2),
    SC_WAIT(30),
    SC_FREE,
};

/// gObjectMotion_WaitScript
const s32 gData_080f341c[] = {
    SC_WAIT(40),
    SC_FREE,
};

/// gObjectMotion_ActionKind2Script: wander near home, pausing between steps.
const s32 gData_080f3428[] = {
    SC_SET(SW_SPEED, 0x8000),
    SC_SET(SW_ACCEL, 0xccc),
    SC_SET(SW_COLLISION, 0x83),
    SC_LABEL(1),
    SC_ARG(2),
    SC_WANDER_HOME(0x40000, 0x80000, 0x100000),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(20),
    SC_ARG(2),
    SC_WANDER_HOME(0x40000, 0x80000, 0x100000),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(20),
    SC_ARG(2),
    SC_WANDER_HOME(0x40000, 0x80000, 0x100000),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(60),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

/// gObjectMotion_ActionKind1Script
const s32 gData_080f34e4[] = {
    SC_END,
};

/// gObjectMotion_ResetActionScript
const s32 gData_080f34e8[] = {
    SC_CALL(Object_ClearChildAndCollisionFlags),
    SC_END,
};

const s32 gData_080f34f4[] = {
    SC_CALL(Object_ClearChildFlags),
    SC_END,
};

const s32 gData_080f3500[] = {
    SC_CALL(Object_SetPriorityBit5),
    SC_END,
};

/// gObjectMotion_ActionKind3Script: wander a shorter way near home.
const s32 gData_080f350c[] = {
    SC_SET(SW_SPEED, 0x8000),
    SC_SET(SW_ACCEL, 0xccc),
    SC_SET(SW_COLLISION, 0x83),
    SC_LABEL(1),
    SC_ARG(2),
    SC_WANDER_HOME(0x20000, 0x20000, 0x80000),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(20),
    SC_ARG(2),
    SC_WANDER_HOME(0x20000, 0x20000, 0x80000),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(20),
    SC_ARG(2),
    SC_WANDER_HOME(0x20000, 0x20000, 0x80000),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(60),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

/// gObjectMotion_ActionKind4Script: wander near home without pausing.
const s32 gData_080f35c8[] = {
    SC_SET(SW_SPEED, 0x6666),
    SC_SET(SW_ACCEL, 0x3333),
    SC_ARG(2),
    SC_LABEL(1),
    SC_WANDER_HOME(0x20000, 0x20000, 0x60000),
    SC_SYNC,
    SC_WAIT(4),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

/// Squash and stretch, twice in each round, for ever.
const s32 gData_080f3614[] = {
    SC_LABEL(1),
    SC_SET(SW_SCALE_X, 0x10000),
    SC_SET(SW_SCALE_Y, 0x10000),
    SC_ADD(SW_SCALE_X, -0x4000),
    SC_ADD(SW_SCALE_Y, 0x2000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x2000),
    SC_ADD(SW_SCALE_Y, -0x1000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -0x800),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -0x800),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_X, -0x4000),
    SC_ADD(SW_SCALE_Y, 0x2000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x2000),
    SC_ADD(SW_SCALE_Y, -0x1000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -0x800),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -0x800),
    SC_WAIT(4),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

/// gObjectMotion_MoveTowardTargetScript
const s32 gData_080f373c[] = {
    SC_CALL(ObjectMotion_MoveTowardTarget),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// gObjectMotion_TurnTowardLinkedScript
const s32 gData_080f3750[] = {
    SC_CALL(SceneActor_UpdateFacingTowardTarget),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// Keep the leader's display layer. The Spanish and Italian scaffolds name
/// the function by the far-call slot that reaches it in the other editions.
const s32 gData_080f3764[] = {
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
    SC_CALL(Func_080da908),
#else
    SC_CALL(Func_080d4ab4),
#endif
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// gObjectMotion_LinkedActionScript
const s32 gObject_LinkedMotionScript[] = {
    SC_FACE_LINK,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};
