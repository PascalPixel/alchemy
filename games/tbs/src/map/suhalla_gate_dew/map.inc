	.global Field_SuhallaGateDew
Field_SuhallaGateDew:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_SuhallaGateDew, .LField_SuhallaGateDew_CELL_PICTURES
	map_offset Field_SuhallaGateDew, .LField_SuhallaGateDew_SHAPES
	map_offset Field_SuhallaGateDew, .LField_SuhallaGateDew_GRID
	map_offset Field_SuhallaGateDew, .LField_SuhallaGateDew_ANIMATION
	map_offset Field_SuhallaGateDew, .LField_SuhallaGateDew_BLEND
	map_offset Field_SuhallaGateDew, .LField_SuhallaGateDew_MARKERS
.LField_SuhallaGateDew_CELL_PICTURES:
	.incbin "games/tbs/src/map/suhalla_gate_dew/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_SuhallaGateDew_SHAPES:
	.incbin "games/tbs/src/map/suhalla_gate_dew/shapes.table.lz"
	.balign 4, 0
.LField_SuhallaGateDew_GRID:
	.incbin "games/tbs/src/map/suhalla_gate_dew/map.grid.lz"
	.balign 4, 0
.LField_SuhallaGateDew_ANIMATION:
	.incbin "games/tbs/src/map/suhalla_gate_dew/tileset_anims.script.lz"
	.balign 4, 0
.LField_SuhallaGateDew_BLEND:
	.incbin "games/tbs/src/map/goma_cave_entrance/blend.script.lz"
	.balign 4, 0
.LField_SuhallaGateDew_MARKERS:
	treasure_spot 10, 22, 100
	treasure_spots_end
	.balign 4, 0
