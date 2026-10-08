	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Monster_SmallLizardSprites
Monster_SmallLizardSprites:
	.incbin "GRAPHICS/MONSTER/SMALL_LIZARD.frames.lz"
	.ifeq GAME_TLA
	.byte 0x70	@ PACKER: left in the buffer by Monster_FairySprites+0xc6f
	.endif
