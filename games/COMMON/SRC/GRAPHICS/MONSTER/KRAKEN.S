	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_KrakenSprites
Monster_KrakenSprites:
	.incbin "GRAPHICS/MONSTER/KRAKEN.frames.lz"
	.ifeq GAME_TLA
	.byte 0xbd, 0xca, 0x95	@ PACKER: left in the buffer by Monster_FireDragonSprites+0x40c5
	.endif
