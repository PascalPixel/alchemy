	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map223
Field_Map223:
	map_header 0, 0, 64, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map223, .LField_Map223_CELL_PICTURES
	map_block Field_Map223, .LField_Map223_SHAPES
	map_block Field_Map223, .LField_Map223_GRID
	map_block Field_Map223, .LField_Map223_SHAPE_GRID
	map_block Field_Map223, .LField_Map223_ANIMATION
	map_block Field_Map223, .LField_Map223_BLEND
	map_block Field_Map223, .LField_Map223_SCENE
.LField_Map223_CELL_PICTURES:
	.incbin "games/tla/src/map/map_223/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map223_SHAPES:
	.incbin "games/tla/src/map/map_223/shapes.table.lz"
	.balign 4, 0
.LField_Map223_GRID:
	.incbin "games/tla/src/map/map_223/grid.parts.lz"
	.balign 4, 0
.LField_Map223_SHAPE_GRID:
	.incbin "games/tla/src/map/map_223/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map223_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map223_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map223_SCENE:
	.incbin "games/tla/src/map/map_223/scene.parts.lz"
	.global Tileset_Set96Palette
Tileset_Set96Palette:
	.incbin "games/tla/src/graphics/tileset/set_96/palette.gbapal.lz"
	.byte 0x92, 0x44, 0x80	@ PACKER: left in the buffer by Field_Map223+0x135
	.global Tileset_Set96TilesA
Tileset_Set96TilesA:
	.incbin "games/tla/src/graphics/tileset/set_96/tiles_a.4bpp.mtf"
	.global Tileset_Set96TilesB
Tileset_Set96TilesB:
	.incbin "games/tla/src/graphics/tileset/set_96/tiles_b.4bpp.mtf"
	.byte 0xf2, 0x8b, 0xdd	@ PACKER: left in the buffer by Tileset_Set96TilesA+0x1365
	.global Tileset_Set96TilesC
Tileset_Set96TilesC:
	.incbin "games/tla/src/graphics/tileset/set_96/tiles_c.4bpp.mtf"
	.byte 0x16	@ PACKER: left in the buffer by Tileset_Set96TilesB+0xc87
	.global Tileset_Set96TilesD
Tileset_Set96TilesD:
	.incbin "games/tla/src/graphics/tileset/set_96/tiles_d.4bpp.mtf"
	.byte 0x0e	@ PACKER: left in the buffer by Tileset_Set96TilesC+0x13f
