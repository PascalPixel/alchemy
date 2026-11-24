	.global Field_YampiDesertGuardianRingChest
Field_YampiDesertGuardianRingChest:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_YampiDesertGuardianRingChest, .LField_YampiDesertGuardianRingChest_CELL_TILES
	map_block Field_YampiDesertGuardianRingChest, .LField_YampiDesertGuardianRingChest_SHAPES
	map_block Field_YampiDesertGuardianRingChest, .LField_YampiDesertGuardianRingChest_GRID
	map_block Field_YampiDesertGuardianRingChest, .LField_YampiDesertGuardianRingChest_SHAPE_GRID
	map_block Field_YampiDesertGuardianRingChest, .LField_YampiDesertGuardianRingChest_ANIMATION
	map_block Field_YampiDesertGuardianRingChest, .LField_YampiDesertGuardianRingChest_BLEND
	map_block Field_YampiDesertGuardianRingChest, .LField_YampiDesertGuardianRingChest_SCENE
.LField_YampiDesertGuardianRingChest_CELL_TILES:
	.incbin "games/tla/src/map/yampi_desert_guardian_ring_chest/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_YampiDesertGuardianRingChest_SHAPES:
	.incbin "games/tla/src/map/yampi_desert_guardian_ring_chest/shapes.table.lz"
	.balign 4, 0
.LField_YampiDesertGuardianRingChest_GRID:
	.incbin "games/tla/src/map/yampi_desert_guardian_ring_chest/map.grid.lz"
	.balign 4, 0
.LField_YampiDesertGuardianRingChest_SHAPE_GRID:
	.incbin "games/tla/src/map/yampi_desert_guardian_ring_chest/shape_grid.plane.lz"
	.balign 4, 0
.LField_YampiDesertGuardianRingChest_ANIMATION:
	.incbin "games/tla/src/map/yampi_desert_guardian_ring_chest/animation.script.lz"
	.balign 4, 0
.LField_YampiDesertGuardianRingChest_BLEND:
	.incbin "games/tla/src/map/yampi_desert_guardian_ring_chest/blend.script.lz"
	.balign 4, 0
.LField_YampiDesertGuardianRingChest_SCENE:
	.incbin "games/tla/src/map/yampi_desert_guardian_ring_chest/scene.scene.lz"
	.balign 4, 0
	.global Tileset_YampiDesertGuardianRingChestPalette
Tileset_YampiDesertGuardianRingChestPalette:
	.incbin "games/tla/src/graphics/tileset/yampi_desert_guardian_ring_chest/palette.gbapal.lz"
	.byte 0x87, 0x37, 0x18	@ PACKER: left in the buffer by Field_YampiDesert1+0x12d
	.global Tileset_YampiDesertGuardianRingChestTilesA
Tileset_YampiDesertGuardianRingChestTilesA:
	.incbin "games/tla/src/graphics/tileset/yampi_desert_guardian_ring_chest/tiles_a.4bpp.mtf"
	.byte 0xe6, 0x08	@ PACKER: left in the buffer by Tileset_MadraDrawbridgeTilesA+0x2ab6
	.global Tileset_YampiDesertGuardianRingChestTilesB
Tileset_YampiDesertGuardianRingChestTilesB:
	.incbin "games/tla/src/graphics/tileset/yampi_desert_guardian_ring_chest/tiles_b.4bpp.mtf"
	.byte 0xa9, 0x03	@ PACKER: left in the buffer by Tileset_YampiDesertGuardianRingChestTilesA+0x1b56
	.global Tileset_YampiDesertGuardianRingChestTilesC
Tileset_YampiDesertGuardianRingChestTilesC:
	.incbin "games/tla/src/graphics/tileset/yampi_desert_guardian_ring_chest/tiles_c.4bpp.mtf"
	.byte 0xf5, 0xc0, 0x8e	@ PACKER: left in the buffer by Tileset_YampiDesertGuardianRingChestTilesB+0xf45
	.global Tileset_YampiDesertGuardianRingChestTilesD
Tileset_YampiDesertGuardianRingChestTilesD:
	.incbin "games/tla/src/graphics/tileset/yampi_desert_guardian_ring_chest/tiles_d.4bpp.mtf"
	.byte 0x6f, 0xd1	@ PACKER: left in the buffer by Tileset_YampiDesertGuardianRingChestTilesB+0x1a06
