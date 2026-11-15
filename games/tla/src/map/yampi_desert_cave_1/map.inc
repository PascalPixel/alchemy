	.global Field_YampiDesertCave1
Field_YampiDesertCave1:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_YampiDesertCave1, .LField_YampiDesertCave1_CELL_PICTURES
	map_block Field_YampiDesertCave1, .LField_YampiDesertCave1_SHAPES
	map_block Field_YampiDesertCave1, .LField_YampiDesertCave1_GRID
	map_block Field_YampiDesertCave1, .LField_YampiDesertCave1_SHAPE_GRID
	map_block Field_YampiDesertCave1, .LField_YampiDesertCave1_ANIMATION
	map_block Field_YampiDesertCave1
	map_block Field_YampiDesertCave1, .LField_YampiDesertCave1_SCENE
.LField_YampiDesertCave1_CELL_PICTURES:
	.incbin "games/tla/src/map/yampi_desert_cave_1/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_YampiDesertCave1_SHAPES:
	.incbin "games/tla/src/map/yampi_desert_cave_1/shapes.table.lz"
	.balign 4, 0
.LField_YampiDesertCave1_GRID:
	.incbin "games/tla/src/map/yampi_desert_cave_1/map.grid.lz"
	.balign 4, 0
.LField_YampiDesertCave1_SHAPE_GRID:
	.incbin "games/tla/src/map/yampi_desert_cave_1/shape_grid.plane.lz"
	.balign 4, 0
.LField_YampiDesertCave1_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_2/animation.script.lz"
	.balign 4, 0
.LField_YampiDesertCave1_SCENE:
	.incbin "games/tla/src/map/yampi_desert_cave_1/scene.scene.lz"
	.balign 4, 0
	.global Tileset_YampiDesertCave1Palette
Tileset_YampiDesertCave1Palette:
	.incbin "games/tla/src/graphics/tileset/yampi_desert_cave_1/palette.gbapal.lz"
	.byte 0x1b	@ PACKER: left in the buffer by Field_YampiDesertCave1+0x14f
	.global Tileset_YampiDesertCave1TilesA
Tileset_YampiDesertCave1TilesA:
	.incbin "games/tla/src/graphics/tileset/yampi_desert_cave_1/tiles_a.4bpp.mtf"
	.byte 0x05, 0x08	@ PACKER: unused post-stream residue; Resource_DecodeType2 uses decoded RAM and output length; writer unknown
