@ BattleFx_FlareImage: 4 frames of 12x24, stacked, with no palette of their own, drawn with
@ the caller's; shown with BattleFx_EmberStreakSheet's. Its pixels as a packed
@ 8-bit bitmap.
	.section .rodata
	.global BattleFx_FlareImage
BattleFx_FlareImage:
	.incbin "GRAPHICS/BATTLE_FX/FLARE_CELS.bitmap.lz"
