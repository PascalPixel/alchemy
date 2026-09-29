@ BattleFx_TwelveModeImage: 4 frames of 18x48, stacked, with no palette of their own, drawn with
@ BattleFx_BlueBeamSheet's; its pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_TwelveModeImage
BattleFx_TwelveModeImage:
	.incbin "GRAPHICS/BATTLE_FX/TWELVE_MODE.bitmap.lz"
