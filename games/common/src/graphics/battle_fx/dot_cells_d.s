	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_ParticleSpritesD
BattleFx_ParticleSpritesD:
	.incbin "games/common/src/graphics/battle_fx/dot_cells_d.parts.lz"
	.ifne GAME_TLA
	.byte 0x0c	@ PACKER: left in the buffer by BattleFx_ParticleSpritesC+0x417
	.endif
