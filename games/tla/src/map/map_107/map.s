	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map107
Field_Map107:
	map_header 0, 0, 100, 80, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map107, .LField_Map107_CELL_PICTURES
	map_block Field_Map107, .LField_Map107_SHAPES
	map_block Field_Map107, .LField_Map107_GRID
	map_block Field_Map107, .LField_Map107_SHAPE_GRID
	map_block Field_Map107, .LField_Map107_ANIMATION
	map_block Field_Map107, .LField_Map107_BLEND
	map_block Field_Map107, .LField_Map107_SCENE
.LField_Map107_CELL_PICTURES:
	.incbin "games/tla/src/map/map_107/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map107_SHAPES:
	.incbin "games/tla/src/map/map_107/shapes.table.lz"
	.balign 4, 0
.LField_Map107_GRID:
	.incbin "games/tla/src/map/map_107/grid.parts.lz"
	.balign 4, 0
.LField_Map107_SHAPE_GRID:
	.incbin "games/tla/src/map/map_107/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map107_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map107_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map107_SCENE:
	.incbin "games/tla/src/map/map_107/scene.parts.lz"
	.balign 4, 0
	.global Tileset_Set55Palette
Tileset_Set55Palette:
	.incbin "games/tla/src/graphics/tileset/set_55/palette.gbapal.lz"
	.byte 0x9f	@ PACKER: left in the buffer by Field_Map107+0xe3
	.global Tileset_Set55TilesA
Tileset_Set55TilesA:
	.incbin "games/tla/src/graphics/tileset/set_55/tiles_a.4bpp.mtf"
	.byte 0x22, 0xe3, 0x07	@ PACKER: left in the buffer by Field_Map106+0x1949
	.global Tileset_Set55TilesB
Tileset_Set55TilesB:
	.incbin "games/tla/src/graphics/tileset/set_55/tiles_b.4bpp.mtf"
	.global Tileset_Set55TilesC
Tileset_Set55TilesC:
	.incbin "games/tla/src/graphics/tileset/set_55/tiles_c.4bpp.mtf"
	.global Tileset_Set55TilesD
Tileset_Set55TilesD:
	.incbin "games/tla/src/graphics/tileset/set_55/tiles_d.4bpp.mtf"
	.byte 0x20	@ PACKER: left in the buffer by Tileset_Set55TilesC+0x13f
