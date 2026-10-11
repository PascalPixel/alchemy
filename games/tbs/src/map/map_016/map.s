	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map016
Field_Map016:
	map_header 0, 0, 255, 81, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 81, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 161, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map016, .LField_Map016_METATILES
	map_block Field_Map016, .LField_Map016_SHAPES
	map_block Field_Map016, .LField_Map016_GRID
	map_block Field_Map016, .LField_Map016_ANIMATION
	map_block Field_Map016, .LField_Map016_BLEND
	map_block Field_Map016, .LField_Map016_MARKERS
.LField_Map016_METATILES:
	.incbin "map/map_016/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map016_SHAPES:
	.incbin "common/map/map_015/shapes.table.lz"
	.balign 4, 0
.LField_Map016_GRID:
	.incbin "map/map_016/grid.parts.lz"
	.balign 4, 0
.LField_Map016_ANIMATION:
	.incbin "map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map016_BLEND:
	.incbin "map/map_012/blend.script.lz"
	.balign 4, 0
.LField_Map016_MARKERS:
	.incbin "map/map_016/markers.table"
	.balign 4, 0
	.global Tileset_Palette00
Tileset_Palette00:
	.incbin "graphics/tileset/palettes/pal_00.gbapal.lz"
	.byte 0x41, 0x28, 0x40	@ PACKER: left in the buffer by Field_Map016+0x159
