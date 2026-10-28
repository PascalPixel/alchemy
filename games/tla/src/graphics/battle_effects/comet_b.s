	.include "games/tla/include/ed_asm.h"
	.section .rodata
	.global BattleEffect_CometSheetB
BattleEffect_CometSheetB:
	.incbin "games/tla/src/graphics/battle_effects/comet_b.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/comet_b.bitmap.lz"
	.ifdef TLA_EDITION_JA
	.byte 0xaf	@ PACKER: left in the buffer by BattleEffect_ThornSheet+0x2fb
	.else
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.endif
