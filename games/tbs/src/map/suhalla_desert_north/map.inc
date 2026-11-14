	.global Field_SuhallaDesertNorth
Field_SuhallaDesertNorth:
	map_header 0, 0, 64, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0x7f, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0x7f, 0xff
	map_layer 128, 0, 20, 20, -121, 35, 0x1f, 0x1f
	map_offset Field_SuhallaDesertNorth, .LField_SuhallaDesertNorth_CELL_PICTURES
	map_offset Field_SuhallaDesertNorth, .LField_SuhallaDesertNorth_SHAPES
	map_offset Field_SuhallaDesertNorth, .LField_SuhallaDesertNorth_GRID
	map_offset Field_SuhallaDesertNorth, .LField_SuhallaDesertNorth_ANIMATION
	map_offset Field_SuhallaDesertNorth
	map_offset Field_SuhallaDesertNorth, .LField_SuhallaDesertNorth_MARKERS
.LField_SuhallaDesertNorth_CELL_PICTURES:
	.incbin "games/tbs/src/map/suhalla_desert_north/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_SuhallaDesertNorth_SHAPES:
	.incbin "games/tbs/src/map/suhalla_desert_north/shapes.table.lz"
	.balign 4, 0
.LField_SuhallaDesertNorth_GRID:
	.incbin "games/tbs/src/map/suhalla_desert_north/map.grid.lz"
	.balign 4, 0
.LField_SuhallaDesertNorth_ANIMATION:
	.incbin "games/tbs/src/map/suhalla_desert_north/tileset_anims.script.lz"
	.balign 4, 0
.LField_SuhallaDesertNorth_MARKERS:
	treasure_spot 26, 27, 103
	treasure_spot 18, 45, 102
	treasure_spots_end
	.balign 4, 0
	.global Tileset_PaletteSuhallaDesert
Tileset_PaletteSuhallaDesert:
	.incbin "games/tbs/src/graphics/tileset/palettes/suhalla_desert.gbapal.lz"
	.balign 4, 0
