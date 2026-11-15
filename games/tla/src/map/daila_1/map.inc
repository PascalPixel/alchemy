	.global Field_Daila1
Field_Daila1:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Daila1, .LField_Daila1_CELL_PICTURES
	map_block Field_Daila1, .LField_Daila1_SHAPES
	map_block Field_Daila1, .LField_Daila1_GRID
	map_block Field_Daila1, .LField_Daila1_SHAPE_GRID
	map_block Field_Daila1, .LField_Daila1_ANIMATION
	map_block Field_Daila1
	map_block Field_Daila1, .LField_Daila1_SCENE
.LField_Daila1_CELL_PICTURES:
	.incbin "games/tla/src/map/daila_1/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Daila1_SHAPES:
	.incbin "games/tla/src/map/daila_1/shapes.table.lz"
	.balign 4, 0
.LField_Daila1_GRID:
	.incbin "games/tla/src/map/daila_1/map.grid.lz"
	.balign 4, 0
.LField_Daila1_SHAPE_GRID:
	.incbin "games/tla/src/map/daila_1/shape_grid.plane.lz"
	.balign 4, 0
.LField_Daila1_ANIMATION:
	.incbin "games/tla/src/map/daila_1/animation.script.lz"
	.balign 4, 0
.LField_Daila1_SCENE:
	.incbin "games/tla/src/map/daila_1/scene.scene.lz"
	.global Tileset_Daila1Palette
Tileset_Daila1Palette:
	.incbin "games/tla/src/graphics/tileset/daila_1/palette.gbapal.lz"
	.byte 0x81, 0xc2	@ PACKER: left in the buffer by Field_Daila1+0x196
	.global Tileset_Daila1TilesA
Tileset_Daila1TilesA:
	.incbin "games/tla/src/graphics/tileset/daila_1/tiles_a.4bpp.mtf"
	.global Tileset_Daila1TilesB
Tileset_Daila1TilesB:
	.incbin "games/tla/src/graphics/tileset/daila_1/tiles_b.4bpp.mtf"
	.byte 0x00	@ PACKER: left in the buffer by Tileset_Daila1TilesA+0x270b
	.global Tileset_Daila1TilesC
Tileset_Daila1TilesC:
	.incbin "games/tla/src/graphics/tileset/daila_1/tiles_c.4bpp.mtf"
	.byte 0xc1	@ PACKER: left in the buffer by Tileset_Daila1TilesA+0x2943
	.global Tileset_Daila1TilesD
Tileset_Daila1TilesD:
	.incbin "games/tla/src/graphics/tileset/daila_1/tiles_d.4bpp.mtf"
	.byte 0x67, 0x14	@ PACKER: left in the buffer by Tileset_Daila1TilesC+0x14d2
