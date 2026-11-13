	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_SolSanctumElementalStarRoom
Field_SolSanctumElementalStarRoom:
	map_header 0, 0, 112, 80, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 80, 16, 16, 0, 0, 0xff, 0xff
	map_layer 112, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_SolSanctumElementalStarRoom, .LField_SolSanctumElementalStarRoom_CELL_PICTURES
	map_offset Field_SolSanctumElementalStarRoom, .LField_SolSanctumElementalStarRoom_SHAPES
	map_offset Field_SolSanctumElementalStarRoom, .LField_SolSanctumElementalStarRoom_GRID
	map_offset Field_SolSanctumElementalStarRoom, .LField_SolSanctumElementalStarRoom_ANIMATION
	map_offset Field_SolSanctumElementalStarRoom, .LField_SolSanctumElementalStarRoom_BLEND
	map_offset Field_SolSanctumElementalStarRoom, .LField_SolSanctumElementalStarRoom_MARKERS
.LField_SolSanctumElementalStarRoom_CELL_PICTURES:
	.incbin "games/tbs/src/map/sol_sanctum_elemental_star_room/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_SolSanctumElementalStarRoom_SHAPES:
	.incbin "games/tbs/src/map/sol_sanctum_elemental_star_room/shapes.table.lz"
	.balign 4, 0
.LField_SolSanctumElementalStarRoom_GRID:
	.incbin "games/tbs/src/map/sol_sanctum_elemental_star_room/map.grid.lz"
	.balign 4, 0
.LField_SolSanctumElementalStarRoom_ANIMATION:
	.incbin "games/tbs/src/map/sol_sanctum_elemental_star_room/tileset_anims.script.lz"
	.balign 4, 0
.LField_SolSanctumElementalStarRoom_BLEND:
	.incbin "games/tbs/src/map/sol_sanctum_elemental_star_room/blend.script.lz"
	.balign 4, 0
.LField_SolSanctumElementalStarRoom_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_SolSanctumElementalStarRoomPalette
Tileset_SolSanctumElementalStarRoomPalette:
	.incbin "games/tbs/src/graphics/tileset/sol_sanctum_elemental_star_room/palette.gbapal.lz"
	.byte 0xb1	@ PACKER: left in the buffer by Field_SolSanctumElementalStarRoom+0x127
	.global Tileset_SolSanctumElementalStarRoomTilesA
Tileset_SolSanctumElementalStarRoomTilesA:
	.incbin "games/tbs/src/graphics/tileset/sol_sanctum_elemental_star_room/tiles_a.4bpp.mtf"
	.byte 0x7a	@ PACKER: left in the buffer by Field_SolSanctumElementalStarRoom+0x1887
	.global Tileset_SolSanctumElementalStarRoomTilesB
Tileset_SolSanctumElementalStarRoomTilesB:
	.incbin "games/tbs/src/graphics/tileset/sol_sanctum_elemental_star_room/tiles_b.4bpp.mtf"
	.global Tileset_SolSanctumElementalStarRoomTilesC
Tileset_SolSanctumElementalStarRoomTilesC:
	.incbin "games/tbs/src/graphics/tileset/sol_sanctum_elemental_star_room/tiles_c.4bpp.mtf"
	.byte 0x8a, 0x18, 0x32	@ PACKER: left in the buffer by Tileset_SolSanctumElementalStarRoomTilesB+0xf3d
	.global Tileset_SolSanctumElementalStarRoomTilesD
Tileset_SolSanctumElementalStarRoomTilesD:
	.incbin "games/tbs/src/graphics/tileset/sol_sanctum_elemental_star_room/tiles_d.4bpp.mtf"
	.byte 0x01	@ PACKER: left in the buffer by Tileset_SolSanctumElementalStarRoomTilesC+0x13f
