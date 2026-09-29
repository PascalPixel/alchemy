@ Field_Map023: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map023
Field_Map023:
	.ifdef TBS_EDITION_ES
	map_header 0, 0, 96, 115, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 115, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map023, .LField_Map023_METATILES
	map_block Field_Map023, .LField_Map023_SHAPES
	map_block Field_Map023, .LField_Map023_GRID
	map_block Field_Map023, .LField_Map023_ANIMATION
	map_block Field_Map023, .LField_Map023_BLEND
	map_block Field_Map023, .LField_Map023_MARKERS
.LField_Map023_METATILES:
	.incbin "MAP/MAP_023/METATILES_ES.delta2.lz"
	.balign 4, 0
.LField_Map023_SHAPES:
	.incbin "MAP/MAP_023/SHAPES.table.lz"
	.balign 4, 0
.LField_Map023_GRID:
	.incbin "MAP/MAP_023/GRID_ES.parts.lz"
	.balign 4, 0
.LField_Map023_ANIMATION:
	.incbin "MAP/MAP_023/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map023_BLEND:
	.incbin "MAP/MAP_022/BLEND.script.lz"
	.balign 4, 0
.LField_Map023_MARKERS:
	.incbin "MAP/MAP_023/MARKERS_ES.table"
	.else
	.ifdef TBS_EDITION_FR
	map_header 0, 0, 96, 115, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 115, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map023, .LField_Map023_METATILES
	map_block Field_Map023, .LField_Map023_SHAPES
	map_block Field_Map023, .LField_Map023_GRID
	map_block Field_Map023, .LField_Map023_ANIMATION
	map_block Field_Map023, .LField_Map023_BLEND
	map_block Field_Map023, .LField_Map023_MARKERS
.LField_Map023_METATILES:
	.incbin "MAP/MAP_023/METATILES_ES.delta2.lz"
	.balign 4, 0
.LField_Map023_SHAPES:
	.incbin "MAP/MAP_023/SHAPES.table.lz"
	.balign 4, 0
.LField_Map023_GRID:
	.incbin "MAP/MAP_023/GRID_ES.parts.lz"
	.balign 4, 0
.LField_Map023_ANIMATION:
	.incbin "MAP/MAP_023/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map023_BLEND:
	.incbin "MAP/MAP_022/BLEND.script.lz"
	.balign 4, 0
.LField_Map023_MARKERS:
	.incbin "MAP/MAP_023/MARKERS_ES.table"
	.else
	.ifdef TBS_EDITION_IT
	map_header 0, 0, 96, 115, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 115, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map023, .LField_Map023_METATILES
	map_block Field_Map023, .LField_Map023_SHAPES
	map_block Field_Map023, .LField_Map023_GRID
	map_block Field_Map023, .LField_Map023_ANIMATION
	map_block Field_Map023, .LField_Map023_BLEND
	map_block Field_Map023, .LField_Map023_MARKERS
.LField_Map023_METATILES:
	.incbin "MAP/MAP_023/METATILES_ES.delta2.lz"
	.balign 4, 0
.LField_Map023_SHAPES:
	.incbin "MAP/MAP_023/SHAPES.table.lz"
	.balign 4, 0
.LField_Map023_GRID:
	.incbin "MAP/MAP_023/GRID_ES.parts.lz"
	.balign 4, 0
.LField_Map023_ANIMATION:
	.incbin "MAP/MAP_023/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map023_BLEND:
	.incbin "MAP/MAP_022/BLEND.script.lz"
	.balign 4, 0
.LField_Map023_MARKERS:
	.incbin "MAP/MAP_023/MARKERS_ES.table"
	.else
	map_header 0, 0, 96, 115, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 115, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map023, .LField_Map023_METATILES
	map_block Field_Map023, .LField_Map023_SHAPES
	map_block Field_Map023, .LField_Map023_GRID
	map_block Field_Map023, .LField_Map023_ANIMATION
	map_block Field_Map023, .LField_Map023_BLEND
	map_block Field_Map023, .LField_Map023_MARKERS
.LField_Map023_METATILES:
	.incbin "MAP/MAP_023/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map023_SHAPES:
	.incbin "MAP/MAP_023/SHAPES.table.lz"
	.balign 4, 0
.LField_Map023_GRID:
	.incbin "MAP/MAP_023/GRID.parts.lz"
	.balign 4, 0
.LField_Map023_ANIMATION:
	.incbin "MAP/MAP_023/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map023_BLEND:
	.incbin "MAP/MAP_022/BLEND.script.lz"
	.balign 4, 0
.LField_Map023_MARKERS:
	.incbin "MAP/MAP_023/MARKERS.table"
	.endif
	.endif
	.endif
