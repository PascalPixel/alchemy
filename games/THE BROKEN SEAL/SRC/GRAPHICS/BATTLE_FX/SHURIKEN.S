	.section .rodata
	.global BattleFx_ShurikenSheet
BattleFx_ShurikenSheet:
	.incbin "GRAPHICS/BATTLE_FX/SHURIKEN.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/SHURIKEN.bitmap.lz"
	/* PACKER: left in the packer's buffer by BattleFx_VioletLightningSheetA at +0x395 */
	.byte 0x47, 0x8d, 0x13
