	.global Field_ValeCaveKite
Field_ValeCaveKite:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_ValeCaveKite, .LField_ValeCaveKite_CELL_PICTURES
	map_offset Field_ValeCaveKite, .LField_ValeCaveKite_SHAPES
	map_offset Field_ValeCaveKite, .LField_ValeCaveKite_GRID
	map_offset Field_ValeCaveKite, .LField_ValeCaveKite_ANIMATION
	map_offset Field_ValeCaveKite, .LField_ValeCaveKite_BLEND
	map_offset Field_ValeCaveKite, .LField_ValeCaveKite_MARKERS
.LField_ValeCaveKite_CELL_PICTURES:
	.incbin "games/tbs/src/map/vale_cave_kite/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_ValeCaveKite_SHAPES:
	.incbin "games/tbs/src/map/vale_cave_kite/shapes.table.lz"
	.balign 4, 0
.LField_ValeCaveKite_GRID:
	.incbin "games/tbs/src/map/vale_cave_kite/map.grid.lz"
	.balign 4, 0
.LField_ValeCaveKite_ANIMATION:
	.incbin "games/tbs/src/map/vale_cave_kite/tileset_anims.script.lz"
	.balign 4, 0
.LField_ValeCaveKite_BLEND:
	.incbin "games/tbs/src/map/vale_cave_kite/blend.script.lz"
	.balign 4, 0
.LField_ValeCaveKite_MARKERS:
	treasure_spot 51, 6, 100
	treasure_spot 46, 10, 101
	treasure_spots_end
	.balign 4, 0
	.global Tileset_ValeCaveKitePalette
Tileset_ValeCaveKitePalette:
	.incbin "games/tbs/src/graphics/tileset/vale_cave_kite/palette.gbapal.lz"
	.global Tileset_ValeCaveKiteTilesA
Tileset_ValeCaveKiteTilesA:
	.incbin "games/tbs/src/graphics/tileset/vale_cave_kite/tiles_a.4bpp.mtf"
	.byte 0x04, 0x1c, 0xf2	@ PACKER: left in the buffer by Tileset_ValeCaveMouthTilesA+0x2b3d
	.global Tileset_ValeCaveKiteTilesB
Tileset_ValeCaveKiteTilesB:
	.incbin "games/tbs/src/graphics/tileset/vale_cave_kite/tiles_b.4bpp.mtf"
	.byte 0xf9	@ PACKER: left in the buffer by Tileset_ValeCaveKiteTilesA+0x1dcf
	.global Tileset_ValeCaveKiteTilesC
Tileset_ValeCaveKiteTilesC:
	.incbin "games/tbs/src/graphics/tileset/vale_cave_kite/tiles_c.4bpp.mtf"
	.byte 0xf0	@ PACKER: left in the buffer by Tileset_ValeCaveKiteTilesB+0x13f
	.balign 4, 0
