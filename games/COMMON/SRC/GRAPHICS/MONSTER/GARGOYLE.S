	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_GargoyleSprites
Monster_GargoyleSprites:
	.incbin "GRAPHICS/MONSTER/GARGOYLE.frames.lz"
	.ifeq GAME_TLA
	.byte 0xb1, 0x5b	@ PACKER: left in the buffer by Monster_OrcLordSprites+0x3092
	.endif
