	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Battle_VinasuChojoStormBackdrop
Battle_VinasuChojoStormBackdrop:
	.incbin "graphics/backdrop/vinasu_chojo_storm.gbapal"
	.incbin "graphics/backdrop/vinasu_chojo_storm.bitmap.d7"
	.ifeq GAME_TLA
	.byte 0x92, 0x11	@ PACKER: left in the buffer by Battle_KorosseoBackdrop+0x3d3a
	.endif
