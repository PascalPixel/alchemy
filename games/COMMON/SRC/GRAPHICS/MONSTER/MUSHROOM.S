	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_MushroomSprites
Monster_MushroomSprites:
	.incbin "GRAPHICS/MONSTER/MUSHROOM.frames.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by Monster_BlueWispSprites at +0xd09 */
	.byte 0x47, 0x7d, 0x5a
	.endif
