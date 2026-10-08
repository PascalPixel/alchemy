	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map111
Field_Map111:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map111, .LField_Map111_METATILES
	map_block Field_Map111, .LField_Map111_SHAPES
	map_block Field_Map111, .LField_Map111_GRID
	map_block Field_Map111, .LField_Map111_ANIMATION
	map_block Field_Map111, .LField_Map111_BLEND
	map_block Field_Map111, .LField_Map111_MARKERS
.LField_Map111_METATILES:
	.incbin "map/map_111/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map111_SHAPES:
	.incbin "map/map_111/shapes.table.lz"
	.balign 4, 0
.LField_Map111_GRID:
	.incbin "map/map_111/grid.parts.lz"
	.balign 4, 0
.LField_Map111_ANIMATION:
	.incbin "map/map_111/animation.script.lz"
	.balign 4, 0
.LField_Map111_BLEND:
	.incbin "map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map111_MARKERS:
	.incbin "map/map_111/markers.table"
	.balign 4, 0
