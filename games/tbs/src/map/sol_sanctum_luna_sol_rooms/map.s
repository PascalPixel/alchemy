	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_SolSanctumLunaSolRooms
Field_SolSanctumLunaSolRooms:
	map_header 0, 0, 100, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_SolSanctumLunaSolRooms, .LField_SolSanctumLunaSolRooms_CELL_PICTURES
	map_offset Field_SolSanctumLunaSolRooms, .LField_SolSanctumLunaSolRooms_SHAPES
	map_offset Field_SolSanctumLunaSolRooms, .LField_SolSanctumLunaSolRooms_GRID
	map_offset Field_SolSanctumLunaSolRooms, .LField_SolSanctumLunaSolRooms_ANIMATION
	map_offset Field_SolSanctumLunaSolRooms, .LField_SolSanctumLunaSolRooms_BLEND
	map_offset Field_SolSanctumLunaSolRooms, .LField_SolSanctumLunaSolRooms_MARKERS
.LField_SolSanctumLunaSolRooms_CELL_PICTURES:
	.incbin "games/tbs/src/map/sol_sanctum_luna_sol_rooms/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_SolSanctumLunaSolRooms_SHAPES:
	.incbin "games/tbs/src/map/sol_sanctum_luna_sol_rooms/shapes.table.lz"
	.balign 4, 0
.LField_SolSanctumLunaSolRooms_GRID:
	.incbin "games/tbs/src/map/sol_sanctum_luna_sol_rooms/map.grid.lz"
	.balign 4, 0
.LField_SolSanctumLunaSolRooms_ANIMATION:
	.incbin "games/tbs/src/map/sol_sanctum_luna_sol_rooms/tileset_anims.script.lz"
	.balign 4, 0
.LField_SolSanctumLunaSolRooms_BLEND:
	.incbin "games/tbs/src/map/sol_sanctum_luna_sol_rooms/blend.script.lz"
	.balign 4, 0
.LField_SolSanctumLunaSolRooms_MARKERS:
	treasure_spot 3, 15, 100
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Set08Palette
Tileset_Set08Palette:
	.incbin "games/tbs/src/graphics/tileset/set_08/palette.gbapal.lz"
	.byte 0xcf, 0x83, 0x07	@ PACKER: left in the buffer by Field_SolSanctumLunaSolRooms+0x159
	.global Tileset_Set08TilesA
Tileset_Set08TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_08/tiles_a.4bpp.mtf"
	.global Tileset_Set08TilesB
Tileset_Set08TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_08/tiles_b.4bpp.mtf"
	.byte 0x18	@ PACKER: left in the buffer by Tileset_Set08TilesA+0x1a4b
	.global Tileset_Set08TilesC
Tileset_Set08TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_08/tiles_c.4bpp.mtf"
	.global Tileset_Set08TilesD
Tileset_Set08TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_08/tiles_d.4bpp.mtf"
	.byte 0xe1, 0x71	@ PACKER: left in the buffer by Tileset_Set08TilesC+0x842
