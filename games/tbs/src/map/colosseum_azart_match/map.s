	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_ColosseumAzartMatch
Field_ColosseumAzartMatch:
	map_header 28, 0, 180, 56, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 56, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 112, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_ColosseumAzartMatch, .LField_ColosseumAzartMatch_CELL_PICTURES
	map_offset Field_ColosseumAzartMatch, .LField_ColosseumAzartMatch_SHAPES
	map_offset Field_ColosseumAzartMatch, .LField_ColosseumAzartMatch_GRID
	map_offset Field_ColosseumAzartMatch, .LField_ColosseumAzartMatch_ANIMATION
	map_offset Field_ColosseumAzartMatch, .LField_ColosseumAzartMatch_BLEND
	map_offset Field_ColosseumAzartMatch, .LField_ColosseumAzartMatch_MARKERS
.LField_ColosseumAzartMatch_CELL_PICTURES:
	.incbin "games/tbs/src/map/colosseum_azart_match/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_ColosseumAzartMatch_SHAPES:
	.incbin "games/tbs/src/map/colosseum_azart_match/shapes.table.lz"
	.balign 4, 0
.LField_ColosseumAzartMatch_GRID:
	.incbin "games/tbs/src/map/colosseum_azart_match/map.grid.lz"
	.balign 4, 0
.LField_ColosseumAzartMatch_ANIMATION:
	.incbin "games/tbs/src/map/colosseum_azart_match/tileset_anims.script.lz"
	.balign 4, 0
.LField_ColosseumAzartMatch_BLEND:
	.incbin "games/tbs/src/map/goma_cave_entrance/blend.script.lz"
	.balign 4, 0
.LField_ColosseumAzartMatch_MARKERS:
	treasure_spot 28, 5, 100
	treasure_spot 48, 6, 101
	treasure_spot 61, 13, 103
	treasure_spots_end
	.global Tileset_ColosseumAzartMatchPalette
Tileset_ColosseumAzartMatchPalette:
	.incbin "games/tbs/src/graphics/tileset/colosseum_azart_match/palette.gbapal.lz"
	.byte 0x31, 0x9d, 0x0f	@ PACKER: left in the buffer by Field_ColosseumAzartMatch+0x14d
	.global Tileset_ColosseumAzartMatchTilesA
Tileset_ColosseumAzartMatchTilesA:
	.incbin "games/tbs/src/graphics/tileset/colosseum_azart_match/tiles_a.4bpp.mtf"
	.byte 0x63	@ PACKER: left in the buffer by Tileset_Set59TilesA+0x29df
	.global Tileset_ColosseumAzartMatchTilesB
Tileset_ColosseumAzartMatchTilesB:
	.incbin "games/tbs/src/graphics/tileset/colosseum_azart_match/tiles_b.4bpp.mtf"
	.global Tileset_ColosseumAzartMatchTilesC
Tileset_ColosseumAzartMatchTilesC:
	.incbin "games/tbs/src/graphics/tileset/colosseum_azart_match/tiles_c.4bpp.mtf"
	.byte 0x25, 0x96, 0x85	@ PACKER: left in the buffer by Tileset_ColosseumAzartMatchTilesB+0x1895
	.global Tileset_ColosseumAzartMatchTilesD
Tileset_ColosseumAzartMatchTilesD:
	.incbin "games/tbs/src/graphics/tileset/colosseum_azart_match/tiles_d.4bpp.mtf"
	.byte 0x69	@ PACKER: left in the buffer by Tileset_ColosseumAzartMatchTilesB+0x1907
