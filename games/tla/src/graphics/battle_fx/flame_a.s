	.section .rodata
	.global BattleEffect_FlameSheetA
BattleEffect_FlameSheetA:
	.incbin "games/tla/src/graphics/battle_fx/flame_a.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/flame_a.bitmap.lz"
	.byte 0x2f, 0x18	@ PACKER: left in the buffer by BattleEffect_RainSheet+0x4a2
