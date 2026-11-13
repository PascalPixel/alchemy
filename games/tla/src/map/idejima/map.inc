	.global Field_Idejima
Field_Idejima:
	map_header 0, 0, 128, 128, 3, 3, 3, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, -1, -2, 0x1, 0x1
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Idejima, .LField_Idejima_CELL_PICTURES
	map_block Field_Idejima, .LField_Idejima_SHAPES
	map_block Field_Idejima, .LField_Idejima_GRID
	map_block Field_Idejima, .LField_Idejima_SHAPE_GRID
	map_block Field_Idejima, .LField_Idejima_ANIMATION
	map_block Field_Idejima, .LField_Idejima_BLEND
	map_block Field_Idejima, .LField_Idejima_SCENE
.LField_Idejima_CELL_PICTURES:
	.incbin "games/tla/src/map/idejima/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Idejima_SHAPES:
	.incbin "games/tla/src/map/idejima/shapes.table.lz"
	.balign 4, 0
.LField_Idejima_GRID:
	.incbin "games/tla/src/map/idejima/map.grid.lz"
	.balign 4, 0
.LField_Idejima_SHAPE_GRID:
	.incbin "games/tla/src/map/idejima/shape_grid.plane.lz"
	.balign 4, 0
.LField_Idejima_ANIMATION:
	.incbin "games/tla/src/map/idejima/animation.script.lz"
	.balign 4, 0
.LField_Idejima_BLEND:
	.incbin "games/tla/src/map/idejima/blend.script.lz"
	.balign 4, 0
.LField_Idejima_SCENE:
	.incbin "games/tla/src/map/idejima/scene.scene.lz"
	.balign 4, 0
	.global Tileset_IdejimaPalette
Tileset_IdejimaPalette:
	.incbin "games/tla/src/graphics/tileset/idejima/palette.gbapal.lz"
	.byte 0xaa, 0x2e	@ PACKER: left in the buffer by Field_Idejima+0xfe
	.global Tileset_IdejimaTilesA
Tileset_IdejimaTilesA:
	.incbin "games/tla/src/graphics/tileset/idejima/tiles_a.4bpp.mtf"
	.global Tileset_IdejimaTilesB
Tileset_IdejimaTilesB:
	.incbin "games/tla/src/graphics/tileset/idejima/tiles_b.4bpp.mtf"
	.byte 0x04, 0x92, 0x9e	@ PACKER: left in the buffer by Tileset_Set04TilesC+0x1c7d
	.global Tileset_IdejimaTilesC
Tileset_IdejimaTilesC:
	.incbin "games/tla/src/graphics/tileset/idejima/tiles_c.4bpp.mtf"
	.global Tileset_IdejimaTilesD
Tileset_IdejimaTilesD:
	.incbin "games/tla/src/graphics/tileset/idejima/tiles_d.4bpp.mtf"
	.byte 0x22, 0x60, 0x76	@ PACKER: left in the buffer by Tileset_IdejimaTilesC+0x8dd
