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
/// SCRIPT_CMD_COUNT on act as SC_END.
const ScriptCommand gActorScript_CommandTable[47] = {
    [SCRIPT_CMD_WAIT] = ActorScript_Wait,
    [SCRIPT_CMD_SYNC] = ActorScript_WaitForArrival,
    [SCRIPT_CMD_POSITION] = ActorScript_SetPositionAndResetMotion,
    [SCRIPT_CMD_MOVE] = ActorScript_ApplyAbsolutePosition,
    [SCRIPT_CMD_MOVE_BY] = ActorScript_ApplyRelativePosition,
    [SCRIPT_CMD_FACE_WATCHED] = ActorScript_StoreAngleToWatchedActor,
    [SCRIPT_CMD_MOVE_TO_WATCHED] = ActorScript_ApplyWatchedActorPosition,
    [SCRIPT_CMD_MOVE_LOCAL] = ActorScript_ApplyLocalOffsetPosition,
    [SCRIPT_CMD_WANDER] = Actor_Wander,
    [SCRIPT_CMD_WANDER_HOME] = ActorScript_WanderNearHome,
    [SCRIPT_CMD_APPROACH_WATCHED] = Actor_ApproachWatchedActor,
    [SCRIPT_CMD_CAMERA] = FieldCamera_FollowLeaderInBounds,
    [SCRIPT_CMD_LOOP] = ActorScript_RepeatOrJump,
    [SCRIPT_CMD_JUMP] = ActorScript_JumpToLabel,
    [SCRIPT_CMD_IF] = ActorScript_JumpIfTrue,
    [SCRIPT_CMD_IFNOT] = ActorScript_JumpIfFalse,
    [SCRIPT_CMD_END] = ActorScript_End,
    [SCRIPT_CMD_FLAG_CHECK] = ActorScript_TestFlag,
    [SCRIPT_CMD_FLAG_ON] = ActorScript_SetFlagAndTest,
    [SCRIPT_CMD_FLAG_OFF] = ActorScript_ClearFlagAndTest,
    [SCRIPT_CMD_FLAG_REV] = ActorScript_ToggleFlagAndTest,
    [SCRIPT_CMD_SET] = ActorScript_ApplyOperandSet,
    [SCRIPT_CMD_ADD] = ActorScript_ApplyOperandAdd,
    [SCRIPT_CMD_CMP] = ActorScript_ApplyOperandCompare,
    [SCRIPT_CMD_PLAYER] = Actor_UpdatePlayerControl,
    [SCRIPT_CMD_KEYS] = Actor_MoveByKeys,
    [SCRIPT_CMD_WORLD] = Actor_MoveOnWorldMap,
    [SCRIPT_CMD_RELEASE] = ActorScript_Release,
    [SCRIPT_CMD_ANIMATION] = ActorScript_SetAnimation,
    [SCRIPT_CMD_NOP] = ActorScript_Nop,
    [SCRIPT_CMD_SE] = ActorScript_PlaySound,
    [SCRIPT_CMD_BASE] = ActorScript_SetBase,
    [SCRIPT_CMD_HIDE_SPRITES] = ActorScript_HideSprites,
    [SCRIPT_CMD_SHOW_SPRITE] = ActorScript_ShowSprite,
    [SCRIPT_CMD_CALL] = ActorScript_InvokeCallback,
    [SCRIPT_CMD_KEY_COUNT] = Field_CheckConfiguredKeysAndCount,
    // SCRIPT_CMD_COUNT onwards.
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
