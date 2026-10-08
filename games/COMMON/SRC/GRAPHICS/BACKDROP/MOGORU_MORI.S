	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global Battle_MogoruMoriBackdrop
Battle_MogoruMoriBackdrop:
	.incbin "GRAPHICS/BACKDROP/MOGORU_MORI.gbapal"
	.incbin "GRAPHICS/BACKDROP/MOGORU_MORI.bitmap.d7"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by Battle_BabiIriguchiBackdrop at +0x4326 */
	.byte 0x50, 0x00
	.endif
