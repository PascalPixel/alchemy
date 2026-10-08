	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_BatSprites
Monster_BatSprites:
	.incbin "GRAPHICS/MONSTER/BAT.frames.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by Monster_FairySprites at +0x7dd */
	.byte 0xf2, 0x2e, 0xa3
	.endif
