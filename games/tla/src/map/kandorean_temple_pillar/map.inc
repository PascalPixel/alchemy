	.global Field_KandoreanTemplePillar
Field_KandoreanTemplePillar:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_KandoreanTemplePillar, .LField_KandoreanTemplePillar_CELL_TILES
	map_block Field_KandoreanTemplePillar, .LField_KandoreanTemplePillar_SHAPES
	map_block Field_KandoreanTemplePillar, .LField_KandoreanTemplePillar_GRID
	map_block Field_KandoreanTemplePillar, .LField_KandoreanTemplePillar_SHAPE_GRID
	map_block Field_KandoreanTemplePillar, .LField_KandoreanTemplePillar_ANIMATION
	map_block Field_KandoreanTemplePillar, .LField_KandoreanTemplePillar_BLEND
	map_block Field_KandoreanTemplePillar, .LField_KandoreanTemplePillar_SCENE
.LField_KandoreanTemplePillar_CELL_TILES:
	.incbin "games/tla/src/map/kandorean_temple_pillar/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_KandoreanTemplePillar_SHAPES:
	.incbin "games/tla/src/map/kandorean_temple_pillar/shapes.table.lz"
	.balign 4, 0
.LField_KandoreanTemplePillar_GRID:
	.incbin "games/tla/src/map/kandorean_temple_pillar/map.grid.lz"
	.balign 4, 0
.LField_KandoreanTemplePillar_SHAPE_GRID:
	.incbin "games/tla/src/map/kandorean_temple_pillar/shape_grid.plane.lz"
	.balign 4, 0
.LField_KandoreanTemplePillar_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_KandoreanTemplePillar_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_KandoreanTemplePillar_SCENE:
	.incbin "games/tla/src/map/kandorean_temple_pillar/scene.scene.lz"
	.balign 4, 0
