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
	overlay_veneer BattleParty_ListActorIds
	overlay_veneer GetBattleObjectSlot
	overlay_veneer Func_081197f0
	overlay_veneer Func_0811bd50
	overlay_veneer Func_0811c2b4
	overlay_veneer Func_0811c314
	overlay_veneer Func_0811a188
	overlay_veneer Func_0811a038
	overlay_veneer Camera_ConfigureScene
	overlay_veneer GetMotionRecord
	overlay_veneer BattlePres_SetActorModes
	overlay_veneer Func_08122c4c
	overlay_veneer ColorBuffer_Scale
	overlay_veneer BattlePlacement_UpdateTimedEntries
	overlay_veneer Func_0811a484
	.global BattlePlacement_UpdateTimedEntriesTwentyTimesFar
	.thumb_func
BattlePlacement_UpdateTimedEntriesTwentyTimesFar:
	overlay_veneer BattlePlacement_UpdateTimedEntriesTwentyTimes
	.global DebugParty_LoadPresetFar
	.thumb_func
DebugParty_LoadPresetFar:
	overlay_veneer DebugParty_LoadPreset
