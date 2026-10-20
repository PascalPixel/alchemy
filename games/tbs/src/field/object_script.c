/// @file
/// Field object scripts: the ones the engine starts itself, and the command
/// table.

#include "unit/field/object_script.h"
#include "unit/field/field.h"
#include "object_dispatch.h"
#include "field.h"
#include "objcmd.h"
#include "types.h"

/// The script FieldObject_Create gives every new object: it does nothing and
/// returns to the idle script.
const s32 gObjectDispatch_DefaultScript[] = {
    SC_END,
};

/// Runs SC_KEY_COUNT every frame; started by ObjectDispatch_StartKeyCheck.
/// This and the scripts below each repeat one command every frame.
const s32 gObjectDispatch_KeyCheckScript[] = {
    SC_KEY_COUNT,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// Started by ObjectDispatch_StartMoveByKeys.
const s32 gObjectDispatch_KeysScript[] = {
    SC_KEYS,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// Started by ObjectDispatch_StartPlayerControl.
const s32 gObjectDispatch_PlayerScript[] = {
    SC_PLAYER,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// Started by ObjectDispatch_StartWorldMapControl.
const s32 gObjectDispatch_WorldMapScript[] = {
    SC_WORLD,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// Started by ObjectDispatch_StartCamera.
const s32 gObjectDispatch_CameraScript[] = {
    SC_CAMERA,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// Started by ObjectDispatch_StartApproachLink.
const s32 gObjectDispatch_ApproachLinkScript[] = {
    SC_APPROACH_LINK,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// The idle script ObjectDispatch_StartIdle gives an object.
const s32 gObjectDispatch_EndScript[] = {
    SC_END,
};

/// The handler for each script command number; 0x24 onwards act as SC_END.
/// Object_UpdateAll (update.c) and Object_UpdateAllThumb (object_motion.c)
/// call through it.
const ScriptCommand gScriptObject_CommandTable[47] = {
    Script_Wait,                       // 0x00 SC_WAIT
    Script_WaitForArrival,             // 0x01 SC_SYNC
    Script_SetPositionAndResetMotion,  // 0x02 SC_POS
    Script_ApplyAbsolutePosition,      // 0x03 SC_MOVE
    Script_ApplyRelativePosition,      // 0x04 SC_MOVE_BY
    Script_StoreAngleToLinkedObject,   // 0x05 SC_FACE_LINK
    Script_ApplyLinkedObjectPosition,  // 0x06 SC_MOVE_LINK
    Script_ApplyLocalOffsetPosition,   // 0x07 SC_MOVE_LOCAL
    Object_Wander,                     // 0x08 SC_WANDER
    ScriptObject_WanderNearHome,       // 0x09 SC_WANDER_HOME
    Object_ApproachLinkedObject,       // 0x0a SC_APPROACH_LINK
    Camera_FollowLeaderInBounds,       // 0x0b SC_CAMERA
    Script_RepeatOrJump,               // 0x0c SC_LOOP
    Script_JumpToLabel,                // 0x0d SC_JUMP
    Script_JumpIfTrue,                 // 0x0e SC_IF
    Script_JumpIfFalse,                // 0x0f SC_IFNOT
    Script_End,                        // 0x10 SC_END
    Script_TestFlag,                   // 0x11 SC_FLAG_CHECK
    Script_SetFlagAndTest,             // 0x12 SC_FLAG_ON
    Script_ClearFlagAndTest,           // 0x13 SC_FLAG_OFF
    Script_ToggleFlagAndTest,          // 0x14 SC_FLAG_REV
    Script_ApplyOperandSet,            // 0x15 SC_SET
    Script_ApplyOperandAdd,            // 0x16 SC_ADD
    Script_ApplyOperandCompare,        // 0x17 SC_CMP
    FieldObject_UpdatePlayerControl,   // 0x18 SC_PLAYER
    Object_MoveByKeys,                 // 0x19 SC_KEYS
    Object_MoveOnWorldMap,             // 0x1a SC_WORLD
    Script_FreeObject,                 // 0x1b SC_FREE
    Script_SetAnimation,               // 0x1c SC_ARG
    Script_Nop,                        // 0x1d SC_NOP
    Script_PlaySound,                  // 0x1e SC_SE
    Script_SetBase,                    // 0x1f SC_BASE
    Script_ClearByte54,                // 0x20 SC_B54_OFF
    Script_SetByte54,                  // 0x21 SC_B54_ON
    Script_InvokeCallback,             // 0x22 SC_CALL
    Field_CheckConfiguredKeysAndCount, // 0x23 SC_KEY_COUNT
    Script_End,                        // 0x24
    Script_End,                        // 0x25
    Script_End,                        // 0x26
    Script_End,                        // 0x27
    Script_End,                        // 0x28
    Script_End,                        // 0x29
    Script_End,                        // 0x2a
    Script_End,                        // 0x2b
    Script_End,                        // 0x2c
    Script_End,                        // 0x2d
    Script_End,                        // 0x2e
};
