	.section .rodata
	.global BattleEffect_CurseSealSheet
BattleEffect_CurseSealSheet:
	.incbin "games/tla/src/graphics/battle_effects/curse_seal.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/curse_seal.bitmap.lz"
	.byte 0xe1, 0xef	@ PACKER: left in the buffer by BattleEffect_BoltSheet+0x1b5a
