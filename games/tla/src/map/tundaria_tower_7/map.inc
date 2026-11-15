	.global Field_TundariaTower7
Field_TundariaTower7:
	map_header 0, 0, 128, 128, 2, 2, 3, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0x1f, 0x1f
	map_layer 0, 128, 12, 12, 0, 0, 0xff, 0xff
	map_block Field_TundariaTower7, .LField_TundariaTower7_CELL_PICTURES
	map_block Field_TundariaTower7, .LField_TundariaTower7_SHAPES
	map_block Field_TundariaTower7, .LField_TundariaTower7_GRID
	map_block Field_TundariaTower7, .LField_TundariaTower7_SHAPE_GRID
	map_block Field_TundariaTower7, .LField_TundariaTower7_ANIMATION
	map_block Field_TundariaTower7
	map_block Field_TundariaTower7, .LField_TundariaTower7_SCENE
.LField_TundariaTower7_CELL_PICTURES:
	.incbin "games/tla/src/map/tundaria_tower_7/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_TundariaTower7_SHAPES:
	.incbin "games/tla/src/map/tundaria_tower_7/shapes.table.lz"
	.balign 4, 0
.LField_TundariaTower7_GRID:
	.incbin "games/tla/src/map/tundaria_tower_7/map.grid.lz"
	.balign 4, 0
.LField_TundariaTower7_SHAPE_GRID:
	.incbin "games/tla/src/map/tundaria_tower_7/shape_grid.plane.lz"
	.balign 4, 0
.LField_TundariaTower7_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_2/animation.script.lz"
	.balign 4, 0
.LField_TundariaTower7_SCENE:
	.incbin "games/tla/src/map/tundaria_tower_7/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteTundariaTower7
Tileset_PaletteTundariaTower7:
	.incbin "games/tla/src/graphics/tileset/palettes/tundaria_tower_7.gbapal.lz"
