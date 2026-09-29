@ Far-call stub table heading the palette effect code at 0x080f2000: fixed 8-byte
@ veneers, each loading its target into r4 and branching, which the calling
@ convention permits. No compiler emits them; the shared overlay veneer macro
@ builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.global FarCall_PaletteTable
FarCall_PaletteTable:
	.global Func_080f2000
	.thumb_func
Func_080f2000:
	overlay_veneer Func_080f26ec
	overlay_veneer Graphics_NoOpColor
	overlay_veneer Func_080f2b6c
	.global Title_ShowSplashScreenFar
	.thumb_func
Title_ShowSplashScreenFar:
	overlay_veneer Title_ShowSplashScreen
	.global Func_080f2020
	.thumb_func
Func_080f2020:
	overlay_veneer Unnamed_080f2d54