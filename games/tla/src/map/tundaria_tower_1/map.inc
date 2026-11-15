	.global Field_TundariaTower1
Field_TundariaTower1:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_TundariaTower1, .LField_TundariaTower1_CELL_PICTURES
	map_block Field_TundariaTower1, .LField_TundariaTower1_SHAPES
	map_block Field_TundariaTower1, .LField_TundariaTower1_GRID
	map_block Field_TundariaTower1, .LField_TundariaTower1_SHAPE_GRID
	map_block Field_TundariaTower1, .LField_TundariaTower1_ANIMATION
	map_block Field_TundariaTower1
	map_block Field_TundariaTower1, .LField_TundariaTower1_SCENE
.LField_TundariaTower1_CELL_PICTURES:
	.incbin "games/tla/src/map/tundaria_tower_1/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_TundariaTower1_SHAPES:
	.incbin "games/tla/src/map/tundaria_tower_1/shapes.table.lz"
	.balign 4, 0
.LField_TundariaTower1_GRID:
	.incbin "games/tla/src/map/tundaria_tower_1/map.grid.lz"
	.balign 4, 0
.LField_TundariaTower1_SHAPE_GRID:
	.incbin "games/tla/src/map/tundaria_tower_1/shape_grid.plane.lz"
	.balign 4, 0
.LField_TundariaTower1_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_2/animation.script.lz"
	.balign 4, 0
.LField_TundariaTower1_SCENE:
	.incbin "games/tla/src/map/tundaria_tower_1/scene.scene.lz"
	.balign 4, 0
	.global Tileset_TundariaTower1Palette
Tileset_TundariaTower1Palette:
	.incbin "games/tla/src/graphics/tileset/tundaria_tower_1/palette.gbapal.lz"
	.byte 0xe6	@ PACKER: left in the buffer by Field_TundariaTower1+0x16b
	.global Tileset_TundariaTower1TilesA
Tileset_TundariaTower1TilesA:
	.incbin "games/tla/src/graphics/tileset/tundaria_tower_1/tiles_a.4bpp.mtf"
	.byte 0xf8, 0x61	@ PACKER: left in the buffer by Field_TundariaTower6+0xfbe
