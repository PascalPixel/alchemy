	.global Field_YampiDesertCaveCrystal
Field_YampiDesertCaveCrystal:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_YampiDesertCaveCrystal, .LField_YampiDesertCaveCrystal_CELL_TILES
	map_block Field_YampiDesertCaveCrystal, .LField_YampiDesertCaveCrystal_SHAPES
	map_block Field_YampiDesertCaveCrystal, .LField_YampiDesertCaveCrystal_GRID
	map_block Field_YampiDesertCaveCrystal, .LField_YampiDesertCaveCrystal_SHAPE_GRID
	map_block Field_YampiDesertCaveCrystal, .LField_YampiDesertCaveCrystal_ANIMATION
	map_block Field_YampiDesertCaveCrystal
	map_block Field_YampiDesertCaveCrystal, .LField_YampiDesertCaveCrystal_SCENE
.LField_YampiDesertCaveCrystal_CELL_TILES:
	.incbin "games/tla/src/map/yampi_desert_cave_crystal/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_YampiDesertCaveCrystal_SHAPES:
	.incbin "games/tla/src/map/yampi_desert_cave_crystal/shapes.table.lz"
	.balign 4, 0
.LField_YampiDesertCaveCrystal_GRID:
	.incbin "games/tla/src/map/yampi_desert_cave_crystal/map.grid.lz"
	.balign 4, 0
.LField_YampiDesertCaveCrystal_SHAPE_GRID:
	.incbin "games/tla/src/map/yampi_desert_cave_crystal/shape_grid.plane.lz"
	.balign 4, 0
.LField_YampiDesertCaveCrystal_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_YampiDesertCaveCrystal_SCENE:
	.incbin "games/tla/src/map/yampi_desert_cave_crystal/scene.scene.lz"
	.balign 4, 0
	.global Tileset_YampiDesertCaveCrystalPalette
Tileset_YampiDesertCaveCrystalPalette:
	.incbin "games/tla/src/graphics/tileset/yampi_desert_cave_crystal/palette.gbapal.lz"
	.byte 0x46	@ PACKER: left in the buffer by Field_YampiDesertCaveCrystal+0x14f
	.global Tileset_YampiDesertCaveCrystalTilesA
Tileset_YampiDesertCaveCrystalTilesA:
	.incbin "games/tla/src/graphics/tileset/yampi_desert_cave_crystal/tiles_a.4bpp.mtf"
	.byte 0x21, 0x0d	@ PACKER: left in the buffer by Field_YampiDesertCaveCrystal+0x1a06
