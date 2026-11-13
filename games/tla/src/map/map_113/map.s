	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map113
Field_Map113:
	map_header 0, 0, 100, 80, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map113, .LField_Map113_CELL_PICTURES
	map_block Field_Map113, .LField_Map113_SHAPES
	map_block Field_Map113, .LField_Map113_GRID
	map_block Field_Map113, .LField_Map113_SHAPE_GRID
	map_block Field_Map113, .LField_Map113_ANIMATION
	map_block Field_Map113, .LField_Map113_BLEND
	map_block Field_Map113, .LField_Map113_SCENE
.LField_Map113_CELL_PICTURES:
	.incbin "games/tla/src/map/map_113/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map113_SHAPES:
	.incbin "games/tla/src/map/map_113/shapes.table.lz"
	.balign 4, 0
.LField_Map113_GRID:
	.incbin "games/tla/src/map/map_113/map.grid.lz"
	.balign 4, 0
.LField_Map113_SHAPE_GRID:
	.incbin "games/tla/src/map/map_113/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map113_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map113_BLEND:
	.incbin "games/tla/src/map/map_113/blend.script.lz"
	.balign 4, 0
.LField_Map113_SCENE:
	.incbin "games/tla/src/map/map_113/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Set56Palette
Tileset_Set56Palette:
	.incbin "games/tla/src/graphics/tileset/set_56/palette.gbapal.lz"
	.byte 0x11, 0x04	@ PACKER: left in the buffer by Field_Map113+0xfa
	.global Tileset_Set56TilesA
Tileset_Set56TilesA:
	.incbin "games/tla/src/graphics/tileset/set_56/tiles_a.4bpp.mtf"
	.byte 0x00	@ PACKER: left in the buffer by Field_Map106+0x1e57
	.global Tileset_Set56TilesB
Tileset_Set56TilesB:
	.incbin "games/tla/src/graphics/tileset/set_56/tiles_b.4bpp.mtf"
	.byte 0x27, 0xe0, 0xfb	@ PACKER: left in the buffer by Tileset_Set56TilesA+0xe0d
	.global Tileset_Set56TilesC
Tileset_Set56TilesC:
	.incbin "games/tla/src/graphics/tileset/set_56/tiles_c.4bpp.mtf"
	.global Tileset_Set56TilesD
Tileset_Set56TilesD:
	.incbin "games/tla/src/graphics/tileset/set_56/tiles_d.4bpp.mtf"
