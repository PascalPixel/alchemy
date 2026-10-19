	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_GoldShellSheet
BattleFx_GoldShellSheet:
	.incbin "games/tla/src/graphics/battle_fx/gold_shell_00.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/gold_shell.parts.lz"
	.ifne GAME_TLA
	.byte 0x15, 0xb2	@ PACKER: left in the buffer by BattleFx_IceBlockSheet+0x1016
	.endif
