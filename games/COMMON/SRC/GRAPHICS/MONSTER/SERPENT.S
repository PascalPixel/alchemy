	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_SerpentSprites
Monster_SerpentSprites:
	.incbin "GRAPHICS/MONSTER/SERPENT.frames.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by Monster_SirenSprites at +0x2712 */
	.byte 0x0c, 0xb5
	.endif
