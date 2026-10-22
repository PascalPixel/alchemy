	.section .rodata
	.global BattleEffect_BoltCardSheet
BattleEffect_BoltCardSheet:
	.incbin "games/tla/src/graphics/battle_fx/bolt_card.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/bolt_card.bitmap.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
