	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_MercuryDjinnSheet
BattleFx_MercuryDjinnSheet:
	.incbin "graphics/battle_fx/mercury_djinn.gbapal"
	.incbin "graphics/battle_fx/mercury_djinn.bitmap.lz"
	.ifeq GAME_TLA
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.endif
