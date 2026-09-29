@ BattleFx_TargetBurstImage: 6 frames of 24x48, stacked, with no palette of their own, drawn with
@ BattleFx_PinkBurstSheet's; its pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_TargetBurstImage
BattleFx_TargetBurstImage:
	.incbin "GRAPHICS/BATTLE_FX/TARGET_BURST.bitmap.lz"
