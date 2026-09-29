@ Field_WhirlwindMap: a 16x18 tilemap that stacks a whirlwind's two
@ rows of 26 tiles into a funnel, mirrored about its middle, drawn with
@ background palette 2 and packed by the resource packer.
	.section .rodata
	.global Field_WhirlwindMap
Field_WhirlwindMap:
	.incbin "GRAPHICS/FIELD/WHIRLWIND_MAP.table.lz"
