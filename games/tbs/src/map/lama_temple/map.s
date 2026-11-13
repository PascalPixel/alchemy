	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_LamaTemple
Field_LamaTemple:
	map_header 0, 0, 55, 68, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_LamaTemple, .LField_LamaTemple_CELL_PICTURES
	map_offset Field_LamaTemple, .LField_LamaTemple_SHAPES
	map_offset Field_LamaTemple, .LField_LamaTemple_GRID
	map_offset Field_LamaTemple, .LField_LamaTemple_ANIMATION
	map_offset Field_LamaTemple, .LField_LamaTemple_BLEND
	map_offset Field_LamaTemple, .LField_LamaTemple_MARKERS
.LField_LamaTemple_CELL_PICTURES:
	.incbin "games/tbs/src/map/lama_temple/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_LamaTemple_SHAPES:
	.incbin "games/tbs/src/map/lama_temple/shapes.table.lz"
	.balign 4, 0
.LField_LamaTemple_GRID:
	.incbin "games/tbs/src/map/lama_temple/map.grid.lz"
	.balign 4, 0
.LField_LamaTemple_ANIMATION:
	.incbin "games/tbs/src/map/vale_cave_mouth/tileset_anims.script.lz"
	.balign 4, 0
.LField_LamaTemple_BLEND:
	.incbin "games/tbs/src/map/goma_cave_entrance/blend.script.lz"
	.balign 4, 0
.LField_LamaTemple_MARKERS:
	treasure_spot 21, 9, 100
	treasure_spot 4, 15, 110
	treasure_spots_end
	.balign 4, 0
