	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map163
Field_Map163:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map163, .LField_Map163_CELL_PICTURES
	map_block Field_Map163, .LField_Map163_SHAPES
	map_block Field_Map163, .LField_Map163_GRID
	map_block Field_Map163, .LField_Map163_SHAPE_GRID
	map_block Field_Map163, .LField_Map163_ANIMATION
	map_block Field_Map163
	map_block Field_Map163, .LField_Map163_SCENE
.LField_Map163_CELL_PICTURES:
	.incbin "games/tla/src/map/map_163/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map163_SHAPES:
	.incbin "games/tla/src/map/map_163/shapes.table.lz"
	.balign 4, 0
.LField_Map163_GRID:
	.incbin "games/tla/src/map/map_163/map.grid.lz"
	.balign 4, 0
.LField_Map163_SHAPE_GRID:
	.incbin "games/tla/src/map/map_163/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map163_ANIMATION:
	.incbin "games/tla/src/map/map_163/animation.script.lz"
	.balign 4, 0
.LField_Map163_SCENE:
	.incbin "games/tla/src/map/map_163/scene.scene.lz"
	.global Tileset_Set76Palette
Tileset_Set76Palette:
	.incbin "games/tla/src/graphics/tileset/set_76/palette.gbapal.lz"
	.byte 0xfd, 0xa6, 0x32	@ PACKER: left in the buffer by Field_Map163+0x14d
	.global Tileset_Set76TilesA
Tileset_Set76TilesA:
	.incbin "games/tla/src/graphics/tileset/set_76/tiles_a.4bpp.mtf"
	.byte 0x04, 0xc3, 0x4b	@ PACKER: left in the buffer by Field_Map163+0x4d1
