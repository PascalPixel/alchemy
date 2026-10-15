	.section .rodata
	.global Field_DefaultMetatileAttributes
Field_DefaultMetatileAttributes:
	.incbin "games/tla/src/map/default/attributes.table.lz"
	.byte 0xea, 0x57, 0x66	@ PACKER: left in the buffer by Field_DefaultMapCells+0x4b5
