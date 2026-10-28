	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map006
Field_Map006:
	map_header 0, 0, 120, 120, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map006, .LField_Map006_CELL_PICTURES
	map_offset Field_Map006, .LField_Map006_SHAPES
	map_offset Field_Map006, .LField_Map006_GRID
	map_offset Field_Map006, .LField_Map006_ANIMATION
	map_offset Field_Map006, .LField_Map006_BLEND
	map_offset Field_Map006, .LField_Map006_MARKERS
.LField_Map006_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_006/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map006_SHAPES:
	.incbin "games/tbs/src/map/map_006/shapes.table.lz"
	.balign 4, 0
.LField_Map006_GRID:
	.incbin "games/tbs/src/map/map_006/map.grid.lz"
	.balign 4, 0
.LField_Map006_ANIMATION:
	.incbin "games/tbs/src/map/map_006/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map006_BLEND:
	.incbin "games/tbs/src/map/map_005/blend.script.lz"
	.balign 4, 0
.LField_Map006_MARKERS:
	treasure_spot 20, 3, 206
	treasure_spot 21, 3, 206
	treasure_spot 24, 3, 207
	treasure_spot 25, 3, 207
	treasure_spot 26, 3, 207
	treasure_spot 27, 3, 207
	treasure_spot 28, 3, 208
	treasure_spot 29, 3, 208
	treasure_spot 30, 3, 208
	treasure_spot 40, 3, 108
	treasure_spot 20, 4, 206
	treasure_spot 21, 4, 206
	treasure_spot 47, 5, 107
	treasure_spot 42, 6, 109
	treasure_spot 26, 8, 101
	treasure_spot 31, 8, 209
	treasure_spot 32, 8, 209
	treasure_spot 4, 12, 100
	treasure_spot 49, 19, 207
	treasure_spot 50, 19, 207
	treasure_spot 52, 19, 208
	treasure_spot 53, 19, 208
	treasure_spot 55, 20, 209
	treasure_spot 56, 20, 209
	treasure_spot 55, 21, 209
	treasure_spot 56, 21, 209
	treasure_spot 28, 22, 210
	treasure_spot 29, 22, 210
	treasure_spot 30, 22, 210
	treasure_spot 3, 23, 212
	treasure_spot 4, 23, 212
	treasure_spot 54, 24, 103
	treasure_spot 9, 40, 102
	treasure_spot 9, 41, 102
	treasure_spot 31, 41, 211
	treasure_spot 32, 41, 211
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Set05Palette
Tileset_Set05Palette:
	.incbin "games/tbs/src/graphics/tileset/set_05/palette.gbapal.lz"
	.byte 0x64, 0x36, 0x80	@ PACKER: left in the buffer by Field_Map006+0x18d
	.global Tileset_Set05TilesA
Tileset_Set05TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_05/tiles_a.4bpp.mtf"
	.byte 0x86, 0x47	@ PACKER: left in the buffer by Field_Map006+0x1dae
	.global Tileset_Set05TilesB
Tileset_Set05TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_05/tiles_b.4bpp.mtf"
	.global Tileset_Set05TilesC
Tileset_Set05TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_05/tiles_c.4bpp.mtf"
	.byte 0x38, 0xf0, 0x3d	@ PACKER: left in the buffer by Tileset_Set05TilesB+0x1609
	.global Tileset_Set05TilesD
Tileset_Set05TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_05/tiles_d.4bpp.mtf"
	.byte 0x7e	@ PACKER: left in the buffer by Tileset_Set05TilesC+0x150b
