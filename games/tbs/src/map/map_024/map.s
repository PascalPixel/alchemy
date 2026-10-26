	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map024
Field_Map024:
	.ifdef TBS_EDITION_DE
	map_header 0, 0, 96, 60, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 60, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map024, .LField_Map024_METATILES
	map_offset Field_Map024, .LField_Map024_SHAPES
	map_offset Field_Map024, .LField_Map024_GRID
	map_offset Field_Map024, .LField_Map024_ANIMATION
	map_offset Field_Map024, .LField_Map024_BLEND
	map_offset Field_Map024, .LField_Map024_MARKERS
.LField_Map024_METATILES:
	.incbin "games/tbs/src/map/map_024/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map024_SHAPES:
	.incbin "games/tbs/src/map/map_024/shapes.table.lz"
	.balign 4, 0
.LField_Map024_GRID:
	.incbin "games/tbs/src/map/map_024/map_de.grid.lz"
	.balign 4, 0
.LField_Map024_ANIMATION:
	.incbin "games/tbs/src/map/map_022/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map024_BLEND:
	.incbin "games/tbs/src/map/map_022/blend.script.lz"
	.balign 4, 0
.LField_Map024_MARKERS:
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_ES
	map_header 0, 0, 96, 60, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 60, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map024, .LField_Map024_METATILES
	map_offset Field_Map024, .LField_Map024_SHAPES
	map_offset Field_Map024, .LField_Map024_GRID
	map_offset Field_Map024, .LField_Map024_ANIMATION
	map_offset Field_Map024, .LField_Map024_BLEND
	map_offset Field_Map024, .LField_Map024_MARKERS
.LField_Map024_METATILES:
	.incbin "games/tbs/src/map/map_024/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map024_SHAPES:
	.incbin "games/tbs/src/map/map_024/shapes.table.lz"
	.balign 4, 0
.LField_Map024_GRID:
	.incbin "games/tbs/src/map/map_024/map_de.grid.lz"
	.balign 4, 0
.LField_Map024_ANIMATION:
	.incbin "games/tbs/src/map/map_022/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map024_BLEND:
	.incbin "games/tbs/src/map/map_022/blend.script.lz"
	.balign 4, 0
.LField_Map024_MARKERS:
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_FR
	map_header 0, 0, 96, 60, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 60, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map024, .LField_Map024_METATILES
	map_offset Field_Map024, .LField_Map024_SHAPES
	map_offset Field_Map024, .LField_Map024_GRID
	map_offset Field_Map024, .LField_Map024_ANIMATION
	map_offset Field_Map024, .LField_Map024_BLEND
	map_offset Field_Map024, .LField_Map024_MARKERS
.LField_Map024_METATILES:
	.incbin "games/tbs/src/map/map_024/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map024_SHAPES:
	.incbin "games/tbs/src/map/map_024/shapes.table.lz"
	.balign 4, 0
.LField_Map024_GRID:
	.incbin "games/tbs/src/map/map_024/map_de.grid.lz"
	.balign 4, 0
.LField_Map024_ANIMATION:
	.incbin "games/tbs/src/map/map_022/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map024_BLEND:
	.incbin "games/tbs/src/map/map_022/blend.script.lz"
	.balign 4, 0
.LField_Map024_MARKERS:
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_IT
	map_header 0, 0, 96, 60, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 60, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map024, .LField_Map024_METATILES
	map_offset Field_Map024, .LField_Map024_SHAPES
	map_offset Field_Map024, .LField_Map024_GRID
	map_offset Field_Map024, .LField_Map024_ANIMATION
	map_offset Field_Map024, .LField_Map024_BLEND
	map_offset Field_Map024, .LField_Map024_MARKERS
.LField_Map024_METATILES:
	.incbin "games/tbs/src/map/map_024/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map024_SHAPES:
	.incbin "games/tbs/src/map/map_024/shapes.table.lz"
	.balign 4, 0
.LField_Map024_GRID:
	.incbin "games/tbs/src/map/map_024/map_de.grid.lz"
	.balign 4, 0
.LField_Map024_ANIMATION:
	.incbin "games/tbs/src/map/map_022/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map024_BLEND:
	.incbin "games/tbs/src/map/map_022/blend.script.lz"
	.balign 4, 0
.LField_Map024_MARKERS:
	treasure_spots_end
	.else
	map_header 0, 0, 96, 60, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 60, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map024, .LField_Map024_METATILES
	map_offset Field_Map024, .LField_Map024_SHAPES
	map_offset Field_Map024, .LField_Map024_GRID
	map_offset Field_Map024, .LField_Map024_ANIMATION
	map_offset Field_Map024, .LField_Map024_BLEND
	map_offset Field_Map024, .LField_Map024_MARKERS
.LField_Map024_METATILES:
	.incbin "games/tbs/src/map/map_024/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map024_SHAPES:
	.incbin "games/tbs/src/map/map_024/shapes.table.lz"
	.balign 4, 0
.LField_Map024_GRID:
	.incbin "games/tbs/src/map/map_024/map.grid.lz"
	.balign 4, 0
.LField_Map024_ANIMATION:
	.incbin "games/tbs/src/map/map_022/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map024_BLEND:
	.incbin "games/tbs/src/map/map_022/blend.script.lz"
	.balign 4, 0
.LField_Map024_MARKERS:
	treasure_spots_end
	.endif
	.endif
	.endif
	.endif
	.balign 4, 0
