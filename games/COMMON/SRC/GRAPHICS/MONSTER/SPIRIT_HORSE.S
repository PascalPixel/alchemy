	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_SpiritHorseSprites
Monster_SpiritHorseSprites:
	.incbin "GRAPHICS/MONSTER/SPIRIT_HORSE.frames.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by Monster_SeaBeastSprites at +0xcc5 */
	.byte 0x31, 0x20, 0x83
	.endif
