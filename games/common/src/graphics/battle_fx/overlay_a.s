	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_OverlayImageA
BattleFx_OverlayImageA:
	.incbin "games/common/src/graphics/battle_fx/overlay_a.parts.lz"
	.ifne GAME_TLA
	.byte 0x88	@ PACKER: left in the buffer by BattleFx_FireBurstSheet+0x43b
	.endif
