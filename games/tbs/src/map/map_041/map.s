	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map041
Field_Map041:
	map_header 0, 0, 64, 84, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map041, .LField_Map041_METATILES
	map_block Field_Map041, .LField_Map041_SHAPES
	map_block Field_Map041, .LField_Map041_GRID
	map_block Field_Map041, .LField_Map041_ANIMATION
	map_block Field_Map041
	map_block Field_Map041, .LField_Map041_MARKERS
.LField_Map041_METATILES:
	.incbin "map/map_041/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map041_SHAPES:
	.incbin "map/map_041/shapes.table.lz"
	.balign 4, 0
.LField_Map041_GRID:
	.incbin "map/map_041/grid.parts.lz"
	.balign 4, 0
.LField_Map041_ANIMATION:
	.incbin "map/map_041/animation.script.lz"
	.balign 4, 0
.LField_Map041_MARKERS:
	.incbin "map/map_000/markers.table"
	.balign 4, 0
	.global Tileset_Set32Palette
Tileset_Set32Palette:
	.incbin "graphics/tileset/set_32/palette.gbapal.lz"
	.byte 0xb1, 0xc1	@ PACKER: left in the buffer by Field_Map041+0x176
	.global Tileset_Set32TilesA
Tileset_Set32TilesA:
	.incbin "graphics/tileset/set_32/tiles_a.4bpp.mtf"
	.global Tileset_Set32TilesB
Tileset_Set32TilesB:
	.incbin "graphics/tileset/set_32/tiles_b.4bpp.mtf"
	.byte 0x4d, 0x0c, 0x83	@ PACKER: left in the buffer by Tileset_Set32TilesA+0x1d6d
	.global Tileset_Set32TilesC
Tileset_Set32TilesC:
	.incbin "graphics/tileset/set_32/tiles_c.4bpp.mtf"
	.byte 0xb8, 0x92, 0x64	@ PACKER: left in the buffer by Tileset_Set32TilesB+0xa65
	.global Tileset_Set32TilesD
Tileset_Set32TilesD:
	.incbin "graphics/tileset/set_32/tiles_d.4bpp.mtf"
