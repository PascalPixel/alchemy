	.section .rodata
@ World map C cell picture attributes.
	.global Resource_Data1AE
Resource_Data1AE:
	.incbin "games/tla/src/graphics/world_map/attr_c.table.lz"
	.byte 0xef, 0x01, 0x7d	@ PACKER: left in the buffer by WorldMap_TilesB6+0x45d
