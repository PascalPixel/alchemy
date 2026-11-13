	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map204
Field_Map204:
	map_header 0, 0, 128, 64, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map204, .LField_Map204_CELL_PICTURES
	map_block Field_Map204, .LField_Map204_SHAPES
	map_block Field_Map204, .LField_Map204_GRID
	map_block Field_Map204, .LField_Map204_SHAPE_GRID
	map_block Field_Map204, .LField_Map204_ANIMATION
	map_block Field_Map204, .LField_Map204_BLEND
	map_block Field_Map204, .LField_Map204_SCENE
.LField_Map204_CELL_PICTURES:
	.incbin "games/tla/src/map/map_204/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Map204_SHAPES:
	.incbin "games/tla/src/map/map_204/shapes.table.lz"
	.balign 4, 0
.LField_Map204_GRID:
	.incbin "games/tla/src/map/map_204/map.grid.lz"
	.balign 4, 0
.LField_Map204_SHAPE_GRID:
	.incbin "games/tla/src/map/map_204/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map204_ANIMATION:
	.incbin "games/tla/src/map/map_204/animation.script.lz"
	.balign 4, 0
.LField_Map204_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map204_SCENE:
	.incbin "games/tla/src/map/map_204/scene.parts.lz"
	.global Tileset_Set89Palette
Tileset_Set89Palette:
	.incbin "games/tla/src/graphics/tileset/set_89/palette.gbapal.lz"
	.global Tileset_Set89TilesA
Tileset_Set89TilesA:
	.incbin "games/tla/src/graphics/tileset/set_89/tiles_a.4bpp.mtf"
	.global Tileset_Set89TilesB
Tileset_Set89TilesB:
	.incbin "games/tla/src/graphics/tileset/set_89/tiles_b.4bpp.mtf"
	.byte 0x04, 0x7a	@ PACKER: left in the buffer by Tileset_Set89TilesA+0x199a
	.global Tileset_Set89TilesC
Tileset_Set89TilesC:
	.incbin "games/tla/src/graphics/tileset/set_89/tiles_c.4bpp.mtf"
	.global Tileset_Set89TilesD
Tileset_Set89TilesD:
	.incbin "games/tla/src/graphics/tileset/set_89/tiles_d.4bpp.mtf"
	.byte 0x0d	@ PACKER: left in the buffer by Tileset_Set89TilesC+0xb83
