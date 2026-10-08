	.section .rodata
	.global BattleFx_RevealMaskF
BattleFx_RevealMaskF:
	.incbin "GRAPHICS/BATTLE_FX/REVEAL_F.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/REVEAL_F.bitmap.lz"
	/* PACKER: left in the packer's buffer by BattleFx_RevealMaskE at +0x3e9 */
	.byte 0x30, 0xe1, 0x08
