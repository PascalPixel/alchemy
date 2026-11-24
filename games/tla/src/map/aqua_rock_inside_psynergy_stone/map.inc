	.global Field_AquaRockInsidePsynergyStone
Field_AquaRockInsidePsynergyStone:
	map_header 0, 0, 64, 64, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AquaRockInsidePsynergyStone, .LField_AquaRockInsidePsynergyStone_CELL_TILES
	map_block Field_AquaRockInsidePsynergyStone, .LField_AquaRockInsidePsynergyStone_SHAPES
	map_block Field_AquaRockInsidePsynergyStone, .LField_AquaRockInsidePsynergyStone_GRID
	map_block Field_AquaRockInsidePsynergyStone, .LField_AquaRockInsidePsynergyStone_SHAPE_GRID
	map_block Field_AquaRockInsidePsynergyStone, .LField_AquaRockInsidePsynergyStone_ANIMATION
	map_block Field_AquaRockInsidePsynergyStone, .LField_AquaRockInsidePsynergyStone_BLEND
	map_block Field_AquaRockInsidePsynergyStone, .LField_AquaRockInsidePsynergyStone_SCENE
.LField_AquaRockInsidePsynergyStone_CELL_TILES:
	.incbin "games/tla/src/map/aqua_rock_inside_psynergy_stone/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_AquaRockInsidePsynergyStone_SHAPES:
	.incbin "games/tla/src/map/aqua_rock_inside_psynergy_stone/shapes.table.lz"
	.balign 4, 0
.LField_AquaRockInsidePsynergyStone_GRID:
	.incbin "games/tla/src/map/aqua_rock_inside_psynergy_stone/map.grid.lz"
	.balign 4, 0
.LField_AquaRockInsidePsynergyStone_SHAPE_GRID:
	.incbin "games/tla/src/map/aqua_rock_inside_psynergy_stone/shape_grid.plane.lz"
	.balign 4, 0
.LField_AquaRockInsidePsynergyStone_ANIMATION:
	.incbin "games/tla/src/map/aqua_rock_inside_psynergy_stone/animation.script.lz"
	.balign 4, 0
.LField_AquaRockInsidePsynergyStone_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_AquaRockInsidePsynergyStone_SCENE:
	.incbin "games/tla/src/map/aqua_rock_inside_psynergy_stone/scene.scene.lz"
	.balign 4, 0
