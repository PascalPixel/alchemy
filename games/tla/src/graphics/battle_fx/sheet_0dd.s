	.section .rodata
	.global Resource_Data0DD
Resource_Data0DD:
	.incbin "games/tla/src/graphics/battle_fx/sheet_0dd_00.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/sheet_0dd.parts.lz"
	.byte 0xbf	@ PACKER: left in the buffer by Resource_Data0DC+0x11cb
