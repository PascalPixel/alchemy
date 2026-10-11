	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map126
Field_Map126:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map126, .LField_Map126_METATILES
	map_block Field_Map126, .LField_Map126_SHAPES
	map_block Field_Map126, .LField_Map126_GRID
	map_block Field_Map126, .LField_Map126_ANIMATION
	map_block Field_Map126, .LField_Map126_BLEND
	map_block Field_Map126, .LField_Map126_MARKERS
.LField_Map126_METATILES:
	.incbin "map/map_126/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map126_SHAPES:
	.incbin "map/map_126/shapes.table.lz"
	.balign 4, 0
.LField_Map126_GRID:
	.incbin "map/map_126/grid.parts.lz"
	.balign 4, 0
.LField_Map126_ANIMATION:
	.incbin "map/map_093/animation.script.lz"
	.balign 4, 0
.LField_Map126_BLEND:
	.incbin "map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map126_MARKERS:
	.incbin "map/map_000/markers.table"
	.balign 4, 0
	.global Tileset_Palette30
Tileset_Palette30:
	.incbin "graphics/tileset/palettes/pal_30.gbapal.lz"
	.byte 0x7f, 0x45, 0xd8	@ PACKER: left in the buffer by Field_Map126+0x18d
