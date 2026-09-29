@ Field_LightningMap: a 4x16 tilemap that stacks a lightning bolt's
@ two rows of 32 tiles into one tall strike, drawn with background
@ palette 2 and packed by the resource packer.
	.section .rodata
	.global Field_LightningMap
Field_LightningMap:
	.incbin "GRAPHICS/FIELD/LIGHTNING_MAP.table.lz"
