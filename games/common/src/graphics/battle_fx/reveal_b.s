	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_RevealMaskB
BattleFx_RevealMaskB:
	.incbin "games/common/src/graphics/battle_fx/reveal_b.gbapal"
	.incbin "games/common/src/graphics/battle_fx/reveal_b.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0x00, 0x7b, 0x09	@ PACKER: left in the buffer by BattleFx_RevealMaskA+0x2e5
	.else
	.byte 0x00, 0x7b, 0x09	@ PACKER: left in the buffer by BattleFx_RevealMaskA+0x2e5
	.endif
