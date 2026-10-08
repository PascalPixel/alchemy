	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_SpiritHorseSprites
Monster_SpiritHorseSprites:
	.incbin "GRAPHICS/MONSTER/SPIRIT_HORSE.frames.lz"
	.ifeq GAME_TLA
	.byte 0x31, 0x20, 0x83	@ PACKER: left in the buffer by Monster_SeaBeastSprites+0xcc5
	.endif
