@ Field_Map134: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map134
Field_Map134:
	.ifdef TBS_EDITION_DE
	map_header 0, 0, 96, 96, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map134, .LField_Map134_METATILES
	map_block Field_Map134, .LField_Map134_SHAPES
	map_block Field_Map134, .LField_Map134_GRID
	map_block Field_Map134, .LField_Map134_ANIMATION
	map_block Field_Map134, .LField_Map134_BLEND
	map_block Field_Map134, .LField_Map134_MARKERS
.LField_Map134_METATILES:
	.incbin "MAP/MAP_134/METATILES_DE.delta1.lz"
	.balign 4, 0
.LField_Map134_SHAPES:
	.incbin "COMMON/MAP/MAP_130/SHAPES.table.lz"
	.balign 4, 0
.LField_Map134_GRID:
	.incbin "MAP/MAP_134/GRID_DE.parts.lz"
	.balign 4, 0
.LField_Map134_ANIMATION:
	.incbin "MAP/MAP_134/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map134_BLEND:
	.incbin "MAP/MAP_020/BLEND.script.lz"
	.balign 4, 0
.LField_Map134_MARKERS:
	.incbin "MAP/MAP_134/MARKERS_DE.table"
	.else
	.ifdef TBS_EDITION_ES
	map_header 0, 0, 96, 96, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map134, .LField_Map134_METATILES
	map_block Field_Map134, .LField_Map134_SHAPES
	map_block Field_Map134, .LField_Map134_GRID
	map_block Field_Map134, .LField_Map134_ANIMATION
	map_block Field_Map134, .LField_Map134_BLEND
	map_block Field_Map134, .LField_Map134_MARKERS
.LField_Map134_METATILES:
	.incbin "MAP/MAP_134/METATILES_DE.delta1.lz"
	.balign 4, 0
.LField_Map134_SHAPES:
	.incbin "COMMON/MAP/MAP_130/SHAPES.table.lz"
	.balign 4, 0
.LField_Map134_GRID:
	.incbin "MAP/MAP_134/GRID_DE.parts.lz"
	.balign 4, 0
.LField_Map134_ANIMATION:
	.incbin "MAP/MAP_134/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map134_BLEND:
	.incbin "MAP/MAP_020/BLEND.script.lz"
	.balign 4, 0
.LField_Map134_MARKERS:
	.incbin "MAP/MAP_134/MARKERS_DE.table"
	.else
	.ifdef TBS_EDITION_FR
	map_header 0, 0, 96, 96, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map134, .LField_Map134_METATILES
	map_block Field_Map134, .LField_Map134_SHAPES
	map_block Field_Map134, .LField_Map134_GRID
	map_block Field_Map134, .LField_Map134_ANIMATION
	map_block Field_Map134, .LField_Map134_BLEND
	map_block Field_Map134, .LField_Map134_MARKERS
.LField_Map134_METATILES:
	.incbin "MAP/MAP_134/METATILES_DE.delta1.lz"
	.balign 4, 0
.LField_Map134_SHAPES:
	.incbin "COMMON/MAP/MAP_130/SHAPES.table.lz"
	.balign 4, 0
.LField_Map134_GRID:
	.incbin "MAP/MAP_134/GRID_DE.parts.lz"
	.balign 4, 0
.LField_Map134_ANIMATION:
	.incbin "MAP/MAP_134/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map134_BLEND:
	.incbin "MAP/MAP_020/BLEND.script.lz"
	.balign 4, 0
.LField_Map134_MARKERS:
	.incbin "MAP/MAP_134/MARKERS_DE.table"
	.else
	.ifdef TBS_EDITION_IT
	map_header 0, 0, 96, 96, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map134, .LField_Map134_METATILES
	map_block Field_Map134, .LField_Map134_SHAPES
	map_block Field_Map134, .LField_Map134_GRID
	map_block Field_Map134, .LField_Map134_ANIMATION
	map_block Field_Map134, .LField_Map134_BLEND
	map_block Field_Map134, .LField_Map134_MARKERS
.LField_Map134_METATILES:
	.incbin "MAP/MAP_134/METATILES_DE.delta1.lz"
	.balign 4, 0
.LField_Map134_SHAPES:
	.incbin "COMMON/MAP/MAP_130/SHAPES.table.lz"
	.balign 4, 0
.LField_Map134_GRID:
	.incbin "MAP/MAP_134/GRID_DE.parts.lz"
	.balign 4, 0
.LField_Map134_ANIMATION:
	.incbin "MAP/MAP_134/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map134_BLEND:
	.incbin "MAP/MAP_020/BLEND.script.lz"
	.balign 4, 0
.LField_Map134_MARKERS:
	.incbin "MAP/MAP_134/MARKERS_DE.table"
	.else
	map_header 0, 0, 96, 96, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map134, .LField_Map134_METATILES
	map_block Field_Map134, .LField_Map134_SHAPES
	map_block Field_Map134, .LField_Map134_GRID
	map_block Field_Map134, .LField_Map134_ANIMATION
	map_block Field_Map134, .LField_Map134_BLEND
	map_block Field_Map134, .LField_Map134_MARKERS
.LField_Map134_METATILES:
	.incbin "MAP/MAP_134/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map134_SHAPES:
	.incbin "COMMON/MAP/MAP_130/SHAPES.table.lz"
	.balign 4, 0
.LField_Map134_GRID:
	.incbin "MAP/MAP_134/GRID.parts.lz"
	.balign 4, 0
.LField_Map134_ANIMATION:
	.incbin "MAP/MAP_134/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map134_BLEND:
	.incbin "MAP/MAP_020/BLEND.script.lz"
	.balign 4, 0
.LField_Map134_MARKERS:
	.incbin "MAP/MAP_134/MARKERS.table"
	.endif
	.endif
	.endif
	.endif
