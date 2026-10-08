	.section .rodata
	.global BattleFx_RevealMaskA
BattleFx_RevealMaskA:
	.incbin "GRAPHICS/BATTLE_FX/REVEAL_A.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/REVEAL_A.bitmap.lz"
	/* PACKER: left in the packer's buffer by BattleFx_CyanSparkSheet at +0x6c2 */
	.byte 0x0a, 0x21
