	.section .rodata
	.global BattleEffect_VortexSheet
BattleEffect_VortexSheet:
	.incbin "games/tla/src/graphics/battle_fx/vortex.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/vortex.bitmap.lz"
	.byte 0xdd, 0x20	@ PACKER: left in the buffer by BattleEffect_WindStreakSheet+0x912
