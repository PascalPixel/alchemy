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
	.irp target, 0x080b56e1, 0x080bffb9
	overlay_veneer \target
	.endr
	.global BattleActor_SpawnObjectsForListFar
	.global Func_080b5010
	.thumb_func
BattleActor_SpawnObjectsForListFar:
Func_080b5010:
	.irp target, 0x080b7b6d, 0x080c0e39, 0x080c0e71
	overlay_veneer \target
	.endr
	.global Func_080b5028
	.thumb_func
Func_080b5028:
	.irp target, 0x080c0ced, 0x080c0df5, 0x080c0775, 0x080c08ed
	overlay_veneer \target
	.endr
	.irp target, 0x080c0701, 0x080c2455
	overlay_veneer \target
	.endr
	.global Func_080b5058
	.thumb_func
Func_080b5058:
	.irp target, 0x080c2435, 0x080c2411, 0x080c23c1
	overlay_veneer \target
	.endr
	.global Battle_GetObjectTableValueFar
	.global Func_080b5070
	.thumb_func
Battle_GetObjectTableValueFar:
Func_080b5070:
	.irp target, 0x080b8531, 0x080b82c5, 0x080b81c9
	overlay_veneer \target
	.endr
	.global BattleMotion_ApplyVariantMotionFar
	.global Func_080b5088
	.thumb_func
BattleMotion_ApplyVariantMotionFar:
Func_080b5088:
	.irp target, 0x080b8229, 0x080b6c09, 0x080b7dd1, 0x080b63c9
	overlay_veneer \target
	.endr
	.irp target, 0x080b7f21, 0x080b845d, 0x080b84c1, 0x080b6b41
	overlay_veneer \target
	.endr
	.global BattleParty_PrepareActiveOwnersFar
	.global Func_080b50c8
	.thumb_func
BattleParty_PrepareActiveOwnersFar:
Func_080b50c8:
	.irp target, 0x080b6a61, 0x080b8fd5, 0x080b7f71, 0x080c10e9
	overlay_veneer \target
	.endr
	.irp target, 0x080bd7dd, 0x080c1725, 0x080bf5a9, 0x080b6cd1
	overlay_veneer \target
	.endr
	.irp target, 0x080bf65d, 0x080b5369
	overlay_veneer \target
	.endr
	.global BattleActor_CommitPlacementFar
	.global Func_080b5118
	.thumb_func
BattleActor_CommitPlacementFar:
Func_080b5118:
	.irp target, 0x080b6c91, 0x080b7b31
	overlay_veneer \target
	.endr
	.global Func_080b5128
	.thumb_func
Func_080b5128:
	.irp target, 0x080bb7c1, 0x080be0b5
	overlay_veneer \target
	.endr
