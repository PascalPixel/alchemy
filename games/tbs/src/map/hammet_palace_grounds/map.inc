	.global Field_HammetPalaceGrounds
Field_HammetPalaceGrounds:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_HammetPalaceGrounds, .LField_HammetPalaceGrounds_CELL_PICTURES
	map_offset Field_HammetPalaceGrounds, .LField_HammetPalaceGrounds_SHAPES
	map_offset Field_HammetPalaceGrounds, .LField_HammetPalaceGrounds_GRID
	map_offset Field_HammetPalaceGrounds, .LField_HammetPalaceGrounds_ANIMATION
	map_offset Field_HammetPalaceGrounds, .LField_HammetPalaceGrounds_BLEND
	map_offset Field_HammetPalaceGrounds, .LField_HammetPalaceGrounds_MARKERS
.LField_HammetPalaceGrounds_CELL_PICTURES:
	.incbin "games/tbs/src/map/hammet_palace_grounds/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_HammetPalaceGrounds_SHAPES:
	.incbin "games/tbs/src/map/hammet_palace_grounds/shapes.table.lz"
	.balign 4, 0
.LField_HammetPalaceGrounds_GRID:
	.incbin "games/tbs/src/map/hammet_palace_grounds/map.grid.lz"
	.balign 4, 0
.LField_HammetPalaceGrounds_ANIMATION:
	.incbin "games/tbs/src/map/kalay/tileset_anims.script.lz"
	.balign 4, 0
.LField_HammetPalaceGrounds_BLEND:
	.incbin "games/tbs/src/map/goma_cave_entrance/blend.script.lz"
	.balign 4, 0
.LField_HammetPalaceGrounds_MARKERS:
	treasure_spots_end
	.balign 4, 0
