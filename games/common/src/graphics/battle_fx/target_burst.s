	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_TargetBurstImage
BattleFx_TargetBurstImage:
	.incbin "games/common/src/graphics/battle_fx/target_burst.bitmap.lz"
	.ifne GAME_TLA
	.byte 0x2f, 0x00	@ PACKER: left in the buffer by BattleFx_SlashSheet+0x83e
	.endif
