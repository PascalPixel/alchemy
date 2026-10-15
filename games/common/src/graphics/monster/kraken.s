	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Monster_KrakenSprites
Monster_KrakenSprites:
	.incbin "games/common/src/graphics/monster/kraken.frames.lz"
	.byte 0xbd, 0xca, 0x95	@ PACKER: left in the buffer by Monster_FireDragonSprites+0x40c5
