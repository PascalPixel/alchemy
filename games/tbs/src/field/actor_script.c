/// @file
/// Actor scripts: the ones the field starts itself, and the command
/// table.

#include "module/field/actor_script.h"
#include "module/field/field.h"
#include "actor_builtin_scripts.h"
#include "field.h"
#include "actor_script.h"
#include "types.h"

/// The script Actor_Create gives every new actor: it does nothing and
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

/// Started by Actor_StartApproachWatched.
const s32 gActor_ApproachWatchedScript[] = {
    SC_APPROACH_WATCHED,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// The idle script Actor_StartIdle gives an actor.
const s32 gActor_IdleScript[] = {
    SC_END,
};

/// The handler for each script command number; 36 onwards act as SC_END.
/// Actor_UpdateAll (actor_update_arm.c) and Actor_UpdateAllThumb
/// (actor_motion.c) call through it. They take any word up to 63 as a command,
/// so 47 to 63 would call through the data after the table, but every script
/// in the game is written in actor_script.h's macros, the last SC_KEY_COUNT,
/// and none holds a word from 36 to 63 where a command goes.
const ScriptCommand gActorScript_CommandTable[47] = {
    ActorScript_Wait,                      // 0 SC_WAIT
    ActorScript_WaitForArrival,            // 1 SC_SYNC
    ActorScript_SetPositionAndResetMotion, // 2 SC_POSITION
    ActorScript_ApplyAbsolutePosition,     // 3 SC_MOVE
    ActorScript_ApplyRelativePosition,     // 4 SC_MOVE_BY
    ActorScript_StoreAngleToWatchedActor,  // 5 SC_FACE_WATCHED
    ActorScript_ApplyWatchedActorPosition, // 6 SC_MOVE_TO_WATCHED
    ActorScript_ApplyLocalOffsetPosition,  // 7 SC_MOVE_LOCAL
    Actor_Wander,                          // 8 SC_WANDER
    ActorScript_WanderNearHome,            // 9 SC_WANDER_HOME
    Actor_ApproachWatchedActor,            // 10 SC_APPROACH_WATCHED
    FieldCamera_FollowLeaderInBounds,      // 11 SC_CAMERA
    ActorScript_RepeatOrJump,              // 12 SC_LOOP
    ActorScript_JumpToLabel,               // 13 SC_JUMP
    ActorScript_JumpIfTrue,                // 14 SC_IF
    ActorScript_JumpIfFalse,               // 15 SC_IFNOT
    ActorScript_End,                       // 16 SC_END
    ActorScript_TestFlag,                  // 17 SC_FLAG_CHECK
    ActorScript_SetFlagAndTest,            // 18 SC_FLAG_ON
    ActorScript_ClearFlagAndTest,          // 19 SC_FLAG_OFF
    ActorScript_ToggleFlagAndTest,         // 20 SC_FLAG_REV
    ActorScript_ApplyOperandSet,           // 21 SC_SET
    ActorScript_ApplyOperandAdd,           // 22 SC_ADD
    ActorScript_ApplyOperandCompare,       // 23 SC_CMP
    Actor_UpdatePlayerControl,             // 24 SC_PLAYER
    Actor_MoveByKeys,                      // 25 SC_KEYS
    Actor_MoveOnWorldMap,                  // 26 SC_WORLD
    ActorScript_Release,                   // 27 SC_RELEASE
    ActorScript_SetAnimation,              // 28 SC_ANIMATION
    ActorScript_Nop,                       // 29 SC_NOP
    ActorScript_PlaySound,                 // 30 SC_SE
    ActorScript_SetBase,                   // 31 SC_BASE
    ActorScript_HideSprites,               // 32 SC_HIDE_SPRITES
    ActorScript_ShowSprite,                // 33 SC_SHOW_SPRITE
    ActorScript_InvokeCallback,            // 34 SC_CALL
    Field_CheckConfiguredKeysAndCount,     // 35 SC_KEY_COUNT
    ActorScript_End,                       // 36
    ActorScript_End,                       // 37
    ActorScript_End,                       // 38
    ActorScript_End,                       // 39
    ActorScript_End,                       // 40
    ActorScript_End,                       // 41
    ActorScript_End,                       // 42
    ActorScript_End,                       // 43
    ActorScript_End,                       // 44
    ActorScript_End,                       // 45
    ActorScript_End,                       // 46
};
