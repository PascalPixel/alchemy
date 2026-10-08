	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_RevealMaskB
BattleFx_RevealMaskB:
	.incbin "GRAPHICS/BATTLE_FX/REVEAL_B.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/REVEAL_B.bitmap.lz"
	.ifeq GAME_TLA
	.byte 0x00, 0x7b, 0x09	@ PACKER: left in the buffer by BattleFx_RevealMaskA+0x2e5
	.endif
