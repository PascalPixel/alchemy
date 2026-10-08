	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.section .rodata
	.global BattleFx_RevealMaskB
BattleFx_RevealMaskB:
	.incbin "GRAPHICS/BATTLE_FX/REVEAL_B.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/REVEAL_B.bitmap.lz"
	.ifeq GAME_TLA
	/* PACKER: left in the packer's buffer by BattleFx_RevealMaskA at +0x2e5 */
	.byte 0x00, 0x7b, 0x09
	.endif
