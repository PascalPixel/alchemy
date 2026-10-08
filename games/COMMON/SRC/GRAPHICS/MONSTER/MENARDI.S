	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_MenardiSprites
Monster_MenardiSprites:
	.incbin "GRAPHICS/MONSTER/MENARDI.frames.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by MONSTER/SATUROS.o at +0x3da7 */
	.byte 0x52
	.endif
