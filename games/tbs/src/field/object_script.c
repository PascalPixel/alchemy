/// @file
/// Object scripts: the ones the engine starts itself, and the command
/// table.

#include "unit/field/object_script.h"
#include "unit/field/field.h"
#include "object_dispatch.h"
#include "field.h"
#include "objcmd.h"
#include "types.h"

/// The script Object_CreateField gives every new object: it does nothing and
/// returns to the idle script.
const s32 gObject_DefaultScript[] = {
    SC_END,
};

/// Runs SC_KEY_COUNT every frame; started by Object_StartKeyCheck.
/// This and the scripts below each repeat one command every frame.
const s32 gObject_KeyCheckScript[] = {
    SC_KEY_COUNT,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// Started by Object_StartMoveByKeys.
const s32 gObject_KeysScript[] = {
    SC_KEYS,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// Started by Object_StartPlayerControl.
const s32 gObject_PlayerScript[] = {
    SC_PLAYER,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// Started by Object_StartWorldMapControl.
const s32 gObject_WorldMapScript[] = {
    SC_WORLD,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// Started by Object_StartCamera.
const s32 gObject_CameraScript[] = {
    SC_CAMERA,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// Started by Object_StartApproachLink.
const s32 gObject_ApproachLinkScript[] = {
    SC_APPROACH_LINK,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// The idle script Object_StartIdle gives an object.
const s32 gObject_IdleScript[] = {
    SC_END,
};

/// The handler for each script command number; 0x24 onwards act as SC_END.
/// Object_UpdateAll (update.c) and Object_UpdateAllThumb (object_motion.c)
/// call through it. They take any word up to 63 as a command, so 0x2f to 63
/// would call through the data after the table, but every script in the game
/// is written in objcmd.h's macros, the last SC_KEY_COUNT, and none holds a
/// word from 0x24 to 63 where a command goes.
const ScriptCommand gObjectScript_CommandTable[47] = {
    ObjectScript_Wait,                      // 0x00 SC_WAIT
    ObjectScript_WaitForArrival,            // 0x01 SC_SYNC
    ObjectScript_SetPositionAndResetMotion, // 0x02 SC_POSITION
    ObjectScript_ApplyAbsolutePosition,     // 0x03 SC_MOVE
    ObjectScript_ApplyRelativePosition,     // 0x04 SC_MOVE_BY
    ObjectScript_StoreAngleToLinkedObject,  // 0x05 SC_FACE_LINK
    ObjectScript_ApplyLinkedObjectPosition, // 0x06 SC_MOVE_LINK
    ObjectScript_ApplyLocalOffsetPosition,  // 0x07 SC_MOVE_LOCAL
    Object_Wander,                          // 0x08 SC_WANDER
    ObjectScript_WanderNearHome,            // 0x09 SC_WANDER_HOME
    Object_ApproachLinkedObject,            // 0x0a SC_APPROACH_LINK
    FieldCamera_FollowLeaderInBounds,       // 0x0b SC_CAMERA
    ObjectScript_RepeatOrJump,              // 0x0c SC_LOOP
    ObjectScript_JumpToLabel,               // 0x0d SC_JUMP
    ObjectScript_JumpIfTrue,                // 0x0e SC_IF
    ObjectScript_JumpIfFalse,               // 0x0f SC_IFNOT
    ObjectScript_End,                       // 0x10 SC_END
    ObjectScript_TestFlag,                  // 0x11 SC_FLAG_CHECK
    ObjectScript_SetFlagAndTest,            // 0x12 SC_FLAG_ON
    ObjectScript_ClearFlagAndTest,          // 0x13 SC_FLAG_OFF
    ObjectScript_ToggleFlagAndTest,         // 0x14 SC_FLAG_REV
    ObjectScript_ApplyOperandSet,           // 0x15 SC_SET
    ObjectScript_ApplyOperandAdd,           // 0x16 SC_ADD
    ObjectScript_ApplyOperandCompare,       // 0x17 SC_CMP
    Object_UpdateLeaderControl,             // 0x18 SC_PLAYER
    Object_MoveByKeys,                      // 0x19 SC_KEYS
    Object_MoveOnWorldMap,                  // 0x1a SC_WORLD
    ObjectScript_FreeObject,                // 0x1b SC_FREE
    ObjectScript_SetAnimation,              // 0x1c SC_ANIMATION
    ObjectScript_Nop,                       // 0x1d SC_NOP
    ObjectScript_PlaySound,                 // 0x1e SC_SE
    ObjectScript_SetBase,                   // 0x1f SC_BASE
    ObjectScript_ClearByte54,               // 0x20 SC_HIDE_SPRITES
    ObjectScript_SetByte54,                 // 0x21 SC_SHOW_SPRITE
    ObjectScript_InvokeCallback,            // 0x22 SC_CALL
    Field_CheckConfiguredKeysAndCount,      // 0x23 SC_KEY_COUNT
    ObjectScript_End,                       // 0x24
    ObjectScript_End,                       // 0x25
    ObjectScript_End,                       // 0x26
    ObjectScript_End,                       // 0x27
    ObjectScript_End,                       // 0x28
    ObjectScript_End,                       // 0x29
    ObjectScript_End,                       // 0x2a
    ObjectScript_End,                       // 0x2b
    ObjectScript_End,                       // 0x2c
    ObjectScript_End,                       // 0x2d
    ObjectScript_End,                       // 0x2e
};
