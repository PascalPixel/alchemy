	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Monster_SpiritHorseSprites
Monster_SpiritHorseSprites:
	.incbin "games/common/src/graphics/monster/spirit_horse.frames.lz"
	.ifeq GAME_TLA
	.byte 0x31, 0x20, 0x83	@ PACKER: left in the buffer by Monster_SeaBeastSprites+0xcc5
	.else
	.byte 0x31, 0x20, 0x83	@ PACKER: left in the buffer by Monster_SeaBeastSprites+0xcc5
	.endif
