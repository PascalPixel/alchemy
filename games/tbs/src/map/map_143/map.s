	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map143
Field_Map143:
	map_header 0, 0, 72, 72, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map143, .LField_Map143_CELL_PICTURES
	map_offset Field_Map143, .LField_Map143_SHAPES
	map_offset Field_Map143, .LField_Map143_GRID
	map_offset Field_Map143, .LField_Map143_ANIMATION
	map_offset Field_Map143, .LField_Map143_BLEND
	map_offset Field_Map143, .LField_Map143_MARKERS
.LField_Map143_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_143/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Map143_SHAPES:
	.incbin "games/tbs/src/map/map_143/shapes.table.lz"
	.balign 4, 0
.LField_Map143_GRID:
	.incbin "games/tbs/src/map/map_143/map.grid.lz"
	.balign 4, 0
.LField_Map143_ANIMATION:
	.incbin "games/tbs/src/map/map_028/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map143_BLEND:
	.incbin "games/tbs/src/map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map143_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Palette37
Tileset_Palette37:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_37.gbapal.lz"
