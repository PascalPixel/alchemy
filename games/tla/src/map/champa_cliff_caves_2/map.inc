	.global Field_ChampaCliffCaves2
Field_ChampaCliffCaves2:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_ChampaCliffCaves2, .LField_ChampaCliffCaves2_CELL_PICTURES
	map_block Field_ChampaCliffCaves2, .LField_ChampaCliffCaves2_SHAPES
	map_block Field_ChampaCliffCaves2, .LField_ChampaCliffCaves2_GRID
	map_block Field_ChampaCliffCaves2, .LField_ChampaCliffCaves2_SHAPE_GRID
	map_block Field_ChampaCliffCaves2, .LField_ChampaCliffCaves2_ANIMATION
	map_block Field_ChampaCliffCaves2, .LField_ChampaCliffCaves2_BLEND
	map_block Field_ChampaCliffCaves2, .LField_ChampaCliffCaves2_SCENE
.LField_ChampaCliffCaves2_CELL_PICTURES:
	.incbin "games/tla/src/map/champa_cliff_caves_2/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_ChampaCliffCaves2_SHAPES:
	.incbin "games/tla/src/map/champa_cliff_caves_2/shapes.table.lz"
	.balign 4, 0
.LField_ChampaCliffCaves2_GRID:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/champa_cliff_caves_2/map_ja.grid.lz"
	.else
	.incbin "games/tla/src/map/champa_cliff_caves_2/map.grid.lz"
	.endif
	.balign 4, 0
.LField_ChampaCliffCaves2_SHAPE_GRID:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/champa_cliff_caves_2/shape_grid_ja.plane.lz"
	.else
	.incbin "games/tla/src/map/champa_cliff_caves_2/shape_grid.plane.lz"
	.endif
	.balign 4, 0
.LField_ChampaCliffCaves2_ANIMATION:
	.incbin "games/tla/src/map/champa_cliff_caves_2/animation.script.lz"
	.balign 4, 0
.LField_ChampaCliffCaves2_BLEND:
	.incbin "games/tla/src/map/garoh_4/blend.script.lz"
	.balign 4, 0
.LField_ChampaCliffCaves2_SCENE:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/champa_cliff_caves_2/scene_ja.scene.lz"
	.else
	.incbin "games/tla/src/map/champa_cliff_caves_2/scene.scene.lz"
	.endif
	.balign 4, 0
