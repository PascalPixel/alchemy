	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_BlueArcSheetB
BattleFx_BlueArcSheetB:
	.incbin "games/common/src/graphics/battle_fx/blue_arc_b.gbapal"
	.incbin "games/common/src/graphics/battle_fx/blue_arc_b.bitmap.lz"
	.byte 0xaa, 0x30	@ PACKER: left in the buffer by BattleFx_EmberStreakSheet+0xace
