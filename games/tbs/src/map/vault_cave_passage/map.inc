	.global Field_VaultCavePassage
Field_VaultCavePassage:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_VaultCavePassage, .LField_VaultCavePassage_CELL_PICTURES
	map_offset Field_VaultCavePassage, .LField_VaultCavePassage_SHAPES
	map_offset Field_VaultCavePassage, .LField_VaultCavePassage_GRID
	map_offset Field_VaultCavePassage, .LField_VaultCavePassage_ANIMATION
	map_offset Field_VaultCavePassage, .LField_VaultCavePassage_BLEND
	map_offset Field_VaultCavePassage, .LField_VaultCavePassage_MARKERS
.LField_VaultCavePassage_CELL_PICTURES:
	.incbin "games/tbs/src/map/vault_cave_passage/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_VaultCavePassage_SHAPES:
	.incbin "games/tbs/src/map/vault_cave_passage/shapes.table.lz"
	.balign 4, 0
.LField_VaultCavePassage_GRID:
	.incbin "games/tbs/src/map/vault_cave_passage/map.grid.lz"
	.balign 4, 0
.LField_VaultCavePassage_ANIMATION:
	.incbin "games/tbs/src/map/vault_cave_passage/tileset_anims.script.lz"
	.balign 4, 0
.LField_VaultCavePassage_BLEND:
	.incbin "games/tbs/src/map/vault_cave_passage/blend.script.lz"
	.balign 4, 0
.LField_VaultCavePassage_MARKERS:
	treasure_spot 16, 47, 100
	treasure_spots_end
	.balign 4, 0
	.global Tileset_VaultCavePassagePalette
Tileset_VaultCavePassagePalette:
	.incbin "games/tbs/src/graphics/tileset/vault_cave_passage/palette.gbapal.lz"
	.byte 0xe2, 0x17	@ PACKER: left in the buffer by Field_VaultCavePassage+0x19a
	.global Tileset_VaultCavePassageTilesA
Tileset_VaultCavePassageTilesA:
	.incbin "games/tbs/src/graphics/tileset/vault_cave_passage/tiles_a.4bpp.mtf"
	.balign 4, 0
	.global Tileset_VaultCavePassageTilesB
Tileset_VaultCavePassageTilesB:
	.incbin "games/tbs/src/graphics/tileset/vault_cave_passage/tiles_b.4bpp.mtf"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global Tileset_VaultCavePassageTilesC
Tileset_VaultCavePassageTilesC:
	.incbin "games/tbs/src/graphics/tileset/vault_cave_passage/tiles_c.4bpp.mtf"
	.global Tileset_VaultCavePassageTilesD
Tileset_VaultCavePassageTilesD:
	.incbin "games/tbs/src/graphics/tileset/vault_cave_passage/tiles_d.4bpp.mtf"
	.byte 0xc6, 0x39	@ PACKER: left in the buffer by Tileset_VaultCavePassageTilesB+0x1a36
	.balign 4, 0
