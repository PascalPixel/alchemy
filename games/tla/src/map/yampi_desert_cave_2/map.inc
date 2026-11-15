	.global Field_YampiDesertCave2
Field_YampiDesertCave2:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_YampiDesertCave2, .LField_YampiDesertCave2_CELL_PICTURES
	map_block Field_YampiDesertCave2, .LField_YampiDesertCave2_SHAPES
	map_block Field_YampiDesertCave2, .LField_YampiDesertCave2_GRID
	map_block Field_YampiDesertCave2, .LField_YampiDesertCave2_SHAPE_GRID
	map_block Field_YampiDesertCave2, .LField_YampiDesertCave2_ANIMATION
	map_block Field_YampiDesertCave2
	map_block Field_YampiDesertCave2, .LField_YampiDesertCave2_SCENE
.LField_YampiDesertCave2_CELL_PICTURES:
	.incbin "games/tla/src/map/yampi_desert_cave_2/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_YampiDesertCave2_SHAPES:
	.incbin "games/tla/src/map/yampi_desert_cave_2/shapes.table.lz"
	.balign 4, 0
.LField_YampiDesertCave2_GRID:
	.incbin "games/tla/src/map/yampi_desert_cave_2/map.grid.lz"
	.balign 4, 0
.LField_YampiDesertCave2_SHAPE_GRID:
	.incbin "games/tla/src/map/yampi_desert_cave_2/shape_grid.plane.lz"
	.balign 4, 0
.LField_YampiDesertCave2_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_2/animation.script.lz"
	.balign 4, 0
.LField_YampiDesertCave2_SCENE:
	.incbin "games/tla/src/map/yampi_desert_cave_2/scene.scene.lz"
	.global Tileset_YampiDesertCave2Palette
Tileset_YampiDesertCave2Palette:
	.incbin "games/tla/src/graphics/tileset/yampi_desert_cave_2/palette.gbapal.lz"
	.byte 0x68	@ PACKER: left in the buffer by Field_YampiDesertCave2+0x14f
	.global Tileset_YampiDesertCave2TilesA
Tileset_YampiDesertCave2TilesA:
	.incbin "games/tla/src/graphics/tileset/yampi_desert_cave_2/tiles_a.4bpp.mtf"
	.byte 0x65, 0x33	@ PACKER: left in the buffer by Field_YampiDesertCave2+0x1a06
