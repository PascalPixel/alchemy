	.global Field_Garoh1
Field_Garoh1:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Garoh1, .LField_Garoh1_CELL_PICTURES
	map_block Field_Garoh1, .LField_Garoh1_SHAPES
	map_block Field_Garoh1, .LField_Garoh1_GRID
	map_block Field_Garoh1, .LField_Garoh1_SHAPE_GRID
	map_block Field_Garoh1, .LField_Garoh1_ANIMATION
	map_block Field_Garoh1, .LField_Garoh1_BLEND
	map_block Field_Garoh1, .LField_Garoh1_SCENE
.LField_Garoh1_CELL_PICTURES:
	.incbin "games/tla/src/map/garoh_1/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Garoh1_SHAPES:
	.incbin "games/tla/src/map/garoh_1/shapes.table.lz"
	.balign 4, 0
.LField_Garoh1_GRID:
	.incbin "games/tla/src/map/garoh_1/map.grid.lz"
	.balign 4, 0
.LField_Garoh1_SHAPE_GRID:
	.incbin "games/tla/src/map/garoh_1/shape_grid.plane.lz"
	.balign 4, 0
.LField_Garoh1_ANIMATION:
	.incbin "games/tla/src/map/garoh_1/animation.script.lz"
	.balign 4, 0
.LField_Garoh1_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_Garoh1_SCENE:
	.incbin "games/tla/src/map/garoh_1/scene.scene.lz"
	.global Tileset_PaletteGaroh1
Tileset_PaletteGaroh1:
	.incbin "games/tla/src/graphics/tileset/palettes/garoh_1.gbapal.lz"
	.byte 0x0e	@ PACKER: left in the buffer by Field_Garoh1+0x173
