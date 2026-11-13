	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_FuchinFallsCaveDragonsEye
Field_FuchinFallsCaveDragonsEye:
	map_header 0, 0, 72, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_FuchinFallsCaveDragonsEye, .LField_FuchinFallsCaveDragonsEye_CELL_PICTURES
	map_offset Field_FuchinFallsCaveDragonsEye, .LField_FuchinFallsCaveDragonsEye_SHAPES
	map_offset Field_FuchinFallsCaveDragonsEye, .LField_FuchinFallsCaveDragonsEye_GRID
	map_offset Field_FuchinFallsCaveDragonsEye, .LField_FuchinFallsCaveDragonsEye_ANIMATION
	map_offset Field_FuchinFallsCaveDragonsEye, .LField_FuchinFallsCaveDragonsEye_BLEND
	map_offset Field_FuchinFallsCaveDragonsEye, .LField_FuchinFallsCaveDragonsEye_MARKERS
.LField_FuchinFallsCaveDragonsEye_CELL_PICTURES:
	.incbin "games/tbs/src/map/fuchin_falls_cave_dragons_eye/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_FuchinFallsCaveDragonsEye_SHAPES:
	.incbin "games/tbs/src/map/fuchin_falls_cave_dragons_eye/shapes.table.lz"
	.balign 4, 0
.LField_FuchinFallsCaveDragonsEye_GRID:
	.incbin "games/tbs/src/map/fuchin_falls_cave_dragons_eye/map.grid.lz"
	.balign 4, 0
.LField_FuchinFallsCaveDragonsEye_ANIMATION:
	.incbin "games/tbs/src/map/bilibin_treasury/tileset_anims.script.lz"
	.balign 4, 0
.LField_FuchinFallsCaveDragonsEye_BLEND:
	.incbin "games/tbs/src/map/fuchin_falls_cave_zephyr/blend.script.lz"
	.balign 4, 0
.LField_FuchinFallsCaveDragonsEye_MARKERS:
	treasure_spots_end
	.balign 4, 0
