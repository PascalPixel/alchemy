@ BattleFx_MemberBurstImage: 4 frames of 20x32, stacked, with no palette of their own, drawn with
@ BattleFx_PinkBurstSheet's; its pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_MemberBurstImage
BattleFx_MemberBurstImage:
	.incbin "GRAPHICS/BATTLE_FX/MEMBER_BURST.bitmap.lz"
