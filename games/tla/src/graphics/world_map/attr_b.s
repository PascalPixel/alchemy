	.section .rodata
@ World map B attributes; Field_DefaultCellPictureAttributes format.
	.global Resource_Data1A5
Resource_Data1A5:
	.incbin "games/tla/src/graphics/world_map/attr_b.table.lz"
	.byte 0x11, 0xf9, 0xdc	@ PACKER: left in the buffer by WorldMap_TilesA6+0x3bd
