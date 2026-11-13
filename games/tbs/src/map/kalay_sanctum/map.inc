	.global Field_KalaySanctum
Field_KalaySanctum:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_KalaySanctum, .LField_KalaySanctum_CELL_PICTURES
	map_offset Field_KalaySanctum, .LField_KalaySanctum_SHAPES
	map_offset Field_KalaySanctum, .LField_KalaySanctum_GRID
	map_offset Field_KalaySanctum, .LField_KalaySanctum_ANIMATION
	map_offset Field_KalaySanctum, .LField_KalaySanctum_BLEND
	map_offset Field_KalaySanctum, .LField_KalaySanctum_MARKERS
.LField_KalaySanctum_CELL_PICTURES:
	.incbin "games/tbs/src/map/kalay_sanctum/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_KalaySanctum_SHAPES:
	.incbin "games/tbs/src/map/kalay_sanctum/shapes.table.lz"
	.balign 4, 0
.LField_KalaySanctum_GRID:
	.incbin "games/tbs/src/map/kalay_sanctum/map.grid.lz"
	.balign 4, 0
.LField_KalaySanctum_ANIMATION:
	.incbin "games/tbs/src/map/stucco_rooms_a/tileset_anims.script.lz"
	.balign 4, 0
.LField_KalaySanctum_BLEND:
	.incbin "games/tbs/src/map/vault_log_rooms/blend.script.lz"
	.balign 4, 0
.LField_KalaySanctum_MARKERS:
	treasure_spots_end
	.balign 4, 0
