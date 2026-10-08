	.section .rodata
	.global BattleFx_RedCrescentSheetA
BattleFx_RedCrescentSheetA:
	.incbin "GRAPHICS/BATTLE_FX/RED_CRESCENT_A_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/RED_CRESCENT_A.parts.lz"
	.byte 0x34	@ PACKER: left in the buffer by BattleFx_CounterRevealSheet+0x49b
