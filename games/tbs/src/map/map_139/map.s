	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map139
Field_Map139:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map139, .LField_Map139_CELL_PICTURES
	map_offset Field_Map139, .LField_Map139_SHAPES
	map_offset Field_Map139, .LField_Map139_GRID
	map_offset Field_Map139, .LField_Map139_ANIMATION
	map_offset Field_Map139, .LField_Map139_BLEND
	map_offset Field_Map139, .LField_Map139_MARKERS
.LField_Map139_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_139/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map139_SHAPES:
	.incbin "games/tbs/src/map/map_139/shapes.table.lz"
	.balign 4, 0
.LField_Map139_GRID:
	.incbin "games/tbs/src/map/map_139/map.grid.lz"
	.balign 4, 0
.LField_Map139_ANIMATION:
	.incbin "games/tbs/src/map/map_029/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map139_BLEND:
	.incbin "games/tbs/src/map/map_139/blend.script.lz"
	.balign 4, 0
.LField_Map139_MARKERS:
	treasure_spot 29, 24, 104
	treasure_spot 47, 52, 100
	treasure_spot 49, 53, 101
	treasure_spot 46, 54, 103
	treasure_spot 48, 55, 102
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Palette36
Tileset_Palette36:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_36.gbapal.lz"
