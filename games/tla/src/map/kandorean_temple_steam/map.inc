	.global Field_KandoreanTempleSteam
Field_KandoreanTempleSteam:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_KandoreanTempleSteam, .LField_KandoreanTempleSteam_CELL_PICTURES
	map_block Field_KandoreanTempleSteam, .LField_KandoreanTempleSteam_SHAPES
	map_block Field_KandoreanTempleSteam, .LField_KandoreanTempleSteam_GRID
	map_block Field_KandoreanTempleSteam, .LField_KandoreanTempleSteam_SHAPE_GRID
	map_block Field_KandoreanTempleSteam, .LField_KandoreanTempleSteam_ANIMATION
	map_block Field_KandoreanTempleSteam, .LField_KandoreanTempleSteam_BLEND
	map_block Field_KandoreanTempleSteam, .LField_KandoreanTempleSteam_SCENE
.LField_KandoreanTempleSteam_CELL_PICTURES:
	.incbin "games/tla/src/map/kandorean_temple_steam/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_KandoreanTempleSteam_SHAPES:
	.incbin "games/tla/src/map/kandorean_temple_steam/shapes.table.lz"
	.balign 4, 0
.LField_KandoreanTempleSteam_GRID:
	.incbin "games/tla/src/map/kandorean_temple_steam/map.grid.lz"
	.balign 4, 0
.LField_KandoreanTempleSteam_SHAPE_GRID:
	.incbin "games/tla/src/map/kandorean_temple_steam/shape_grid.plane.lz"
	.balign 4, 0
.LField_KandoreanTempleSteam_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_steam/animation.script.lz"
	.balign 4, 0
.LField_KandoreanTempleSteam_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_KandoreanTempleSteam_SCENE:
	.incbin "games/tla/src/map/kandorean_temple_steam/scene.scene.lz"
	.balign 4, 0
