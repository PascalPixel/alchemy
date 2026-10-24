	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout024
Field_Layout024:
	.ifdef TBS_EDITION_DE
	layout_header 0, 0, 96, 60, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 60, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout024, .LField_Layout024_METATILES
	layout_offset Field_Layout024, .LField_Layout024_SHAPES
	layout_offset Field_Layout024, .LField_Layout024_GRID
	layout_offset Field_Layout024, .LField_Layout024_ANIMATION
	layout_offset Field_Layout024, .LField_Layout024_BLEND
	layout_offset Field_Layout024, .LField_Layout024_MARKERS
.LField_Layout024_METATILES:
	.incbin "games/tbs/src/layout/layout_024/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout024_SHAPES:
	.incbin "games/tbs/src/layout/layout_024/shapes.table.lz"
	.balign 4, 0
.LField_Layout024_GRID:
	.incbin "games/tbs/src/layout/layout_024/map_de.grid.lz"
	.balign 4, 0
.LField_Layout024_ANIMATION:
	.incbin "games/tbs/src/layout/layout_022/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout024_BLEND:
	.incbin "games/tbs/src/layout/layout_022/blend.script.lz"
	.balign 4, 0
.LField_Layout024_MARKERS:
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_ES
	layout_header 0, 0, 96, 60, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 60, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout024, .LField_Layout024_METATILES
	layout_offset Field_Layout024, .LField_Layout024_SHAPES
	layout_offset Field_Layout024, .LField_Layout024_GRID
	layout_offset Field_Layout024, .LField_Layout024_ANIMATION
	layout_offset Field_Layout024, .LField_Layout024_BLEND
	layout_offset Field_Layout024, .LField_Layout024_MARKERS
.LField_Layout024_METATILES:
	.incbin "games/tbs/src/layout/layout_024/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout024_SHAPES:
	.incbin "games/tbs/src/layout/layout_024/shapes.table.lz"
	.balign 4, 0
.LField_Layout024_GRID:
	.incbin "games/tbs/src/layout/layout_024/map_de.grid.lz"
	.balign 4, 0
.LField_Layout024_ANIMATION:
	.incbin "games/tbs/src/layout/layout_022/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout024_BLEND:
	.incbin "games/tbs/src/layout/layout_022/blend.script.lz"
	.balign 4, 0
.LField_Layout024_MARKERS:
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_FR
	layout_header 0, 0, 96, 60, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 60, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout024, .LField_Layout024_METATILES
	layout_offset Field_Layout024, .LField_Layout024_SHAPES
	layout_offset Field_Layout024, .LField_Layout024_GRID
	layout_offset Field_Layout024, .LField_Layout024_ANIMATION
	layout_offset Field_Layout024, .LField_Layout024_BLEND
	layout_offset Field_Layout024, .LField_Layout024_MARKERS
.LField_Layout024_METATILES:
	.incbin "games/tbs/src/layout/layout_024/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout024_SHAPES:
	.incbin "games/tbs/src/layout/layout_024/shapes.table.lz"
	.balign 4, 0
.LField_Layout024_GRID:
	.incbin "games/tbs/src/layout/layout_024/map_de.grid.lz"
	.balign 4, 0
.LField_Layout024_ANIMATION:
	.incbin "games/tbs/src/layout/layout_022/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout024_BLEND:
	.incbin "games/tbs/src/layout/layout_022/blend.script.lz"
	.balign 4, 0
.LField_Layout024_MARKERS:
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_IT
	layout_header 0, 0, 96, 60, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 60, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout024, .LField_Layout024_METATILES
	layout_offset Field_Layout024, .LField_Layout024_SHAPES
	layout_offset Field_Layout024, .LField_Layout024_GRID
	layout_offset Field_Layout024, .LField_Layout024_ANIMATION
	layout_offset Field_Layout024, .LField_Layout024_BLEND
	layout_offset Field_Layout024, .LField_Layout024_MARKERS
.LField_Layout024_METATILES:
	.incbin "games/tbs/src/layout/layout_024/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout024_SHAPES:
	.incbin "games/tbs/src/layout/layout_024/shapes.table.lz"
	.balign 4, 0
.LField_Layout024_GRID:
	.incbin "games/tbs/src/layout/layout_024/map_de.grid.lz"
	.balign 4, 0
.LField_Layout024_ANIMATION:
	.incbin "games/tbs/src/layout/layout_022/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout024_BLEND:
	.incbin "games/tbs/src/layout/layout_022/blend.script.lz"
	.balign 4, 0
.LField_Layout024_MARKERS:
	treasure_spots_end
	.else
	layout_header 0, 0, 96, 60, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 60, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout024, .LField_Layout024_METATILES
	layout_offset Field_Layout024, .LField_Layout024_SHAPES
	layout_offset Field_Layout024, .LField_Layout024_GRID
	layout_offset Field_Layout024, .LField_Layout024_ANIMATION
	layout_offset Field_Layout024, .LField_Layout024_BLEND
	layout_offset Field_Layout024, .LField_Layout024_MARKERS
.LField_Layout024_METATILES:
	.incbin "games/tbs/src/layout/layout_024/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout024_SHAPES:
	.incbin "games/tbs/src/layout/layout_024/shapes.table.lz"
	.balign 4, 0
.LField_Layout024_GRID:
	.incbin "games/tbs/src/layout/layout_024/map.grid.lz"
	.balign 4, 0
.LField_Layout024_ANIMATION:
	.incbin "games/tbs/src/layout/layout_022/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout024_BLEND:
	.incbin "games/tbs/src/layout/layout_022/blend.script.lz"
	.balign 4, 0
.LField_Layout024_MARKERS:
	treasure_spots_end
	.endif
	.endif
	.endif
	.endif
	.balign 4, 0
