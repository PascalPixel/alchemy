	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_LightningPillarSheet
BattleFx_LightningPillarSheet:
	.incbin "games/common/src/graphics/battle_fx/lightning_pillar.gbapal"
	.incbin "games/common/src/graphics/battle_fx/lightning_pillar.bitmap.lz"
	.ifeq GAME_TLA
	.balign 4, 0xff	@ PACKER: 0xff fill, not an earlier stream's bytes
	.endif
