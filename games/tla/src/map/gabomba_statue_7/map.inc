	.global Field_GabombaStatue7
Field_GabombaStatue7:
	map_header 0, 0, 128, 70, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GabombaStatue7, .LField_GabombaStatue7_CELL_PICTURES
	map_block Field_GabombaStatue7, .LField_GabombaStatue7_SHAPES
	map_block Field_GabombaStatue7, .LField_GabombaStatue7_GRID
	map_block Field_GabombaStatue7, .LField_GabombaStatue7_SHAPE_GRID
	map_block Field_GabombaStatue7, .LField_GabombaStatue7_ANIMATION
	map_block Field_GabombaStatue7, .LField_GabombaStatue7_BLEND
	map_block Field_GabombaStatue7, .LField_GabombaStatue7_SCENE
.LField_GabombaStatue7_CELL_PICTURES:
	.incbin "games/tla/src/map/gabomba_statue_7/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_GabombaStatue7_SHAPES:
	.incbin "games/tla/src/map/gabomba_statue_7/shapes.table.lz"
	.balign 4, 0
.LField_GabombaStatue7_GRID:
	.incbin "games/tla/src/map/gabomba_statue_7/map.grid.lz"
	.balign 4, 0
.LField_GabombaStatue7_SHAPE_GRID:
	.incbin "games/tla/src/map/gabomba_statue_7/shape_grid.plane.lz"
	.balign 4, 0
.LField_GabombaStatue7_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_2/animation.script.lz"
	.balign 4, 0
.LField_GabombaStatue7_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_GabombaStatue7_SCENE:
	.incbin "games/tla/src/map/gabomba_statue_7/scene.scene.lz"
	.global Tileset_GabombaStatue7Palette
Tileset_GabombaStatue7Palette:
	.incbin "games/tla/src/graphics/tileset/gabomba_statue_7/palette.gbapal.lz"
	.byte 0xf0, 0x0c, 0xa6	@ PACKER: left in the buffer by Field_GabombaStatue7+0xc1
	.global Tileset_GabombaStatue7TilesA
Tileset_GabombaStatue7TilesA:
	.incbin "games/tla/src/graphics/tileset/gabomba_statue_7/tiles_a.4bpp.mtf"
	.global Tileset_GabombaStatue7TilesB
Tileset_GabombaStatue7TilesB:
	.incbin "games/tla/src/graphics/tileset/gabomba_statue_7/tiles_b.4bpp.mtf"
	.global Tileset_GabombaStatue7TilesC
Tileset_GabombaStatue7TilesC:
	.incbin "games/tla/src/graphics/tileset/gabomba_statue_7/tiles_c.4bpp.mtf"
	.byte 0xff	@ PACKER: left in the buffer by Tileset_GabombaStatue7TilesB+0x293
	.global Tileset_GabombaStatue7TilesD
Tileset_GabombaStatue7TilesD:
	.incbin "games/tla/src/graphics/tileset/gabomba_statue_7/tiles_d.4bpp.mtf"
	.byte 0xf3	@ PACKER: left in the buffer by Tileset_GabombaStatue7TilesC+0x13f
