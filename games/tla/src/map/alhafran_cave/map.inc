	.global Field_AlhafranCave
Field_AlhafranCave:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AlhafranCave, .LField_AlhafranCave_CELL_TILES
	map_block Field_AlhafranCave, .LField_AlhafranCave_SHAPES
	map_block Field_AlhafranCave, .LField_AlhafranCave_GRID
	map_block Field_AlhafranCave, .LField_AlhafranCave_SHAPE_GRID
	map_block Field_AlhafranCave, .LField_AlhafranCave_ANIMATION
	map_block Field_AlhafranCave, .LField_AlhafranCave_BLEND
	map_block Field_AlhafranCave, .LField_AlhafranCave_SCENE
.LField_AlhafranCave_CELL_TILES:
	.incbin "games/tla/src/map/alhafran_cave/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_AlhafranCave_SHAPES:
	.incbin "games/tla/src/map/alhafran_cave/shapes.table.lz"
	.balign 4, 0
.LField_AlhafranCave_GRID:
	.incbin "games/tla/src/map/alhafran_cave/map.grid.lz"
	.balign 4, 0
.LField_AlhafranCave_SHAPE_GRID:
	.incbin "games/tla/src/map/alhafran_cave/shape_grid.plane.lz"
	.balign 4, 0
.LField_AlhafranCave_ANIMATION:
	.incbin "games/tla/src/map/alhafran_cave/animation.script.lz"
	.balign 4, 0
.LField_AlhafranCave_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_AlhafranCave_SCENE:
	.incbin "games/tla/src/map/alhafran_cave/scene.scene.lz"
	.global Tileset_AlhafranCavePalette
Tileset_AlhafranCavePalette:
	.incbin "games/tla/src/graphics/tileset/alhafran_cave/palette.gbapal.lz"
	.byte 0xf3	@ PACKER: left in the buffer by Field_AlhafranCave+0x19b
	.global Tileset_AlhafranCaveTilesA
Tileset_AlhafranCaveTilesA:
	.incbin "games/tla/src/graphics/tileset/alhafran_cave/tiles_a.4bpp.mtf"
	.byte 0xdd, 0x56	@ PACKER: left in the buffer by Field_AirsRock1+0x2c8a
	.global Tileset_AlhafranCaveTilesB
Tileset_AlhafranCaveTilesB:
	.incbin "games/tla/src/graphics/tileset/alhafran_cave/tiles_b.4bpp.mtf"
	.byte 0x8b, 0x70	@ PACKER: left in the buffer by Tileset_AlhafranCaveTilesA+0x104a
	.global Tileset_AlhafranCaveTilesC
Tileset_AlhafranCaveTilesC:
	.incbin "games/tla/src/graphics/tileset/alhafran_cave/tiles_c.4bpp.mtf"
	.byte 0x4a	@ PACKER: left in the buffer by Tileset_AlhafranCaveTilesA+0x10fb
	.global Tileset_AlhafranCaveTilesD
Tileset_AlhafranCaveTilesD:
	.incbin "games/tla/src/graphics/tileset/alhafran_cave/tiles_d.4bpp.mtf"
	.byte 0x87	@ PACKER: left in the buffer by Tileset_AlhafranCaveTilesA+0x17d3
