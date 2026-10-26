	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map086
Field_Map086:
	map_header 0, 0, 100, 68, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 68, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map086, .LField_Map086_METATILES
	map_offset Field_Map086, .LField_Map086_SHAPES
	map_offset Field_Map086, .LField_Map086_GRID
	map_offset Field_Map086, .LField_Map086_ANIMATION
	map_offset Field_Map086, .LField_Map086_BLEND
	map_offset Field_Map086, .LField_Map086_MARKERS
.LField_Map086_METATILES:
	.incbin "games/tbs/src/map/map_086/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map086_SHAPES:
	.incbin "games/tbs/src/map/map_086/shapes.table.lz"
	.balign 4, 0
.LField_Map086_GRID:
	.incbin "games/tbs/src/map/map_086/map.grid.lz"
	.balign 4, 0
.LField_Map086_ANIMATION:
	.incbin "games/tbs/src/map/map_086/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map086_BLEND:
	.incbin "games/tbs/src/map/map_086/blend.script.lz"
	.balign 4, 0
.LField_Map086_MARKERS:
	treasure_spot 20, 4, 100
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Palette06
Tileset_Palette06:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_06.gbapal.lz"
	.byte 0x2a, 0x02, 0x98	@ PACKER: left in the buffer by Field_Map086+0x15d
