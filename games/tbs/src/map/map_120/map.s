	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map120
Field_Map120:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map120, .LField_Map120_METATILES
	map_block Field_Map120, .LField_Map120_SHAPES
	map_block Field_Map120, .LField_Map120_GRID
	map_block Field_Map120, .LField_Map120_ANIMATION
	map_block Field_Map120, .LField_Map120_BLEND
	map_block Field_Map120, .LField_Map120_MARKERS
.LField_Map120_METATILES:
	.incbin "games/tbs/src/map/map_120/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map120_SHAPES:
	.incbin "games/tbs/src/map/map_120/shapes.table.lz"
	.balign 4, 0
.LField_Map120_GRID:
	.incbin "games/tbs/src/map/map_120/grid.parts.lz"
	.balign 4, 0
.LField_Map120_ANIMATION:
	.incbin "games/tbs/src/map/map_094/animation.script.lz"
	.balign 4, 0
.LField_Map120_BLEND:
	.incbin "games/common/src/map/map_018/blend.script.lz"
	.balign 4, 0
.LField_Map120_MARKERS:
	.incbin "games/tbs/src/map/map_120/markers.table"
	.balign 4, 0
	.global Tileset_Palette26
Tileset_Palette26:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_26.gbapal.lz"
	.byte 0x9c, 0x00	@ PACKER: left in the buffer by Field_Map120+0x1de
