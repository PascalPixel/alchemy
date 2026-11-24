	.global Field_GabombaStatueArrows
Field_GabombaStatueArrows:
	map_header 0, 0, 100, 80, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GabombaStatueArrows, .LField_GabombaStatueArrows_CELL_TILES
	map_block Field_GabombaStatueArrows, .LField_GabombaStatueArrows_SHAPES
	map_block Field_GabombaStatueArrows, .LField_GabombaStatueArrows_GRID
	map_block Field_GabombaStatueArrows, .LField_GabombaStatueArrows_SHAPE_GRID
	map_block Field_GabombaStatueArrows, .LField_GabombaStatueArrows_ANIMATION
	map_block Field_GabombaStatueArrows, .LField_GabombaStatueArrows_BLEND
	map_block Field_GabombaStatueArrows, .LField_GabombaStatueArrows_SCENE
.LField_GabombaStatueArrows_CELL_TILES:
	.incbin "games/tla/src/map/gabomba_statue_arrows/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_GabombaStatueArrows_SHAPES:
	.incbin "games/tla/src/map/gabomba_statue_arrows/shapes.table.lz"
	.balign 4, 0
.LField_GabombaStatueArrows_GRID:
	.incbin "games/tla/src/map/gabomba_statue_arrows/map.grid.lz"
	.balign 4, 0
.LField_GabombaStatueArrows_SHAPE_GRID:
	.incbin "games/tla/src/map/gabomba_statue_arrows/shape_grid.plane.lz"
	.balign 4, 0
.LField_GabombaStatueArrows_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_GabombaStatueArrows_BLEND:
	.incbin "games/tla/src/map/gabomba_statue_arrows/blend.script.lz"
	.balign 4, 0
.LField_GabombaStatueArrows_SCENE:
	.incbin "games/tla/src/map/gabomba_statue_arrows/scene.scene.lz"
	.balign 4, 0
	.global Tileset_GabombaStatueArrowsPalette
Tileset_GabombaStatueArrowsPalette:
	.incbin "games/tla/src/graphics/tileset/gabomba_statue_arrows/palette.gbapal.lz"
	.byte 0x11, 0x04	@ PACKER: left in the buffer by Field_GabombaStatue6+0xfa
	.global Tileset_GabombaStatueArrowsTilesA
Tileset_GabombaStatueArrowsTilesA:
	.incbin "games/tla/src/graphics/tileset/gabomba_statue_arrows/tiles_a.4bpp.mtf"
	.byte 0x00	@ PACKER: left in the buffer by Field_KibomboHouses+0x1e57
	.global Tileset_GabombaStatueArrowsTilesB
Tileset_GabombaStatueArrowsTilesB:
	.incbin "games/tla/src/graphics/tileset/gabomba_statue_arrows/tiles_b.4bpp.mtf"
	.byte 0x27, 0xe0, 0xfb	@ PACKER: left in the buffer by Tileset_GabombaStatueArrowsTilesA+0xe0d
	.global Tileset_GabombaStatueArrowsTilesC
Tileset_GabombaStatueArrowsTilesC:
	.incbin "games/tla/src/graphics/tileset/gabomba_statue_arrows/tiles_c.4bpp.mtf"
	.global Tileset_GabombaStatueArrowsTilesD
Tileset_GabombaStatueArrowsTilesD:
	.incbin "games/tla/src/graphics/tileset/gabomba_statue_arrows/tiles_d.4bpp.mtf"
