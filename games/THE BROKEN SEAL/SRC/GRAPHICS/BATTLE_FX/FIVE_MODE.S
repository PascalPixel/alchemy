@ BattleFx_FiveModeImage: 18 frames of 20x40, stacked, with no palette of their own, drawn with
@ BattleFx_EmberStreakSheet's; its pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_FiveModeImage
BattleFx_FiveModeImage:
	.incbin "GRAPHICS/BATTLE_FX/FIVE_MODE.bitmap.lz"
