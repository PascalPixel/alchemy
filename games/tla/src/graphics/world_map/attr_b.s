	.section .rodata
@ 世界地図 B のメタタイル属性 (Field_DefaultMetatileAttributes と同じ形)
	.global Resource_Data1A5
Resource_Data1A5:
	.incbin "games/tla/src/graphics/world_map/attr_b.table.lz"
	.byte 0x11, 0xf9, 0xdc	@ PACKER: left in the buffer by WorldMap_TilesA6+0x3bd
