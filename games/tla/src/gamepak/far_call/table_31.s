.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global ObjectTable_SnapshotFar
	.thumb_func
ObjectTable_SnapshotFar:
	overlay_veneer ObjectTable_Snapshot
	.global Func_080c8630
	.thumb_func
Func_080c8630:
	overlay_veneer Func_080cae5c
	.global Func_080c8638
	.thumb_func
Func_080c8638:
	overlay_veneer Func_080d46a4
	.global Func_080c8640
	.thumb_func
Func_080c8640:
	overlay_veneer Func_080d4714
