	.global Field_Prox4
Field_Prox4:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Prox4, .LField_Prox4_CELL_PICTURES
	map_block Field_Prox4, .LField_Prox4_SHAPES
	map_block Field_Prox4, .LField_Prox4_GRID
	map_block Field_Prox4, .LField_Prox4_SHAPE_GRID
	map_block Field_Prox4, .LField_Prox4_ANIMATION
	map_block Field_Prox4, .LField_Prox4_BLEND
	map_block Field_Prox4, .LField_Prox4_SCENE
.LField_Prox4_CELL_PICTURES:
	.incbin "games/tla/src/map/prox_4/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Prox4_SHAPES:
	.incbin "games/tla/src/map/prox_4/shapes.table.lz"
	.balign 4, 0
.LField_Prox4_GRID:
	.incbin "games/tla/src/map/prox_4/map.grid.lz"
	.balign 4, 0
.LField_Prox4_SHAPE_GRID:
	.incbin "games/tla/src/map/prox_4/shape_grid.plane.lz"
	.balign 4, 0
.LField_Prox4_ANIMATION:
	.incbin "games/tla/src/map/settlements_8/animation.script.lz"
	.balign 4, 0
.LField_Prox4_BLEND:
	.incbin "games/tla/src/map/daila_interiors_1/blend.script.lz"
	.balign 4, 0
.LField_Prox4_SCENE:
	.incbin "games/tla/src/map/prox_4/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteProx4A
Tileset_PaletteProx4A:
	.incbin "games/tla/src/graphics/tileset/palettes/prox_4_a.gbapal.lz"
	.byte 0x0f	@ PACKER: left in the buffer by Field_Prox4+0x1b7
	.global Tileset_PaletteProx4B
Tileset_PaletteProx4B:
	.incbin "games/tla/src/graphics/tileset/palettes/prox_4_b.gbapal.lz"
	.byte 0xb4	@ PACKER: left in the buffer by Tileset_PaletteProx4A+0x187
