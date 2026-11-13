	.global Field_FuchinFallsCaveZephyr
Field_FuchinFallsCaveZephyr:
	map_header 0, 0, 72, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_FuchinFallsCaveZephyr, .LField_FuchinFallsCaveZephyr_CELL_PICTURES
	map_offset Field_FuchinFallsCaveZephyr, .LField_FuchinFallsCaveZephyr_SHAPES
	map_offset Field_FuchinFallsCaveZephyr, .LField_FuchinFallsCaveZephyr_GRID
	map_offset Field_FuchinFallsCaveZephyr, .LField_FuchinFallsCaveZephyr_ANIMATION
	map_offset Field_FuchinFallsCaveZephyr, .LField_FuchinFallsCaveZephyr_BLEND
	map_offset Field_FuchinFallsCaveZephyr, .LField_FuchinFallsCaveZephyr_MARKERS
.LField_FuchinFallsCaveZephyr_CELL_PICTURES:
	.incbin "games/tbs/src/map/fuchin_falls_cave_zephyr/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_FuchinFallsCaveZephyr_SHAPES:
	.incbin "games/tbs/src/map/fuchin_falls_cave_zephyr/shapes.table.lz"
	.balign 4, 0
.LField_FuchinFallsCaveZephyr_GRID:
	.incbin "games/tbs/src/map/fuchin_falls_cave_zephyr/map.grid.lz"
	.balign 4, 0
.LField_FuchinFallsCaveZephyr_ANIMATION:
	.incbin "games/tbs/src/map/bilibin_treasury/tileset_anims.script.lz"
	.balign 4, 0
.LField_FuchinFallsCaveZephyr_BLEND:
	.incbin "games/tbs/src/map/fuchin_falls_cave_zephyr/blend.script.lz"
	.balign 4, 0
.LField_FuchinFallsCaveZephyr_MARKERS:
	treasure_spots_end
	.balign 4, 0
