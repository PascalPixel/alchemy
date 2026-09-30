@ Far-call stub table at 0x08118118: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global BattleActor_CommitPlacementFar
	.thumb_func
BattleActor_CommitPlacementFar:
	overlay_veneer BattleActor_CommitPlacement
	.global ReleaseBattleObjectRecordsFar
	.thumb_func
ReleaseBattleObjectRecordsFar:
	overlay_veneer ReleaseBattleObjectRecords
	.global Func_08118128
	.thumb_func
Func_08118128:
	overlay_veneer Func_08120060
	.global Func_08118130
	.thumb_func
Func_08118130:
	overlay_veneer Func_08123574
	.global Func_08118138
	.thumb_func
Func_08118138:
	overlay_veneer Func_0811a0b0
	.global Func_08118140
	.thumb_func
Func_08118140:
	overlay_veneer Func_0811a39c
	.global Func_08118148
	.thumb_func
Func_08118148:
	overlay_veneer Func_0811b75c
	.global Func_08118150
	.thumb_func
Func_08118150:
	overlay_veneer Func_08119734
	.global Func_08118158
	.thumb_func
Func_08118158:
	overlay_veneer Func_08118f6c
