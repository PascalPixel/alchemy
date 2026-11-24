	.global Field_Madra
Field_Madra:
	map_header 0, 0, 100, 68, 3, 1, 1, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 1, -1, 0x3f, 0x3f
	map_block Field_Madra, .LField_Madra_CELL_TILES
	map_block Field_Madra, .LField_Madra_SHAPES
	map_block Field_Madra, .LField_Madra_GRID
	map_block Field_Madra, .LField_Madra_SHAPE_GRID
	map_block Field_Madra, .LField_Madra_ANIMATION
	map_block Field_Madra, .LField_Madra_BLEND
	map_block Field_Madra, .LField_Madra_SCENE
.LField_Madra_CELL_TILES:
	.incbin "games/tla/src/map/madra/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_Madra_SHAPES:
	.incbin "games/tla/src/map/madra/shapes.table.lz"
	.balign 4, 0
.LField_Madra_GRID:
	.incbin "games/tla/src/map/madra/map.grid.lz"
	.balign 4, 0
.LField_Madra_SHAPE_GRID:
	.incbin "games/tla/src/map/madra/shape_grid.plane.lz"
	.balign 4, 0
.LField_Madra_ANIMATION:
	.incbin "games/tla/src/map/madra/animation.script.lz"
	.balign 4, 0
.LField_Madra_BLEND:
	.incbin "games/tla/src/map/madra/blend.script.lz"
	.balign 4, 0
.LField_Madra_SCENE:
	.incbin "games/tla/src/map/madra/scene.scene.lz"
	.global Tileset_MadraPalette
Tileset_MadraPalette:
	.incbin "games/tla/src/graphics/tileset/madra/palette.gbapal.lz"
	.byte 0x12, 0x05, 0x08	@ PACKER: left in the buffer by Field_Madra+0x14d
	.global Tileset_MadraTilesA
Tileset_MadraTilesA:
	.incbin "games/tla/src/graphics/tileset/madra/tiles_a.4bpp.mtf"
	.byte 0x00, 0x02	@ PACKER: left in the buffer by Field_DehkanPlateauTightropes+0x288a
	.global Tileset_MadraTilesB
Tileset_MadraTilesB:
	.incbin "games/tla/src/graphics/tileset/madra/tiles_b.4bpp.mtf"
	.byte 0x08, 0x04	@ PACKER: left in the buffer by Tileset_MadraTilesA+0x27ee
	.global Tileset_MadraTilesC
Tileset_MadraTilesC:
	.incbin "games/tla/src/graphics/tileset/madra/tiles_c.4bpp.mtf"
	.byte 0x41, 0xf2	@ PACKER: left in the buffer by Tileset_MadraTilesB+0xf9e
	.global Tileset_MadraTilesD
Tileset_MadraTilesD:
	.incbin "games/tla/src/graphics/tileset/madra/tiles_d.4bpp.mtf"
	.byte 0x03, 0x86, 0xc0	@ PACKER: left in the buffer by Tileset_MadraTilesB+0x2151
