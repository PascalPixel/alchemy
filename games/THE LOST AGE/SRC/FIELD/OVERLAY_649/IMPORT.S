@ Import veneers after the OVERLAY_649 overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_649 overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer WaitFramesFar
	overlay_veneer Func_080000d0
	overlay_veneer Func_08000170
	overlay_veneer Sys_FreeFar
	overlay_veneer Func_080001a8
	overlay_veneer VramBlock_LoadCachedFar
	overlay_veneer Resource_FindFreeEntryFar
	overlay_veneer Func_080001e8
	overlay_veneer Resource_GetTableEntryFar
	overlay_veneer Func_08000298
	overlay_veneer Func_080002a8
	overlay_veneer Func_080002d0
	overlay_veneer Func_080382f0
	overlay_veneer Func_08038348
	overlay_veneer Func_080ad218
	overlay_veneer Func_080ad220
	overlay_veneer Battle_WaitMode0Far
	overlay_veneer Object_GetByIdFar
	overlay_veneer Func_080c8268
	overlay_veneer Func_080c83a8
	overlay_veneer Func_080c83b8
	overlay_veneer Func_081a0000
	overlay_veneer Func_081a6000
	overlay_veneer Func_081a6018
	overlay_veneer Func_081a6020
	overlay_veneer Func_081a6028
	overlay_veneer Audio_PlayCue
