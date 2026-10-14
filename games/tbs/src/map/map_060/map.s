	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map060
Field_Map060:
	map_header 0, 0, 55, 68, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map060, .LField_Map060_METATILES
	map_block Field_Map060, .LField_Map060_SHAPES
	map_block Field_Map060, .LField_Map060_GRID
	map_block Field_Map060, .LField_Map060_ANIMATION
	map_block Field_Map060, .LField_Map060_BLEND
	map_block Field_Map060, .LField_Map060_MARKERS
.LField_Map060_METATILES:
	.incbin "games/tbs/src/map/map_060/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map060_SHAPES:
	.incbin "games/tbs/src/map/map_060/shapes.table.lz"
	.balign 4, 0
.LField_Map060_GRID:
	.incbin "games/tbs/src/map/map_060/grid.parts.lz"
	.balign 4, 0
.LField_Map060_ANIMATION:
	.incbin "games/tbs/src/map/map_028/animation.script.lz"
	.balign 4, 0
.LField_Map060_BLEND:
	.incbin "games/tbs/src/map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map060_MARKERS:
	.incbin "games/tbs/src/map/map_060/markers.table"
	.balign 4, 0
	.global Tileset_Set44Palette
Tileset_Set44Palette:
	.incbin "games/tbs/src/graphics/tileset/set_44/palette.gbapal.lz"
	.byte 0x32, 0xc8	@ PACKER: left in the buffer by Field_Map060+0x1be
	.global Tileset_Set44TilesA
Tileset_Set44TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_44/tiles_a.4bpp.mtf"
