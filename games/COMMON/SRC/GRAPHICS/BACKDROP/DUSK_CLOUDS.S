	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Battle_DuskCloudsBackdrop
Battle_DuskCloudsBackdrop:
	.incbin "GRAPHICS/BACKDROP/DUSK_CLOUDS.gbapal"
	.incbin "GRAPHICS/BACKDROP/DUSK_CLOUDS.bitmap.d7"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by Battle_ForestBackdrop at +0x39aa */
	.byte 0x33, 0x00
	.endif
