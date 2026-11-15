	.global Field_Garoh5
Field_Garoh5:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Garoh5, .LField_Garoh5_CELL_PICTURES
	map_block Field_Garoh5, .LField_Garoh5_SHAPES
	map_block Field_Garoh5, .LField_Garoh5_GRID
	map_block Field_Garoh5, .LField_Garoh5_SHAPE_GRID
	map_block Field_Garoh5, .LField_Garoh5_ANIMATION
	map_block Field_Garoh5, .LField_Garoh5_BLEND
	map_block Field_Garoh5, .LField_Garoh5_SCENE
.LField_Garoh5_CELL_PICTURES:
	.incbin "games/tla/src/map/garoh_5/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Garoh5_SHAPES:
	.incbin "games/tla/src/map/garoh_5/shapes.table.lz"
	.balign 4, 0
.LField_Garoh5_GRID:
	.incbin "games/tla/src/map/garoh_5/map.grid.lz"
	.balign 4, 0
.LField_Garoh5_SHAPE_GRID:
	.incbin "games/tla/src/map/garoh_5/shape_grid.plane.lz"
	.balign 4, 0
.LField_Garoh5_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_2/animation.script.lz"
	.balign 4, 0
.LField_Garoh5_BLEND:
	.incbin "games/tla/src/map/garoh_4/blend.script.lz"
	.balign 4, 0
.LField_Garoh5_SCENE:
	.incbin "games/tla/src/map/garoh_5/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteGaroh5A
Tileset_PaletteGaroh5A:
	.incbin "games/tla/src/graphics/tileset/palettes/garoh_5_a.gbapal.lz"
	.global Tileset_PaletteGaroh5B
Tileset_PaletteGaroh5B:
	.incbin "games/tla/src/graphics/tileset/palettes/garoh_5_b.gbapal.lz"
	.byte 0x33, 0xbc, 0x5a	@ PACKER: left in the buffer by Field_Garoh5+0x13d
