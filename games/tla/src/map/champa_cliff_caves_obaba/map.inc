	.global Field_ChampaCliffCavesObaba
Field_ChampaCliffCavesObaba:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_ChampaCliffCavesObaba, .LField_ChampaCliffCavesObaba_CELL_TILES
	map_block Field_ChampaCliffCavesObaba, .LField_ChampaCliffCavesObaba_SHAPES
	map_block Field_ChampaCliffCavesObaba, .LField_ChampaCliffCavesObaba_GRID
	map_block Field_ChampaCliffCavesObaba, .LField_ChampaCliffCavesObaba_SHAPE_GRID
	map_block Field_ChampaCliffCavesObaba, .LField_ChampaCliffCavesObaba_ANIMATION
	map_block Field_ChampaCliffCavesObaba, .LField_ChampaCliffCavesObaba_BLEND
	map_block Field_ChampaCliffCavesObaba, .LField_ChampaCliffCavesObaba_SCENE
.LField_ChampaCliffCavesObaba_CELL_TILES:
	.incbin "games/tla/src/map/champa_cliff_caves_obaba/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_ChampaCliffCavesObaba_SHAPES:
	.incbin "games/tla/src/map/champa_cliff_caves_obaba/shapes.table.lz"
	.balign 4, 0
.LField_ChampaCliffCavesObaba_GRID:
	.incbin "games/tla/src/map/champa_cliff_caves_obaba/map.grid.lz"
	.balign 4, 0
.LField_ChampaCliffCavesObaba_SHAPE_GRID:
	.incbin "games/tla/src/map/champa_cliff_caves_obaba/shape_grid.plane.lz"
	.balign 4, 0
.LField_ChampaCliffCavesObaba_ANIMATION:
	.incbin "games/tla/src/map/champa_cliff_caves_obaba/animation.script.lz"
	.balign 4, 0
.LField_ChampaCliffCavesObaba_BLEND:
	.incbin "games/tla/src/map/garoh_tunnels/blend.script.lz"
	.balign 4, 0
.LField_ChampaCliffCavesObaba_SCENE:
	.incbin "games/tla/src/map/champa_cliff_caves_obaba/scene.scene.lz"
	.balign 4, 0
