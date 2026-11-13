	.global Field_FuchinFallsCaveArcticBlade
Field_FuchinFallsCaveArcticBlade:
	map_header 0, 0, 72, 106, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 106, 16, 16, 0, 0, 0xff, 0xff
	map_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_FuchinFallsCaveArcticBlade, .LField_FuchinFallsCaveArcticBlade_CELL_PICTURES
	map_offset Field_FuchinFallsCaveArcticBlade, .LField_FuchinFallsCaveArcticBlade_SHAPES
	map_offset Field_FuchinFallsCaveArcticBlade, .LField_FuchinFallsCaveArcticBlade_GRID
	map_offset Field_FuchinFallsCaveArcticBlade, .LField_FuchinFallsCaveArcticBlade_ANIMATION
	map_offset Field_FuchinFallsCaveArcticBlade, .LField_FuchinFallsCaveArcticBlade_BLEND
	map_offset Field_FuchinFallsCaveArcticBlade, .LField_FuchinFallsCaveArcticBlade_MARKERS
.LField_FuchinFallsCaveArcticBlade_CELL_PICTURES:
	.incbin "games/tbs/src/map/fuchin_falls_cave_arctic_blade/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_FuchinFallsCaveArcticBlade_SHAPES:
	.incbin "games/tbs/src/map/fuchin_falls_cave_entrance/shapes.table.lz"
	.balign 4, 0
.LField_FuchinFallsCaveArcticBlade_GRID:
	.incbin "games/tbs/src/map/fuchin_falls_cave_arctic_blade/map.grid.lz"
	.balign 4, 0
.LField_FuchinFallsCaveArcticBlade_ANIMATION:
	.incbin "games/tbs/src/map/bilibin_treasury/tileset_anims.script.lz"
	.balign 4, 0
.LField_FuchinFallsCaveArcticBlade_BLEND:
	.incbin "games/tbs/src/map/fuchin_falls_cave_zephyr/blend.script.lz"
	.balign 4, 0
.LField_FuchinFallsCaveArcticBlade_MARKERS:
	treasure_spot 11, 8, 100
	treasure_spots_end
	.balign 4, 0
