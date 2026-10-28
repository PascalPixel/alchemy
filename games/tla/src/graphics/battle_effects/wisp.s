	.section .rodata
	.global BattleEffect_WispSheet
BattleEffect_WispSheet:
	.incbin "games/tla/src/graphics/battle_effects/wisp.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/wisp.bitmap.lz"
	.byte 0x2e, 0x06	@ PACKER: left in the buffer by BattleEffect_SmokeSheet+0x7ae
