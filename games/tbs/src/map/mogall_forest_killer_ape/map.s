	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_MogallForestKillerApe
Field_MogallForestKillerApe:
	map_header 0, 0, 106, 80, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 108, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 90, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_MogallForestKillerApe, .LField_MogallForestKillerApe_CELL_PICTURES
	map_offset Field_MogallForestKillerApe, .LField_MogallForestKillerApe_SHAPES
	map_offset Field_MogallForestKillerApe, .LField_MogallForestKillerApe_GRID
	map_offset Field_MogallForestKillerApe, .LField_MogallForestKillerApe_ANIMATION
	map_offset Field_MogallForestKillerApe
	map_offset Field_MogallForestKillerApe, .LField_MogallForestKillerApe_MARKERS
.LField_MogallForestKillerApe_CELL_PICTURES:
	.incbin "games/tbs/src/map/mogall_forest_killer_ape/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_MogallForestKillerApe_SHAPES:
	.incbin "games/tbs/src/map/mogall_forest_killer_ape/shapes.table.lz"
	.balign 4, 0
.LField_MogallForestKillerApe_GRID:
	.incbin "games/tbs/src/map/mogall_forest_killer_ape/map.grid.lz"
	.balign 4, 0
.LField_MogallForestKillerApe_ANIMATION:
	.incbin "games/tbs/src/map/mogall_forest_sign/tileset_anims.script.lz"
	.balign 4, 0
.LField_MogallForestKillerApe_MARKERS:
	treasure_spot 43, 2, 100
	treasure_spots_end
	.balign 4, 0
