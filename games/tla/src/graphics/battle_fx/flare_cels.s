	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_FlareImage
BattleFx_FlareImage:
	.incbin "games/tla/src/graphics/battle_fx/flare_cels.bitmap.lz"
	.ifne GAME_TLA
	.byte 0x05	@ PACKER: left in the buffer by BattleFx_FlareSheet+0x277
	.endif
