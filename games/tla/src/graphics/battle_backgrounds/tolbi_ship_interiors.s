	.section .rodata
	.global Battle_TolbiShipInteriorsBackdrop
Battle_TolbiShipInteriorsBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/tolbi_ship_interiors.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/tolbi_ship_interiors.bitmap.d7"
	.byte 0x39, 0x66	@ PACKER: left in the buffer by Battle_MogallForestBackdrop+0x4076
