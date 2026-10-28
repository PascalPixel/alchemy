	.section .rodata
	.global Battle_OrbAltarBackdrop
Battle_OrbAltarBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/orb_altar.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/orb_altar.bitmap.d7"
	.byte 0x11, 0x5d	@ PACKER: left in the buffer by Battle_IceHallBackdrop+0x3bf6
