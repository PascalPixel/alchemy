	.section .rodata
	.global BattleEffect_DragonCardSheet
BattleEffect_DragonCardSheet:
	.incbin "games/tla/src/graphics/battle_effects/dragon_card.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/dragon_card.bitmap.lz"
	.byte 0x01	@ PACKER: left in the buffer by BattleEffect_VineCardSheet+0x653
