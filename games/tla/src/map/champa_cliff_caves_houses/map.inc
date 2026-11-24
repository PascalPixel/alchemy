	.global Field_ChampaCliffCavesHouses
Field_ChampaCliffCavesHouses:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_ChampaCliffCavesHouses, .LField_ChampaCliffCavesHouses_CELL_TILES
	map_block Field_ChampaCliffCavesHouses, .LField_ChampaCliffCavesHouses_SHAPES
	map_block Field_ChampaCliffCavesHouses, .LField_ChampaCliffCavesHouses_GRID
	map_block Field_ChampaCliffCavesHouses, .LField_ChampaCliffCavesHouses_SHAPE_GRID
	map_block Field_ChampaCliffCavesHouses, .LField_ChampaCliffCavesHouses_ANIMATION
	map_block Field_ChampaCliffCavesHouses, .LField_ChampaCliffCavesHouses_BLEND
	map_block Field_ChampaCliffCavesHouses, .LField_ChampaCliffCavesHouses_SCENE
.LField_ChampaCliffCavesHouses_CELL_TILES:
	.incbin "games/tla/src/map/champa_cliff_caves_houses/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_ChampaCliffCavesHouses_SHAPES:
	.incbin "games/tla/src/map/champa_cliff_caves_houses/shapes.table.lz"
	.balign 4, 0
.LField_ChampaCliffCavesHouses_GRID:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/champa_cliff_caves_houses/map_ja.grid.lz"
	.else
	.incbin "games/tla/src/map/champa_cliff_caves_houses/map.grid.lz"
	.endif
	.balign 4, 0
.LField_ChampaCliffCavesHouses_SHAPE_GRID:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/champa_cliff_caves_houses/shape_grid_ja.plane.lz"
	.else
	.incbin "games/tla/src/map/champa_cliff_caves_houses/shape_grid.plane.lz"
	.endif
	.balign 4, 0
.LField_ChampaCliffCavesHouses_ANIMATION:
	.incbin "games/tla/src/map/champa_cliff_caves_houses/animation.script.lz"
	.balign 4, 0
.LField_ChampaCliffCavesHouses_BLEND:
	.incbin "games/tla/src/map/garoh_tunnels/blend.script.lz"
	.balign 4, 0
.LField_ChampaCliffCavesHouses_SCENE:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/champa_cliff_caves_houses/scene_ja.scene.lz"
	.else
	.incbin "games/tla/src/map/champa_cliff_caves_houses/scene.scene.lz"
	.endif
	.balign 4, 0
