	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Vault
Field_Vault:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Vault, .LField_Vault_CELL_PICTURES
	map_offset Field_Vault, .LField_Vault_SHAPES
	map_offset Field_Vault, .LField_Vault_GRID
	map_offset Field_Vault
	map_offset Field_Vault
	map_offset Field_Vault, .LField_Vault_MARKERS
.LField_Vault_CELL_PICTURES:
	.incbin "games/tbs/src/map/vault/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Vault_SHAPES:
	.incbin "games/tbs/src/map/vault/shapes.table.lz"
	.balign 4, 0
.LField_Vault_GRID:
	.incbin "games/tbs/src/map/vault/map.grid.lz"
	.balign 4, 0
.LField_Vault_MARKERS:
	treasure_spot 4, 9, 101
	treasure_spots_end
	.balign 4, 0
	.global Tileset_VaultPalette
Tileset_VaultPalette:
	.incbin "games/tbs/src/graphics/tileset/vault/palette.gbapal.lz"
	.byte 0xc8	@ PACKER: left in the buffer by Field_Vault+0x183
	.global Tileset_VaultTilesA
Tileset_VaultTilesA:
	.incbin "games/tbs/src/graphics/tileset/vault/tiles_a.4bpp.mtf"
	.byte 0x8a, 0x33, 0x2a	@ PACKER: left in the buffer by Field_CottageRoomsB+0x2b51
	.global Tileset_VaultTilesB
Tileset_VaultTilesB:
	.incbin "games/tbs/src/graphics/tileset/vault/tiles_b.4bpp.mtf"
	.byte 0x13, 0x1d	@ PACKER: left in the buffer by Tileset_VaultTilesA+0x26c2
	.global Tileset_VaultTilesC
Tileset_VaultTilesC:
	.incbin "games/tbs/src/graphics/tileset/vault/tiles_c.4bpp.mtf"
	.byte 0x0a, 0xcd	@ PACKER: left in the buffer by Tileset_VaultTilesA+0x2756
	.global Tileset_VaultTilesD
Tileset_VaultTilesD:
	.incbin "games/tbs/src/graphics/tileset/vault/tiles_d.4bpp.mtf"
	.byte 0x5e, 0x11, 0x7d	@ PACKER: left in the buffer by Tileset_VaultTilesC+0x1851
