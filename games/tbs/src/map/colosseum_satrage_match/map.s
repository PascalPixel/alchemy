	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_ColosseumSatrageMatch
Field_ColosseumSatrageMatch:
	map_header 0, 0, 166, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_ColosseumSatrageMatch, .LField_ColosseumSatrageMatch_CELL_PICTURES
	map_offset Field_ColosseumSatrageMatch, .LField_ColosseumSatrageMatch_SHAPES
	map_offset Field_ColosseumSatrageMatch, .LField_ColosseumSatrageMatch_GRID
	map_offset Field_ColosseumSatrageMatch, .LField_ColosseumSatrageMatch_ANIMATION
	map_offset Field_ColosseumSatrageMatch, .LField_ColosseumSatrageMatch_BLEND
	map_offset Field_ColosseumSatrageMatch, .LField_ColosseumSatrageMatch_MARKERS
.LField_ColosseumSatrageMatch_CELL_PICTURES:
	.incbin "games/tbs/src/map/colosseum_satrage_match/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_ColosseumSatrageMatch_SHAPES:
	.incbin "games/tbs/src/map/colosseum_satrage_match/shapes.table.lz"
	.balign 4, 0
.LField_ColosseumSatrageMatch_GRID:
	.incbin "games/tbs/src/map/colosseum_satrage_match/map.grid.lz"
	.balign 4, 0
.LField_ColosseumSatrageMatch_ANIMATION:
	.incbin "games/tbs/src/map/colosseum_satrage_match/tileset_anims.script.lz"
	.balign 4, 0
.LField_ColosseumSatrageMatch_BLEND:
	.incbin "games/tbs/src/map/goma_cave_entrance/blend.script.lz"
	.balign 4, 0
.LField_ColosseumSatrageMatch_MARKERS:
	treasure_spot 51, 6, 103
	treasure_spot 23, 7, 101
	treasure_spot 56, 7, 104
	treasure_spot 34, 16, 102
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Set61Palette
Tileset_Set61Palette:
	.incbin "games/tbs/src/graphics/tileset/set_61/palette.gbapal.lz"
	.global Tileset_Set61TilesA
Tileset_Set61TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_61/tiles_a.4bpp.mtf"
	.byte 0x91	@ PACKER: left in the buffer by Tileset_Set60TilesA+0x2907
	.global Tileset_Set61TilesB
Tileset_Set61TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_61/tiles_b.4bpp.mtf"
	.byte 0x6d	@ PACKER: left in the buffer by Tileset_Set61TilesA+0x1c4b
	.global Tileset_Set61TilesC
Tileset_Set61TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_61/tiles_c.4bpp.mtf"
	.byte 0x06, 0x21, 0x87	@ PACKER: left in the buffer by Tileset_Set61TilesB+0x1445
	.global Tileset_Set61TilesD
Tileset_Set61TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_61/tiles_d.4bpp.mtf"
	.byte 0x7f, 0x3d, 0x47	@ PACKER: left in the buffer by Tileset_Set61TilesA+0x1e7d
