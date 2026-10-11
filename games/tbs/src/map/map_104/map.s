	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map104
Field_Map104:
	map_header 0, 0, 46, 46, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 92, 6, 6, 0, 0, 0xff, 0xff
	map_layer 0, 46, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map104, .LField_Map104_METATILES
	map_block Field_Map104, .LField_Map104_SHAPES
	map_block Field_Map104, .LField_Map104_GRID
	map_block Field_Map104, .LField_Map104_ANIMATION
	map_block Field_Map104
	map_block Field_Map104, .LField_Map104_MARKERS
.LField_Map104_METATILES:
	.incbin "map/map_104/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map104_SHAPES:
	.incbin "map/map_104/shapes.table.lz"
	.balign 4, 0
.LField_Map104_GRID:
	.incbin "map/map_104/grid.parts.lz"
	.balign 4, 0
.LField_Map104_ANIMATION:
	.incbin "map/map_104/animation.script.lz"
	.balign 4, 0
.LField_Map104_MARKERS:
	.incbin "map/map_000/markers.table"
	.balign 4, 0
	.global Tileset_Palette11
Tileset_Palette11:
	.incbin "graphics/tileset/palettes/pal_11.gbapal.lz"
	.byte 0xe7, 0x99	@ PACKER: left in the buffer by Field_Map104+0x1d2
