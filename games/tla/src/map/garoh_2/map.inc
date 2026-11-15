	.global Field_Garoh2
Field_Garoh2:
	map_header 0, 0, 128, 128, 2, 3, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 11, 14, 0, 0, 0x1f, 0x1f
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Garoh2, .LField_Garoh2_CELL_PICTURES
	map_block Field_Garoh2, .LField_Garoh2_SHAPES
	map_block Field_Garoh2, .LField_Garoh2_GRID
	map_block Field_Garoh2, .LField_Garoh2_SHAPE_GRID
	map_block Field_Garoh2, .LField_Garoh2_ANIMATION
	map_block Field_Garoh2, .LField_Garoh2_BLEND
	map_block Field_Garoh2, .LField_Garoh2_SCENE
.LField_Garoh2_CELL_PICTURES:
	.incbin "games/tla/src/map/garoh_2/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Garoh2_SHAPES:
	.incbin "games/tla/src/map/garoh_2/shapes.table.lz"
	.balign 4, 0
.LField_Garoh2_GRID:
	.incbin "games/tla/src/map/garoh_2/map.grid.lz"
	.balign 4, 0
.LField_Garoh2_SHAPE_GRID:
	.incbin "games/tla/src/map/garoh_2/shape_grid.plane.lz"
	.balign 4, 0
.LField_Garoh2_ANIMATION:
	.incbin "games/tla/src/map/garoh_2/animation.script.lz"
	.balign 4, 0
.LField_Garoh2_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_Garoh2_SCENE:
	.incbin "games/tla/src/map/garoh_2/scene.scene.lz"
	.global Tileset_PaletteGaroh2
Tileset_PaletteGaroh2:
	.incbin "games/tla/src/graphics/tileset/palettes/garoh_2.gbapal.lz"
	.byte 0x98, 0xc4	@ PACKER: left in the buffer by Field_Garoh2+0x182
