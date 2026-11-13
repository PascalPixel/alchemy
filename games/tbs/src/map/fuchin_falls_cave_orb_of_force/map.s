	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_FuchinFallsCaveOrbOfForce
Field_FuchinFallsCaveOrbOfForce:
	map_header 0, 0, 72, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_FuchinFallsCaveOrbOfForce, .LField_FuchinFallsCaveOrbOfForce_CELL_PICTURES
	map_offset Field_FuchinFallsCaveOrbOfForce, .LField_FuchinFallsCaveOrbOfForce_SHAPES
	map_offset Field_FuchinFallsCaveOrbOfForce, .LField_FuchinFallsCaveOrbOfForce_GRID
	map_offset Field_FuchinFallsCaveOrbOfForce, .LField_FuchinFallsCaveOrbOfForce_ANIMATION
	map_offset Field_FuchinFallsCaveOrbOfForce, .LField_FuchinFallsCaveOrbOfForce_BLEND
	map_offset Field_FuchinFallsCaveOrbOfForce, .LField_FuchinFallsCaveOrbOfForce_MARKERS
.LField_FuchinFallsCaveOrbOfForce_CELL_PICTURES:
	.incbin "games/tbs/src/map/fuchin_falls_cave_orb_of_force/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_FuchinFallsCaveOrbOfForce_SHAPES:
	.incbin "games/tbs/src/map/fuchin_falls_cave_orb_of_force/shapes.table.lz"
	.balign 4, 0
.LField_FuchinFallsCaveOrbOfForce_GRID:
	.incbin "games/tbs/src/map/fuchin_falls_cave_orb_of_force/map.grid.lz"
	.balign 4, 0
.LField_FuchinFallsCaveOrbOfForce_ANIMATION:
	.incbin "games/tbs/src/map/bilibin_treasury/tileset_anims.script.lz"
	.balign 4, 0
.LField_FuchinFallsCaveOrbOfForce_BLEND:
	.incbin "games/tbs/src/map/fuchin_falls_cave_zephyr/blend.script.lz"
	.balign 4, 0
.LField_FuchinFallsCaveOrbOfForce_MARKERS:
	treasure_spot 17, 5, 100
	treasure_spots_end
	.balign 4, 0
