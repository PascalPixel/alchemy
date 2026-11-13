	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map287
Field_Map287:
	map_header 0, 0, 128, 128, 3, 2, 0, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 100, 100, 0, 0, 0xff, 0xff
	map_block Field_Map287, .LField_Map287_CELL_PICTURES
	map_block Field_Map287, .LField_Map287_SHAPES
	map_block Field_Map287, .LField_Map287_GRID
	map_block Field_Map287, .LField_Map287_SHAPE_GRID
	map_block Field_Map287, .LField_Map287_ANIMATION
	map_block Field_Map287, .LField_Map287_BLEND
	map_block Field_Map287, .LField_Map287_SCENE
.LField_Map287_CELL_PICTURES:
	.incbin "games/tla/src/map/map_287/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map287_SHAPES:
	.incbin "games/tla/src/map/map_287/shapes.table.lz"
	.balign 4, 0
.LField_Map287_GRID:
	.incbin "games/tla/src/map/map_287/map.grid.lz"
	.balign 4, 0
.LField_Map287_SHAPE_GRID:
	.incbin "games/tla/src/map/map_287/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map287_ANIMATION:
	.incbin "games/tla/src/map/map_062/animation.script.lz"
	.balign 4, 0
.LField_Map287_BLEND:
	.incbin "games/tla/src/map/map_060/blend.script.lz"
	.balign 4, 0
.LField_Map287_SCENE:
	.incbin "games/tla/src/map/map_287/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Set117Palette
Tileset_Set117Palette:
	.incbin "games/tla/src/graphics/tileset/set_117/palette.gbapal.lz"
	.byte 0x74, 0x6c, 0x5a	@ PACKER: left in the buffer by Field_Map287+0x121
	.global Tileset_Set117TilesA
Tileset_Set117TilesA:
	.incbin "games/tla/src/graphics/tileset/set_117/tiles_a.4bpp.mtf"
	.byte 0xe2	@ PACKER: left in the buffer by Tileset_Set116TilesA+0x2afb
	.global Tileset_Set117TilesB
Tileset_Set117TilesB:
	.incbin "games/tla/src/graphics/tileset/set_117/tiles_b.4bpp.mtf"
	.byte 0xe0, 0x28	@ PACKER: left in the buffer by Tileset_Set117TilesA+0xdf6
	.global Tileset_Set117TilesC
Tileset_Set117TilesC:
	.incbin "games/tla/src/graphics/tileset/set_117/tiles_c.4bpp.mtf"
	.global Tileset_Set117TilesD
Tileset_Set117TilesD:
	.incbin "games/tla/src/graphics/tileset/set_117/tiles_d.4bpp.mtf"
	.byte 0xc5, 0xde, 0x76	@ PACKER: left in the buffer by Tileset_Set117TilesA+0x1839
