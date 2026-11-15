	.global Field_KandoreanTemple6
Field_KandoreanTemple6:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_KandoreanTemple6, .LField_KandoreanTemple6_CELL_PICTURES
	map_block Field_KandoreanTemple6, .LField_KandoreanTemple6_SHAPES
	map_block Field_KandoreanTemple6, .LField_KandoreanTemple6_GRID
	map_block Field_KandoreanTemple6, .LField_KandoreanTemple6_SHAPE_GRID
	map_block Field_KandoreanTemple6, .LField_KandoreanTemple6_ANIMATION
	map_block Field_KandoreanTemple6, .LField_KandoreanTemple6_BLEND
	map_block Field_KandoreanTemple6, .LField_KandoreanTemple6_SCENE
.LField_KandoreanTemple6_CELL_PICTURES:
	.incbin "games/tla/src/map/kandorean_temple_6/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_KandoreanTemple6_SHAPES:
	.incbin "games/tla/src/map/kandorean_temple_6/shapes.table.lz"
	.balign 4, 0
.LField_KandoreanTemple6_GRID:
	.incbin "games/tla/src/map/kandorean_temple_6/map.grid.lz"
	.balign 4, 0
.LField_KandoreanTemple6_SHAPE_GRID:
	.incbin "games/tla/src/map/kandorean_temple_6/shape_grid.plane.lz"
	.balign 4, 0
.LField_KandoreanTemple6_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_6/animation.script.lz"
	.balign 4, 0
.LField_KandoreanTemple6_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_KandoreanTemple6_SCENE:
	.incbin "games/tla/src/map/kandorean_temple_6/scene.scene.lz"
	.balign 4, 0
