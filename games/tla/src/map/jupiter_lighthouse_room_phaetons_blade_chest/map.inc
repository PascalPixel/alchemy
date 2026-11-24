	.global Field_JupiterLighthouseRoomPhaetonsBladeChest
Field_JupiterLighthouseRoomPhaetonsBladeChest:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_JupiterLighthouseRoomPhaetonsBladeChest, .LField_JupiterLighthouseRoomPhaetonsBladeChest_CELL_TILES
	map_block Field_JupiterLighthouseRoomPhaetonsBladeChest, .LField_JupiterLighthouseRoomPhaetonsBladeChest_SHAPES
	map_block Field_JupiterLighthouseRoomPhaetonsBladeChest, .LField_JupiterLighthouseRoomPhaetonsBladeChest_GRID
	map_block Field_JupiterLighthouseRoomPhaetonsBladeChest, .LField_JupiterLighthouseRoomPhaetonsBladeChest_SHAPE_GRID
	map_block Field_JupiterLighthouseRoomPhaetonsBladeChest, .LField_JupiterLighthouseRoomPhaetonsBladeChest_ANIMATION
	map_block Field_JupiterLighthouseRoomPhaetonsBladeChest
	map_block Field_JupiterLighthouseRoomPhaetonsBladeChest, .LField_JupiterLighthouseRoomPhaetonsBladeChest_SCENE
.LField_JupiterLighthouseRoomPhaetonsBladeChest_CELL_TILES:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_phaetons_blade_chest/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoomPhaetonsBladeChest_SHAPES:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_phaetons_blade_chest/shapes.table.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoomPhaetonsBladeChest_GRID:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_phaetons_blade_chest/map.grid.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoomPhaetonsBladeChest_SHAPE_GRID:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_phaetons_blade_chest/shape_grid.plane.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoomPhaetonsBladeChest_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoomPhaetonsBladeChest_SCENE:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_phaetons_blade_chest/scene.scene.lz"
	.balign 4, 0
