	.global Field_TundariaTowerCenterProng
Field_TundariaTowerCenterProng:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_TundariaTowerCenterProng, .LField_TundariaTowerCenterProng_CELL_TILES
	map_block Field_TundariaTowerCenterProng, .LField_TundariaTowerCenterProng_SHAPES
	map_block Field_TundariaTowerCenterProng, .LField_TundariaTowerCenterProng_GRID
	map_block Field_TundariaTowerCenterProng, .LField_TundariaTowerCenterProng_SHAPE_GRID
	map_block Field_TundariaTowerCenterProng, .LField_TundariaTowerCenterProng_ANIMATION
	map_block Field_TundariaTowerCenterProng, .LField_TundariaTowerCenterProng_BLEND
	map_block Field_TundariaTowerCenterProng, .LField_TundariaTowerCenterProng_SCENE
.LField_TundariaTowerCenterProng_CELL_TILES:
	.incbin "games/tla/src/map/tundaria_tower_center_prong/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_TundariaTowerCenterProng_SHAPES:
	.incbin "games/tla/src/map/tundaria_tower_center_prong/shapes.table.lz"
	.balign 4, 0
.LField_TundariaTowerCenterProng_GRID:
	.incbin "games/tla/src/map/tundaria_tower_center_prong/map.grid.lz"
	.balign 4, 0
.LField_TundariaTowerCenterProng_SHAPE_GRID:
	.incbin "games/tla/src/map/tundaria_tower_center_prong/shape_grid.plane.lz"
	.balign 4, 0
.LField_TundariaTowerCenterProng_ANIMATION:
	.incbin "games/tla/src/map/tundaria_tower_center_prong/animation.script.lz"
	.balign 4, 0
.LField_TundariaTowerCenterProng_BLEND:
	.incbin "games/tla/src/map/tundaria_tower_center_prong/blend.script.lz"
	.balign 4, 0
.LField_TundariaTowerCenterProng_SCENE:
	.incbin "games/tla/src/map/tundaria_tower_center_prong/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteTundariaTowerCenterProng
Tileset_PaletteTundariaTowerCenterProng:
	.incbin "games/tla/src/graphics/tileset/palettes/tundaria_tower_center_prong.gbapal.lz"
