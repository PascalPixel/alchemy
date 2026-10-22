	.section .rodata
	.global BattleEffect_ShurikenSheet
BattleEffect_ShurikenSheet:
	.incbin "games/tla/src/graphics/battle_fx/shuriken.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/shuriken.bitmap.lz"
	.byte 0x47, 0x8d, 0x13	@ PACKER: left in the buffer by BattleEffect_VioletLightningSheetA+0x395
