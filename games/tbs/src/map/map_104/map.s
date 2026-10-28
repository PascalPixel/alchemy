	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map104
Field_Map104:
	map_header 0, 0, 46, 46, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 92, 6, 6, 0, 0, 0xff, 0xff
	map_layer 0, 46, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map104, .LField_Map104_CELL_PICTURES
	map_offset Field_Map104, .LField_Map104_SHAPES
	map_offset Field_Map104, .LField_Map104_GRID
	map_offset Field_Map104, .LField_Map104_ANIMATION
	map_offset Field_Map104
	map_offset Field_Map104, .LField_Map104_MARKERS
.LField_Map104_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_104/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map104_SHAPES:
	.incbin "games/tbs/src/map/map_104/shapes.table.lz"
	.balign 4, 0
.LField_Map104_GRID:
	.incbin "games/tbs/src/map/map_104/map.grid.lz"
	.balign 4, 0
.LField_Map104_ANIMATION:
	.incbin "games/tbs/src/map/map_104/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map104_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Palette11
Tileset_Palette11:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_11.gbapal.lz"
	.byte 0xe7, 0x99	@ PACKER: left in the buffer by Field_Map104+0x1d2
