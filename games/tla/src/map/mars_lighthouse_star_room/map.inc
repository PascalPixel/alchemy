	.global Field_MarsLighthouseStarRoom
Field_MarsLighthouseStarRoom:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MarsLighthouseStarRoom, .LField_MarsLighthouseStarRoom_CELL_PICTURES
	map_block Field_MarsLighthouseStarRoom, .LField_MarsLighthouseStarRoom_SHAPES
	map_block Field_MarsLighthouseStarRoom, .LField_MarsLighthouseStarRoom_GRID
	map_block Field_MarsLighthouseStarRoom, .LField_MarsLighthouseStarRoom_SHAPE_GRID
	map_block Field_MarsLighthouseStarRoom, .LField_MarsLighthouseStarRoom_ANIMATION
	map_block Field_MarsLighthouseStarRoom, .LField_MarsLighthouseStarRoom_BLEND
	map_block Field_MarsLighthouseStarRoom, .LField_MarsLighthouseStarRoom_SCENE
.LField_MarsLighthouseStarRoom_CELL_PICTURES:
	.incbin "games/tla/src/map/mars_lighthouse_star_room/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_MarsLighthouseStarRoom_SHAPES:
	.incbin "games/tla/src/map/mars_lighthouse_star_room/shapes.table.lz"
	.balign 4, 0
.LField_MarsLighthouseStarRoom_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_star_room/map.grid.lz"
	.balign 4, 0
.LField_MarsLighthouseStarRoom_SHAPE_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_star_room/shape_grid.plane.lz"
	.balign 4, 0
.LField_MarsLighthouseStarRoom_ANIMATION:
	.incbin "games/tla/src/map/map_259/animation.script.lz"
	.balign 4, 0
.LField_MarsLighthouseStarRoom_BLEND:
	.incbin "games/tla/src/map/map_260/blend.script.lz"
	.balign 4, 0
.LField_MarsLighthouseStarRoom_SCENE:
	.incbin "games/tla/src/map/mars_lighthouse_star_room/scene.scene.lz"
	.balign 4, 0
