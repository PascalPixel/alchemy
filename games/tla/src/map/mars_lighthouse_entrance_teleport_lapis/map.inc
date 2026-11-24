	.global Field_MarsLighthouseEntranceTeleportLapis
Field_MarsLighthouseEntranceTeleportLapis:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MarsLighthouseEntranceTeleportLapis, .LField_MarsLighthouseEntranceTeleportLapis_CELL_TILES
	map_block Field_MarsLighthouseEntranceTeleportLapis, .LField_MarsLighthouseEntranceTeleportLapis_SHAPES
	map_block Field_MarsLighthouseEntranceTeleportLapis, .LField_MarsLighthouseEntranceTeleportLapis_GRID
	map_block Field_MarsLighthouseEntranceTeleportLapis, .LField_MarsLighthouseEntranceTeleportLapis_SHAPE_GRID
	map_block Field_MarsLighthouseEntranceTeleportLapis, .LField_MarsLighthouseEntranceTeleportLapis_ANIMATION
	map_block Field_MarsLighthouseEntranceTeleportLapis, .LField_MarsLighthouseEntranceTeleportLapis_BLEND
	map_block Field_MarsLighthouseEntranceTeleportLapis, .LField_MarsLighthouseEntranceTeleportLapis_SCENE
.LField_MarsLighthouseEntranceTeleportLapis_CELL_TILES:
	.incbin "games/tla/src/map/mars_lighthouse_entrance_teleport_lapis/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_MarsLighthouseEntranceTeleportLapis_SHAPES:
	.incbin "games/tla/src/map/mars_lighthouse_entrance_teleport_lapis/shapes.table.lz"
	.balign 4, 0
.LField_MarsLighthouseEntranceTeleportLapis_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_entrance_teleport_lapis/map.grid.lz"
	.balign 4, 0
.LField_MarsLighthouseEntranceTeleportLapis_SHAPE_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_entrance_teleport_lapis/shape_grid.plane.lz"
	.balign 4, 0
.LField_MarsLighthouseEntranceTeleportLapis_ANIMATION:
	.incbin "games/tla/src/map/mars_lighthouse_mars_tower/animation.script.lz"
	.balign 4, 0
.LField_MarsLighthouseEntranceTeleportLapis_BLEND:
	.incbin "games/tla/src/map/mars_lighthouse_mercury_tower/blend.script.lz"
	.balign 4, 0
.LField_MarsLighthouseEntranceTeleportLapis_SCENE:
	.incbin "games/tla/src/map/mars_lighthouse_entrance_teleport_lapis/scene.scene.lz"
	.balign 4, 0
