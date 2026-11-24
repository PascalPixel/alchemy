	.global Field_ChampaCliffCavesAvimander
Field_ChampaCliffCavesAvimander:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_ChampaCliffCavesAvimander, .LField_ChampaCliffCavesAvimander_CELL_TILES
	map_block Field_ChampaCliffCavesAvimander, .LField_ChampaCliffCavesAvimander_SHAPES
	map_block Field_ChampaCliffCavesAvimander, .LField_ChampaCliffCavesAvimander_GRID
	map_block Field_ChampaCliffCavesAvimander, .LField_ChampaCliffCavesAvimander_SHAPE_GRID
	map_block Field_ChampaCliffCavesAvimander, .LField_ChampaCliffCavesAvimander_ANIMATION
	map_block Field_ChampaCliffCavesAvimander, .LField_ChampaCliffCavesAvimander_BLEND
	map_block Field_ChampaCliffCavesAvimander, .LField_ChampaCliffCavesAvimander_SCENE
.LField_ChampaCliffCavesAvimander_CELL_TILES:
	.incbin "games/tla/src/map/champa_cliff_caves_avimander/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_ChampaCliffCavesAvimander_SHAPES:
	.incbin "games/tla/src/map/champa_cliff_caves_avimander/shapes.table.lz"
	.balign 4, 0
.LField_ChampaCliffCavesAvimander_GRID:
	.incbin "games/tla/src/map/champa_cliff_caves_avimander/map.grid.lz"
	.balign 4, 0
.LField_ChampaCliffCavesAvimander_SHAPE_GRID:
	.incbin "games/tla/src/map/champa_cliff_caves_avimander/shape_grid.plane.lz"
	.balign 4, 0
.LField_ChampaCliffCavesAvimander_ANIMATION:
	.incbin "games/tla/src/map/champa_cliff_caves_avimander/animation.script.lz"
	.balign 4, 0
.LField_ChampaCliffCavesAvimander_BLEND:
	.incbin "games/tla/src/map/garoh_tunnels/blend.script.lz"
	.balign 4, 0
.LField_ChampaCliffCavesAvimander_SCENE:
	.incbin "games/tla/src/map/champa_cliff_caves_avimander/scene.scene.lz"
	.balign 4, 0
