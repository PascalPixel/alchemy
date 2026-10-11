@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	import __divsi3, IwramSignedDivide
	import Engine_TaskWait, WaitFramesFar
	import Trig_Sin, Trig_SinFar
	import Trig_Cos, Trig_CosFar
	import Object_SetMode, ObjectDispatch_ApplyArgumentToChildrenFar
	import Engine_ObjectSetScript, ObjectDispatch_InitializeFar
	import Engine_ObjectCreate, Object_CreateFar
	import Engine_MapCopyCellAttributes, Map_CopyCellAttributeRectFar
	import Engine_ActorSetSpriteFlags, ObjectDispatch_SetSingleChildFlagsFar
	import Engine_ObjectSetBlendMode, ObjectDispatch_SetChildBlendModeFar
	import Engine_WorkSetValuesIfNonNegative, Runtime_SetWorkTripleIfNonNegativeFar
	import Map_WaitWorkValuesBelow256, Map_WaitWorkValuesBelow256Far
	import Engine_GameFlagIsSet, GameFlag_TestFar
	import Engine_GameFlagSet, GameFlag_SetBitFar
	import Engine_GameFlagClear, GameFlag_ClearBitFar
	import Object_GetById, Object_GetByIdFar
	import Engine_ActorSetSpeed, ObjectMotion_SetSpeedParametersFar
	import Engine_ActorSetDestination, ObjectMotion_ResetAndSetPositionFar
	import Engine_ActorWaitForMove, ObjectMotion_CommitCurrentPositionAndActivateFar
	import Engine_ActorSetPosition, ObjectMotion_SetHorizontalPositionWithTerrainFar
	import Engine_ActorSetAnimation, Object_SetModeByIdFar
	import ObjectGroup_SetChildValue, ObjectGroup_SetChildValueFar
	import Engine_AudioPlayCue, AudioCommand_PlayFar
