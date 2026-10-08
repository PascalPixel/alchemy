	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_BatSprites
Monster_BatSprites:
	.incbin "GRAPHICS/MONSTER/BAT.frames.lz"
	.ifeq GAME_TLA
	.byte 0xf2, 0x2e, 0xa3	@ PACKER: left in the buffer by Monster_FairySprites+0x7dd
	.endif
