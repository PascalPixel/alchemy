	.global Field_YampiDesertCaveGuardian
Field_YampiDesertCaveGuardian:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_YampiDesertCaveGuardian, .LField_YampiDesertCaveGuardian_CELL_TILES
	map_block Field_YampiDesertCaveGuardian, .LField_YampiDesertCaveGuardian_SHAPES
	map_block Field_YampiDesertCaveGuardian, .LField_YampiDesertCaveGuardian_GRID
	map_block Field_YampiDesertCaveGuardian, .LField_YampiDesertCaveGuardian_SHAPE_GRID
	map_block Field_YampiDesertCaveGuardian, .LField_YampiDesertCaveGuardian_ANIMATION
	map_block Field_YampiDesertCaveGuardian
	map_block Field_YampiDesertCaveGuardian, .LField_YampiDesertCaveGuardian_SCENE
.LField_YampiDesertCaveGuardian_CELL_TILES:
	.incbin "games/tla/src/map/yampi_desert_cave_guardian/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_YampiDesertCaveGuardian_SHAPES:
	.incbin "games/tla/src/map/yampi_desert_cave_guardian/shapes.table.lz"
	.balign 4, 0
.LField_YampiDesertCaveGuardian_GRID:
	.incbin "games/tla/src/map/yampi_desert_cave_guardian/map.grid.lz"
	.balign 4, 0
.LField_YampiDesertCaveGuardian_SHAPE_GRID:
	.incbin "games/tla/src/map/yampi_desert_cave_guardian/shape_grid.plane.lz"
	.balign 4, 0
.LField_YampiDesertCaveGuardian_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_YampiDesertCaveGuardian_SCENE:
	.incbin "games/tla/src/map/yampi_desert_cave_guardian/scene.scene.lz"
	.global Tileset_YampiDesertCaveGuardianPalette
Tileset_YampiDesertCaveGuardianPalette:
	.incbin "games/tla/src/graphics/tileset/yampi_desert_cave_guardian/palette.gbapal.lz"
	.byte 0x68	@ PACKER: left in the buffer by Field_YampiDesertCave2+0x14f
	.global Tileset_YampiDesertCaveGuardianTilesA
Tileset_YampiDesertCaveGuardianTilesA:
	.incbin "games/tla/src/graphics/tileset/yampi_desert_cave_guardian/tiles_a.4bpp.mtf"
	.byte 0x65, 0x33	@ PACKER: left in the buffer by Field_YampiDesertCave2+0x1a06
