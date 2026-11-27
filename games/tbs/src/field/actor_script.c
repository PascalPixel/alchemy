/* @file */
/* Actor scripts: the ones the field starts itself, and the command */
/* table. */

#include "field/actor_script.h"
#include "field/field.h"

/* The script fdActorCreate gives every new actor: it does nothing and */
/* returns to the idle script. */
const s32 Actor_DefaultScript[] = {
	SC_END,
};

/* Runs SC_KEY_COUNT every frame; started by fdActorStartKeyCheck. */
/* This and the scripts below each repeat one command every frame. */
const s32 Actor_KeyCheckScript[] = {
	SC_KEY_COUNT,
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

const s32 Actor_KeysScript[] = {
	SC_KEYS,
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

const s32 Actor_PlayerScript[] = {
	SC_PLAYER,
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

const s32 Actor_WorldMapScript[] = {
	SC_WORLD,
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

const s32 Actor_CameraScript[] = {
	SC_CAMERA,
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

const s32 Actor_ApproachWatchedScript[] = {
	SC_APPROACH_WATCHED,
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* The idle script fdActorStartIdle gives an actor. */
const s32 Actor_IdleScript[] = {
	SC_END,
};

/* The handler for each script command number; the numbers from */
/* SCRIPT_COMMAND_COUNT on act as SC_END. */
const SCRIPT_COMMAND ActorScript_CommandTable[47] = {
	[SCRIPT_COMMAND_WAIT] = fdActorScriptWait,
	[SCRIPT_COMMAND_SYNC] = fdActorScriptWaitForArrival,
	[SCRIPT_COMMAND_POSITION] = fdActorScriptSetPosAndResetMotion,
	[SCRIPT_COMMAND_MOVE] = fdActorScriptApplyAbsolutePos,
	[SCRIPT_COMMAND_MOVE_BY] = fdActorScriptApplyRelativePos,
	[SCRIPT_COMMAND_FACE_WATCHED] = fdActorScriptStoreAngToWatchedActor,
	[SCRIPT_COMMAND_MOVE_TO_WATCHED] = fdActorScriptApplyWatchedActorPos,
	[SCRIPT_COMMAND_MOVE_LOCAL] = fdActorScriptApplyLocalOffsetPos,
	[SCRIPT_COMMAND_WANDER] = fdActorWander,
	[SCRIPT_COMMAND_WANDER_HOME] = fdActorScriptWanderNearHome,
	[SCRIPT_COMMAND_APPROACH_WATCHED] = fdActorApproachWatchedActor,
	[SCRIPT_COMMAND_CAMERA] = fdCameraFollowLeaderInBounds,
	[SCRIPT_COMMAND_LOOP] = fdActorScriptRepeatOrJump,
	[SCRIPT_COMMAND_JUMP] = fdActorScriptJumpToLabel,
	[SCRIPT_COMMAND_IF] = fdActorScriptJumpIfTrue,
	[SCRIPT_COMMAND_IFNOT] = fdActorScriptJumpIfFalse,
	[SCRIPT_COMMAND_END] = fdActorScriptEnd,
	[SCRIPT_COMMAND_FLAG_CHECK] = fdActorScriptTestFlag,
	[SCRIPT_COMMAND_FLAG_ON] = fdActorScriptSetFlagAndTest,
	[SCRIPT_COMMAND_FLAG_OFF] = fdActorScriptClearFlagAndTest,
	[SCRIPT_COMMAND_FLAG_REV] = fdActorScriptToggleFlagAndTest,
	[SCRIPT_COMMAND_SET] = fdActorScriptApplyOperandSet,
	[SCRIPT_COMMAND_ADD] = fdActorScriptApplyOperandAdd,
	[SCRIPT_COMMAND_CMP] = fdActorScriptApplyOperandCompare,
	[SCRIPT_COMMAND_PLAYER] = fdActorUpdatePlayerCtrl,
	[SCRIPT_COMMAND_KEYS] = fdActorMoveByKeys,
	[SCRIPT_COMMAND_WORLD] = fdActorMoveOnWorldMap,
	[SCRIPT_COMMAND_RELEASE] = fdActorScriptRelease,
	[SCRIPT_COMMAND_ANIMATION] = fdActorScriptSetAnimation,
	[SCRIPT_COMMAND_NOP] = fdActorScriptNop,
	[SCRIPT_COMMAND_SE] = fdActorScriptPlaySnd,
	[SCRIPT_COMMAND_BASE] = fdActorScriptSetBase,
	[SCRIPT_COMMAND_HIDE_SPRITES] = fdActorScriptHideSprs,
	[SCRIPT_COMMAND_SHOW_SPRITE] = fdActorScriptShowSpr,
	[SCRIPT_COMMAND_CALL] = fdActorScriptInvokeCallback,
	[SCRIPT_COMMAND_KEY_COUNT] = fdCheckConfiguredKeysAndCount,
	/* SCRIPT_COMMAND_COUNT onwards. */
	fdActorScriptEnd,
	fdActorScriptEnd,
	fdActorScriptEnd,
	fdActorScriptEnd,
	fdActorScriptEnd,
	fdActorScriptEnd,
	fdActorScriptEnd,
	fdActorScriptEnd,
	fdActorScriptEnd,
	fdActorScriptEnd,
	fdActorScriptEnd,
};
