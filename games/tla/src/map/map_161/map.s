	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map161
Field_Map161:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map161, .LField_Map161_CELL_PICTURES
	map_block Field_Map161, .LField_Map161_SHAPES
	map_block Field_Map161, .LField_Map161_GRID
	map_block Field_Map161, .LField_Map161_SHAPE_GRID
	map_block Field_Map161, .LField_Map161_ANIMATION
	map_block Field_Map161, .LField_Map161_BLEND
	map_block Field_Map161, .LField_Map161_SCENE
.LField_Map161_CELL_PICTURES:
	.incbin "games/tla/src/map/map_161/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map161_SHAPES:
	.incbin "games/tla/src/map/map_161/shapes.table.lz"
	.balign 4, 0
.LField_Map161_GRID:
	.incbin "games/tla/src/map/map_161/grid.parts.lz"
	.balign 4, 0
.LField_Map161_SHAPE_GRID:
	.incbin "games/tla/src/map/map_161/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map161_ANIMATION:
	.incbin "games/tla/src/map/map_161/animation.script.lz"
	.balign 4, 0
.LField_Map161_BLEND:
	.incbin "games/tla/src/map/map_161/blend.script.lz"
	.balign 4, 0
.LField_Map161_SCENE:
	.incbin "games/tla/src/map/map_161/scene.parts.lz"
	.balign 4, 0
	.global Tileset_Set74Palette
Tileset_Set74Palette:
	.incbin "games/tla/src/graphics/tileset/set_74/palette.gbapal.lz"
	.byte 0xcc, 0x0c, 0x26	@ PACKER: left in the buffer by Field_Map161+0x1b1
	.global Tileset_Set74TilesA
Tileset_Set74TilesA:
	.incbin "games/tla/src/graphics/tileset/set_74/tiles_a.4bpp.mtf"
	.byte 0x05, 0x80, 0x00	@ PACKER: left in the buffer by Field_Map161+0x1701
	.global Tileset_Set74TilesB
Tileset_Set74TilesB:
	.incbin "games/tla/src/graphics/tileset/set_74/tiles_b.4bpp.mtf"
	.global Tileset_Set74TilesC
Tileset_Set74TilesC:
	.incbin "games/tla/src/graphics/tileset/set_74/tiles_c.4bpp.mtf"
	.byte 0x7a	@ PACKER: left in the buffer by Tileset_Set74TilesB+0xc8f
	.global Tileset_Set74TilesD
Tileset_Set74TilesD:
	.incbin "games/tla/src/graphics/tileset/set_74/tiles_d.4bpp.mtf"
	.byte 0xb1, 0x74, 0x4f	@ PACKER: left in the buffer by Tileset_Set74TilesB+0x1349
