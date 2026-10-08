	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Battle_VinasuChojoBackdrop
Battle_VinasuChojoBackdrop:
	.incbin "graphics/backdrop/vinasu_chojo.gbapal"
	.incbin "graphics/backdrop/vinasu_chojo.bitmap.d7"
	.ifeq GAME_TLA
	.byte 0x34, 0x54	@ PACKER: left in the buffer by Battle_VinasuChojoStormBackdrop+0x35ee
	.endif
