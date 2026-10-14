	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map099
Field_Map099:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map099, .LField_Map099_METATILES
	map_block Field_Map099, .LField_Map099_SHAPES
	map_block Field_Map099, .LField_Map099_GRID
	map_block Field_Map099, .LField_Map099_ANIMATION
	map_block Field_Map099, .LField_Map099_BLEND
	map_block Field_Map099, .LField_Map099_MARKERS
.LField_Map099_METATILES:
	.incbin "games/tbs/src/map/map_099/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map099_SHAPES:
	.incbin "games/tbs/src/map/map_099/shapes.table.lz"
	.balign 4, 0
.LField_Map099_GRID:
	.incbin "games/tbs/src/map/map_099/grid.parts.lz"
	.balign 4, 0
.LField_Map099_ANIMATION:
	.incbin "games/common/src/map/map_052/animation.script.lz"
	.balign 4, 0
.LField_Map099_BLEND:
	.incbin "games/common/src/map/map_018/blend.script.lz"
	.balign 4, 0
.LField_Map099_MARKERS:
	.incbin "games/tbs/src/map/map_099/markers.table"
	.balign 4, 0
	.global Tileset_Palette09
Tileset_Palette09:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_09.gbapal.lz"
	.byte 0xcc	@ PACKER: left in the buffer by Field_Map099+0x1c3
