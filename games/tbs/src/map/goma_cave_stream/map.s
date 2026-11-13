	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_GomaCaveStream
Field_GomaCaveStream:
	.ifdef TBS_EDITION_ES
	map_header 0, 0, 96, 115, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 115, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_GomaCaveStream, .LField_GomaCaveStream_CELL_PICTURES
	map_offset Field_GomaCaveStream, .LField_GomaCaveStream_SHAPES
	map_offset Field_GomaCaveStream, .LField_GomaCaveStream_GRID
	map_offset Field_GomaCaveStream, .LField_GomaCaveStream_ANIMATION
	map_offset Field_GomaCaveStream, .LField_GomaCaveStream_BLEND
	map_offset Field_GomaCaveStream, .LField_GomaCaveStream_MARKERS
.LField_GomaCaveStream_CELL_PICTURES:
	.incbin "games/tbs/src/map/goma_cave_stream/cell_pictures_es.delta2.lz"
	.balign 4, 0
.LField_GomaCaveStream_SHAPES:
	.incbin "games/tbs/src/map/goma_cave_stream/shapes.table.lz"
	.balign 4, 0
.LField_GomaCaveStream_GRID:
	.incbin "games/tbs/src/map/goma_cave_stream/map_es.grid.lz"
	.balign 4, 0
.LField_GomaCaveStream_ANIMATION:
	.incbin "games/tbs/src/map/goma_cave_stream/tileset_anims.script.lz"
	.balign 4, 0
.LField_GomaCaveStream_BLEND:
	.incbin "games/tbs/src/map/goma_cave_pillar/blend.script.lz"
	.balign 4, 0
.LField_GomaCaveStream_MARKERS:
	treasure_spot 38, 41, 100
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_FR
	map_header 0, 0, 96, 115, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 115, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_GomaCaveStream, .LField_GomaCaveStream_CELL_PICTURES
	map_offset Field_GomaCaveStream, .LField_GomaCaveStream_SHAPES
	map_offset Field_GomaCaveStream, .LField_GomaCaveStream_GRID
	map_offset Field_GomaCaveStream, .LField_GomaCaveStream_ANIMATION
	map_offset Field_GomaCaveStream, .LField_GomaCaveStream_BLEND
	map_offset Field_GomaCaveStream, .LField_GomaCaveStream_MARKERS
.LField_GomaCaveStream_CELL_PICTURES:
	.incbin "games/tbs/src/map/goma_cave_stream/cell_pictures_es.delta2.lz"
	.balign 4, 0
.LField_GomaCaveStream_SHAPES:
	.incbin "games/tbs/src/map/goma_cave_stream/shapes.table.lz"
	.balign 4, 0
.LField_GomaCaveStream_GRID:
	.incbin "games/tbs/src/map/goma_cave_stream/map_es.grid.lz"
	.balign 4, 0
.LField_GomaCaveStream_ANIMATION:
	.incbin "games/tbs/src/map/goma_cave_stream/tileset_anims.script.lz"
	.balign 4, 0
.LField_GomaCaveStream_BLEND:
	.incbin "games/tbs/src/map/goma_cave_pillar/blend.script.lz"
	.balign 4, 0
.LField_GomaCaveStream_MARKERS:
	treasure_spot 38, 41, 100
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_IT
	map_header 0, 0, 96, 115, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 115, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_GomaCaveStream, .LField_GomaCaveStream_CELL_PICTURES
	map_offset Field_GomaCaveStream, .LField_GomaCaveStream_SHAPES
	map_offset Field_GomaCaveStream, .LField_GomaCaveStream_GRID
	map_offset Field_GomaCaveStream, .LField_GomaCaveStream_ANIMATION
	map_offset Field_GomaCaveStream, .LField_GomaCaveStream_BLEND
	map_offset Field_GomaCaveStream, .LField_GomaCaveStream_MARKERS
.LField_GomaCaveStream_CELL_PICTURES:
	.incbin "games/tbs/src/map/goma_cave_stream/cell_pictures_es.delta2.lz"
	.balign 4, 0
.LField_GomaCaveStream_SHAPES:
	.incbin "games/tbs/src/map/goma_cave_stream/shapes.table.lz"
	.balign 4, 0
.LField_GomaCaveStream_GRID:
	.incbin "games/tbs/src/map/goma_cave_stream/map_es.grid.lz"
	.balign 4, 0
.LField_GomaCaveStream_ANIMATION:
	.incbin "games/tbs/src/map/goma_cave_stream/tileset_anims.script.lz"
	.balign 4, 0
.LField_GomaCaveStream_BLEND:
	.incbin "games/tbs/src/map/goma_cave_pillar/blend.script.lz"
	.balign 4, 0
.LField_GomaCaveStream_MARKERS:
	treasure_spot 38, 41, 100
	treasure_spots_end
	.else
	map_header 0, 0, 96, 115, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 115, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_GomaCaveStream, .LField_GomaCaveStream_CELL_PICTURES
	map_offset Field_GomaCaveStream, .LField_GomaCaveStream_SHAPES
	map_offset Field_GomaCaveStream, .LField_GomaCaveStream_GRID
	map_offset Field_GomaCaveStream, .LField_GomaCaveStream_ANIMATION
	map_offset Field_GomaCaveStream, .LField_GomaCaveStream_BLEND
	map_offset Field_GomaCaveStream, .LField_GomaCaveStream_MARKERS
.LField_GomaCaveStream_CELL_PICTURES:
	.incbin "games/tbs/src/map/goma_cave_stream/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_GomaCaveStream_SHAPES:
	.incbin "games/tbs/src/map/goma_cave_stream/shapes.table.lz"
	.balign 4, 0
.LField_GomaCaveStream_GRID:
	.incbin "games/tbs/src/map/goma_cave_stream/map.grid.lz"
	.balign 4, 0
.LField_GomaCaveStream_ANIMATION:
	.incbin "games/tbs/src/map/goma_cave_stream/tileset_anims.script.lz"
	.balign 4, 0
.LField_GomaCaveStream_BLEND:
	.incbin "games/tbs/src/map/goma_cave_pillar/blend.script.lz"
	.balign 4, 0
.LField_GomaCaveStream_MARKERS:
	treasure_spot 38, 41, 100
	treasure_spots_end
	.endif
	.endif
	.endif
	.balign 4, 0
