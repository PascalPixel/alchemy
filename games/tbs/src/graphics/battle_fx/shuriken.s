	.section .rodata
	.global BattleFx_ShurikenSheet
BattleFx_ShurikenSheet:
	.incbin "graphics/battle_fx/shuriken.gbapal"
	.incbin "graphics/battle_fx/shuriken.bitmap.lz"
	.byte 0x47, 0x8d, 0x13	@ PACKER: left in the buffer by BattleFx_VioletLightningSheetA+0x395
