	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_StuccoRoomsC
Field_StuccoRoomsC:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_StuccoRoomsC, .LField_StuccoRoomsC_CELL_PICTURES
	map_offset Field_StuccoRoomsC, .LField_StuccoRoomsC_SHAPES
	map_offset Field_StuccoRoomsC, .LField_StuccoRoomsC_GRID
	map_offset Field_StuccoRoomsC, .LField_StuccoRoomsC_ANIMATION
	map_offset Field_StuccoRoomsC, .LField_StuccoRoomsC_BLEND
	map_offset Field_StuccoRoomsC, .LField_StuccoRoomsC_MARKERS
.LField_StuccoRoomsC_CELL_PICTURES:
	.incbin "games/tbs/src/map/stucco_rooms_c/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_StuccoRoomsC_SHAPES:
	.incbin "games/tbs/src/map/stucco_rooms_c/shapes.table.lz"
	.balign 4, 0
.LField_StuccoRoomsC_GRID:
	.incbin "games/tbs/src/map/stucco_rooms_c/map.grid.lz"
	.balign 4, 0
.LField_StuccoRoomsC_ANIMATION:
	.incbin "games/tbs/src/map/stucco_rooms_a/tileset_anims.script.lz"
	.balign 4, 0
.LField_StuccoRoomsC_BLEND:
	.incbin "games/tbs/src/map/vault_log_rooms/blend.script.lz"
	.balign 4, 0
.LField_StuccoRoomsC_MARKERS:
	treasure_spots_end
	.balign 4, 0
