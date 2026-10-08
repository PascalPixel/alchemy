	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_MushroomSprites
Monster_MushroomSprites:
	.incbin "GRAPHICS/MONSTER/MUSHROOM.frames.lz"
	.ifeq GAME_TLA
	.byte 0x47, 0x7d, 0x5a	@ PACKER: left in the buffer by Monster_BlueWispSprites+0xd09
	.endif
