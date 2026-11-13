	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_FuchinFallsCaveDragonStatue
Field_FuchinFallsCaveDragonStatue:
	map_header 0, 0, 72, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_FuchinFallsCaveDragonStatue, .LField_FuchinFallsCaveDragonStatue_CELL_PICTURES
	map_offset Field_FuchinFallsCaveDragonStatue, .LField_FuchinFallsCaveDragonStatue_SHAPES
	map_offset Field_FuchinFallsCaveDragonStatue, .LField_FuchinFallsCaveDragonStatue_GRID
	map_offset Field_FuchinFallsCaveDragonStatue, .LField_FuchinFallsCaveDragonStatue_ANIMATION
	map_offset Field_FuchinFallsCaveDragonStatue, .LField_FuchinFallsCaveDragonStatue_BLEND
	map_offset Field_FuchinFallsCaveDragonStatue, .LField_FuchinFallsCaveDragonStatue_MARKERS
.LField_FuchinFallsCaveDragonStatue_CELL_PICTURES:
	.incbin "games/tbs/src/map/fuchin_falls_cave_dragon_statue/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_FuchinFallsCaveDragonStatue_SHAPES:
	.incbin "games/tbs/src/map/fuchin_falls_cave_dragon_statue/shapes.table.lz"
	.balign 4, 0
.LField_FuchinFallsCaveDragonStatue_GRID:
	.incbin "games/tbs/src/map/fuchin_falls_cave_dragon_statue/map.grid.lz"
	.balign 4, 0
.LField_FuchinFallsCaveDragonStatue_ANIMATION:
	.incbin "games/tbs/src/map/bilibin_treasury/tileset_anims.script.lz"
	.balign 4, 0
.LField_FuchinFallsCaveDragonStatue_BLEND:
	.incbin "games/tbs/src/map/fuchin_falls_cave_zephyr/blend.script.lz"
	.balign 4, 0
.LField_FuchinFallsCaveDragonStatue_MARKERS:
	treasure_spots_end
	.balign 4, 0
