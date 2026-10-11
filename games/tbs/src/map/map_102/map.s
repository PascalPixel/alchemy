	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map102
Field_Map102:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map102, .LField_Map102_METATILES
	map_block Field_Map102, .LField_Map102_SHAPES
	map_block Field_Map102, .LField_Map102_GRID
	map_block Field_Map102, .LField_Map102_ANIMATION
	map_block Field_Map102, .LField_Map102_BLEND
	map_block Field_Map102, .LField_Map102_MARKERS
.LField_Map102_METATILES:
	.incbin "map/map_102/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map102_SHAPES:
	.incbin "map/map_102/shapes.table.lz"
	.balign 4, 0
.LField_Map102_GRID:
	.incbin "map/map_102/grid.parts.lz"
	.balign 4, 0
.LField_Map102_ANIMATION:
	.incbin "map/map_018/animation.script.lz"
	.balign 4, 0
.LField_Map102_BLEND:
	.incbin "common/map/map_018/blend.script.lz"
	.balign 4, 0
.LField_Map102_MARKERS:
	.incbin "map/map_000/markers.table"
	.balign 4, 0
	.global Tileset_Set55Palette
Tileset_Set55Palette:
	.incbin "graphics/tileset/set_55/palette.gbapal.lz"
	.byte 0x05	@ PACKER: left in the buffer by Field_Map102+0x1bf
	.global Tileset_Set55TilesA
Tileset_Set55TilesA:
	.incbin "graphics/tileset/set_55/tiles_a.4bpp.mtf"
