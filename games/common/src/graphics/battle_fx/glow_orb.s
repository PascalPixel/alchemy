	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_GlowOrbSheet
BattleFx_GlowOrbSheet:
	.incbin "games/common/src/graphics/battle_fx/glow_orb.gbapal"
	.incbin "games/common/src/graphics/battle_fx/glow_orb.bitmap.lz"
	.byte 0xe0	@ PACKER: left in the buffer by BattleFx_EmberStreakSheet+0xb3b
