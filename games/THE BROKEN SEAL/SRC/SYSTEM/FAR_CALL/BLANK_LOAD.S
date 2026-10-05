.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.global FarCall_BlankLoadTable
FarCall_BlankLoadTable:
	.global Func_080f6000
	.thumb_func
Func_080f6000:
	overlay_veneer Runtime_BlankDisplayLoadValueAndRun