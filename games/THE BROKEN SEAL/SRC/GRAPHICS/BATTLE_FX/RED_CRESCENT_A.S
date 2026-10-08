	.section .rodata
	.global BattleFx_RedCrescentSheetA
BattleFx_RedCrescentSheetA:
	.incbin "GRAPHICS/BATTLE_FX/RED_CRESCENT_A_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/RED_CRESCENT_A.parts.lz"
	/* PACKER: left in the packer's buffer by BattleFx_CounterRevealSheet at +0x49b */
	.byte 0x34
