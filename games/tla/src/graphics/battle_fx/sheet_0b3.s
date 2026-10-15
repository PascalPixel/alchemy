	.section .rodata
@ Eighteen cels of a fluttering silhouette in their own 64-colour palette, at
@ the sizes and offsets of Func_0815b764's cel tables
	.global Resource_Data0B3
Resource_Data0B3:
	.incbin "games/tla/src/graphics/battle_fx/sheet_0b3_00.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/sheet_0b3.parts.lz"
	.byte 0x03, 0x71	@ PACKER: left in the buffer by Resource_Data0B2+0x196
