	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map205
Field_Map205:
	map_header 0, 0, 52, 54, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 52, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 54, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map205, .LField_Map205_CELL_PICTURES
	map_block Field_Map205, .LField_Map205_SHAPES
	map_block Field_Map205, .LField_Map205_GRID
	map_block Field_Map205, .LField_Map205_SHAPE_GRID
	map_block Field_Map205, .LField_Map205_ANIMATION
	map_block Field_Map205, .LField_Map205_BLEND
	map_block Field_Map205, .LField_Map205_SCENE
.LField_Map205_CELL_PICTURES:
	.incbin "games/tla/src/map/map_205/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map205_SHAPES:
	.incbin "games/tla/src/map/map_205/shapes.table.lz"
	.balign 4, 0
.LField_Map205_GRID:
	.incbin "games/tla/src/map/map_205/map.grid.lz"
	.balign 4, 0
.LField_Map205_SHAPE_GRID:
	.incbin "games/tla/src/map/map_205/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map205_ANIMATION:
	.incbin "games/tla/src/map/map_205/animation.script.lz"
	.balign 4, 0
.LField_Map205_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map205_SCENE:
	.incbin "games/tla/src/map/map_205/scene.parts.lz"
	.balign 4, 0
	.global Tileset_Set90Palette
Tileset_Set90Palette:
	.incbin "games/tla/src/graphics/tileset/set_90/palette.gbapal.lz"
	.byte 0xc0, 0x39	@ PACKER: left in the buffer by Field_Map205+0x166
	.global Tileset_Set90TilesA
Tileset_Set90TilesA:
	.incbin "games/tla/src/graphics/tileset/set_90/tiles_a.4bpp.mtf"
	.global Tileset_Set90TilesB
Tileset_Set90TilesB:
	.incbin "games/tla/src/graphics/tileset/set_90/tiles_b.4bpp.mtf"
	.global Tileset_Set90TilesC
Tileset_Set90TilesC:
	.incbin "games/tla/src/graphics/tileset/set_90/tiles_c.4bpp.mtf"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global Tileset_Set90TilesD
Tileset_Set90TilesD:
	.incbin "games/tla/src/graphics/tileset/set_90/tiles_d.4bpp.mtf"
	.byte 0xe1	@ PACKER: left in the buffer by Tileset_Set90TilesC+0x5db
