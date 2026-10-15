	.section .rodata
	.global Field_LightningMap
Field_LightningMap:
	.incbin "games/tla/src/graphics/field/lightning_map.table.lz"
	.byte 0xf1, 0x2e	@ PACKER: left in the buffer by Resource_Data1C0+0x92
