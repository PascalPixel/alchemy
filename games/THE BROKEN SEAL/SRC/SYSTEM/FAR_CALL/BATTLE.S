@ Far-call stub table heading the battle code at 0x080b5000: fixed 8-byte
@ veneers, each loading its target into r4 and branching, which the calling
@ convention permits. No compiler emits them; the shared overlay veneer macro
@ builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.global Func_080b5000
	.thumb_func
Func_080b5000:
	overlay_veneer 0x080b56e1
	overlay_veneer 0x080bffb9
	.global BattleActor_SpawnObjectsForListFar
	.global Func_080b5010
	.thumb_func
BattleActor_SpawnObjectsForListFar:
Func_080b5010:
	overlay_veneer 0x080b7b6d
	overlay_veneer 0x080c0e39
	overlay_veneer 0x080c0e71
	.global Func_080b5028
	.thumb_func
Func_080b5028:
	overlay_veneer 0x080c0ced
	overlay_veneer 0x080c0df5
	.global BattlePresentation_ConfigurePaletteFadeFar
	.thumb_func
BattlePresentation_ConfigurePaletteFadeFar:
	overlay_veneer 0x080c0775
	.global BattleBackground_LoadFar
	.thumb_func
BattleBackground_LoadFar:
	overlay_veneer 0x080c08ed
	.global Func_080b5048
	.thumb_func
Func_080b5048:
	overlay_veneer 0x080c0701
	overlay_veneer 0x080c2455
	.global Func_080b5058
	.thumb_func
Func_080b5058:
	overlay_veneer 0x080c2435
	overlay_veneer 0x080c2411
	overlay_veneer 0x080c23c1
	.global Battle_GetObjectTableValueFar
	.global Func_080b5070
	.thumb_func
Battle_GetObjectTableValueFar:
Func_080b5070:
	overlay_veneer 0x080b8531
	.global BattleMotion_ApproachTargetFar
	.thumb_func
BattleMotion_ApproachTargetFar:
	overlay_veneer 0x080b82c5
	overlay_veneer 0x080b81c9
	.global BattleMotion_ApplyVariantMotionFar
	.global Func_080b5088
	.thumb_func
BattleMotion_ApplyVariantMotionFar:
Func_080b5088:
	overlay_veneer 0x080b8229
	overlay_veneer 0x080b6c09
	.global GetBattleObjectSlotFar
	.thumb_func
GetBattleObjectSlotFar:
	overlay_veneer 0x080b7dd1
	overlay_veneer 0x080b63c9
	.global BattleMotion_ProjectPositionFar
	.thumb_func
BattleMotion_ProjectPositionFar:
	overlay_veneer 0x080b7f21
	.global BattleMotion_ProjectScaledPositionFar
	.thumb_func
BattleMotion_ProjectScaledPositionFar:
	overlay_veneer 0x080b845d
	overlay_veneer 0x080b84c1
	overlay_veneer 0x080b6b41
	.global BattleParty_PrepareActiveOwnersFar
	.global Func_080b50c8
	.thumb_func
BattleParty_PrepareActiveOwnersFar:
Func_080b50c8:
	overlay_veneer 0x080b6a61
	.global Camera_ConfigureSceneFar
	.thumb_func
Camera_ConfigureSceneFar:
	overlay_veneer 0x080b8fd5
	.global GetMotionRecordFar
	.thumb_func
GetMotionRecordFar:
	overlay_veneer 0x080b7f71
	.global BattlePres_SetActorModesFar
	.thumb_func
BattlePres_SetActorModesFar:
	overlay_veneer 0x080c10e9
	.global BattleEventRuntime_BeginPhaseFar
	.thumb_func
BattleEventRuntime_BeginPhaseFar:
	.global Func_080b50e8
	.thumb_func
Func_080b50e8:
	overlay_veneer 0x080bd7dd
	.global Graphics_ScaleRgb555ClampedFar
	.thumb_func
Graphics_ScaleRgb555ClampedFar:
	overlay_veneer 0x080c1725
	.global Func_080b50f8
	.thumb_func
Func_080b50f8:
	overlay_veneer 0x080bf5a9
	.global BattleMotion_GetSlotField14Far
	.thumb_func
BattleMotion_GetSlotField14Far:
	overlay_veneer 0x080b6cd1
	overlay_veneer 0x080bf65d
	overlay_veneer 0x080b5369
	.global BattleActor_CommitPlacementFar
	.global Func_080b5118
	.thumb_func
BattleActor_CommitPlacementFar:
Func_080b5118:
	overlay_veneer 0x080b6c91
	overlay_veneer 0x080b7b31
	.global Func_080b5128
	.thumb_func
Func_080b5128:
	overlay_veneer 0x080bb7c1
	overlay_veneer 0x080be0b5
