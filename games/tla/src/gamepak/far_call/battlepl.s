.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global Func_08118088
	.thumb_func
Func_08118088:
	overlay_veneer Func_0811c074
	.global BattleParty_ListActorIdsFar
	.thumb_func
BattleParty_ListActorIdsFar:
	overlay_veneer BattleParty_ListActorIds
	.global GetBattleObjectSlotFar
	.thumb_func
GetBattleObjectSlotFar:
	overlay_veneer BattleObject_Get
	.global Battle_RunEncounterFar
	.thumb_func
Battle_RunEncounterFar:
	overlay_veneer Func_081197f0
	.global BattleMotion_ProjectPositionFar
	.thumb_func
BattleMotion_ProjectPositionFar:
	overlay_veneer BattleMotion_ProjectPosition
	.global BattleMotion_ProjectScaledPositionFar
	.thumb_func
BattleMotion_ProjectScaledPositionFar:
	overlay_veneer Func_0811c2b4
	.global BattleMotion_ProjectConditionalPositionFar
	.thumb_func
BattleMotion_ProjectConditionalPositionFar:
	overlay_veneer Func_0811c314
	.global BattleParty_ListLivingUnitsFar
	.thumb_func
BattleParty_ListLivingUnitsFar:
	overlay_veneer BattleParty_ListLivingUnits
	.global BattleParty_PrepareActiveOwnersFar
	.thumb_func
BattleParty_PrepareActiveOwnersFar:
	overlay_veneer BattleParty_PrepareActiveOwners
	.global Camera_ConfigureSceneFar
	.thumb_func
Camera_ConfigureSceneFar:
	overlay_veneer Camera_ConfigureScene
	.global GetMotionRecordFar
	.thumb_func
GetMotionRecordFar:
	overlay_veneer GetMotionRecord
	.global BattleHighlight_SetModesFar
	.thumb_func
BattleHighlight_SetModesFar:
	overlay_veneer BattleHighlight_SetModes
	.global Func_081180e8
	.thumb_func
Func_081180e8:
	overlay_veneer BattlePlayback_Start
	.global ColorBuffer_ScaleFar
	.thumb_func
ColorBuffer_ScaleFar:
	overlay_veneer ColorBuffer_Scale
	.global DjinnRecovery_CountDownFar
	.thumb_func
DjinnRecovery_CountDownFar:
	overlay_veneer DjinnRecovery_CountDown
	.global BattleMotion_GetSlotPaletteFar
	.thumb_func
BattleMotion_GetSlotPaletteFar:
	overlay_veneer BattleMotion_GetSlotPalette
	.global DjinnRecovery_CountDownTwentyFar
	.thumb_func
DjinnRecovery_CountDownTwentyFar:
	overlay_veneer DjinnRecovery_CountDownTwenty
	.global DebugParty_LoadPresetFar
	.thumb_func
DebugParty_LoadPresetFar:
	overlay_veneer DebugParty_LoadPreset
