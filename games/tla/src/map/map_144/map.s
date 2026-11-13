	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map144
Field_Map144:
	map_header 0, 0, 88, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map144, .LField_Map144_CELL_PICTURES
	map_block Field_Map144, .LField_Map144_SHAPES
	map_block Field_Map144, .LField_Map144_GRID
	map_block Field_Map144, .LField_Map144_SHAPE_GRID
	map_block Field_Map144, .LField_Map144_ANIMATION
	map_block Field_Map144, .LField_Map144_BLEND
	map_block Field_Map144, .LField_Map144_SCENE
.LField_Map144_CELL_PICTURES:
	.incbin "games/tla/src/map/map_144/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map144_SHAPES:
	.incbin "games/tla/src/map/map_144/shapes.table.lz"
	.balign 4, 0
.LField_Map144_GRID:
	.incbin "games/tla/src/map/map_144/map.grid.lz"
	.balign 4, 0
.LField_Map144_SHAPE_GRID:
	.incbin "games/tla/src/map/map_144/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map144_ANIMATION:
	.incbin "games/tla/src/map/map_144/animation.script.lz"
	.balign 4, 0
.LField_Map144_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map144_SCENE:
	.incbin "games/tla/src/map/map_144/scene.parts.lz"
	.balign 4, 0
	.global Tileset_Set67Palette
Tileset_Set67Palette:
	.incbin "games/tla/src/graphics/tileset/set_67/palette.gbapal.lz"
	.byte 0x78, 0x9f	@ PACKER: left in the buffer by Field_Map144+0x156
	.global Tileset_Set67TilesA
Tileset_Set67TilesA:
	.incbin "games/tla/src/graphics/tileset/set_67/tiles_a.4bpp.mtf"
	.global Tileset_Set67TilesB
Tileset_Set67TilesB:
	.incbin "games/tla/src/graphics/tileset/set_67/tiles_b.4bpp.mtf"
	.byte 0xa3	@ PACKER: left in the buffer by Tileset_Set67TilesA+0x1ab7
	.global Tileset_Set67TilesC
Tileset_Set67TilesC:
	.incbin "games/tla/src/graphics/tileset/set_67/tiles_c.4bpp.mtf"
	.byte 0x0f, 0x74, 0xf6	@ PACKER: left in the buffer by Tileset_Set67TilesB+0x1881
	.global Tileset_Set67TilesD
Tileset_Set67TilesD:
	.incbin "games/tla/src/graphics/tileset/set_67/tiles_d.4bpp.mtf"
	.byte 0x6d, 0x93, 0xa2	@ PACKER: left in the buffer by Field_Map144+0x1d25
