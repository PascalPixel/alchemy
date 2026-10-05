.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.global FarCall_BlankRunTable
FarCall_BlankRunTable:
	.global Func_080f4000
	.thumb_func
Func_080f4000:
	overlay_veneer Runtime_BlankDisplayAndRun