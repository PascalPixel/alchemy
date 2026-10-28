	.section .rodata
	.global BattleEffect_RedCrescentSheetA
BattleEffect_RedCrescentSheetA:
	.incbin "games/tla/src/graphics/battle_effects/red_crescent_a_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/red_crescent_a.parts.lz"
	.byte 0x34	@ PACKER: left in the buffer by BattleEffect_CounterRevealSheet+0x49b
