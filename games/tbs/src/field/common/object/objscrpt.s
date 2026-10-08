	.section .rodata
	.global ObjectDispatch_DefaultScript
ObjectDispatch_DefaultScript:
	.4byte 16
	.global ObjectDispatch_Table0Script
ObjectDispatch_Table0Script:
	.4byte 35, 0, 1, 12, 0xffff, 0
	.global ObjectDispatch_Table1Script
ObjectDispatch_Table1Script:
	.4byte 25, 0, 1, 12, 0xffff, 0
	.global ObjectDispatch_Table2Script
ObjectDispatch_Table2Script:
	.4byte 24, 0, 1, 12, 0xffff, 0
	.global ObjectDispatch_Table3Script
ObjectDispatch_Table3Script:
	.4byte 26, 0, 1, 12, 0xffff, 0
	.global ObjectDispatch_Table4Script
ObjectDispatch_Table4Script:
	.4byte 11, 0, 1, 12, 0xffff, 0
	.global ObjectDispatch_Table5Script
ObjectDispatch_Table5Script:
	.4byte 10, 0, 1, 12, 0xffff, 0
	.global ObjectDispatch_Table6Script
ObjectDispatch_Table6Script:
	.4byte 16

@ オブジェクトスクリプトの命令
	.global ScriptObject_CommandTable
ScriptObject_CommandTable:
	.4byte Script_StoreLookupResult
	.4byte Script_WaitForEvent
	.4byte Script_SetPositionAndResetMotion
	.4byte Script_ApplyAbsolutePosition
	.4byte Script_ApplyRelativePosition
	.4byte Script_StoreAngleToLinkedObject
	.4byte Script_ApplyLinkedObjectPosition
	.4byte Script_ApplyLocalOffsetPosition
	.4byte Object_Wander
	.4byte ScriptObject_WanderNearHome
	.4byte Object_ApproachLinkedObject
	.4byte Camera_FollowLeaderInBounds
	.4byte Script_RepeatOrJump
	.4byte Script_JumpToLabel
	.4byte Script_JumpIfTrue
	.4byte Script_JumpIfFalse
	.4byte Script_LoadMainScript
	.4byte Script_TestFlag
	.4byte Script_SetFlagAndTest
	.4byte Script_ClearFlagAndTest
	.4byte Script_ToggleFlagAndTest
	.4byte Script_ApplyOperandSet
	.4byte Script_ApplyOperandAdd
	.4byte Script_ApplyOperandCompare
	.4byte FieldObject_UpdatePlayerControl
	.4byte Object_MoveByKeys
	.4byte Object_MoveOnWorldMap
	.4byte ObjectDispatch_RunHookAndReturnZero
	.4byte Script_ApplyObjectArgument
	.4byte Script_SkipCommand
	.4byte Script_PlayAudioCue
	.4byte Script_AdvancePastCursor
	.4byte Script_ClearByte54
	.4byte Script_SetByte54
	.4byte Script_InvokeCallback
	.4byte Field_CheckConfiguredKeysAndCount
	.4byte Script_LoadMainScript
	.4byte Script_LoadMainScript
	.4byte Script_LoadMainScript
	.4byte Script_LoadMainScript
	.4byte Script_LoadMainScript
	.4byte Script_LoadMainScript
	.4byte Script_LoadMainScript
	.4byte Script_LoadMainScript
	.4byte Script_LoadMainScript
	.4byte Script_LoadMainScript
	.4byte Script_LoadMainScript
