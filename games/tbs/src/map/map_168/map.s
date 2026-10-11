	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map168
Field_Map168:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map168, .LField_Map168_METATILES
	map_block Field_Map168, .LField_Map168_SHAPES
	map_block Field_Map168, .LField_Map168_GRID
	map_block Field_Map168, .LField_Map168_ANIMATION
	map_block Field_Map168
	map_block Field_Map168, .LField_Map168_MARKERS
.LField_Map168_METATILES:
	.incbin "map/map_168/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map168_SHAPES:
	.incbin "map/map_168/shapes.table.lz"
	.balign 4, 0
.LField_Map168_GRID:
	.incbin "map/map_168/grid.parts.lz"
	.balign 4, 0
.LField_Map168_ANIMATION:
	.incbin "map/map_054/animation.script.lz"
	.balign 4, 0
.LField_Map168_MARKERS:
	.incbin "map/map_000/markers.table"
	.balign 4, 0
	.global Tileset_Palette52
Tileset_Palette52:
	.incbin "graphics/tileset/palettes/pal_52.gbapal.lz"
	.byte 0x06	@ PACKER: left in the buffer by Field_Map168+0x103
