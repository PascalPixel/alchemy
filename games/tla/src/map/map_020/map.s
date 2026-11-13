	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map020
Field_Map020:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map020, .LField_Map020_CELL_PICTURES
	map_block Field_Map020, .LField_Map020_SHAPES
	map_block Field_Map020, .LField_Map020_GRID
	map_block Field_Map020, .LField_Map020_SHAPE_GRID
	map_block Field_Map020, .LField_Map020_ANIMATION
	map_block Field_Map020, .LField_Map020_BLEND
	map_block Field_Map020, .LField_Map020_SCENE
.LField_Map020_CELL_PICTURES:
	.incbin "games/tla/src/map/map_020/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map020_SHAPES:
	.incbin "games/tla/src/map/map_020/shapes.table.lz"
	.balign 4, 0
.LField_Map020_GRID:
	.incbin "games/tla/src/map/map_020/map.grid.lz"
	.balign 4, 0
.LField_Map020_SHAPE_GRID:
	.incbin "games/tla/src/map/map_020/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map020_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map020_BLEND:
	.incbin "games/tla/src/map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map020_SCENE:
	.incbin "games/tla/src/map/map_020/scene.parts.lz"
	.balign 4, 0
	.global Tileset_Set11Palette
Tileset_Set11Palette:
	.incbin "games/tla/src/graphics/tileset/set_11/palette.gbapal.lz"
	.global Tileset_Set11TilesA
Tileset_Set11TilesA:
	.incbin "games/tla/src/graphics/tileset/set_11/tiles_a.4bpp.mtf"
	.global Tileset_Set11TilesB
Tileset_Set11TilesB:
	.incbin "games/tla/src/graphics/tileset/set_11/tiles_b.4bpp.mtf"
	.global Tileset_Set11TilesC
Tileset_Set11TilesC:
	.incbin "games/tla/src/graphics/tileset/set_11/tiles_c.4bpp.mtf"
	.byte 0xc6	@ PACKER: left in the buffer by Tileset_Set11TilesA+0x2287
	.global Tileset_Set11TilesD
Tileset_Set11TilesD:
	.incbin "games/tla/src/graphics/tileset/set_11/tiles_d.4bpp.mtf"
	.byte 0x3d, 0x87, 0xa4	@ PACKER: left in the buffer by Tileset_Set11TilesC+0x4d1
