	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_FlareSheet
BattleFx_FlareSheet:
	.incbin "GRAPHICS/BATTLE_FX/FLARE_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/FLARE.parts.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by earlier streams, which differ by edition */
	.ifdef TBS_EDITION_ES
	.byte 0x10, 0xb6
	.else
	.ifdef TBS_EDITION_JA
	.byte 0x80, 0x47
	.else
	.ifdef TBS_EDITION_EN
	.byte 0x90, 0x38
	.else
	.ifdef TBS_EDITION_DE
	.byte 0xc5, 0x01
	.else
	.ifdef TBS_EDITION_IT
	.byte 0xd1, 0x0a
	.else
	.byte 0x0a, 0x16
	.endif
	.endif
	.endif
	.endif
	.endif
	.endif
