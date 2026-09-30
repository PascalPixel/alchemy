@ Far-call stub table at 0x08118088: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
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
	overlay_veneer GetBattleObjectSlot
	.global Func_081180a0
	.thumb_func
Func_081180a0:
	overlay_veneer Func_081197f0
	.global Func_081180a8
	.thumb_func
Func_081180a8:
	overlay_veneer Func_0811bd50
	.global Func_081180b0
	.thumb_func
Func_081180b0:
	overlay_veneer Func_0811c2b4
	.global Func_081180b8
	.thumb_func
Func_081180b8:
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
	.global BattlePres_SetActorModesFar
	.thumb_func
BattlePres_SetActorModesFar:
	overlay_veneer BattlePres_SetActorModes
	.global Func_081180e8
	.thumb_func
Func_081180e8:
	overlay_veneer Func_08122c4c
	.global ColorBuffer_ScaleFar
	.thumb_func
ColorBuffer_ScaleFar:
	overlay_veneer ColorBuffer_Scale
	.global BattlePlacement_UpdateTimedEntriesFar
	.thumb_func
BattlePlacement_UpdateTimedEntriesFar:
	overlay_veneer BattlePlacement_UpdateTimedEntries
	.global Func_08118100
	.thumb_func
Func_08118100:
	overlay_veneer Func_0811a484
	.global BattlePlacement_UpdateTimedEntriesTwentyTimesFar
	.thumb_func
BattlePlacement_UpdateTimedEntriesTwentyTimesFar:
	overlay_veneer BattlePlacement_UpdateTimedEntriesTwentyTimes
	.global DebugParty_LoadPresetFar
	.thumb_func
DebugParty_LoadPresetFar:
	overlay_veneer DebugParty_LoadPreset
