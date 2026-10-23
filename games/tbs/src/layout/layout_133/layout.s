	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout133
Field_Layout133:
	.ifdef TBS_EDITION_DE
	layout_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout133, .LField_Layout133_METATILES
	layout_offset Field_Layout133, .LField_Layout133_SHAPES
	layout_offset Field_Layout133, .LField_Layout133_GRID
	layout_offset Field_Layout133, .LField_Layout133_ANIMATION
	layout_offset Field_Layout133, .LField_Layout133_BLEND
	layout_offset Field_Layout133, .LField_Layout133_MARKERS
.LField_Layout133_METATILES:
	.incbin "games/tbs/src/layout/layout_133/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout133_SHAPES:
	.incbin "games/tbs/src/layout/layout_133/shapes.table.lz"
	.balign 4, 0
.LField_Layout133_GRID:
	.incbin "games/tbs/src/layout/layout_133/grid_de.parts.lz"
	.balign 4, 0
.LField_Layout133_ANIMATION:
	.incbin "games/tbs/src/layout/layout_133/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout133_BLEND:
	.incbin "games/tbs/src/layout/layout_133/blend.script.lz"
	.balign 4, 0
.LField_Layout133_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/treasure_spots.table"
	.else
	.ifdef TBS_EDITION_ES
	layout_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout133, .LField_Layout133_METATILES
	layout_offset Field_Layout133, .LField_Layout133_SHAPES
	layout_offset Field_Layout133, .LField_Layout133_GRID
	layout_offset Field_Layout133, .LField_Layout133_ANIMATION
	layout_offset Field_Layout133, .LField_Layout133_BLEND
	layout_offset Field_Layout133, .LField_Layout133_MARKERS
.LField_Layout133_METATILES:
	.incbin "games/tbs/src/layout/layout_133/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout133_SHAPES:
	.incbin "games/tbs/src/layout/layout_133/shapes.table.lz"
	.balign 4, 0
.LField_Layout133_GRID:
	.incbin "games/tbs/src/layout/layout_133/grid_de.parts.lz"
	.balign 4, 0
.LField_Layout133_ANIMATION:
	.incbin "games/tbs/src/layout/layout_133/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout133_BLEND:
	.incbin "games/tbs/src/layout/layout_133/blend.script.lz"
	.balign 4, 0
.LField_Layout133_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/treasure_spots.table"
	.else
	.ifdef TBS_EDITION_FR
	layout_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout133, .LField_Layout133_METATILES
	layout_offset Field_Layout133, .LField_Layout133_SHAPES
	layout_offset Field_Layout133, .LField_Layout133_GRID
	layout_offset Field_Layout133, .LField_Layout133_ANIMATION
	layout_offset Field_Layout133, .LField_Layout133_BLEND
	layout_offset Field_Layout133, .LField_Layout133_MARKERS
.LField_Layout133_METATILES:
	.incbin "games/tbs/src/layout/layout_133/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout133_SHAPES:
	.incbin "games/tbs/src/layout/layout_133/shapes.table.lz"
	.balign 4, 0
.LField_Layout133_GRID:
	.incbin "games/tbs/src/layout/layout_133/grid_de.parts.lz"
	.balign 4, 0
.LField_Layout133_ANIMATION:
	.incbin "games/tbs/src/layout/layout_133/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout133_BLEND:
	.incbin "games/tbs/src/layout/layout_133/blend.script.lz"
	.balign 4, 0
.LField_Layout133_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/treasure_spots.table"
	.else
	.ifdef TBS_EDITION_IT
	layout_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout133, .LField_Layout133_METATILES
	layout_offset Field_Layout133, .LField_Layout133_SHAPES
	layout_offset Field_Layout133, .LField_Layout133_GRID
	layout_offset Field_Layout133, .LField_Layout133_ANIMATION
	layout_offset Field_Layout133, .LField_Layout133_BLEND
	layout_offset Field_Layout133, .LField_Layout133_MARKERS
.LField_Layout133_METATILES:
	.incbin "games/tbs/src/layout/layout_133/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout133_SHAPES:
	.incbin "games/tbs/src/layout/layout_133/shapes.table.lz"
	.balign 4, 0
.LField_Layout133_GRID:
	.incbin "games/tbs/src/layout/layout_133/grid_de.parts.lz"
	.balign 4, 0
.LField_Layout133_ANIMATION:
	.incbin "games/tbs/src/layout/layout_133/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout133_BLEND:
	.incbin "games/tbs/src/layout/layout_133/blend.script.lz"
	.balign 4, 0
.LField_Layout133_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/treasure_spots.table"
	.else
	layout_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout133, .LField_Layout133_METATILES
	layout_offset Field_Layout133, .LField_Layout133_SHAPES
	layout_offset Field_Layout133, .LField_Layout133_GRID
	layout_offset Field_Layout133, .LField_Layout133_ANIMATION
	layout_offset Field_Layout133, .LField_Layout133_BLEND
	layout_offset Field_Layout133, .LField_Layout133_MARKERS
.LField_Layout133_METATILES:
	.incbin "games/tbs/src/layout/layout_133/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout133_SHAPES:
	.incbin "games/tbs/src/layout/layout_133/shapes.table.lz"
	.balign 4, 0
.LField_Layout133_GRID:
	.incbin "games/tbs/src/layout/layout_133/grid.parts.lz"
	.balign 4, 0
.LField_Layout133_ANIMATION:
	.incbin "games/tbs/src/layout/layout_133/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout133_BLEND:
	.incbin "games/tbs/src/layout/layout_133/blend.script.lz"
	.balign 4, 0
.LField_Layout133_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/treasure_spots.table"
	.endif
	.endif
	.endif
	.endif
	.balign 4, 0
	.global Tileset_Palette32
Tileset_Palette32:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_32.gbapal.lz"
	.byte 0xf9, 0x56	@ PACKER: left in the buffer by Field_Layout133+0x16a
