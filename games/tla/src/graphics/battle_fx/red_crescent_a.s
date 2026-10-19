	.section .rodata
	.global BattleFx_RedCrescentSheetA
BattleFx_RedCrescentSheetA:
	.incbin "games/tla/src/graphics/battle_fx/red_crescent_a_00.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/red_crescent_a.parts.lz"
	.byte 0x34	@ PACKER: left in the buffer by BattleFx_CounterRevealSheet+0x49b
