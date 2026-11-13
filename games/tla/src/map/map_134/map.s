	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map134
Field_Map134:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map134, .LField_Map134_CELL_PICTURES
	map_block Field_Map134, .LField_Map134_SHAPES
	map_block Field_Map134, .LField_Map134_GRID
	map_block Field_Map134, .LField_Map134_SHAPE_GRID
	map_block Field_Map134, .LField_Map134_ANIMATION
	map_block Field_Map134, .LField_Map134_BLEND
	map_block Field_Map134, .LField_Map134_SCENE
.LField_Map134_CELL_PICTURES:
	.incbin "games/tla/src/map/map_134/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map134_SHAPES:
	.incbin "games/tla/src/map/map_134/shapes.table.lz"
	.balign 4, 0
.LField_Map134_GRID:
	.incbin "games/tla/src/map/map_134/grid.parts.lz"
	.balign 4, 0
.LField_Map134_SHAPE_GRID:
	.incbin "games/tla/src/map/map_134/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map134_ANIMATION:
	.incbin "games/tla/src/map/map_134/animation.script.lz"
	.balign 4, 0
.LField_Map134_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map134_SCENE:
	.incbin "games/tla/src/map/map_134/scene.parts.lz"
	.balign 4, 0
	.global Tileset_Set65Palette
Tileset_Set65Palette:
	.incbin "games/tla/src/graphics/tileset/set_65/palette.gbapal.lz"
	.byte 0x27, 0x95	@ PACKER: left in the buffer by Field_Map134+0x10a
	.global Tileset_Set65TilesA
Tileset_Set65TilesA:
	.incbin "games/tla/src/graphics/tileset/set_65/tiles_a.4bpp.mtf"
	.byte 0x4c, 0x4f, 0x3a	@ PACKER: left in the buffer by Field_Map134+0x2091
	.global Tileset_Set65TilesB
Tileset_Set65TilesB:
	.incbin "games/tla/src/graphics/tileset/set_65/tiles_b.4bpp.mtf"
	.global Tileset_Set65TilesC
Tileset_Set65TilesC:
	.incbin "games/tla/src/graphics/tileset/set_65/tiles_c.4bpp.mtf"
	.global Tileset_Set65TilesD
Tileset_Set65TilesD:
	.incbin "games/tla/src/graphics/tileset/set_65/tiles_d.4bpp.mtf"
	.byte 0x4b	@ PACKER: left in the buffer by Tileset_Set65TilesB+0x162b
