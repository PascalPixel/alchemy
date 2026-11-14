	.global Field_SolSanctumReliefs
Field_SolSanctumReliefs:
	map_header 0, 0, 255, 81, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 81, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 161, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_SolSanctumReliefs, .LField_SolSanctumReliefs_CELL_PICTURES
	map_offset Field_SolSanctumReliefs, .LField_SolSanctumReliefs_SHAPES
	map_offset Field_SolSanctumReliefs, .LField_SolSanctumReliefs_GRID
	map_offset Field_SolSanctumReliefs, .LField_SolSanctumReliefs_ANIMATION
	map_offset Field_SolSanctumReliefs, .LField_SolSanctumReliefs_BLEND
	map_offset Field_SolSanctumReliefs, .LField_SolSanctumReliefs_MARKERS
.LField_SolSanctumReliefs_CELL_PICTURES:
	.incbin "games/tbs/src/map/sol_sanctum_reliefs/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_SolSanctumReliefs_SHAPES:
	.incbin "games/tbs/src/map/sol_sanctum_eruption/shapes.table.lz"
	.balign 4, 0
.LField_SolSanctumReliefs_GRID:
	.incbin "games/tbs/src/map/sol_sanctum_reliefs/map.grid.lz"
	.balign 4, 0
.LField_SolSanctumReliefs_ANIMATION:
	.incbin "games/tbs/src/map/sol_sanctum_maze/tileset_anims.script.lz"
	.balign 4, 0
.LField_SolSanctumReliefs_BLEND:
	.incbin "games/tbs/src/map/sol_sanctum_maze/blend.script.lz"
	.balign 4, 0
.LField_SolSanctumReliefs_MARKERS:
	treasure_spot 17, 3, 100
	treasure_spots_end
	.balign 4, 0
	.global Tileset_PaletteSolSanctumReliefs
Tileset_PaletteSolSanctumReliefs:
	.incbin "games/tbs/src/graphics/tileset/palettes/sol_sanctum_reliefs.gbapal.lz"
	.byte 0x41, 0x28, 0x40	@ PACKER: left in the buffer by Field_SolSanctumReliefs+0x159
	.balign 4, 0
