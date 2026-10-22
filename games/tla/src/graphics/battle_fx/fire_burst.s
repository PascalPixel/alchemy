	.section .rodata
	.global BattleEffect_FireBurstSheet
BattleEffect_FireBurstSheet:
	.incbin "games/tla/src/graphics/battle_fx/fire_burst.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/fire_burst.bitmap.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
