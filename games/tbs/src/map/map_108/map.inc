	.global Field_Map108
Field_Map108:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map108, .LField_Map108_CELL_PICTURES
	map_offset Field_Map108, .LField_Map108_SHAPES
	map_offset Field_Map108, .LField_Map108_GRID
	map_offset Field_Map108, .LField_Map108_ANIMATION
	map_offset Field_Map108, .LField_Map108_BLEND
	map_offset Field_Map108, .LField_Map108_MARKERS
.LField_Map108_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_108/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map108_SHAPES:
	.incbin "games/tbs/src/map/map_108/shapes.table.lz"
	.balign 4, 0
.LField_Map108_GRID:
	.incbin "games/tbs/src/map/map_108/map.grid.lz"
	.balign 4, 0
.LField_Map108_ANIMATION:
	.incbin "games/tbs/src/map/kolima/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map108_BLEND:
	.incbin "games/tbs/src/map/map_108/blend.script.lz"
	.balign 4, 0
.LField_Map108_MARKERS:
	treasure_spot 47, 26, 100
	treasure_spot 41, 29, 103
	treasure_spot 42, 42, 102
	treasure_spot 55, 43, 101
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Map108Palette
Tileset_Map108Palette:
	.incbin "games/tbs/src/graphics/tileset/map_108/palette.gbapal.lz"
	.byte 0xbe, 0xee	@ PACKER: left in the buffer by Field_Map108+0x186
	.global Tileset_Map108TilesA
Tileset_Map108TilesA:
	.incbin "games/tbs/src/graphics/tileset/map_108/tiles_a.4bpp.mtf"
	.global Tileset_Map108TilesB
Tileset_Map108TilesB:
	.incbin "games/tbs/src/graphics/tileset/map_108/tiles_b.4bpp.mtf"
	.byte 0x08, 0xe8	@ PACKER: left in the buffer by Tileset_KalayDocksTilesA+0x2672
	.global Tileset_Map108TilesC
Tileset_Map108TilesC:
	.incbin "games/tbs/src/graphics/tileset/map_108/tiles_c.4bpp.mtf"
	.byte 0xfe	@ PACKER: left in the buffer by Tileset_Map108TilesB+0x153
	.global Tileset_Map108TilesD
Tileset_Map108TilesD:
	.incbin "games/tbs/src/graphics/tileset/map_108/tiles_d.4bpp.mtf"
	.byte 0xff	@ PACKER: left in the buffer by Tileset_Map108TilesC+0x13f
	.balign 4, 0
