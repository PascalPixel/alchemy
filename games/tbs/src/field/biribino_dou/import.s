@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	import Engine_TaskWait, WaitFramesFar
	import Object_SetMode, ObjectDispatch_ApplyArgumentToChildrenFar
	import Engine_ObjectSetPosition, Object_SetMoveTargetFar
	import Object_CommitPosition, Script_WaitForEventTimeoutFar
	import Engine_MapCopyCellsTo, Map_CopyMetatileIndicesRectFar
	import Engine_MapCopyCellAttributes, Map_CopyCellAttributeRectFar
	import Object_CheckMovementCollision, Map_CheckObjectStepFar
	import Engine_ActorSetSpriteFlags, ObjectDispatch_SetSingleChildFlagsFar
	import Engine_GameFlagIsSet, GameFlag_TestFar
	import Engine_GameFlagSet, GameFlag_SetBitFar
	import Engine_GameFlagClear, GameFlag_ClearBitFar
	import Engine_EventWait, Battle_WaitMode0Far
	import Engine_EventBegin, Battle_ResetFar
	import Engine_EventEnd, BattleFx_FinishActionFar
	import Object_GetById, Object_GetByIdFar
	import Engine_ActorSetPosition, ObjectMotion_SetHorizontalPositionWithTerrainFar
	import Engine_ActorSetAnimation, Object_SetModeByIdFar
	import Engine_ActorSetAnimationAndWait, Motion_SetModeAndWaitAnimationFar
	import Engine_ActorSetChildValue, ObjectGroup_ConfigureChildValueFar
	import Engine_ActorSetSpritePriority, ObjectMotion_SetActionVariantFar
	import Engine_AudioPlayCue, AudioCommand_PlayFar
