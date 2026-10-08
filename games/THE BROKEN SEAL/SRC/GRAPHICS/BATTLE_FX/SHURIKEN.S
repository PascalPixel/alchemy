	.section .rodata
	.global BattleFx_ShurikenSheet
BattleFx_ShurikenSheet:
	.incbin "GRAPHICS/BATTLE_FX/SHURIKEN.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/SHURIKEN.bitmap.lz"
	.byte 0x47, 0x8d, 0x13	@ PACKER: left in the buffer by BattleFx_VioletLightningSheetA+0x395
