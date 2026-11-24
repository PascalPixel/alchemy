	.global Field_MarsLighthouseEntranceValkyrieMailChest
Field_MarsLighthouseEntranceValkyrieMailChest:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MarsLighthouseEntranceValkyrieMailChest, .LField_MarsLighthouseEntranceValkyrieMailChest_CELL_TILES
	map_block Field_MarsLighthouseEntranceValkyrieMailChest, .LField_MarsLighthouseEntranceValkyrieMailChest_SHAPES
	map_block Field_MarsLighthouseEntranceValkyrieMailChest, .LField_MarsLighthouseEntranceValkyrieMailChest_GRID
	map_block Field_MarsLighthouseEntranceValkyrieMailChest, .LField_MarsLighthouseEntranceValkyrieMailChest_SHAPE_GRID
	map_block Field_MarsLighthouseEntranceValkyrieMailChest, .LField_MarsLighthouseEntranceValkyrieMailChest_ANIMATION
	map_block Field_MarsLighthouseEntranceValkyrieMailChest, .LField_MarsLighthouseEntranceValkyrieMailChest_BLEND
	map_block Field_MarsLighthouseEntranceValkyrieMailChest, .LField_MarsLighthouseEntranceValkyrieMailChest_SCENE
.LField_MarsLighthouseEntranceValkyrieMailChest_CELL_TILES:
	.incbin "games/tla/src/map/mars_lighthouse_entrance_valkyrie_mail_chest/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_MarsLighthouseEntranceValkyrieMailChest_SHAPES:
	.incbin "games/tla/src/map/mars_lighthouse_entrance_valkyrie_mail_chest/shapes.table.lz"
	.balign 4, 0
.LField_MarsLighthouseEntranceValkyrieMailChest_GRID:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/mars_lighthouse_entrance_valkyrie_mail_chest/map_ja.grid.lz"
	.else
	.incbin "games/tla/src/map/mars_lighthouse_entrance_valkyrie_mail_chest/map.grid.lz"
	.endif
	.balign 4, 0
.LField_MarsLighthouseEntranceValkyrieMailChest_SHAPE_GRID:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/mars_lighthouse_entrance_valkyrie_mail_chest/shape_grid_ja.plane.lz"
	.else
	.incbin "games/tla/src/map/mars_lighthouse_entrance_valkyrie_mail_chest/shape_grid.plane.lz"
	.endif
	.balign 4, 0
.LField_MarsLighthouseEntranceValkyrieMailChest_ANIMATION:
	.incbin "games/tla/src/map/mars_lighthouse_mars_tower/animation.script.lz"
	.balign 4, 0
.LField_MarsLighthouseEntranceValkyrieMailChest_BLEND:
	.incbin "games/tla/src/map/mars_lighthouse_mercury_tower/blend.script.lz"
	.balign 4, 0
.LField_MarsLighthouseEntranceValkyrieMailChest_SCENE:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/mars_lighthouse_entrance_valkyrie_mail_chest/scene_ja.scene.lz"
	.else
	.incbin "games/tla/src/map/mars_lighthouse_entrance_valkyrie_mail_chest/scene.scene.lz"
	.endif
	.balign 4, 0
