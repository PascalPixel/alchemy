	.global Field_LemuriaEntrance
Field_LemuriaEntrance:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_LemuriaEntrance, .LField_LemuriaEntrance_CELL_PICTURES
	map_block Field_LemuriaEntrance, .LField_LemuriaEntrance_SHAPES
	map_block Field_LemuriaEntrance, .LField_LemuriaEntrance_GRID
	map_block Field_LemuriaEntrance, .LField_LemuriaEntrance_SHAPE_GRID
	map_block Field_LemuriaEntrance, .LField_LemuriaEntrance_ANIMATION
	map_block Field_LemuriaEntrance
	map_block Field_LemuriaEntrance, .LField_LemuriaEntrance_SCENE
.LField_LemuriaEntrance_CELL_PICTURES:
	.incbin "games/tla/src/map/lemuria_entrance/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_LemuriaEntrance_SHAPES:
	.incbin "games/tla/src/map/lemuria_entrance/shapes.table.lz"
	.balign 4, 0
.LField_LemuriaEntrance_GRID:
	.incbin "games/tla/src/map/lemuria_entrance/map.grid.lz"
	.balign 4, 0
.LField_LemuriaEntrance_SHAPE_GRID:
	.incbin "games/tla/src/map/lemuria_entrance/shape_grid.plane.lz"
	.balign 4, 0
.LField_LemuriaEntrance_ANIMATION:
	.incbin "games/tla/src/map/lemuria_entrance/animation.script.lz"
	.balign 4, 0
.LField_LemuriaEntrance_SCENE:
	.incbin "games/tla/src/map/lemuria_entrance/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteLemuriaEntrance
Tileset_PaletteLemuriaEntrance:
	.incbin "games/tla/src/graphics/tileset/palettes/lemuria_entrance.gbapal.lz"
	.byte 0xfc, 0x8c, 0x34	@ PACKER: left in the buffer by Field_LemuriaEntrance+0xf5
