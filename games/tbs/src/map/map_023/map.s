	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map023
Field_Map023:
	.ifdef TBS_EDITION_ES
	map_header 0, 0, 96, 115, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 115, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map023, .LField_Map023_METATILES
	map_offset Field_Map023, .LField_Map023_SHAPES
	map_offset Field_Map023, .LField_Map023_GRID
	map_offset Field_Map023, .LField_Map023_ANIMATION
	map_offset Field_Map023, .LField_Map023_BLEND
	map_offset Field_Map023, .LField_Map023_MARKERS
.LField_Map023_METATILES:
	.incbin "games/tbs/src/map/map_023/metatiles_es.delta2.lz"
	.balign 4, 0
.LField_Map023_SHAPES:
	.incbin "games/tbs/src/map/map_023/shapes.table.lz"
	.balign 4, 0
.LField_Map023_GRID:
	.incbin "games/tbs/src/map/map_023/map_es.grid.lz"
	.balign 4, 0
.LField_Map023_ANIMATION:
	.incbin "games/tbs/src/map/map_023/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map023_BLEND:
	.incbin "games/tbs/src/map/map_022/blend.script.lz"
	.balign 4, 0
.LField_Map023_MARKERS:
	treasure_spot 38, 41, 100
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_FR
	map_header 0, 0, 96, 115, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 115, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map023, .LField_Map023_METATILES
	map_offset Field_Map023, .LField_Map023_SHAPES
	map_offset Field_Map023, .LField_Map023_GRID
	map_offset Field_Map023, .LField_Map023_ANIMATION
	map_offset Field_Map023, .LField_Map023_BLEND
	map_offset Field_Map023, .LField_Map023_MARKERS
.LField_Map023_METATILES:
	.incbin "games/tbs/src/map/map_023/metatiles_es.delta2.lz"
	.balign 4, 0
.LField_Map023_SHAPES:
	.incbin "games/tbs/src/map/map_023/shapes.table.lz"
	.balign 4, 0
.LField_Map023_GRID:
	.incbin "games/tbs/src/map/map_023/map_es.grid.lz"
	.balign 4, 0
.LField_Map023_ANIMATION:
	.incbin "games/tbs/src/map/map_023/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map023_BLEND:
	.incbin "games/tbs/src/map/map_022/blend.script.lz"
	.balign 4, 0
.LField_Map023_MARKERS:
	treasure_spot 38, 41, 100
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_IT
	map_header 0, 0, 96, 115, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 115, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map023, .LField_Map023_METATILES
	map_offset Field_Map023, .LField_Map023_SHAPES
	map_offset Field_Map023, .LField_Map023_GRID
	map_offset Field_Map023, .LField_Map023_ANIMATION
	map_offset Field_Map023, .LField_Map023_BLEND
	map_offset Field_Map023, .LField_Map023_MARKERS
.LField_Map023_METATILES:
	.incbin "games/tbs/src/map/map_023/metatiles_es.delta2.lz"
	.balign 4, 0
.LField_Map023_SHAPES:
	.incbin "games/tbs/src/map/map_023/shapes.table.lz"
	.balign 4, 0
.LField_Map023_GRID:
	.incbin "games/tbs/src/map/map_023/map_es.grid.lz"
	.balign 4, 0
.LField_Map023_ANIMATION:
	.incbin "games/tbs/src/map/map_023/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map023_BLEND:
	.incbin "games/tbs/src/map/map_022/blend.script.lz"
	.balign 4, 0
.LField_Map023_MARKERS:
	treasure_spot 38, 41, 100
	treasure_spots_end
	.else
	map_header 0, 0, 96, 115, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 115, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map023, .LField_Map023_METATILES
	map_offset Field_Map023, .LField_Map023_SHAPES
	map_offset Field_Map023, .LField_Map023_GRID
	map_offset Field_Map023, .LField_Map023_ANIMATION
	map_offset Field_Map023, .LField_Map023_BLEND
	map_offset Field_Map023, .LField_Map023_MARKERS
.LField_Map023_METATILES:
	.incbin "games/tbs/src/map/map_023/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map023_SHAPES:
	.incbin "games/tbs/src/map/map_023/shapes.table.lz"
	.balign 4, 0
.LField_Map023_GRID:
	.incbin "games/tbs/src/map/map_023/map.grid.lz"
	.balign 4, 0
.LField_Map023_ANIMATION:
	.incbin "games/tbs/src/map/map_023/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map023_BLEND:
	.incbin "games/tbs/src/map/map_022/blend.script.lz"
	.balign 4, 0
.LField_Map023_MARKERS:
	treasure_spot 38, 41, 100
	treasure_spots_end
	.endif
	.endif
	.endif
	.balign 4, 0
