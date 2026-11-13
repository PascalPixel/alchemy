	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map030
Field_Map030:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map030, .LField_Map030_CELL_PICTURES
	map_block Field_Map030, .LField_Map030_SHAPES
	map_block Field_Map030, .LField_Map030_GRID
	map_block Field_Map030, .LField_Map030_SHAPE_GRID
	map_block Field_Map030, .LField_Map030_ANIMATION
	map_block Field_Map030, .LField_Map030_BLEND
	map_block Field_Map030, .LField_Map030_SCENE
.LField_Map030_CELL_PICTURES:
	.incbin "games/tla/src/map/map_030/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map030_SHAPES:
	.incbin "games/tla/src/map/map_030/shapes.table.lz"
	.balign 4, 0
.LField_Map030_GRID:
	.incbin "games/tla/src/map/map_030/map.grid.lz"
	.balign 4, 0
.LField_Map030_SHAPE_GRID:
	.incbin "games/tla/src/map/map_030/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map030_ANIMATION:
	.incbin "games/tla/src/map/map_030/animation.script.lz"
	.balign 4, 0
.LField_Map030_BLEND:
	.incbin "games/tla/src/map/map_030/blend.script.lz"
	.balign 4, 0
.LField_Map030_SCENE:
	.incbin "games/tla/src/map/map_030/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Set13Palette
Tileset_Set13Palette:
	.incbin "games/tla/src/graphics/tileset/set_13/palette.gbapal.lz"
	.byte 0x85	@ PACKER: left in the buffer by Field_Map030+0x11f
	.global Tileset_Set13TilesA
Tileset_Set13TilesA:
	.incbin "games/tla/src/graphics/tileset/set_13/tiles_a.4bpp.mtf"
	.byte 0x86	@ PACKER: left in the buffer by Tileset_Set12TilesA+0x239f
	.global Tileset_Set13TilesB
Tileset_Set13TilesB:
	.incbin "games/tla/src/graphics/tileset/set_13/tiles_b.4bpp.mtf"
	.byte 0x7f	@ PACKER: left in the buffer by Tileset_Set13TilesA+0x124b
	.global Tileset_Set13TilesC
Tileset_Set13TilesC:
	.incbin "games/tla/src/graphics/tileset/set_13/tiles_c.4bpp.mtf"
	.byte 0xd1	@ PACKER: left in the buffer by Tileset_Set13TilesA+0x238f
	.global Tileset_Set13TilesD
Tileset_Set13TilesD:
	.incbin "games/tla/src/graphics/tileset/set_13/tiles_d.4bpp.mtf"
	.byte 0xc1, 0x01, 0xb0	@ PACKER: left in the buffer by Tileset_Set13TilesC+0x18fd
