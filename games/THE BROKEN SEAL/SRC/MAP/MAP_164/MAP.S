@ Field_Map164: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map164
Field_Map164:
	.ifdef TBS_EDITION_ES
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map164, .LField_Map164_METATILES
	map_block Field_Map164, .LField_Map164_SHAPES
	map_block Field_Map164, .LField_Map164_GRID
	map_block Field_Map164, .LField_Map164_ANIMATION
	map_block Field_Map164, .LField_Map164_BLEND
	map_block Field_Map164, .LField_Map164_MARKERS
.LField_Map164_METATILES:
	.incbin "MAP/MAP_164/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map164_SHAPES:
	.incbin "MAP/MAP_164/SHAPES.table.lz"
	.balign 4, 0
.LField_Map164_GRID:
	.incbin "MAP/MAP_164/GRID_ES.parts.lz"
	.balign 4, 0
.LField_Map164_ANIMATION:
	.incbin "MAP/MAP_164/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map164_BLEND:
	.incbin "MAP/MAP_164/BLEND.script.lz"
	.balign 4, 0
.LField_Map164_MARKERS:
	.incbin "MAP/MAP_164/MARKERS_ES.table"
	.else
	.ifdef TBS_EDITION_FR
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map164, .LField_Map164_METATILES
	map_block Field_Map164, .LField_Map164_SHAPES
	map_block Field_Map164, .LField_Map164_GRID
	map_block Field_Map164, .LField_Map164_ANIMATION
	map_block Field_Map164, .LField_Map164_BLEND
	map_block Field_Map164, .LField_Map164_MARKERS
.LField_Map164_METATILES:
	.incbin "MAP/MAP_164/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map164_SHAPES:
	.incbin "MAP/MAP_164/SHAPES.table.lz"
	.balign 4, 0
.LField_Map164_GRID:
	.incbin "MAP/MAP_164/GRID_ES.parts.lz"
	.balign 4, 0
.LField_Map164_ANIMATION:
	.incbin "MAP/MAP_164/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map164_BLEND:
	.incbin "MAP/MAP_164/BLEND.script.lz"
	.balign 4, 0
.LField_Map164_MARKERS:
	.incbin "MAP/MAP_164/MARKERS_ES.table"
	.else
	.ifdef TBS_EDITION_IT
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map164, .LField_Map164_METATILES
	map_block Field_Map164, .LField_Map164_SHAPES
	map_block Field_Map164, .LField_Map164_GRID
	map_block Field_Map164, .LField_Map164_ANIMATION
	map_block Field_Map164, .LField_Map164_BLEND
	map_block Field_Map164, .LField_Map164_MARKERS
.LField_Map164_METATILES:
	.incbin "MAP/MAP_164/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map164_SHAPES:
	.incbin "MAP/MAP_164/SHAPES.table.lz"
	.balign 4, 0
.LField_Map164_GRID:
	.incbin "MAP/MAP_164/GRID_ES.parts.lz"
	.balign 4, 0
.LField_Map164_ANIMATION:
	.incbin "MAP/MAP_164/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map164_BLEND:
	.incbin "MAP/MAP_164/BLEND.script.lz"
	.balign 4, 0
.LField_Map164_MARKERS:
	.incbin "MAP/MAP_164/MARKERS_ES.table"
	.else
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map164, .LField_Map164_METATILES
	map_block Field_Map164, .LField_Map164_SHAPES
	map_block Field_Map164, .LField_Map164_GRID
	map_block Field_Map164, .LField_Map164_ANIMATION
	map_block Field_Map164, .LField_Map164_BLEND
	map_block Field_Map164, .LField_Map164_MARKERS
.LField_Map164_METATILES:
	.incbin "MAP/MAP_164/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map164_SHAPES:
	.incbin "MAP/MAP_164/SHAPES.table.lz"
	.balign 4, 0
.LField_Map164_GRID:
	.incbin "MAP/MAP_164/GRID.parts.lz"
	.balign 4, 0
.LField_Map164_ANIMATION:
	.incbin "MAP/MAP_164/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map164_BLEND:
	.incbin "MAP/MAP_164/BLEND.script.lz"
	.balign 4, 0
.LField_Map164_MARKERS:
	.incbin "MAP/MAP_164/MARKERS.table"
	.endif
	.endif
	.endif
