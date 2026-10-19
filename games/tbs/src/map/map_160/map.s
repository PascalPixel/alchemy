	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map160
Field_Map160:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map160, .LField_Map160_METATILES
	map_block Field_Map160, .LField_Map160_SHAPES
	map_block Field_Map160, .LField_Map160_GRID
	map_block Field_Map160, .LField_Map160_ANIMATION
	map_block Field_Map160, .LField_Map160_BLEND
	map_block Field_Map160, .LField_Map160_MARKERS
.LField_Map160_METATILES:
	.incbin "games/tbs/src/map/map_160/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map160_SHAPES:
	.incbin "games/tbs/src/map/map_160/shapes.table.lz"
	.balign 4, 0
.LField_Map160_GRID:
	.incbin "games/tbs/src/map/map_160/grid.parts.lz"
	.balign 4, 0
.LField_Map160_ANIMATION:
	.incbin "games/tbs/src/map/map_151/animation.script.lz"
	.balign 4, 0
.LField_Map160_BLEND:
	.incbin "games/tbs/src/map/map_018/blend.script.lz"
	.balign 4, 0
.LField_Map160_MARKERS:
	.incbin "games/tbs/src/map/map_160/markers.table"
	.balign 4, 0
	.global Tileset_Palette48
Tileset_Palette48:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_48.gbapal.lz"
	.byte 0x38, 0x16	@ PACKER: left in the buffer by Field_Map160+0x1e6
