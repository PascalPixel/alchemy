@ Field_Map159: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map159
Field_Map159:
	.ifdef TBS_EDITION_DE
	map_header 0, 0, 64, 56, 2, 1, 3, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map159, .LField_Map159_METATILES
	map_block Field_Map159, .LField_Map159_SHAPES
	map_block Field_Map159, .LField_Map159_GRID
	map_block Field_Map159, .LField_Map159_ANIMATION
	map_block Field_Map159
	map_block Field_Map159, .LField_Map159_MARKERS
.LField_Map159_METATILES:
	.incbin "MAP/MAP_159/METATILES_DE.delta1.lz"
	.balign 4, 0
.LField_Map159_SHAPES:
	.incbin "MAP/MAP_159/SHAPES.table.lz"
	.balign 4, 0
.LField_Map159_GRID:
	.incbin "MAP/MAP_159/GRID_DE.parts.lz"
	.balign 4, 0
.LField_Map159_ANIMATION:
	.incbin "MAP/MAP_150/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map159_MARKERS:
	.incbin "MAP/MAP_159/MARKERS_DE.table"
	.else
	.ifdef TBS_EDITION_ES
	map_header 0, 0, 64, 56, 2, 1, 3, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map159, .LField_Map159_METATILES
	map_block Field_Map159, .LField_Map159_SHAPES
	map_block Field_Map159, .LField_Map159_GRID
	map_block Field_Map159, .LField_Map159_ANIMATION
	map_block Field_Map159
	map_block Field_Map159, .LField_Map159_MARKERS
.LField_Map159_METATILES:
	.incbin "MAP/MAP_159/METATILES_DE.delta1.lz"
	.balign 4, 0
.LField_Map159_SHAPES:
	.incbin "MAP/MAP_159/SHAPES.table.lz"
	.balign 4, 0
.LField_Map159_GRID:
	.incbin "MAP/MAP_159/GRID_DE.parts.lz"
	.balign 4, 0
.LField_Map159_ANIMATION:
	.incbin "MAP/MAP_150/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map159_MARKERS:
	.incbin "MAP/MAP_159/MARKERS_DE.table"
	.else
	.ifdef TBS_EDITION_FR
	map_header 0, 0, 64, 56, 2, 1, 3, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map159, .LField_Map159_METATILES
	map_block Field_Map159, .LField_Map159_SHAPES
	map_block Field_Map159, .LField_Map159_GRID
	map_block Field_Map159, .LField_Map159_ANIMATION
	map_block Field_Map159
	map_block Field_Map159, .LField_Map159_MARKERS
.LField_Map159_METATILES:
	.incbin "MAP/MAP_159/METATILES_DE.delta1.lz"
	.balign 4, 0
.LField_Map159_SHAPES:
	.incbin "MAP/MAP_159/SHAPES.table.lz"
	.balign 4, 0
.LField_Map159_GRID:
	.incbin "MAP/MAP_159/GRID_DE.parts.lz"
	.balign 4, 0
.LField_Map159_ANIMATION:
	.incbin "MAP/MAP_150/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map159_MARKERS:
	.incbin "MAP/MAP_159/MARKERS_DE.table"
	.else
	.ifdef TBS_EDITION_IT
	map_header 0, 0, 64, 56, 2, 1, 3, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map159, .LField_Map159_METATILES
	map_block Field_Map159, .LField_Map159_SHAPES
	map_block Field_Map159, .LField_Map159_GRID
	map_block Field_Map159, .LField_Map159_ANIMATION
	map_block Field_Map159
	map_block Field_Map159, .LField_Map159_MARKERS
.LField_Map159_METATILES:
	.incbin "MAP/MAP_159/METATILES_DE.delta1.lz"
	.balign 4, 0
.LField_Map159_SHAPES:
	.incbin "MAP/MAP_159/SHAPES.table.lz"
	.balign 4, 0
.LField_Map159_GRID:
	.incbin "MAP/MAP_159/GRID_DE.parts.lz"
	.balign 4, 0
.LField_Map159_ANIMATION:
	.incbin "MAP/MAP_150/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map159_MARKERS:
	.incbin "MAP/MAP_159/MARKERS_DE.table"
	.else
	map_header 0, 0, 64, 56, 2, 1, 3, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map159, .LField_Map159_METATILES
	map_block Field_Map159, .LField_Map159_SHAPES
	map_block Field_Map159, .LField_Map159_GRID
	map_block Field_Map159, .LField_Map159_ANIMATION
	map_block Field_Map159
	map_block Field_Map159, .LField_Map159_MARKERS
.LField_Map159_METATILES:
	.incbin "MAP/MAP_159/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map159_SHAPES:
	.incbin "MAP/MAP_159/SHAPES.table.lz"
	.balign 4, 0
.LField_Map159_GRID:
	.incbin "MAP/MAP_159/GRID.parts.lz"
	.balign 4, 0
.LField_Map159_ANIMATION:
	.incbin "MAP/MAP_150/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map159_MARKERS:
	.incbin "MAP/MAP_159/MARKERS.table"
	.endif
	.endif
	.endif
	.endif
