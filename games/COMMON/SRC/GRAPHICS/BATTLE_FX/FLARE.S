	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_FlareSheet
BattleFx_FlareSheet:
	.incbin "GRAPHICS/BATTLE_FX/FLARE_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/FLARE.parts.lz"
	.ifeq GAME_TLA
	.ifdef TBS_EDITION_ES
	.byte 0x10, 0xb6	@ PACKER: left in the buffer by earlier streams, which differ by edition
	.else
	.ifdef TBS_EDITION_JA
	.byte 0x80, 0x47	@ PACKER: left in the buffer by earlier streams, which differ by edition
	.else
	.ifdef TBS_EDITION_EN
	.byte 0x90, 0x38	@ PACKER: left in the buffer by earlier streams, which differ by edition
	.else
	.ifdef TBS_EDITION_DE
	.byte 0xc5, 0x01	@ PACKER: left in the buffer by earlier streams, which differ by edition
	.else
	.ifdef TBS_EDITION_IT
	.byte 0xd1, 0x0a	@ PACKER: left in the buffer by earlier streams, which differ by edition
	.else
	.byte 0x0a, 0x16	@ PACKER: left in the buffer by earlier streams, which differ by edition
	.endif
	.endif
	.endif
	.endif
	.endif
	.endif
