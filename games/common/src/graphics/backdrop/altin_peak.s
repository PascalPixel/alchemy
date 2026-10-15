	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Battle_AltinPeakBackdrop
Battle_AltinPeakBackdrop:
	.incbin "games/common/src/graphics/backdrop/altin_peak.gbapal"
	.incbin "games/common/src/graphics/backdrop/altin_peak.bitmap.d7"
	.ifeq GAME_TLA
	.byte 0x54, 0x14	@ PACKER: left in the buffer by Battle_GomaCaveBackdrop+0x3b4a
	.else
	.byte 0x13, 0x24	@ PACKER: left in the buffer by Battle_TretInteriorsBackdrop+0x3b4a
	.endif
