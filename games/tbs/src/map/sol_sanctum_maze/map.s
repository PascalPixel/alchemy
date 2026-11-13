	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_SolSanctumMaze
Field_SolSanctumMaze:
	map_header 0, 0, 104, 80, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 80, 16, 16, 0, 0, 0xff, 0xff
	map_layer 104, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_SolSanctumMaze, .LField_SolSanctumMaze_CELL_PICTURES
	map_offset Field_SolSanctumMaze, .LField_SolSanctumMaze_SHAPES
	map_offset Field_SolSanctumMaze, .LField_SolSanctumMaze_GRID
	map_offset Field_SolSanctumMaze, .LField_SolSanctumMaze_ANIMATION
	map_offset Field_SolSanctumMaze, .LField_SolSanctumMaze_BLEND
	map_offset Field_SolSanctumMaze, .LField_SolSanctumMaze_MARKERS
.LField_SolSanctumMaze_CELL_PICTURES:
	.incbin "games/tbs/src/map/sol_sanctum_maze/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_SolSanctumMaze_SHAPES:
	.incbin "games/tbs/src/map/map_001/shapes.table.lz"
	.balign 4, 0
.LField_SolSanctumMaze_GRID:
	.incbin "games/tbs/src/map/sol_sanctum_maze/map.grid.lz"
	.balign 4, 0
.LField_SolSanctumMaze_ANIMATION:
	.incbin "games/tbs/src/map/sol_sanctum_maze/tileset_anims.script.lz"
	.balign 4, 0
.LField_SolSanctumMaze_BLEND:
	.incbin "games/tbs/src/map/sol_sanctum_maze/blend.script.lz"
	.balign 4, 0
.LField_SolSanctumMaze_MARKERS:
	treasure_spot 15, 10, 100
	treasure_spots_end
	.balign 4, 0
