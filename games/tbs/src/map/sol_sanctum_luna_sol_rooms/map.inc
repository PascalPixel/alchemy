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
	.global Tileset_SolSanctumLunaSolRoomsPalette
Tileset_SolSanctumLunaSolRoomsPalette:
	.incbin "games/tbs/src/graphics/tileset/sol_sanctum_luna_sol_rooms/palette.gbapal.lz"
	.byte 0xcf, 0x83, 0x07	@ PACKER: left in the buffer by Field_SolSanctumLunaSolRooms+0x159
	.global Tileset_SolSanctumLunaSolRoomsTilesA
Tileset_SolSanctumLunaSolRoomsTilesA:
	.incbin "games/tbs/src/graphics/tileset/sol_sanctum_luna_sol_rooms/tiles_a.4bpp.mtf"
	.global Tileset_SolSanctumLunaSolRoomsTilesB
Tileset_SolSanctumLunaSolRoomsTilesB:
	.incbin "games/tbs/src/graphics/tileset/sol_sanctum_luna_sol_rooms/tiles_b.4bpp.mtf"
	.byte 0x18	@ PACKER: left in the buffer by Tileset_SolSanctumLunaSolRoomsTilesA+0x1a4b
	.global Tileset_SolSanctumLunaSolRoomsTilesC
Tileset_SolSanctumLunaSolRoomsTilesC:
	.incbin "games/tbs/src/graphics/tileset/sol_sanctum_luna_sol_rooms/tiles_c.4bpp.mtf"
	.global Tileset_SolSanctumLunaSolRoomsTilesD
Tileset_SolSanctumLunaSolRoomsTilesD:
	.incbin "games/tbs/src/graphics/tileset/sol_sanctum_luna_sol_rooms/tiles_d.4bpp.mtf"
	.byte 0xe1, 0x71	@ PACKER: left in the buffer by Tileset_SolSanctumLunaSolRoomsTilesC+0x842
	.balign 4, 0
