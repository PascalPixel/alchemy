	.global Field_GondowanCliffsPass
Field_GondowanCliffsPass:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GondowanCliffsPass, .LField_GondowanCliffsPass_CELL_PICTURES
	map_block Field_GondowanCliffsPass, .LField_GondowanCliffsPass_SHAPES
	map_block Field_GondowanCliffsPass, .LField_GondowanCliffsPass_GRID
	map_block Field_GondowanCliffsPass, .LField_GondowanCliffsPass_SHAPE_GRID
	map_block Field_GondowanCliffsPass, .LField_GondowanCliffsPass_ANIMATION
	map_block Field_GondowanCliffsPass, .LField_GondowanCliffsPass_BLEND
	map_block Field_GondowanCliffsPass, .LField_GondowanCliffsPass_SCENE
.LField_GondowanCliffsPass_CELL_PICTURES:
	.incbin "games/tla/src/map/gondowan_cliffs_pass/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_GondowanCliffsPass_SHAPES:
	.incbin "games/tla/src/map/gondowan_cliffs_pass/shapes.table.lz"
	.balign 4, 0
.LField_GondowanCliffsPass_GRID:
	.incbin "games/tla/src/map/gondowan_cliffs_pass/map.grid.lz"
	.balign 4, 0
.LField_GondowanCliffsPass_SHAPE_GRID:
	.incbin "games/tla/src/map/gondowan_cliffs_pass/shape_grid.plane.lz"
	.balign 4, 0
.LField_GondowanCliffsPass_ANIMATION:
	.incbin "games/tla/src/map/gondowan_cliffs/animation.script.lz"
	.balign 4, 0
.LField_GondowanCliffsPass_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_GondowanCliffsPass_SCENE:
	.incbin "games/tla/src/map/gondowan_cliffs_pass/scene.scene.lz"
	.balign 4, 0
