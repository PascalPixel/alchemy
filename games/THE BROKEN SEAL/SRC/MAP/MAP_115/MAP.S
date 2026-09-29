@ Field_Map115: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map115
Field_Map115:
	.ifdef TBS_EDITION_ES
	map_header 0, 0, 80, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map115, .LField_Map115_METATILES
	map_block Field_Map115, .LField_Map115_SHAPES
	map_block Field_Map115, .LField_Map115_GRID
	map_block Field_Map115, .LField_Map115_ANIMATION
	map_block Field_Map115, .LField_Map115_BLEND
	map_block Field_Map115, .LField_Map115_MARKERS
.LField_Map115_METATILES:
	.incbin "MAP/MAP_115/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map115_SHAPES:
	.incbin "MAP/MAP_115/SHAPES.table.lz"
	.balign 4, 0
.LField_Map115_GRID:
	.incbin "MAP/MAP_115/GRID_ES.parts.lz"
	.balign 4, 0
.LField_Map115_ANIMATION:
	.incbin "MAP/MAP_033/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map115_BLEND:
	.incbin "MAP/MAP_033/BLEND.script.lz"
	.balign 4, 0
.LField_Map115_MARKERS:
	.incbin "MAP/MAP_115/MARKERS_ES.table"
	.else
	.ifdef TBS_EDITION_FR
	map_header 0, 0, 80, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map115, .LField_Map115_METATILES
	map_block Field_Map115, .LField_Map115_SHAPES
	map_block Field_Map115, .LField_Map115_GRID
	map_block Field_Map115, .LField_Map115_ANIMATION
	map_block Field_Map115, .LField_Map115_BLEND
	map_block Field_Map115, .LField_Map115_MARKERS
.LField_Map115_METATILES:
	.incbin "MAP/MAP_115/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map115_SHAPES:
	.incbin "MAP/MAP_115/SHAPES.table.lz"
	.balign 4, 0
.LField_Map115_GRID:
	.incbin "MAP/MAP_115/GRID_ES.parts.lz"
	.balign 4, 0
.LField_Map115_ANIMATION:
	.incbin "MAP/MAP_033/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map115_BLEND:
	.incbin "MAP/MAP_033/BLEND.script.lz"
	.balign 4, 0
.LField_Map115_MARKERS:
	.incbin "MAP/MAP_115/MARKERS_ES.table"
	.else
	.ifdef TBS_EDITION_IT
	map_header 0, 0, 80, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map115, .LField_Map115_METATILES
	map_block Field_Map115, .LField_Map115_SHAPES
	map_block Field_Map115, .LField_Map115_GRID
	map_block Field_Map115, .LField_Map115_ANIMATION
	map_block Field_Map115, .LField_Map115_BLEND
	map_block Field_Map115, .LField_Map115_MARKERS
.LField_Map115_METATILES:
	.incbin "MAP/MAP_115/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map115_SHAPES:
	.incbin "MAP/MAP_115/SHAPES.table.lz"
	.balign 4, 0
.LField_Map115_GRID:
	.incbin "MAP/MAP_115/GRID_ES.parts.lz"
	.balign 4, 0
.LField_Map115_ANIMATION:
	.incbin "MAP/MAP_033/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map115_BLEND:
	.incbin "MAP/MAP_033/BLEND.script.lz"
	.balign 4, 0
.LField_Map115_MARKERS:
	.incbin "MAP/MAP_115/MARKERS_ES.table"
	.else
	map_header 0, 0, 80, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map115, .LField_Map115_METATILES
	map_block Field_Map115, .LField_Map115_SHAPES
	map_block Field_Map115, .LField_Map115_GRID
	map_block Field_Map115, .LField_Map115_ANIMATION
	map_block Field_Map115, .LField_Map115_BLEND
	map_block Field_Map115, .LField_Map115_MARKERS
.LField_Map115_METATILES:
	.incbin "MAP/MAP_115/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map115_SHAPES:
	.incbin "MAP/MAP_115/SHAPES.table.lz"
	.balign 4, 0
.LField_Map115_GRID:
	.incbin "MAP/MAP_115/GRID.parts.lz"
	.balign 4, 0
.LField_Map115_ANIMATION:
	.incbin "MAP/MAP_033/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map115_BLEND:
	.incbin "MAP/MAP_033/BLEND.script.lz"
	.balign 4, 0
.LField_Map115_MARKERS:
	.incbin "MAP/MAP_115/MARKERS.table"
	.endif
	.endif
	.endif
