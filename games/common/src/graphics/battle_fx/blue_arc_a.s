	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_BlueArcSheetA
BattleFx_BlueArcSheetA:
	.incbin "games/common/src/graphics/battle_fx/blue_arc_a.gbapal"
	.incbin "games/common/src/graphics/battle_fx/blue_arc_a.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0x15, 0xc0, 0x04	@ PACKER: left in the buffer by BattleFx_EmberStreakSheet+0x7d5
	.endif
