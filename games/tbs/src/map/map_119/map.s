	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map119
Field_Map119:
	map_header 0, 0, 76, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 76, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 76, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map119, .LField_Map119_CELL_PICTURES
	map_offset Field_Map119, .LField_Map119_SHAPES
	map_offset Field_Map119, .LField_Map119_GRID
	map_offset Field_Map119, .LField_Map119_ANIMATION
	map_offset Field_Map119, .LField_Map119_BLEND
	map_offset Field_Map119, .LField_Map119_MARKERS
.LField_Map119_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_119/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map119_SHAPES:
	.incbin "games/tbs/src/map/map_119/shapes.table.lz"
	.balign 4, 0
.LField_Map119_GRID:
	.incbin "games/tbs/src/map/map_119/map.grid.lz"
	.balign 4, 0
.LField_Map119_ANIMATION:
	.incbin "games/tbs/src/map/map_119/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map119_BLEND:
	.incbin "games/tbs/src/map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map119_MARKERS:
	treasure_spot 8, 9, 100
	treasure_spot 18, 18, 239
	treasure_spot 19, 18, 239
	treasure_spot 30, 18, 101
	treasure_spot 3, 32, 102
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Palette25
Tileset_Palette25:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_25.gbapal.lz"
	.byte 0x5b, 0x24, 0xd1	@ PACKER: left in the buffer by Field_Map119+0x18d
