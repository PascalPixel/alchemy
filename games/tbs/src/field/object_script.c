/// @file
/// Object scripts: the ones the engine starts itself, and the command
/// table.

#include "unit/field/object_script.h"
#include "unit/field/field.h"
#include "object_dispatch.h"
#include "field.h"
#include "objcmd.h"
#include "types.h"

/// The script Actor_Create gives every new object: it does nothing and
/// returns to the idle script.
const s32 gActor_DefaultScript[] = {
    SC_END,
};

/// Runs SC_KEY_COUNT every frame; started by Actor_StartKeyCheck.
/// This and the scripts below each repeat one command every frame.
const s32 gActor_KeyCheckScript[] = {
    SC_KEY_COUNT,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// Started by Actor_StartMoveByKeys.
const s32 gActor_KeysScript[] = {
    SC_KEYS,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// Started by Actor_StartPlayerControl.
const s32 gActor_PlayerScript[] = {
    SC_PLAYER,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// Started by Actor_StartWorldMapControl.
const s32 gActor_WorldMapScript[] = {
    SC_WORLD,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// Started by Actor_StartCamera.
const s32 gActor_CameraScript[] = {
    SC_CAMERA,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// Started by Actor_StartApproachLink.
const s32 gActor_ApproachLinkScript[] = {
    SC_APPROACH_LINK,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// The idle script Actor_StartIdle gives an object.
const s32 gActor_IdleScript[] = {
    SC_END,
};

/// The handler for each script command number; 0x24 onwards act as SC_END.
/// Actor_UpdateAll (update.c) and Actor_UpdateAllThumb (object_motion.c)
/// call through it. They take any word up to 63 as a command, so 0x2f to 63
/// would call through the data after the table, but every script in the game
/// is written in objcmd.h's macros, the last SC_KEY_COUNT, and none holds a
/// word from 0x24 to 63 where a command goes.
const ScriptCommand gActorScript_CommandTable[47] = {
    ActorScript_Wait,                      // 0x00 SC_WAIT
    ActorScript_WaitForArrival,            // 0x01 SC_SYNC
    ActorScript_SetPositionAndResetMotion, // 0x02 SC_POSITION
    ActorScript_ApplyAbsolutePosition,     // 0x03 SC_MOVE
    ActorScript_ApplyRelativePosition,     // 0x04 SC_MOVE_BY
    ActorScript_StoreAngleToLinkedActor,   // 0x05 SC_FACE_LINK
    ActorScript_ApplyLinkedActorPosition,  // 0x06 SC_MOVE_LINK
    ActorScript_ApplyLocalOffsetPosition,  // 0x07 SC_MOVE_LOCAL
    Actor_Wander,                          // 0x08 SC_WANDER
    ActorScript_WanderNearHome,            // 0x09 SC_WANDER_HOME
    Actor_ApproachLinkedActor,             // 0x0a SC_APPROACH_LINK
    FieldCamera_FollowLeaderInBounds,      // 0x0b SC_CAMERA
    ActorScript_RepeatOrJump,              // 0x0c SC_LOOP
    ActorScript_JumpToLabel,               // 0x0d SC_JUMP
    ActorScript_JumpIfTrue,                // 0x0e SC_IF
    ActorScript_JumpIfFalse,               // 0x0f SC_IFNOT
    ActorScript_End,                       // 0x10 SC_END
    ActorScript_TestFlag,                  // 0x11 SC_FLAG_CHECK
    ActorScript_SetFlagAndTest,            // 0x12 SC_FLAG_ON
    ActorScript_ClearFlagAndTest,          // 0x13 SC_FLAG_OFF
    ActorScript_ToggleFlagAndTest,         // 0x14 SC_FLAG_REV
    ActorScript_ApplyOperandSet,           // 0x15 SC_SET
    ActorScript_ApplyOperandAdd,           // 0x16 SC_ADD
    ActorScript_ApplyOperandCompare,       // 0x17 SC_CMP
    Actor_UpdatePlayerControl,             // 0x18 SC_PLAYER
    Actor_MoveByKeys,                      // 0x19 SC_KEYS
    Actor_MoveOnWorldMap,                  // 0x1a SC_WORLD
    ActorScript_Release,                   // 0x1b SC_RELEASE
    ActorScript_SetAnimation,              // 0x1c SC_ANIMATION
    ActorScript_Nop,                       // 0x1d SC_NOP
    ActorScript_PlaySound,                 // 0x1e SC_SE
    ActorScript_SetBase,                   // 0x1f SC_BASE
    ActorScript_HideSprites,               // 0x20 SC_HIDE_SPRITES
    ActorScript_ShowSprite,                // 0x21 SC_SHOW_SPRITE
    ActorScript_InvokeCallback,            // 0x22 SC_CALL
    Field_CheckConfiguredKeysAndCount,     // 0x23 SC_KEY_COUNT
    ActorScript_End,                       // 0x24
    ActorScript_End,                       // 0x25
    ActorScript_End,                       // 0x26
    ActorScript_End,                       // 0x27
    ActorScript_End,                       // 0x28
    ActorScript_End,                       // 0x29
    ActorScript_End,                       // 0x2a
    ActorScript_End,                       // 0x2b
    ActorScript_End,                       // 0x2c
    ActorScript_End,                       // 0x2d
    ActorScript_End,                       // 0x2e
};
