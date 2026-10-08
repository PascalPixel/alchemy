	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Battle_DuskCloudsBackdrop
Battle_DuskCloudsBackdrop:
	.incbin "GRAPHICS/BACKDROP/DUSK_CLOUDS.gbapal"
	.incbin "GRAPHICS/BACKDROP/DUSK_CLOUDS.bitmap.d7"
	.ifeq GAME_TLA
	.byte 0x33, 0x00	@ PACKER: left in the buffer by Battle_ForestBackdrop+0x39aa
	.endif
