	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global BattleFx_TileAnimationMask
BattleFx_TileAnimationMask:
	.incbin "games/common/src/graphics/battle_fx/tile_mask_00.gbapal"
	.incbin "games/common/src/graphics/battle_fx/tile_mask.parts.lz"
	.ifeq GAME_TLA
	.ifdef TBS_EDITION_EN
	.byte 0x32, 0xb9, 0x81	@ PACKER: left in the buffer by LuckyDice_Sprites+0x845
	.else
	.ifdef TBS_EDITION_JA
	.byte 0xad, 0x1e, 0xd8	@ PACKER: left in the buffer by LuckyDice_Sprites+0x845
	.else
	.ifdef TBS_EDITION_ES
	.byte 0xc6, 0xaf, 0xea	@ PACKER: left in the buffer by LuckyDice_Sprites+0x845
	.else
	.ifdef TBS_EDITION_IT
	.byte 0xc7, 0x7f, 0xea	@ PACKER: left in the buffer by LuckyDice_Sprites+0x845
	.else
	.ifdef TBS_EDITION_FR
	.byte 0xea, 0x01, 0x59	@ PACKER: left in the buffer by LuckyDice_Sprites+0x845
	.else
	.byte 0x1b, 0x90, 0x2f	@ PACKER: left in the buffer by LuckyDice_Sprites+0x845
	.endif
	.endif
	.endif
	.endif
	.endif
	.endif
