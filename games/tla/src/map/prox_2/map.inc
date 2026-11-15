	.global Field_Prox2
Field_Prox2:
	map_header 0, 0, 90, 72, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 90, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 72, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Prox2, .LField_Prox2_CELL_PICTURES
	map_block Field_Prox2, .LField_Prox2_SHAPES
	map_block Field_Prox2, .LField_Prox2_GRID
	map_block Field_Prox2, .LField_Prox2_SHAPE_GRID
	map_block Field_Prox2, .LField_Prox2_ANIMATION
	map_block Field_Prox2, .LField_Prox2_BLEND
	map_block Field_Prox2, .LField_Prox2_SCENE
.LField_Prox2_CELL_PICTURES:
	.incbin "games/tla/src/map/prox_2/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Prox2_SHAPES:
	.incbin "games/tla/src/map/prox_2/shapes.table.lz"
	.balign 4, 0
.LField_Prox2_GRID:
	.incbin "games/tla/src/map/prox_2/map.grid.lz"
	.balign 4, 0
.LField_Prox2_SHAPE_GRID:
	.incbin "games/tla/src/map/prox_2/shape_grid.plane.lz"
	.balign 4, 0
.LField_Prox2_ANIMATION:
	.incbin "games/tla/src/map/prox_1/animation.script.lz"
	.balign 4, 0
.LField_Prox2_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_Prox2_SCENE:
	.incbin "games/tla/src/map/prox_2/scene.scene.lz"
	.global Tileset_PaletteProx2
Tileset_PaletteProx2:
	.incbin "games/tla/src/graphics/tileset/palettes/prox_2.gbapal.lz"
	.byte 0xbb, 0x1e	@ PACKER: left in the buffer by Field_Prox2+0x162
