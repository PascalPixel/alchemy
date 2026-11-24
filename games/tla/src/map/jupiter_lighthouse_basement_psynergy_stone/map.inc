	.global Field_JupiterLighthouseBasementPsynergyStone
Field_JupiterLighthouseBasementPsynergyStone:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_JupiterLighthouseBasementPsynergyStone, .LField_JupiterLighthouseBasementPsynergyStone_CELL_TILES
	map_block Field_JupiterLighthouseBasementPsynergyStone, .LField_JupiterLighthouseBasementPsynergyStone_SHAPES
	map_block Field_JupiterLighthouseBasementPsynergyStone, .LField_JupiterLighthouseBasementPsynergyStone_GRID
	map_block Field_JupiterLighthouseBasementPsynergyStone, .LField_JupiterLighthouseBasementPsynergyStone_SHAPE_GRID
	map_block Field_JupiterLighthouseBasementPsynergyStone, .LField_JupiterLighthouseBasementPsynergyStone_ANIMATION
	map_block Field_JupiterLighthouseBasementPsynergyStone, .LField_JupiterLighthouseBasementPsynergyStone_BLEND
	map_block Field_JupiterLighthouseBasementPsynergyStone, .LField_JupiterLighthouseBasementPsynergyStone_SCENE
.LField_JupiterLighthouseBasementPsynergyStone_CELL_TILES:
	.incbin "games/tla/src/map/jupiter_lighthouse_basement_psynergy_stone/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_JupiterLighthouseBasementPsynergyStone_SHAPES:
	.incbin "games/tla/src/map/jupiter_lighthouse_basement_psynergy_stone/shapes.table.lz"
	.balign 4, 0
.LField_JupiterLighthouseBasementPsynergyStone_GRID:
	.incbin "games/tla/src/map/jupiter_lighthouse_basement_psynergy_stone/map.grid.lz"
	.balign 4, 0
.LField_JupiterLighthouseBasementPsynergyStone_SHAPE_GRID:
	.incbin "games/tla/src/map/jupiter_lighthouse_basement_psynergy_stone/shape_grid.plane.lz"
	.balign 4, 0
.LField_JupiterLighthouseBasementPsynergyStone_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_JupiterLighthouseBasementPsynergyStone_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_JupiterLighthouseBasementPsynergyStone_SCENE:
	.incbin "games/tla/src/map/jupiter_lighthouse_basement_psynergy_stone/scene.scene.lz"
