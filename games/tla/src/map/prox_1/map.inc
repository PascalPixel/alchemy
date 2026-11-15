	.global Field_Prox1
Field_Prox1:
	map_header 0, 0, 38, 50, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 38, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 50, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Prox1, .LField_Prox1_CELL_PICTURES
	map_block Field_Prox1, .LField_Prox1_SHAPES
	map_block Field_Prox1, .LField_Prox1_GRID
	map_block Field_Prox1, .LField_Prox1_SHAPE_GRID
	map_block Field_Prox1, .LField_Prox1_ANIMATION
	map_block Field_Prox1, .LField_Prox1_BLEND
	map_block Field_Prox1, .LField_Prox1_SCENE
.LField_Prox1_CELL_PICTURES:
	.incbin "games/tla/src/map/prox_1/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Prox1_SHAPES:
	.incbin "games/tla/src/map/prox_1/shapes.table.lz"
	.balign 4, 0
.LField_Prox1_GRID:
	.incbin "games/tla/src/map/prox_1/map.grid.lz"
	.balign 4, 0
.LField_Prox1_SHAPE_GRID:
	.incbin "games/tla/src/map/prox_1/shape_grid.plane.lz"
	.balign 4, 0
.LField_Prox1_ANIMATION:
	.incbin "games/tla/src/map/prox_1/animation.script.lz"
	.balign 4, 0
.LField_Prox1_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_Prox1_SCENE:
	.incbin "games/tla/src/map/prox_1/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Prox1Palette
Tileset_Prox1Palette:
	.incbin "games/tla/src/graphics/tileset/prox_1/palette.gbapal.lz"
	.byte 0x18	@ PACKER: left in the buffer by Field_Prox1+0x15f
	.global Tileset_Prox1TilesA
Tileset_Prox1TilesA:
	.incbin "games/tla/src/graphics/tileset/prox_1/tiles_a.4bpp.mtf"
	.global Tileset_Prox1TilesB
Tileset_Prox1TilesB:
	.incbin "games/tla/src/graphics/tileset/prox_1/tiles_b.4bpp.mtf"
	.byte 0xf6	@ PACKER: left in the buffer by Tileset_Prox1TilesA+0x2173
	.global Tileset_Prox1TilesC
Tileset_Prox1TilesC:
	.incbin "games/tla/src/graphics/tileset/prox_1/tiles_c.4bpp.mtf"
	.byte 0x65, 0xab, 0x31	@ PACKER: left in the buffer by Tileset_Prox1TilesA+0x26bd
	.global Tileset_Prox1TilesD
Tileset_Prox1TilesD:
	.incbin "games/tla/src/graphics/tileset/prox_1/tiles_d.4bpp.mtf"
	.byte 0xe1	@ PACKER: left in the buffer by Tileset_Prox1TilesC+0x5db
