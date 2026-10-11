	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map046
Field_Map046:
	map_header 0, 0, 128, 68, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 68, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map046, .LField_Map046_METATILES
	map_block Field_Map046, .LField_Map046_SHAPES
	map_block Field_Map046, .LField_Map046_GRID
	map_block Field_Map046, .LField_Map046_ANIMATION
	map_block Field_Map046, .LField_Map046_BLEND
	map_block Field_Map046, .LField_Map046_MARKERS
.LField_Map046_METATILES:
	.incbin "map/map_046/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map046_SHAPES:
	.incbin "map/map_046/shapes.table.lz"
	.balign 4, 0
.LField_Map046_GRID:
	.incbin "map/map_046/grid.parts.lz"
	.balign 4, 0
.LField_Map046_ANIMATION:
	.incbin "map/map_037/animation.script.lz"
	.balign 4, 0
.LField_Map046_BLEND:
	.incbin "map/map_046/blend.script.lz"
	.balign 4, 0
.LField_Map046_MARKERS:
	.incbin "map/map_046/markers.table"
	.global Tileset_Set36Palette
Tileset_Set36Palette:
	.incbin "graphics/tileset/set_36/palette.gbapal.lz"
	.byte 0x34	@ PACKER: left in the buffer by Field_Map046+0x18f
	.global Tileset_Set36TilesA
Tileset_Set36TilesA:
	.incbin "graphics/tileset/set_36/tiles_a.4bpp.mtf"
	.balign 4, 0
	.global Tileset_Set36TilesB
Tileset_Set36TilesB:
	.incbin "graphics/tileset/set_36/tiles_b.4bpp.mtf"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global Tileset_Set36TilesC
Tileset_Set36TilesC:
	.incbin "graphics/tileset/set_36/tiles_c.4bpp.mtf"
	.byte 0xe8, 0x95, 0x5a	@ PACKER: left in the buffer by Tileset_Set36TilesB+0xc81
	.global Tileset_Set36TilesD
Tileset_Set36TilesD:
	.incbin "graphics/tileset/set_36/tiles_d.4bpp.mtf"
	.byte 0xa4	@ PACKER: left in the buffer by Tileset_Set36TilesC+0x13f
