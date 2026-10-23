	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout051
Field_Layout051:
	.ifdef TBS_EDITION_DE
	layout_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout051, .LField_Layout051_METATILES
	layout_offset Field_Layout051, .LField_Layout051_SHAPES
	layout_offset Field_Layout051, .LField_Layout051_GRID
	layout_offset Field_Layout051, .LField_Layout051_ANIMATION
	layout_offset Field_Layout051, .LField_Layout051_BLEND
	layout_offset Field_Layout051, .LField_Layout051_MARKERS
.LField_Layout051_METATILES:
	.incbin "games/tbs/src/layout/layout_051/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout051_SHAPES:
	.incbin "games/tbs/src/layout/layout_051/shapes.table.lz"
	.balign 4, 0
.LField_Layout051_GRID:
	.incbin "games/tbs/src/layout/layout_051/grid_de.parts.lz"
	.balign 4, 0
.LField_Layout051_ANIMATION:
	.incbin "games/tbs/src/layout/layout_037/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout051_BLEND:
	.incbin "games/tbs/src/layout/layout_020/blend.script.lz"
	.balign 4, 0
.LField_Layout051_MARKERS:
	.incbin "games/tbs/src/layout/layout_051/treasure_spots_de.table"
	.else
	.ifdef TBS_EDITION_ES
	layout_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout051, .LField_Layout051_METATILES
	layout_offset Field_Layout051, .LField_Layout051_SHAPES
	layout_offset Field_Layout051, .LField_Layout051_GRID
	layout_offset Field_Layout051, .LField_Layout051_ANIMATION
	layout_offset Field_Layout051, .LField_Layout051_BLEND
	layout_offset Field_Layout051, .LField_Layout051_MARKERS
.LField_Layout051_METATILES:
	.incbin "games/tbs/src/layout/layout_051/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout051_SHAPES:
	.incbin "games/tbs/src/layout/layout_051/shapes.table.lz"
	.balign 4, 0
.LField_Layout051_GRID:
	.incbin "games/tbs/src/layout/layout_051/grid_es.parts.lz"
	.balign 4, 0
.LField_Layout051_ANIMATION:
	.incbin "games/tbs/src/layout/layout_037/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout051_BLEND:
	.incbin "games/tbs/src/layout/layout_020/blend.script.lz"
	.balign 4, 0
.LField_Layout051_MARKERS:
	.incbin "games/tbs/src/layout/layout_051/treasure_spots_de.table"
	.else
	.ifdef TBS_EDITION_FR
	layout_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout051, .LField_Layout051_METATILES
	layout_offset Field_Layout051, .LField_Layout051_SHAPES
	layout_offset Field_Layout051, .LField_Layout051_GRID
	layout_offset Field_Layout051, .LField_Layout051_ANIMATION
	layout_offset Field_Layout051, .LField_Layout051_BLEND
	layout_offset Field_Layout051, .LField_Layout051_MARKERS
.LField_Layout051_METATILES:
	.incbin "games/tbs/src/layout/layout_051/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout051_SHAPES:
	.incbin "games/tbs/src/layout/layout_051/shapes.table.lz"
	.balign 4, 0
.LField_Layout051_GRID:
	.incbin "games/tbs/src/layout/layout_051/grid_es.parts.lz"
	.balign 4, 0
.LField_Layout051_ANIMATION:
	.incbin "games/tbs/src/layout/layout_037/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout051_BLEND:
	.incbin "games/tbs/src/layout/layout_020/blend.script.lz"
	.balign 4, 0
.LField_Layout051_MARKERS:
	.incbin "games/tbs/src/layout/layout_051/treasure_spots_de.table"
	.else
	.ifdef TBS_EDITION_IT
	layout_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout051, .LField_Layout051_METATILES
	layout_offset Field_Layout051, .LField_Layout051_SHAPES
	layout_offset Field_Layout051, .LField_Layout051_GRID
	layout_offset Field_Layout051, .LField_Layout051_ANIMATION
	layout_offset Field_Layout051, .LField_Layout051_BLEND
	layout_offset Field_Layout051, .LField_Layout051_MARKERS
.LField_Layout051_METATILES:
	.incbin "games/tbs/src/layout/layout_051/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout051_SHAPES:
	.incbin "games/tbs/src/layout/layout_051/shapes.table.lz"
	.balign 4, 0
.LField_Layout051_GRID:
	.incbin "games/tbs/src/layout/layout_051/grid_es.parts.lz"
	.balign 4, 0
.LField_Layout051_ANIMATION:
	.incbin "games/tbs/src/layout/layout_037/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout051_BLEND:
	.incbin "games/tbs/src/layout/layout_020/blend.script.lz"
	.balign 4, 0
.LField_Layout051_MARKERS:
	.incbin "games/tbs/src/layout/layout_051/treasure_spots_de.table"
	.else
	layout_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout051, .LField_Layout051_METATILES
	layout_offset Field_Layout051, .LField_Layout051_SHAPES
	layout_offset Field_Layout051, .LField_Layout051_GRID
	layout_offset Field_Layout051, .LField_Layout051_ANIMATION
	layout_offset Field_Layout051, .LField_Layout051_BLEND
	layout_offset Field_Layout051, .LField_Layout051_MARKERS
.LField_Layout051_METATILES:
	.incbin "games/tbs/src/layout/layout_051/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout051_SHAPES:
	.incbin "games/tbs/src/layout/layout_051/shapes.table.lz"
	.balign 4, 0
.LField_Layout051_GRID:
	.incbin "games/tbs/src/layout/layout_051/grid.parts.lz"
	.balign 4, 0
.LField_Layout051_ANIMATION:
	.incbin "games/tbs/src/layout/layout_037/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout051_BLEND:
	.incbin "games/tbs/src/layout/layout_020/blend.script.lz"
	.balign 4, 0
.LField_Layout051_MARKERS:
	.incbin "games/tbs/src/layout/layout_051/treasure_spots.table"
	.endif
	.endif
	.endif
	.endif
	.global Tileset_Set41Palette
Tileset_Set41Palette:
	.incbin "games/tbs/src/graphics/tileset/set_41/palette.gbapal.lz"
	.byte 0xc2, 0x82, 0x04	@ PACKER: left in the buffer by Field_Layout051+0x145
	.global Tileset_Set41TilesA
Tileset_Set41TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_41/tiles_a.4bpp.mtf"
	.global Tileset_Set41TilesB
Tileset_Set41TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_41/tiles_b.4bpp.mtf"
	.byte 0x69	@ PACKER: left in the buffer by Tileset_Set41TilesA+0x14b7
	.global Tileset_Set41TilesC
Tileset_Set41TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_41/tiles_c.4bpp.mtf"
	.byte 0xde	@ PACKER: left in the buffer by Tileset_Set40TilesA+0x2633
	.global Tileset_Set41TilesD
Tileset_Set41TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_41/tiles_d.4bpp.mtf"
