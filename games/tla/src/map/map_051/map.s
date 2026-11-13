	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map051
Field_Map051:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map051, .LField_Map051_CELL_PICTURES
	map_block Field_Map051, .LField_Map051_SHAPES
	map_block Field_Map051, .LField_Map051_GRID
	map_block Field_Map051, .LField_Map051_SHAPE_GRID
	map_block Field_Map051, .LField_Map051_ANIMATION
	map_block Field_Map051, .LField_Map051_BLEND
	map_block Field_Map051, .LField_Map051_SCENE
.LField_Map051_CELL_PICTURES:
	.incbin "games/tla/src/map/map_051/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map051_SHAPES:
	.incbin "games/tla/src/map/map_051/shapes.table.lz"
	.balign 4, 0
.LField_Map051_GRID:
	.incbin "games/tla/src/map/map_051/map.grid.lz"
	.balign 4, 0
.LField_Map051_SHAPE_GRID:
	.incbin "games/tla/src/map/map_051/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map051_ANIMATION:
	.incbin "games/tla/src/map/map_051/animation.script.lz"
	.balign 4, 0
.LField_Map051_BLEND:
	.incbin "games/tla/src/map/map_051/blend.script.lz"
	.balign 4, 0
.LField_Map051_SCENE:
	.incbin "games/tla/src/map/map_051/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Set32Palette
Tileset_Set32Palette:
	.incbin "games/tla/src/graphics/tileset/set_32/palette.gbapal.lz"
	.byte 0x74, 0x78, 0x1e	@ PACKER: left in the buffer by Field_Map051+0xe9
	.global Tileset_Set32TilesA
Tileset_Set32TilesA:
	.incbin "games/tla/src/graphics/tileset/set_32/tiles_a.4bpp.mtf"
	.global Tileset_Set32TilesB
Tileset_Set32TilesB:
	.incbin "games/tla/src/graphics/tileset/set_32/tiles_b.4bpp.mtf"
	.byte 0x13, 0xf2	@ PACKER: left in the buffer by Tileset_Set32TilesA+0xaea
	.global Tileset_Set32TilesC
Tileset_Set32TilesC:
	.incbin "games/tla/src/graphics/tileset/set_32/tiles_c.4bpp.mtf"
	.byte 0xe4, 0x78, 0x1a	@ PACKER: left in the buffer by Tileset_Set32TilesB+0x675
	.global Tileset_Set32TilesD
Tileset_Set32TilesD:
	.incbin "games/tla/src/graphics/tileset/set_32/tiles_d.4bpp.mtf"
