	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map171
Field_Map171:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map171, .LField_Map171_METATILES
	map_offset Field_Map171, .LField_Map171_SHAPES
	map_offset Field_Map171, .LField_Map171_GRID
	map_offset Field_Map171, .LField_Map171_ANIMATION
	map_offset Field_Map171, .LField_Map171_BLEND
	map_offset Field_Map171, .LField_Map171_MARKERS
.LField_Map171_METATILES:
	.incbin "games/tbs/src/map/map_171/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map171_SHAPES:
	.incbin "games/tbs/src/map/map_171/shapes.table.lz"
	.balign 4, 0
.LField_Map171_GRID:
	.incbin "games/tbs/src/map/map_171/map.grid.lz"
	.balign 4, 0
.LField_Map171_ANIMATION:
	.incbin "games/tbs/src/map/map_171/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map171_BLEND:
	.incbin "games/tbs/src/map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map171_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Palette53
Tileset_Palette53:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_53.gbapal.lz"
	.byte 0x1c	@ PACKER: left in the buffer by Field_Map171+0x1c7
