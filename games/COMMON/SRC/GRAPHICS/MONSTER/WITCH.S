	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_WitchSprites
Monster_WitchSprites:
	.incbin "GRAPHICS/MONSTER/WITCH.frames.lz"
	.ifeq GAME_TLA
	.byte 0xd8, 0x61	@ PACKER: left in the buffer by Monster_TrollSprites+0x1702
	.endif
