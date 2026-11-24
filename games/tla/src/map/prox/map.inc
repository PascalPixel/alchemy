	.global Field_Prox
Field_Prox:
	map_header 0, 0, 90, 72, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 90, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 72, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Prox, .LField_Prox_CELL_TILES
	map_block Field_Prox, .LField_Prox_SHAPES
	map_block Field_Prox, .LField_Prox_GRID
	map_block Field_Prox, .LField_Prox_SHAPE_GRID
	map_block Field_Prox, .LField_Prox_ANIMATION
	map_block Field_Prox, .LField_Prox_BLEND
	map_block Field_Prox, .LField_Prox_SCENE
.LField_Prox_CELL_TILES:
	.incbin "games/tla/src/map/prox/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_Prox_SHAPES:
	.incbin "games/tla/src/map/prox/shapes.table.lz"
	.balign 4, 0
.LField_Prox_GRID:
	.incbin "games/tla/src/map/prox/map.grid.lz"
	.balign 4, 0
.LField_Prox_SHAPE_GRID:
	.incbin "games/tla/src/map/prox/shape_grid.plane.lz"
	.balign 4, 0
.LField_Prox_ANIMATION:
	.incbin "games/tla/src/map/prox_south_gate/animation.script.lz"
	.balign 4, 0
.LField_Prox_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_Prox_SCENE:
	.incbin "games/tla/src/map/prox/scene.scene.lz"
	.global Tileset_PaletteProx
Tileset_PaletteProx:
	.incbin "games/tla/src/graphics/tileset/palettes/prox.gbapal.lz"
	.byte 0xbb, 0x1e	@ PACKER: left in the buffer by Field_Prox2+0x162
