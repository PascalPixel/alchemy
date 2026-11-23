/// @file
/// Actor scripts: the ones the field starts itself, and the command
/// table.

#include "field/actor_script.h"
#include "field/field.h"

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

const s32 gActor_KeysScript[] = {
    SC_KEYS,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

const s32 gActor_PlayerScript[] = {
    SC_PLAYER,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

const s32 gActor_WorldMapScript[] = {
    SC_WORLD,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

const s32 gActor_CameraScript[] = {
    SC_CAMERA,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

const s32 gActor_ApproachWatchedScript[] = {
    SC_APPROACH_WATCHED,
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// The idle script Actor_StartIdle gives an actor.
const s32 gActor_IdleScript[] = {
    SC_END,
};

/// The handler for each script command number; the numbers from
/// SCRIPT_COMMAND_COUNT on act as SC_END.
const ScriptCommand gActorScript_CommandTable[47] = {
    [SCRIPT_COMMAND_WAIT] = ActorScript_Wait,
    [SCRIPT_COMMAND_SYNC] = ActorScript_WaitForArrival,
    [SCRIPT_COMMAND_POSITION] = ActorScript_SetPositionAndResetMotion,
    [SCRIPT_COMMAND_MOVE] = ActorScript_ApplyAbsolutePosition,
    [SCRIPT_COMMAND_MOVE_BY] = ActorScript_ApplyRelativePosition,
    [SCRIPT_COMMAND_FACE_WATCHED] = ActorScript_StoreAngleToWatchedActor,
    [SCRIPT_COMMAND_MOVE_TO_WATCHED] = ActorScript_ApplyWatchedActorPosition,
    [SCRIPT_COMMAND_MOVE_LOCAL] = ActorScript_ApplyLocalOffsetPosition,
    [SCRIPT_COMMAND_WANDER] = Actor_Wander,
    [SCRIPT_COMMAND_WANDER_HOME] = ActorScript_WanderNearHome,
    [SCRIPT_COMMAND_APPROACH_WATCHED] = Actor_ApproachWatchedActor,
    [SCRIPT_COMMAND_CAMERA] = FieldCamera_FollowLeaderInBounds,
    [SCRIPT_COMMAND_LOOP] = ActorScript_RepeatOrJump,
    [SCRIPT_COMMAND_JUMP] = ActorScript_JumpToLabel,
    [SCRIPT_COMMAND_IF] = ActorScript_JumpIfTrue,
    [SCRIPT_COMMAND_IFNOT] = ActorScript_JumpIfFalse,
    [SCRIPT_COMMAND_END] = ActorScript_End,
    [SCRIPT_COMMAND_FLAG_CHECK] = ActorScript_TestFlag,
    [SCRIPT_COMMAND_FLAG_ON] = ActorScript_SetFlagAndTest,
    [SCRIPT_COMMAND_FLAG_OFF] = ActorScript_ClearFlagAndTest,
    [SCRIPT_COMMAND_FLAG_REV] = ActorScript_ToggleFlagAndTest,
    [SCRIPT_COMMAND_SET] = ActorScript_ApplyOperandSet,
    [SCRIPT_COMMAND_ADD] = ActorScript_ApplyOperandAdd,
    [SCRIPT_COMMAND_CMP] = ActorScript_ApplyOperandCompare,
    [SCRIPT_COMMAND_PLAYER] = Actor_UpdatePlayerControl,
    [SCRIPT_COMMAND_KEYS] = Actor_MoveByKeys,
    [SCRIPT_COMMAND_WORLD] = Actor_MoveOnWorldMap,
    [SCRIPT_COMMAND_RELEASE] = ActorScript_Release,
    [SCRIPT_COMMAND_ANIMATION] = ActorScript_SetAnimation,
    [SCRIPT_COMMAND_NOP] = ActorScript_Nop,
    [SCRIPT_COMMAND_SE] = ActorScript_PlaySound,
    [SCRIPT_COMMAND_BASE] = ActorScript_SetBase,
    [SCRIPT_COMMAND_HIDE_SPRITES] = ActorScript_HideSprites,
    [SCRIPT_COMMAND_SHOW_SPRITE] = ActorScript_ShowSprite,
    [SCRIPT_COMMAND_CALL] = ActorScript_InvokeCallback,
    [SCRIPT_COMMAND_KEY_COUNT] = Field_CheckConfiguredKeysAndCount,
    // SCRIPT_COMMAND_COUNT onwards.
    ActorScript_End,
    ActorScript_End,
    ActorScript_End,
    ActorScript_End,
    ActorScript_End,
    ActorScript_End,
    ActorScript_End,
    ActorScript_End,
    ActorScript_End,
    ActorScript_End,
    ActorScript_End,
};
