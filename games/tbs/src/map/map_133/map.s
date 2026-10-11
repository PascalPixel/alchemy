	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map133
Field_Map133:
	.ifdef TBS_EDITION_DE
	map_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map133, .LField_Map133_METATILES
	map_block Field_Map133, .LField_Map133_SHAPES
	map_block Field_Map133, .LField_Map133_GRID
	map_block Field_Map133, .LField_Map133_ANIMATION
	map_block Field_Map133, .LField_Map133_BLEND
	map_block Field_Map133, .LField_Map133_MARKERS
.LField_Map133_METATILES:
	.incbin "map/map_133/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map133_SHAPES:
	.incbin "map/map_133/shapes.table.lz"
	.balign 4, 0
.LField_Map133_GRID:
	.incbin "map/map_133/grid_de.parts.lz"
	.balign 4, 0
.LField_Map133_ANIMATION:
	.incbin "map/map_133/animation.script.lz"
	.balign 4, 0
.LField_Map133_BLEND:
	.incbin "map/map_133/blend.script.lz"
	.balign 4, 0
.LField_Map133_MARKERS:
	.incbin "map/map_000/markers.table"
	.else
	.ifdef TBS_EDITION_ES
	map_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map133, .LField_Map133_METATILES
	map_block Field_Map133, .LField_Map133_SHAPES
	map_block Field_Map133, .LField_Map133_GRID
	map_block Field_Map133, .LField_Map133_ANIMATION
	map_block Field_Map133, .LField_Map133_BLEND
	map_block Field_Map133, .LField_Map133_MARKERS
.LField_Map133_METATILES:
	.incbin "map/map_133/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map133_SHAPES:
	.incbin "map/map_133/shapes.table.lz"
	.balign 4, 0
.LField_Map133_GRID:
	.incbin "map/map_133/grid_de.parts.lz"
	.balign 4, 0
.LField_Map133_ANIMATION:
	.incbin "map/map_133/animation.script.lz"
	.balign 4, 0
.LField_Map133_BLEND:
	.incbin "map/map_133/blend.script.lz"
	.balign 4, 0
.LField_Map133_MARKERS:
	.incbin "map/map_000/markers.table"
	.else
	.ifdef TBS_EDITION_FR
	map_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map133, .LField_Map133_METATILES
	map_block Field_Map133, .LField_Map133_SHAPES
	map_block Field_Map133, .LField_Map133_GRID
	map_block Field_Map133, .LField_Map133_ANIMATION
	map_block Field_Map133, .LField_Map133_BLEND
	map_block Field_Map133, .LField_Map133_MARKERS
.LField_Map133_METATILES:
	.incbin "map/map_133/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map133_SHAPES:
	.incbin "map/map_133/shapes.table.lz"
	.balign 4, 0
.LField_Map133_GRID:
	.incbin "map/map_133/grid_de.parts.lz"
	.balign 4, 0
.LField_Map133_ANIMATION:
	.incbin "map/map_133/animation.script.lz"
	.balign 4, 0
.LField_Map133_BLEND:
	.incbin "map/map_133/blend.script.lz"
	.balign 4, 0
.LField_Map133_MARKERS:
	.incbin "map/map_000/markers.table"
	.else
	.ifdef TBS_EDITION_IT
	map_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map133, .LField_Map133_METATILES
	map_block Field_Map133, .LField_Map133_SHAPES
	map_block Field_Map133, .LField_Map133_GRID
	map_block Field_Map133, .LField_Map133_ANIMATION
	map_block Field_Map133, .LField_Map133_BLEND
	map_block Field_Map133, .LField_Map133_MARKERS
.LField_Map133_METATILES:
	.incbin "map/map_133/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map133_SHAPES:
	.incbin "map/map_133/shapes.table.lz"
	.balign 4, 0
.LField_Map133_GRID:
	.incbin "map/map_133/grid_de.parts.lz"
	.balign 4, 0
.LField_Map133_ANIMATION:
	.incbin "map/map_133/animation.script.lz"
	.balign 4, 0
.LField_Map133_BLEND:
	.incbin "map/map_133/blend.script.lz"
	.balign 4, 0
.LField_Map133_MARKERS:
	.incbin "map/map_000/markers.table"
	.else
	map_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map133, .LField_Map133_METATILES
	map_block Field_Map133, .LField_Map133_SHAPES
	map_block Field_Map133, .LField_Map133_GRID
	map_block Field_Map133, .LField_Map133_ANIMATION
	map_block Field_Map133, .LField_Map133_BLEND
	map_block Field_Map133, .LField_Map133_MARKERS
.LField_Map133_METATILES:
	.incbin "map/map_133/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map133_SHAPES:
	.incbin "map/map_133/shapes.table.lz"
	.balign 4, 0
.LField_Map133_GRID:
	.incbin "map/map_133/grid.parts.lz"
	.balign 4, 0
.LField_Map133_ANIMATION:
	.incbin "map/map_133/animation.script.lz"
	.balign 4, 0
.LField_Map133_BLEND:
	.incbin "map/map_133/blend.script.lz"
	.balign 4, 0
.LField_Map133_MARKERS:
	.incbin "map/map_000/markers.table"
	.endif
	.endif
	.endif
	.endif
	.balign 4, 0
	.global Tileset_Palette32
Tileset_Palette32:
	.incbin "graphics/tileset/palettes/pal_32.gbapal.lz"
	.byte 0xf9, 0x56	@ PACKER: left in the buffer by Field_Map133+0x16a
