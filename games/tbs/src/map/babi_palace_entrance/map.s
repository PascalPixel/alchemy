	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_BabiPalaceEntrance
Field_BabiPalaceEntrance:
	map_header 0, 0, 48, 56, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 48, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 56, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_BabiPalaceEntrance, .LField_BabiPalaceEntrance_CELL_PICTURES
	map_offset Field_BabiPalaceEntrance, .LField_BabiPalaceEntrance_SHAPES
	map_offset Field_BabiPalaceEntrance, .LField_BabiPalaceEntrance_GRID
	map_offset Field_BabiPalaceEntrance
	map_offset Field_BabiPalaceEntrance
	map_offset Field_BabiPalaceEntrance, .LField_BabiPalaceEntrance_MARKERS
.LField_BabiPalaceEntrance_CELL_PICTURES:
	.incbin "games/tbs/src/map/babi_palace_entrance/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_BabiPalaceEntrance_SHAPES:
	.incbin "games/tbs/src/map/babi_palace_entrance/shapes.table.lz"
	.balign 4, 0
.LField_BabiPalaceEntrance_GRID:
	.incbin "games/tbs/src/map/babi_palace_entrance/map.grid.lz"
	.balign 4, 0
.LField_BabiPalaceEntrance_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Palette27
Tileset_Palette27:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_27.gbapal.lz"
	.byte 0x86, 0xf1	@ PACKER: left in the buffer by Field_BabiPalaceEntrance+0x18e
