	.global Field_VaultCaveMouth
Field_VaultCaveMouth:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_VaultCaveMouth, .LField_VaultCaveMouth_CELL_PICTURES
	map_offset Field_VaultCaveMouth, .LField_VaultCaveMouth_SHAPES
	map_offset Field_VaultCaveMouth, .LField_VaultCaveMouth_GRID
	map_offset Field_VaultCaveMouth, .LField_VaultCaveMouth_ANIMATION
	map_offset Field_VaultCaveMouth, .LField_VaultCaveMouth_BLEND
	map_offset Field_VaultCaveMouth, .LField_VaultCaveMouth_MARKERS
.LField_VaultCaveMouth_CELL_PICTURES:
	.incbin "games/tbs/src/map/vault_cave_mouth/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_VaultCaveMouth_SHAPES:
	.incbin "games/tbs/src/map/vault_cave_mouth/shapes.table.lz"
	.balign 4, 0
.LField_VaultCaveMouth_GRID:
	.incbin "games/tbs/src/map/vault_cave_mouth/map.grid.lz"
	.balign 4, 0
.LField_VaultCaveMouth_ANIMATION:
	.incbin "games/tbs/src/map/vault_cave_passage/tileset_anims.script.lz"
	.balign 4, 0
.LField_VaultCaveMouth_BLEND:
	.incbin "games/tbs/src/map/vault_cave_passage/blend.script.lz"
	.balign 4, 0
.LField_VaultCaveMouth_MARKERS:
	treasure_spots_end
	.balign 4, 0
