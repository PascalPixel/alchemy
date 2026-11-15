	.global Field_Contigo1
Field_Contigo1:
	map_header 0, 0, 128, 128, 3, 1, 1, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 1, 1, 0x1f, 0x1f
	map_block Field_Contigo1, .LField_Contigo1_CELL_PICTURES
	map_block Field_Contigo1, .LField_Contigo1_SHAPES
	map_block Field_Contigo1, .LField_Contigo1_GRID
	map_block Field_Contigo1, .LField_Contigo1_SHAPE_GRID
	map_block Field_Contigo1, .LField_Contigo1_ANIMATION
	map_block Field_Contigo1, .LField_Contigo1_BLEND
	map_block Field_Contigo1, .LField_Contigo1_SCENE
.LField_Contigo1_CELL_PICTURES:
	.incbin "games/tla/src/map/contigo_1/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Contigo1_SHAPES:
	.incbin "games/tla/src/map/contigo_1/shapes.table.lz"
	.balign 4, 0
.LField_Contigo1_GRID:
	.incbin "games/tla/src/map/contigo_1/map.grid.lz"
	.balign 4, 0
.LField_Contigo1_SHAPE_GRID:
	.incbin "games/tla/src/map/contigo_1/shape_grid.plane.lz"
	.balign 4, 0
.LField_Contigo1_ANIMATION:
	.incbin "games/tla/src/map/contigo_1/animation.script.lz"
	.balign 4, 0
.LField_Contigo1_BLEND:
	.incbin "games/tla/src/map/contigo_1/blend.script.lz"
	.balign 4, 0
.LField_Contigo1_SCENE:
	.incbin "games/tla/src/map/contigo_1/scene.scene.lz"
	.global Tileset_Contigo1Palette
Tileset_Contigo1Palette:
	.incbin "games/tla/src/graphics/tileset/contigo_1/palette.gbapal.lz"
	.byte 0xed	@ PACKER: left in the buffer by Field_Contigo1+0x193
	.global Tileset_Contigo1TilesA
Tileset_Contigo1TilesA:
	.incbin "games/tla/src/graphics/tileset/contigo_1/tiles_a.4bpp.mtf"
	.balign 4, 0xff	@ PACKER: 0xff fill, not an earlier stream's bytes
