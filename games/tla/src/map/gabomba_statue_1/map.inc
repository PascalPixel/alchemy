	.global Field_GabombaStatue1
Field_GabombaStatue1:
	map_header 0, 0, 100, 80, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GabombaStatue1, .LField_GabombaStatue1_CELL_PICTURES
	map_block Field_GabombaStatue1, .LField_GabombaStatue1_SHAPES
	map_block Field_GabombaStatue1, .LField_GabombaStatue1_GRID
	map_block Field_GabombaStatue1, .LField_GabombaStatue1_SHAPE_GRID
	map_block Field_GabombaStatue1, .LField_GabombaStatue1_ANIMATION
	map_block Field_GabombaStatue1, .LField_GabombaStatue1_BLEND
	map_block Field_GabombaStatue1, .LField_GabombaStatue1_SCENE
.LField_GabombaStatue1_CELL_PICTURES:
	.incbin "games/tla/src/map/gabomba_statue_1/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_GabombaStatue1_SHAPES:
	.incbin "games/tla/src/map/gabomba_statue_1/shapes.table.lz"
	.balign 4, 0
.LField_GabombaStatue1_GRID:
	.incbin "games/tla/src/map/gabomba_statue_1/map.grid.lz"
	.balign 4, 0
.LField_GabombaStatue1_SHAPE_GRID:
	.incbin "games/tla/src/map/gabomba_statue_1/shape_grid.plane.lz"
	.balign 4, 0
.LField_GabombaStatue1_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_2/animation.script.lz"
	.balign 4, 0
.LField_GabombaStatue1_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_GabombaStatue1_SCENE:
	.incbin "games/tla/src/map/gabomba_statue_1/scene.scene.lz"
	.balign 4, 0
	.global Tileset_GabombaStatue1Palette
Tileset_GabombaStatue1Palette:
	.incbin "games/tla/src/graphics/tileset/gabomba_statue_1/palette.gbapal.lz"
	.byte 0x9f	@ PACKER: left in the buffer by Field_GabombaStatue1+0xe3
	.global Tileset_GabombaStatue1TilesA
Tileset_GabombaStatue1TilesA:
	.incbin "games/tla/src/graphics/tileset/gabomba_statue_1/tiles_a.4bpp.mtf"
	.byte 0x22, 0xe3, 0x07	@ PACKER: left in the buffer by Field_Kibombo3+0x1949
	.global Tileset_GabombaStatue1TilesB
Tileset_GabombaStatue1TilesB:
	.incbin "games/tla/src/graphics/tileset/gabomba_statue_1/tiles_b.4bpp.mtf"
	.global Tileset_GabombaStatue1TilesC
Tileset_GabombaStatue1TilesC:
	.incbin "games/tla/src/graphics/tileset/gabomba_statue_1/tiles_c.4bpp.mtf"
	.global Tileset_GabombaStatue1TilesD
Tileset_GabombaStatue1TilesD:
	.incbin "games/tla/src/graphics/tileset/gabomba_statue_1/tiles_d.4bpp.mtf"
	.byte 0x20	@ PACKER: left in the buffer by Tileset_GabombaStatue1TilesC+0x13f
