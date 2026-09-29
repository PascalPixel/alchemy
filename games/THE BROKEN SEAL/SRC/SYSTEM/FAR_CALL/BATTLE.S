@ Far-call stub table heading the battle code at 0x080b5000: fixed 8-byte
@ veneers, each loading its target into r4 and branching, which the calling
@ convention permits. No compiler emits them; the shared overlay veneer macro
@ builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.global FarCall_BattleTable
FarCall_BattleTable:
	.global Func_080b5000
	.thumb_func
Func_080b5000:
	overlay_veneer Unnamed_080b56e0
	overlay_veneer BattlePres_RunMosaicFadeOut
	.global BattleActor_SpawnObjectsForListFar
	.global Func_080b5010
	.thumb_func
BattleActor_SpawnObjectsForListFar:
Func_080b5010:
	overlay_veneer BattleActor_SpawnObjectsForList
	overlay_veneer Graphics_FadeAlphaOut
	overlay_veneer Graphics_FadeAlphaIn
	.global Func_080b5028
	.thumb_func
Func_080b5028:
	.global BattlePres_SetupTransitionSceneFar
	.thumb_func
BattlePres_SetupTransitionSceneFar:
	overlay_veneer BattlePres_SetupTransitionScene
	.global BattlePres_SetupTransitionAtPairMidpointFar
	.thumb_func
BattlePres_SetupTransitionAtPairMidpointFar:
	overlay_veneer BattlePres_SetupTransitionAtPairMidpoint
	.global BattlePresentation_ConfigurePaletteFadeFar
	.thumb_func
BattlePresentation_ConfigurePaletteFadeFar:
	overlay_veneer BattlePresentation_ConfigurePaletteFade
	.global BattleBackground_LoadFar
	.thumb_func
BattleBackground_LoadFar:
	overlay_veneer Func_080c08ec
	.global Func_080b5048
	.thumb_func
Func_080b5048:
	.global BattlePresentation_SetPaletteLevelFar
	.thumb_func
BattlePresentation_SetPaletteLevelFar:
	overlay_veneer BattlePresentation_SetPaletteLevel
	overlay_veneer Summon_GetEntryByte4
	.global Func_080b5058
	.thumb_func
Func_080b5058:
	.global Summon_IsEntrySecondaryFlaggedFar
	.thumb_func
Summon_IsEntrySecondaryFlaggedFar:
	overlay_veneer Summon_IsEntrySecondaryFlagged
	overlay_veneer Battle_GetEntryField2HighBits
	overlay_veneer Summon_IsEntryFlagged
	.global Battle_GetObjectTableValueFar
	.global Func_080b5070
	.thumb_func
Battle_GetObjectTableValueFar:
Func_080b5070:
	overlay_veneer Battle_GetObjectTableValue
	.global BattleMotion_ApproachTargetFar
	.thumb_func
BattleMotion_ApproachTargetFar:
	overlay_veneer BattleMotion_ApproachTarget
	overlay_veneer BattleMotion_InitializeObject
	.global BattleMotion_ApplyVariantMotionFar
	.global Func_080b5088
	.thumb_func
BattleMotion_ApplyVariantMotionFar:
Func_080b5088:
	overlay_veneer BattleMotion_ApplyVariantMotion
	.global BattleParty_ListActorIdsFar
	.thumb_func
BattleParty_ListActorIdsFar:
	overlay_veneer BattleParty_ListActorIds
	.global GetBattleObjectSlotFar
	.thumb_func
GetBattleObjectSlotFar:
	overlay_veneer GetBattleObjectSlot
	.global Battle_RunEncounterFar
	.thumb_func
Battle_RunEncounterFar:
	overlay_veneer Battle_RunEncounter
	.global BattleMotion_ProjectPositionFar
	.thumb_func
BattleMotion_ProjectPositionFar:
	overlay_veneer BattleMotion_ProjectPosition
	.global BattleMotion_ProjectScaledPositionFar
	.thumb_func
BattleMotion_ProjectScaledPositionFar:
	overlay_veneer BattleMotion_ProjectScaledPosition
	.global BattleMotion_ProjectConditionalPositionFar
	.thumb_func
BattleMotion_ProjectConditionalPositionFar:
	overlay_veneer BattleMotion_ProjectConditionalPosition
	overlay_veneer BattleParty_ListLivingUnits
	.global BattleParty_PrepareActiveOwnersFar
	.global Func_080b50c8
	.thumb_func
BattleParty_PrepareActiveOwnersFar:
Func_080b50c8:
	overlay_veneer BattleParty_PrepareActiveOwners
	.global Camera_ConfigureSceneFar
	.thumb_func
Camera_ConfigureSceneFar:
	overlay_veneer Camera_ConfigureScene
	.global GetMotionRecordFar
	.thumb_func
GetMotionRecordFar:
	overlay_veneer GetMotionRecord
	.global BattlePres_SetActorModesFar
	.thumb_func
BattlePres_SetActorModesFar:
	overlay_veneer BattlePres_SetActorModes
	.global BattleEventRuntime_BeginPhaseFar
	.thumb_func
BattleEventRuntime_BeginPhaseFar:
	.global Func_080b50e8
	.thumb_func
Func_080b50e8:
	overlay_veneer BattleEventRuntime_BeginPhase
	.global Graphics_ScaleRgb555ClampedFar
	.thumb_func
Graphics_ScaleRgb555ClampedFar:
	overlay_veneer Graphics_ScaleRgb555Clamped
	.global Func_080b50f8
	.thumb_func
Func_080b50f8:
	.global BattlePlacement_UpdateTimedEntriesFar
	.thumb_func
BattlePlacement_UpdateTimedEntriesFar:
	overlay_veneer BattlePlacement_UpdateTimedEntries
	.global BattleMotion_GetSlotField14Far
	.thumb_func
BattleMotion_GetSlotField14Far:
	overlay_veneer BattleMotion_GetSlotField14
	.global BattlePlacement_UpdateTimedEntriesTwentyTimesFar
	.thumb_func
BattlePlacement_UpdateTimedEntriesTwentyTimesFar:
	overlay_veneer BattlePlacement_UpdateTimedEntriesTwentyTimes
	.global DebugParty_LoadPresetFar
	.thumb_func
DebugParty_LoadPresetFar:
	overlay_veneer DebugParty_LoadPreset
	.global BattleActor_CommitPlacementFar
	.global Func_080b5118
	.thumb_func
BattleActor_CommitPlacementFar:
Func_080b5118:
	overlay_veneer BattleActor_CommitPlacement
	overlay_veneer ReleaseBattleObjectRecords
	.global Func_080b5128
	.thumb_func
Func_080b5128:
	overlay_veneer Unnamed_080bb7c0
	.global BattlePlacement_CountValidEntriesFar
	.thumb_func
BattlePlacement_CountValidEntriesFar:
	overlay_veneer BattlePlacement_CountValidEntries