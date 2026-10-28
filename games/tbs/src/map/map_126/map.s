	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map126
Field_Map126:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map126, .LField_Map126_CELL_PICTURES
	map_offset Field_Map126, .LField_Map126_SHAPES
	map_offset Field_Map126, .LField_Map126_GRID
	map_offset Field_Map126, .LField_Map126_ANIMATION
	map_offset Field_Map126, .LField_Map126_BLEND
	map_offset Field_Map126, .LField_Map126_MARKERS
.LField_Map126_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_126/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map126_SHAPES:
	.incbin "games/tbs/src/map/map_126/shapes.table.lz"
	.balign 4, 0
.LField_Map126_GRID:
	.incbin "games/tbs/src/map/map_126/map.grid.lz"
	.balign 4, 0
.LField_Map126_ANIMATION:
	.incbin "games/tbs/src/map/map_093/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map126_BLEND:
	.incbin "games/tbs/src/map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map126_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Palette30
Tileset_Palette30:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_30.gbapal.lz"
	.byte 0x7f, 0x45, 0xd8	@ PACKER: left in the buffer by Field_Map126+0x18d
