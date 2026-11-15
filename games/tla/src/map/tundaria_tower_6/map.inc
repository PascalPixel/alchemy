	.global Field_TundariaTower6
Field_TundariaTower6:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_TundariaTower6, .LField_TundariaTower6_CELL_PICTURES
	map_block Field_TundariaTower6, .LField_TundariaTower6_SHAPES
	map_block Field_TundariaTower6, .LField_TundariaTower6_GRID
	map_block Field_TundariaTower6, .LField_TundariaTower6_SHAPE_GRID
	map_block Field_TundariaTower6, .LField_TundariaTower6_ANIMATION
	map_block Field_TundariaTower6, .LField_TundariaTower6_BLEND
	map_block Field_TundariaTower6, .LField_TundariaTower6_SCENE
.LField_TundariaTower6_CELL_PICTURES:
	.incbin "games/tla/src/map/tundaria_tower_6/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_TundariaTower6_SHAPES:
	.incbin "games/tla/src/map/tundaria_tower_6/shapes.table.lz"
	.balign 4, 0
.LField_TundariaTower6_GRID:
	.incbin "games/tla/src/map/tundaria_tower_6/map.grid.lz"
	.balign 4, 0
.LField_TundariaTower6_SHAPE_GRID:
	.incbin "games/tla/src/map/tundaria_tower_6/shape_grid.plane.lz"
	.balign 4, 0
.LField_TundariaTower6_ANIMATION:
	.incbin "games/tla/src/map/tundaria_tower_3/animation.script.lz"
	.balign 4, 0
.LField_TundariaTower6_BLEND:
	.incbin "games/tla/src/map/tundaria_tower_reflux/blend.script.lz"
	.balign 4, 0
.LField_TundariaTower6_SCENE:
	.incbin "games/tla/src/map/tundaria_tower_6/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteTundariaTower6
Tileset_PaletteTundariaTower6:
	.incbin "games/tla/src/graphics/tileset/palettes/tundaria_tower_6.gbapal.lz"
