	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_TileAnimationMask
BattleFx_TileAnimationMask:
	.incbin "GRAPHICS/BATTLE_FX/TILE_MASK_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/TILE_MASK.parts.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by earlier streams, which differ by edition */
	.ifdef TBS_EDITION_EN
	.byte 0x32, 0xb9, 0x81
	.else
	.ifdef TBS_EDITION_JA
	.byte 0xad, 0x1e, 0xd8
	.else
	.ifdef TBS_EDITION_ES
	.byte 0xc6, 0xaf, 0xea
	.else
	.ifdef TBS_EDITION_IT
	.byte 0xc7, 0x7f, 0xea
	.else
	.ifdef TBS_EDITION_FR
	.byte 0xea, 0x01, 0x59
	.else
	.byte 0x1b, 0x90, 0x2f
	.endif
	.endif
	.endif
	.endif
	.endif
	.endif
