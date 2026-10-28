	.section .rodata
	.global BattleEffect_CrystalSheet
BattleEffect_CrystalSheet:
	.incbin "games/tla/src/graphics/battle_effects/crystal_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/crystal.parts.lz"
	.byte 0x13, 0x42, 0x03	@ PACKER: left in the buffer by BattleEffect_TornadoSheet+0x104d
