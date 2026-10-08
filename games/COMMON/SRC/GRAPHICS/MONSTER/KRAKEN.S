	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_KrakenSprites
Monster_KrakenSprites:
	.incbin "GRAPHICS/MONSTER/KRAKEN.frames.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by MONSTER/FIRE_DRAGON.o at +0x40c5 */
	.byte 0xbd, 0xca, 0x95
	.endif
