	.global Field_GabombaStatue6
Field_GabombaStatue6:
	map_header 0, 0, 100, 80, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GabombaStatue6, .LField_GabombaStatue6_CELL_PICTURES
	map_block Field_GabombaStatue6, .LField_GabombaStatue6_SHAPES
	map_block Field_GabombaStatue6, .LField_GabombaStatue6_GRID
	map_block Field_GabombaStatue6, .LField_GabombaStatue6_SHAPE_GRID
	map_block Field_GabombaStatue6, .LField_GabombaStatue6_ANIMATION
	map_block Field_GabombaStatue6, .LField_GabombaStatue6_BLEND
	map_block Field_GabombaStatue6, .LField_GabombaStatue6_SCENE
.LField_GabombaStatue6_CELL_PICTURES:
	.incbin "games/tla/src/map/gabomba_statue_6/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_GabombaStatue6_SHAPES:
	.incbin "games/tla/src/map/gabomba_statue_6/shapes.table.lz"
	.balign 4, 0
.LField_GabombaStatue6_GRID:
	.incbin "games/tla/src/map/gabomba_statue_6/map.grid.lz"
	.balign 4, 0
.LField_GabombaStatue6_SHAPE_GRID:
	.incbin "games/tla/src/map/gabomba_statue_6/shape_grid.plane.lz"
	.balign 4, 0
.LField_GabombaStatue6_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_2/animation.script.lz"
	.balign 4, 0
.LField_GabombaStatue6_BLEND:
	.incbin "games/tla/src/map/gabomba_statue_6/blend.script.lz"
	.balign 4, 0
.LField_GabombaStatue6_SCENE:
	.incbin "games/tla/src/map/gabomba_statue_6/scene.scene.lz"
	.balign 4, 0
	.global Tileset_GabombaStatue6Palette
Tileset_GabombaStatue6Palette:
	.incbin "games/tla/src/graphics/tileset/gabomba_statue_6/palette.gbapal.lz"
	.byte 0x11, 0x04	@ PACKER: left in the buffer by Field_GabombaStatue6+0xfa
	.global Tileset_GabombaStatue6TilesA
Tileset_GabombaStatue6TilesA:
	.incbin "games/tla/src/graphics/tileset/gabomba_statue_6/tiles_a.4bpp.mtf"
	.byte 0x00	@ PACKER: left in the buffer by Field_Kibombo3+0x1e57
	.global Tileset_GabombaStatue6TilesB
Tileset_GabombaStatue6TilesB:
	.incbin "games/tla/src/graphics/tileset/gabomba_statue_6/tiles_b.4bpp.mtf"
	.byte 0x27, 0xe0, 0xfb	@ PACKER: left in the buffer by Tileset_GabombaStatue6TilesA+0xe0d
	.global Tileset_GabombaStatue6TilesC
Tileset_GabombaStatue6TilesC:
	.incbin "games/tla/src/graphics/tileset/gabomba_statue_6/tiles_c.4bpp.mtf"
	.global Tileset_GabombaStatue6TilesD
Tileset_GabombaStatue6TilesD:
	.incbin "games/tla/src/graphics/tileset/gabomba_statue_6/tiles_d.4bpp.mtf"
