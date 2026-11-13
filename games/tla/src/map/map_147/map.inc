	.global Field_Map147
Field_Map147:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map147, .LField_Map147_CELL_PICTURES
	map_block Field_Map147, .LField_Map147_SHAPES
	map_block Field_Map147, .LField_Map147_GRID
	map_block Field_Map147, .LField_Map147_SHAPE_GRID
	map_block Field_Map147, .LField_Map147_ANIMATION
	map_block Field_Map147, .LField_Map147_BLEND
	map_block Field_Map147, .LField_Map147_SCENE
.LField_Map147_CELL_PICTURES:
	.incbin "games/tla/src/map/map_147/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map147_SHAPES:
	.incbin "games/tla/src/map/map_147/shapes.table.lz"
	.balign 4, 0
.LField_Map147_GRID:
	.incbin "games/tla/src/map/map_147/map.grid.lz"
	.balign 4, 0
.LField_Map147_SHAPE_GRID:
	.incbin "games/tla/src/map/map_147/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map147_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map147_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map147_SCENE:
	.incbin "games/tla/src/map/map_147/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Set70Palette
Tileset_Set70Palette:
	.incbin "games/tla/src/graphics/tileset/set_70/palette.gbapal.lz"
	.byte 0x80, 0xa0	@ PACKER: left in the buffer by Field_Map147+0xe2
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
