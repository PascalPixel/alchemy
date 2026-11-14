	.global Field_ValeCaveMouth
Field_ValeCaveMouth:
	map_header 0, 0, 42, 54, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_ValeCaveMouth, .LField_ValeCaveMouth_CELL_PICTURES
	map_offset Field_ValeCaveMouth, .LField_ValeCaveMouth_SHAPES
	map_offset Field_ValeCaveMouth, .LField_ValeCaveMouth_GRID
	map_offset Field_ValeCaveMouth, .LField_ValeCaveMouth_ANIMATION
	map_offset Field_ValeCaveMouth, .LField_ValeCaveMouth_BLEND
	map_offset Field_ValeCaveMouth, .LField_ValeCaveMouth_MARKERS
.LField_ValeCaveMouth_CELL_PICTURES:
	.incbin "games/tbs/src/map/vale_cave_mouth/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_ValeCaveMouth_SHAPES:
	.incbin "games/tbs/src/map/vale_cave_mouth/shapes.table.lz"
	.balign 4, 0
.LField_ValeCaveMouth_GRID:
	.incbin "games/tbs/src/map/vale_cave_mouth/map.grid.lz"
	.balign 4, 0
.LField_ValeCaveMouth_ANIMATION:
	.incbin "games/tbs/src/map/vale_cave_mouth/tileset_anims.script.lz"
	.balign 4, 0
.LField_ValeCaveMouth_BLEND:
	.incbin "games/tbs/src/map/goma_cave_entrance/blend.script.lz"
	.balign 4, 0
.LField_ValeCaveMouth_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_ValeCaveMouthPalette
Tileset_ValeCaveMouthPalette:
	.incbin "games/tbs/src/graphics/tileset/vale_cave_mouth/palette.gbapal.lz"
	.byte 0x98, 0x49	@ PACKER: left in the buffer by Field_ValeCaveMouth+0x19a
	.global Tileset_ValeCaveMouthTilesA
Tileset_ValeCaveMouthTilesA:
	.incbin "games/tbs/src/graphics/tileset/vale_cave_mouth/tiles_a.4bpp.mtf"
	.byte 0xdc, 0x06, 0xaa	@ PACKER: left in the buffer by Field_VaultCaveMouth+0x2bad
	.global Tileset_ValeCaveMouthTilesB
Tileset_ValeCaveMouthTilesB:
	.incbin "games/tbs/src/graphics/tileset/vale_cave_mouth/tiles_b.4bpp.mtf"
	.byte 0xf1, 0xe8, 0x05	@ PACKER: left in the buffer by Tileset_ValeCaveMouthTilesA+0x20d1
	.global Tileset_ValeCaveMouthTilesC
Tileset_ValeCaveMouthTilesC:
	.incbin "games/tbs/src/graphics/tileset/vale_cave_mouth/tiles_c.4bpp.mtf"
	.byte 0x83, 0x38	@ PACKER: left in the buffer by Tileset_ValeCaveMouthTilesB+0x2056
	.global Tileset_ValeCaveMouthTilesD
Tileset_ValeCaveMouthTilesD:
	.incbin "games/tbs/src/graphics/tileset/vale_cave_mouth/tiles_d.4bpp.mtf"
	.byte 0x7c, 0x2f	@ PACKER: left in the buffer by Tileset_ValeCaveMouthTilesC+0x1cea
	.balign 4, 0
