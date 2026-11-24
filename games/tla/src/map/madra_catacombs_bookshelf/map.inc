	.global Field_MadraCatacombsBookshelf
Field_MadraCatacombsBookshelf:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MadraCatacombsBookshelf, .LField_MadraCatacombsBookshelf_CELL_TILES
	map_block Field_MadraCatacombsBookshelf, .LField_MadraCatacombsBookshelf_SHAPES
	map_block Field_MadraCatacombsBookshelf, .LField_MadraCatacombsBookshelf_GRID
	map_block Field_MadraCatacombsBookshelf, .LField_MadraCatacombsBookshelf_SHAPE_GRID
	map_block Field_MadraCatacombsBookshelf, .LField_MadraCatacombsBookshelf_ANIMATION
	map_block Field_MadraCatacombsBookshelf, .LField_MadraCatacombsBookshelf_BLEND
	map_block Field_MadraCatacombsBookshelf, .LField_MadraCatacombsBookshelf_SCENE
.LField_MadraCatacombsBookshelf_CELL_TILES:
	.incbin "games/tla/src/map/madra_catacombs_bookshelf/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_MadraCatacombsBookshelf_SHAPES:
	.incbin "games/tla/src/map/madra_catacombs_bookshelf/shapes.table.lz"
	.balign 4, 0
.LField_MadraCatacombsBookshelf_GRID:
	.incbin "games/tla/src/map/madra_catacombs_bookshelf/map.grid.lz"
	.balign 4, 0
.LField_MadraCatacombsBookshelf_SHAPE_GRID:
	.incbin "games/tla/src/map/madra_catacombs_bookshelf/shape_grid.plane.lz"
	.balign 4, 0
.LField_MadraCatacombsBookshelf_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_MadraCatacombsBookshelf_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_MadraCatacombsBookshelf_SCENE:
	.incbin "games/tla/src/map/madra_catacombs_bookshelf/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteMadraCatacombsBookshelf
Tileset_PaletteMadraCatacombsBookshelf:
	.incbin "games/tla/src/graphics/tileset/palettes/madra_catacombs_bookshelf.gbapal.lz"
	.byte 0xf0, 0x21, 0x38	@ PACKER: left in the buffer by Field_MadraCatacombs5+0x165
