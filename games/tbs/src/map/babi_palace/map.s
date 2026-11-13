	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_BabiPalace
Field_BabiPalace:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_BabiPalace, .LField_BabiPalace_CELL_PICTURES
	map_offset Field_BabiPalace, .LField_BabiPalace_SHAPES
	map_offset Field_BabiPalace, .LField_BabiPalace_GRID
	map_offset Field_BabiPalace, .LField_BabiPalace_ANIMATION
	map_offset Field_BabiPalace, .LField_BabiPalace_BLEND
	map_offset Field_BabiPalace, .LField_BabiPalace_MARKERS
.LField_BabiPalace_CELL_PICTURES:
	.incbin "games/tbs/src/map/babi_palace/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_BabiPalace_SHAPES:
	.incbin "games/tbs/src/map/babi_palace/shapes.table.lz"
	.balign 4, 0
.LField_BabiPalace_GRID:
	.incbin "games/tbs/src/map/babi_palace/map.grid.lz"
	.balign 4, 0
.LField_BabiPalace_ANIMATION:
	.incbin "games/tbs/src/map/babi_palace/tileset_anims.script.lz"
	.balign 4, 0
.LField_BabiPalace_BLEND:
	.incbin "games/tbs/src/map/goma_cave_entrance/blend.script.lz"
	.balign 4, 0
.LField_BabiPalace_MARKERS:
	treasure_spot 6, 3, 203
	treasure_spot 7, 3, 203
	treasure_spot 24, 3, 200
	treasure_spot 25, 3, 200
	treasure_spot 9, 18, 201
	treasure_spot 10, 18, 201
	treasure_spot 12, 18, 204
	treasure_spot 13, 18, 204
	treasure_spot 55, 18, 202
	treasure_spot 56, 18, 202
	treasure_spot 58, 18, 205
	treasure_spot 59, 18, 205
	treasure_spot 58, 33, 206
	treasure_spot 59, 33, 206
	treasure_spot 34, 61, 100
	treasure_spots_end
	.global Tileset_Palette28
Tileset_Palette28:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_28.gbapal.lz"
	.byte 0x15, 0x30	@ PACKER: left in the buffer by Field_BabiPalace+0x186
