	.global Field_ApojiiIslandsCave
Field_ApojiiIslandsCave:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_ApojiiIslandsCave, .LField_ApojiiIslandsCave_CELL_PICTURES
	map_block Field_ApojiiIslandsCave, .LField_ApojiiIslandsCave_SHAPES
	map_block Field_ApojiiIslandsCave, .LField_ApojiiIslandsCave_GRID
	map_block Field_ApojiiIslandsCave, .LField_ApojiiIslandsCave_SHAPE_GRID
	map_block Field_ApojiiIslandsCave, .LField_ApojiiIslandsCave_ANIMATION
	map_block Field_ApojiiIslandsCave, .LField_ApojiiIslandsCave_BLEND
	map_block Field_ApojiiIslandsCave, .LField_ApojiiIslandsCave_SCENE
.LField_ApojiiIslandsCave_CELL_PICTURES:
	.incbin "games/tla/src/map/apojii_islands_cave/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_ApojiiIslandsCave_SHAPES:
	.incbin "games/tla/src/map/apojii_islands_cave/shapes.table.lz"
	.balign 4, 0
.LField_ApojiiIslandsCave_GRID:
	.incbin "games/tla/src/map/apojii_islands_cave/map.grid.lz"
	.balign 4, 0
.LField_ApojiiIslandsCave_SHAPE_GRID:
	.incbin "games/tla/src/map/apojii_islands_cave/shape_grid.plane.lz"
	.balign 4, 0
.LField_ApojiiIslandsCave_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_ApojiiIslandsCave_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_ApojiiIslandsCave_SCENE:
	.incbin "games/tla/src/map/apojii_islands_cave/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Set70Palette
Tileset_Set70Palette:
	.incbin "games/tla/src/graphics/tileset/set_70/palette.gbapal.lz"
	.byte 0x80, 0xa0	@ PACKER: left in the buffer by Field_ApojiiIslandsCave+0xe2
	.global Tileset_Set70TilesA
Tileset_Set70TilesA:
	.incbin "games/tla/src/graphics/tileset/set_70/tiles_a.4bpp.mtf"
	.global Tileset_Set70TilesB
Tileset_Set70TilesB:
	.incbin "games/tla/src/graphics/tileset/set_70/tiles_b.4bpp.mtf"
	.global Tileset_Set70TilesC
Tileset_Set70TilesC:
	.incbin "games/tla/src/graphics/tileset/set_70/tiles_c.4bpp.mtf"
	.byte 0x67	@ PACKER: left in the buffer by Tileset_Set70TilesB+0x13f
	.global Tileset_Set70TilesD
Tileset_Set70TilesD:
	.incbin "games/tla/src/graphics/tileset/set_70/tiles_d.4bpp.mtf"
	.byte 0x67	@ PACKER: left in the buffer by Tileset_Set70TilesC+0x13f
