	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_MogallForestSign
Field_MogallForestSign:
	map_header 0, 0, 106, 80, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 108, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 90, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_MogallForestSign, .LField_MogallForestSign_CELL_PICTURES
	map_offset Field_MogallForestSign, .LField_MogallForestSign_SHAPES
	map_offset Field_MogallForestSign, .LField_MogallForestSign_GRID
	map_offset Field_MogallForestSign, .LField_MogallForestSign_ANIMATION
	map_offset Field_MogallForestSign
	map_offset Field_MogallForestSign, .LField_MogallForestSign_MARKERS
.LField_MogallForestSign_CELL_PICTURES:
	.incbin "games/tbs/src/map/mogall_forest_sign/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_MogallForestSign_SHAPES:
	.incbin "games/tbs/src/map/mogall_forest_sign/shapes.table.lz"
	.balign 4, 0
.LField_MogallForestSign_GRID:
	.incbin "games/tbs/src/map/mogall_forest_sign/map.grid.lz"
	.balign 4, 0
.LField_MogallForestSign_ANIMATION:
	.incbin "games/tbs/src/map/mogall_forest_sign/tileset_anims.script.lz"
	.balign 4, 0
.LField_MogallForestSign_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_MogallForestPalette
Tileset_MogallForestPalette:
	.incbin "games/tbs/src/graphics/tileset/mogall_forest/palette.gbapal.lz"
	.byte 0xcc	@ PACKER: left in the buffer by Field_MogallForestSign+0x16b
	.global Tileset_MogallForestTilesA
Tileset_MogallForestTilesA:
	.incbin "games/tbs/src/graphics/tileset/mogall_forest/tiles_a.4bpp.mtf"
	.byte 0x63	@ PACKER: left in the buffer by Field_MogallForestSign+0x2d9b
	.global Tileset_MogallForestTilesB
Tileset_MogallForestTilesB:
	.incbin "games/tbs/src/graphics/tileset/mogall_forest/tiles_b.4bpp.mtf"
	.byte 0x10, 0xa1	@ PACKER: left in the buffer by Tileset_MogallForestTilesA+0x218e
	.global Tileset_MogallForestTilesC
Tileset_MogallForestTilesC:
	.incbin "games/tbs/src/graphics/tileset/mogall_forest/tiles_c.4bpp.mtf"
	.byte 0xe5, 0xa2	@ PACKER: left in the buffer by Tileset_MogallForestTilesB+0x706
	.global Tileset_MogallForestTilesD
Tileset_MogallForestTilesD:
	.incbin "games/tbs/src/graphics/tileset/mogall_forest/tiles_d.4bpp.mtf"
	.byte 0x0a, 0x07	@ PACKER: left in the buffer by Tileset_MogallForestTilesB+0xc12
