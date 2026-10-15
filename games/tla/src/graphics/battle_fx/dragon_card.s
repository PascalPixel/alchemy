	.section .rodata
	.global BattleFx_DragonCardSheet
BattleFx_DragonCardSheet:
	.incbin "games/tla/src/graphics/battle_fx/dragon_card.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/dragon_card.bitmap.lz"
	.byte 0x01	@ PACKER: left in the buffer by BattleFx_VineCardSheet+0x653
