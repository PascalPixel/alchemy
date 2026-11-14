	.global Field_HammetPalace
Field_HammetPalace:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_HammetPalace, .LField_HammetPalace_CELL_PICTURES
	map_offset Field_HammetPalace, .LField_HammetPalace_SHAPES
	map_offset Field_HammetPalace, .LField_HammetPalace_GRID
	map_offset Field_HammetPalace, .LField_HammetPalace_ANIMATION
	map_offset Field_HammetPalace, .LField_HammetPalace_BLEND
	map_offset Field_HammetPalace, .LField_HammetPalace_MARKERS
.LField_HammetPalace_CELL_PICTURES:
	.incbin "games/tbs/src/map/hammet_palace/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_HammetPalace_SHAPES:
	.incbin "games/tbs/src/map/hammet_palace/shapes.table.lz"
	.balign 4, 0
.LField_HammetPalace_GRID:
	.incbin "games/tbs/src/map/hammet_palace/map.grid.lz"
	.balign 4, 0
.LField_HammetPalace_ANIMATION:
	.incbin "games/tbs/src/map/hammet_palace/tileset_anims.script.lz"
	.balign 4, 0
.LField_HammetPalace_BLEND:
	.incbin "games/tbs/src/map/goma_cave_entrance/blend.script.lz"
	.balign 4, 0
.LField_HammetPalace_MARKERS:
	treasure_spot 6, 4, 201
	treasure_spot 7, 4, 201
	treasure_spot 11, 4, 200
	treasure_spot 12, 4, 200
	treasure_spot 37, 4, 202
	treasure_spot 38, 4, 202
	treasure_spot 4, 15, 100
	treasure_spot 42, 16, 101
	treasure_spots_end
	.balign 4, 0
	.global Tileset_PaletteHammetPalace
Tileset_PaletteHammetPalace:
	.incbin "games/tbs/src/graphics/tileset/palettes/hammet_palace.gbapal.lz"
	.balign 4, 0
