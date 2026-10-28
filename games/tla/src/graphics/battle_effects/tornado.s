	.section .rodata
	.global BattleEffect_TornadoSheet
BattleEffect_TornadoSheet:
	.incbin "games/tla/src/graphics/battle_effects/tornado_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/tornado.parts.lz"
	.byte 0x7f, 0x02	@ PACKER: left in the buffer by BattleEffect_FireBurstSheet+0x1b32
