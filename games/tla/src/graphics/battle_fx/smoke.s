	.section .rodata
	.global BattleEffect_SmokeSheet
BattleEffect_SmokeSheet:
	.incbin "games/tla/src/graphics/battle_fx/smoke.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/smoke.bitmap.lz"
	.byte 0xc1	@ PACKER: left in the buffer by BattleEffect_DaggerSheet+0x88b
