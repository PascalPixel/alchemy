	.section .rodata
	.global BattleFx_VioletMoteSheet
BattleFx_VioletMoteSheet:
	.incbin "GRAPHICS/BATTLE_FX/MOTE.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/MOTE.bitmap.lz"
	/* PACKER: left in the packer's buffer by BattleFx_OverlayImageA at +0x111 */
	.byte 0x28, 0x41, 0x42
