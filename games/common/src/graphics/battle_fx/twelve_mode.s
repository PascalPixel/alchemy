	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_TwelveModeImage
BattleFx_TwelveModeImage:
	.incbin "games/common/src/graphics/battle_fx/twelve_mode.bitmap.lz"
	.ifne GAME_TLA
	.byte 0xa5	@ PACKER: left in the buffer by BattleFx_LightningBoltSheet+0x42f
	.endif
