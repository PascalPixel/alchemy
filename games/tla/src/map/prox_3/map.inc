	.global Field_Prox3
Field_Prox3:
	map_header 0, 0, 90, 66, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 90, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 66, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Prox3, .LField_Prox3_CELL_PICTURES
	map_block Field_Prox3, .LField_Prox3_SHAPES
	map_block Field_Prox3, .LField_Prox3_GRID
	map_block Field_Prox3, .LField_Prox3_SHAPE_GRID
	map_block Field_Prox3, .LField_Prox3_ANIMATION
	map_block Field_Prox3, .LField_Prox3_BLEND
	map_block Field_Prox3, .LField_Prox3_SCENE
.LField_Prox3_CELL_PICTURES:
	.incbin "games/tla/src/map/prox_3/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Prox3_SHAPES:
	.incbin "games/tla/src/map/prox_3/shapes.table.lz"
	.balign 4, 0
.LField_Prox3_GRID:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/prox_3/map_ja.grid.lz"
	.else
	.incbin "games/tla/src/map/prox_3/map.grid.lz"
	.endif
	.balign 4, 0
.LField_Prox3_SHAPE_GRID:
	.incbin "games/tla/src/map/prox_3/shape_grid.plane.lz"
	.balign 4, 0
.LField_Prox3_ANIMATION:
	.incbin "games/tla/src/map/prox_3/animation.script.lz"
	.balign 4, 0
.LField_Prox3_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_Prox3_SCENE:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/prox_3/scene_ja.scene.lz"
	.else
	.incbin "games/tla/src/map/prox_3/scene.scene.lz"
	.endif
	.balign 4, 0
	.global Tileset_PaletteProx3
Tileset_PaletteProx3:
	.incbin "games/tla/src/graphics/tileset/palettes/prox_3.gbapal.lz"
	.byte 0x98, 0x2d	@ PACKER: left in the buffer by Field_Prox3+0x162
