	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map277
Field_Map277:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map277, .LField_Map277_CELL_PICTURES
	map_block Field_Map277, .LField_Map277_SHAPES
	map_block Field_Map277, .LField_Map277_GRID
	map_block Field_Map277, .LField_Map277_SHAPE_GRID
	map_block Field_Map277, .LField_Map277_ANIMATION
	map_block Field_Map277, .LField_Map277_BLEND
	map_block Field_Map277, .LField_Map277_SCENE
.LField_Map277_CELL_PICTURES:
	.incbin "games/tla/src/map/map_277/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map277_SHAPES:
	.incbin "games/tla/src/map/map_277/shapes.table.lz"
	.balign 4, 0
.LField_Map277_GRID:
	.incbin "games/tla/src/map/map_277/grid.parts.lz"
	.balign 4, 0
.LField_Map277_SHAPE_GRID:
	.incbin "games/tla/src/map/map_277/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map277_ANIMATION:
	.incbin "games/tla/src/map/map_277/animation.script.lz"
	.balign 4, 0
.LField_Map277_BLEND:
	.incbin "games/tla/src/map/map_277/blend.script.lz"
	.balign 4, 0
.LField_Map277_SCENE:
	.incbin "games/tla/src/map/map_277/scene.parts.lz"
	.balign 4, 0
	.global Tileset_Set112Palette
Tileset_Set112Palette:
	.incbin "games/tla/src/graphics/tileset/set_112/palette.gbapal.lz"
	.byte 0x25	@ PACKER: left in the buffer by Field_Map277+0x117
	.global Tileset_Set112TilesA
Tileset_Set112TilesA:
	.incbin "games/tla/src/graphics/tileset/set_112/tiles_a.4bpp.mtf"
	.byte 0x7d, 0x00, 0x00	@ PACKER: left in the buffer by Field_Map277+0x1e01
	.global Tileset_Set112TilesB
Tileset_Set112TilesB:
	.incbin "games/tla/src/graphics/tileset/set_112/tiles_b.4bpp.mtf"
	.byte 0x00, 0x07	@ PACKER: left in the buffer by Tileset_Set112TilesA+0x1ac2
	.global Tileset_Set112TilesC
Tileset_Set112TilesC:
	.incbin "games/tla/src/graphics/tileset/set_112/tiles_c.4bpp.mtf"
	.byte 0x10, 0x9e	@ PACKER: left in the buffer by Tileset_Set112TilesB+0x726
	.global Tileset_Set112TilesD
Tileset_Set112TilesD:
	.incbin "games/tla/src/graphics/tileset/set_112/tiles_d.4bpp.mtf"
	.byte 0xfb, 0x4f	@ PACKER: left in the buffer by Tileset_Set112TilesB+0x13fa
