	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_VaultCaveVambrace
Field_VaultCaveVambrace:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_VaultCaveVambrace, .LField_VaultCaveVambrace_CELL_PICTURES
	map_offset Field_VaultCaveVambrace, .LField_VaultCaveVambrace_SHAPES
	map_offset Field_VaultCaveVambrace, .LField_VaultCaveVambrace_GRID
	map_offset Field_VaultCaveVambrace, .LField_VaultCaveVambrace_ANIMATION
	map_offset Field_VaultCaveVambrace, .LField_VaultCaveVambrace_BLEND
	map_offset Field_VaultCaveVambrace, .LField_VaultCaveVambrace_MARKERS
.LField_VaultCaveVambrace_CELL_PICTURES:
	.incbin "games/tbs/src/map/vault_cave_vambrace/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_VaultCaveVambrace_SHAPES:
	.incbin "games/tbs/src/map/vault_cave_vambrace/shapes.table.lz"
	.balign 4, 0
.LField_VaultCaveVambrace_GRID:
	.incbin "games/tbs/src/map/vault_cave_vambrace/map.grid.lz"
	.balign 4, 0
.LField_VaultCaveVambrace_ANIMATION:
	.incbin "games/tbs/src/map/vault_cave_passage/tileset_anims.script.lz"
	.balign 4, 0
.LField_VaultCaveVambrace_BLEND:
	.incbin "games/tbs/src/map/vault_cave_passage/blend.script.lz"
	.balign 4, 0
.LField_VaultCaveVambrace_MARKERS:
	treasure_spot 33, 5, 100
	treasure_spots_end
	.balign 4, 0
