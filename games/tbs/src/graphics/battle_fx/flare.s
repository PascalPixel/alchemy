	.include "games/tbs/include/game/ed_asm.h"
	.section .rodata
	.global BattleEffect_FlareSheet
BattleEffect_FlareSheet:
	.incbin "games/tbs/src/graphics/battle_fx/flare_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/flare.parts.lz"
	.ifdef TBS_EDITION_ES
	.byte 0x10, 0xb6	@ PACKER: left in the buffer by LuckyDice_Sprites+0x1d86
	.else
	.ifdef TBS_EDITION_JA
	.byte 0x80, 0x47	@ PACKER: left in the buffer by LuckyDice_Sprites+0x1d86
	.else
	.ifdef TBS_EDITION_EN
	.byte 0x90, 0x38	@ PACKER: left in the buffer by LuckyDice_Sprites+0x1d86
	.else
	.ifdef TBS_EDITION_DE
	.byte 0xc5, 0x01	@ PACKER: left in the buffer by LuckyDice_Sprites+0x1d86
	.else
	.ifdef TBS_EDITION_IT
	.byte 0xd1, 0x0a	@ PACKER: left in the buffer by LuckyDice_Sprites+0x1d86
	.else
	.byte 0x0a, 0x16	@ PACKER: left in the buffer by LuckyDice_Sprites+0x1d86
	.endif
	.endif
	.endif
	.endif
	.endif
