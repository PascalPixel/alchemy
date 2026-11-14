	.global Field_Kalay
Field_Kalay:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Kalay, .LField_Kalay_CELL_PICTURES
	map_offset Field_Kalay, .LField_Kalay_SHAPES
	map_offset Field_Kalay, .LField_Kalay_GRID
	map_offset Field_Kalay, .LField_Kalay_ANIMATION
	map_offset Field_Kalay, .LField_Kalay_BLEND
	map_offset Field_Kalay, .LField_Kalay_MARKERS
.LField_Kalay_CELL_PICTURES:
	.incbin "games/tbs/src/map/kalay/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Kalay_SHAPES:
	.incbin "games/tbs/src/map/kalay/shapes.table.lz"
	.balign 4, 0
.LField_Kalay_GRID:
	.incbin "games/tbs/src/map/kalay/map.grid.lz"
	.balign 4, 0
.LField_Kalay_ANIMATION:
	.incbin "games/tbs/src/map/kalay/tileset_anims.script.lz"
	.balign 4, 0
.LField_Kalay_BLEND:
	.incbin "games/tbs/src/map/goma_cave_entrance/blend.script.lz"
	.balign 4, 0
.LField_Kalay_MARKERS:
	treasure_spot 6, 8, 100
	treasure_spot 5, 25, 101
	treasure_spots_end
	.balign 4, 0
	.global Tileset_KalayPalette
Tileset_KalayPalette:
	.incbin "games/tbs/src/graphics/tileset/kalay/palette.gbapal.lz"
	.byte 0x94	@ PACKER: left in the buffer by Field_Kalay+0x17b
	.balign 4, 0
	.global Tileset_KalayTilesA
Tileset_KalayTilesA:
	.incbin "games/tbs/src/graphics/tileset/kalay/tiles_a.4bpp.mtf"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global Tileset_KalayTilesB
Tileset_KalayTilesB:
	.incbin "games/tbs/src/graphics/tileset/kalay/tiles_b.4bpp.mtf"
	.byte 0xd1, 0xbc	@ PACKER: left in the buffer by Field_LamakanDesertManticore+0x2776
	.global Tileset_KalayTilesC
Tileset_KalayTilesC:
	.incbin "games/tbs/src/graphics/tileset/kalay/tiles_c.4bpp.mtf"
	.byte 0x24, 0xda, 0xbb	@ PACKER: left in the buffer by Tileset_KalayTilesB+0x1865
	.global Tileset_KalayTilesD
Tileset_KalayTilesD:
	.incbin "games/tbs/src/graphics/tileset/kalay/tiles_d.4bpp.mtf"
	.balign 4, 0
