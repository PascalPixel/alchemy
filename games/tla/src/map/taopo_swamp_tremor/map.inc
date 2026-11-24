	.global Field_TaopoSwampTremor
Field_TaopoSwampTremor:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_TaopoSwampTremor, .LField_TaopoSwampTremor_CELL_TILES
	map_block Field_TaopoSwampTremor, .LField_TaopoSwampTremor_SHAPES
	map_block Field_TaopoSwampTremor, .LField_TaopoSwampTremor_GRID
	map_block Field_TaopoSwampTremor, .LField_TaopoSwampTremor_SHAPE_GRID
	map_block Field_TaopoSwampTremor, .LField_TaopoSwampTremor_ANIMATION
	map_block Field_TaopoSwampTremor, .LField_TaopoSwampTremor_BLEND
	map_block Field_TaopoSwampTremor, .LField_TaopoSwampTremor_SCENE
.LField_TaopoSwampTremor_CELL_TILES:
	.incbin "games/tla/src/map/taopo_swamp_tremor/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_TaopoSwampTremor_SHAPES:
	.incbin "games/tla/src/map/taopo_swamp_tremor/shapes.table.lz"
	.balign 4, 0
.LField_TaopoSwampTremor_GRID:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/taopo_swamp_tremor/map_ja.grid.lz"
	.else
	.incbin "games/tla/src/map/taopo_swamp_tremor/map.grid.lz"
	.endif
	.balign 4, 0
.LField_TaopoSwampTremor_SHAPE_GRID:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/taopo_swamp_tremor/shape_grid_ja.plane.lz"
	.else
	.incbin "games/tla/src/map/taopo_swamp_tremor/shape_grid.plane.lz"
	.endif
	.balign 4, 0
.LField_TaopoSwampTremor_ANIMATION:
	.incbin "games/tla/src/map/taopo_swamp_tremor/animation.script.lz"
	.balign 4, 0
.LField_TaopoSwampTremor_BLEND:
	.incbin "games/tla/src/map/yampi_desert_guardian_ring_chest/blend.script.lz"
	.balign 4, 0
.LField_TaopoSwampTremor_SCENE:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/taopo_swamp_tremor/scene_ja.scene.lz"
	.else
	.incbin "games/tla/src/map/taopo_swamp_tremor/scene.scene.lz"
	.endif
