	.global Field_Kolima
Field_Kolima:
	map_header 0, 0, 58, 60, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Kolima, .LField_Kolima_CELL_PICTURES
	map_offset Field_Kolima, .LField_Kolima_SHAPES
	map_offset Field_Kolima, .LField_Kolima_GRID
	map_offset Field_Kolima, .LField_Kolima_ANIMATION
	map_offset Field_Kolima, .LField_Kolima_BLEND
	map_offset Field_Kolima, .LField_Kolima_MARKERS
.LField_Kolima_CELL_PICTURES:
	.incbin "games/tbs/src/map/kolima/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Kolima_SHAPES:
	.incbin "games/tbs/src/map/kolima/shapes.table.lz"
	.balign 4, 0
.LField_Kolima_GRID:
	.incbin "games/tbs/src/map/kolima/map.grid.lz"
	.balign 4, 0
.LField_Kolima_ANIMATION:
	.incbin "games/tbs/src/map/kolima/tileset_anims.script.lz"
	.balign 4, 0
.LField_Kolima_BLEND:
	.incbin "games/tbs/src/map/goma_cave_entrance/blend.script.lz"
	.balign 4, 0
.LField_Kolima_MARKERS:
	treasure_spot 23, 8, 100
	treasure_spot 13, 23, 101
	treasure_spots_end
	.balign 4, 0
	.global Tileset_KolimaPalette
Tileset_KolimaPalette:
	.incbin "games/tbs/src/graphics/tileset/kolima/palette.gbapal.lz"
	.byte 0xc0, 0x23	@ PACKER: left in the buffer by Field_Kolima+0x182
	.global Tileset_KolimaTilesA
Tileset_KolimaTilesA:
	.incbin "games/tbs/src/graphics/tileset/kolima/tiles_a.4bpp.mtf"
	.global Tileset_KolimaTilesB
Tileset_KolimaTilesB:
	.incbin "games/tbs/src/graphics/tileset/kolima/tiles_b.4bpp.mtf"
	.global Tileset_KolimaTilesC
Tileset_KolimaTilesC:
	.incbin "games/tbs/src/graphics/tileset/kolima/tiles_c.4bpp.mtf"
	.byte 0xba	@ PACKER: left in the buffer by Tileset_KolimaTilesA+0x2847
	.global Tileset_KolimaTilesD
Tileset_KolimaTilesD:
	.incbin "games/tbs/src/graphics/tileset/kolima/tiles_d.4bpp.mtf"
	.byte 0x68, 0x18, 0x2a	@ PACKER: left in the buffer by Tileset_KolimaTilesC+0x509
	.balign 4, 0
