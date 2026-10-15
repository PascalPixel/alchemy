	.section .rodata
	.global Field_WhirlwindMap
Field_WhirlwindMap:
	.incbin "games/tla/src/graphics/field/whirlwind_map.table.lz"
	.byte 0xd9, 0xe1, 0xe4	@ PACKER: left in the buffer by Resource_Data1BE+0xd9
