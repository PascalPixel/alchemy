	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_FiveModeImage
BattleFx_FiveModeImage:
	.incbin "games/common/src/graphics/battle_fx/five_mode.bitmap.lz"
	.ifne GAME_TLA
	.byte 0x96	@ PACKER: left in the buffer by BattleFx_IceTileSheet+0xfc7
	.endif
