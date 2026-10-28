	.section .rodata
	.global BattleEffect_PaletteRampImage
BattleEffect_PaletteRampImage:
	.incbin "games/tla/src/graphics/battle_effects/spark_cells.parts.lz"
	.byte 0x01, 0x0d	@ PACKER: left in the buffer by BattleEffect_DaggerSheet+0x196
