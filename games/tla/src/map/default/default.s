	.section .rodata
	.global Field_DefaultMapCells
Field_DefaultMapCells:
	.incbin "games/tla/src/map/default/cells.table.lz"
	.byte 0x86, 0xb2, 0xb7	@ PACKER: left in the buffer by Ending_VillagersPictureA+0x93c1
	.global Field_DefaultMapEntrances
Field_DefaultMapEntrances:
	.incbin "games/tla/src/map/default/entrances.table.lz"
	.section .rodata.a
	.global Field_DefaultMapAnimation
Field_DefaultMapAnimation:
	.incbin "games/tla/src/map/default/animation.table.lz"
	.byte 0x67, 0x57, 0x6f	@ PACKER: left in the buffer by WorldMap_PaletteA+0x151
	.global Field_DefaultCellTilesAttributes
Field_DefaultCellTilesAttributes:
	.incbin "games/tla/src/map/default/attributes.table.lz"
	.byte 0xea, 0x57, 0x66	@ PACKER: left in the buffer by Field_DefaultMapCells+0x4b5
