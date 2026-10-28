	.section .rodata
	.global BattleEffect_GreyCrescentSheet
BattleEffect_GreyCrescentSheet:
	.incbin "games/tla/src/graphics/battle_effects/grey_crescent.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/grey_crescent.bitmap.lz"
	.byte 0x06	@ PACKER: left in the buffer by BattleEffect_CyanSparkSheet+0xd33
