	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map134
Field_Map134:
	.ifdef TBS_EDITION_DE
	map_header 0, 0, 96, 96, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map134, .LField_Map134_CELL_PICTURES
	map_offset Field_Map134, .LField_Map134_SHAPES
	map_offset Field_Map134, .LField_Map134_GRID
	map_offset Field_Map134, .LField_Map134_ANIMATION
	map_offset Field_Map134, .LField_Map134_BLEND
	map_offset Field_Map134, .LField_Map134_MARKERS
.LField_Map134_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_134/cell_pictures_de.delta1.lz"
	.balign 4, 0
.LField_Map134_SHAPES:
	.incbin "games/tbs/src/map/map_130/shapes.table.lz"
	.balign 4, 0
.LField_Map134_GRID:
	.incbin "games/tbs/src/map/map_134/map_de.grid.lz"
	.balign 4, 0
.LField_Map134_ANIMATION:
	.incbin "games/tbs/src/map/map_134/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map134_BLEND:
	.incbin "games/tbs/src/map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map134_MARKERS:
	treasure_spot 12, 8, 100
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_ES
	map_header 0, 0, 96, 96, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map134, .LField_Map134_CELL_PICTURES
	map_offset Field_Map134, .LField_Map134_SHAPES
	map_offset Field_Map134, .LField_Map134_GRID
	map_offset Field_Map134, .LField_Map134_ANIMATION
	map_offset Field_Map134, .LField_Map134_BLEND
	map_offset Field_Map134, .LField_Map134_MARKERS
.LField_Map134_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_134/cell_pictures_de.delta1.lz"
	.balign 4, 0
.LField_Map134_SHAPES:
	.incbin "games/tbs/src/map/map_130/shapes.table.lz"
	.balign 4, 0
.LField_Map134_GRID:
	.incbin "games/tbs/src/map/map_134/map_de.grid.lz"
	.balign 4, 0
.LField_Map134_ANIMATION:
	.incbin "games/tbs/src/map/map_134/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map134_BLEND:
	.incbin "games/tbs/src/map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map134_MARKERS:
	treasure_spot 12, 8, 100
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_FR
	map_header 0, 0, 96, 96, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map134, .LField_Map134_CELL_PICTURES
	map_offset Field_Map134, .LField_Map134_SHAPES
	map_offset Field_Map134, .LField_Map134_GRID
	map_offset Field_Map134, .LField_Map134_ANIMATION
	map_offset Field_Map134, .LField_Map134_BLEND
	map_offset Field_Map134, .LField_Map134_MARKERS
.LField_Map134_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_134/cell_pictures_de.delta1.lz"
	.balign 4, 0
.LField_Map134_SHAPES:
	.incbin "games/tbs/src/map/map_130/shapes.table.lz"
	.balign 4, 0
.LField_Map134_GRID:
	.incbin "games/tbs/src/map/map_134/map_de.grid.lz"
	.balign 4, 0
.LField_Map134_ANIMATION:
	.incbin "games/tbs/src/map/map_134/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map134_BLEND:
	.incbin "games/tbs/src/map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map134_MARKERS:
	treasure_spot 12, 8, 100
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_IT
	map_header 0, 0, 96, 96, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map134, .LField_Map134_CELL_PICTURES
	map_offset Field_Map134, .LField_Map134_SHAPES
	map_offset Field_Map134, .LField_Map134_GRID
	map_offset Field_Map134, .LField_Map134_ANIMATION
	map_offset Field_Map134, .LField_Map134_BLEND
	map_offset Field_Map134, .LField_Map134_MARKERS
.LField_Map134_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_134/cell_pictures_de.delta1.lz"
	.balign 4, 0
.LField_Map134_SHAPES:
	.incbin "games/tbs/src/map/map_130/shapes.table.lz"
	.balign 4, 0
.LField_Map134_GRID:
	.incbin "games/tbs/src/map/map_134/map_de.grid.lz"
	.balign 4, 0
.LField_Map134_ANIMATION:
	.incbin "games/tbs/src/map/map_134/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map134_BLEND:
	.incbin "games/tbs/src/map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map134_MARKERS:
	treasure_spot 12, 8, 100
	treasure_spots_end
	.else
	map_header 0, 0, 96, 96, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map134, .LField_Map134_CELL_PICTURES
	map_offset Field_Map134, .LField_Map134_SHAPES
	map_offset Field_Map134, .LField_Map134_GRID
	map_offset Field_Map134, .LField_Map134_ANIMATION
	map_offset Field_Map134, .LField_Map134_BLEND
	map_offset Field_Map134, .LField_Map134_MARKERS
.LField_Map134_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_134/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map134_SHAPES:
	.incbin "games/tbs/src/map/map_130/shapes.table.lz"
	.balign 4, 0
.LField_Map134_GRID:
	.incbin "games/tbs/src/map/map_134/map.grid.lz"
	.balign 4, 0
.LField_Map134_ANIMATION:
	.incbin "games/tbs/src/map/map_134/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map134_BLEND:
	.incbin "games/tbs/src/map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map134_MARKERS:
	treasure_spot 12, 8, 100
	treasure_spots_end
	.endif
	.endif
	.endif
	.endif
	.balign 4, 0
