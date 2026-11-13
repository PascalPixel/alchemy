	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map118
Field_Map118:
	map_header 0, 0, 90, 100, 3, 3, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map118, .LField_Map118_CELL_PICTURES
	map_block Field_Map118, .LField_Map118_SHAPES
	map_block Field_Map118, .LField_Map118_GRID
	map_block Field_Map118, .LField_Map118_SHAPE_GRID
	map_block Field_Map118, .LField_Map118_ANIMATION
	map_block Field_Map118, .LField_Map118_BLEND
	map_block Field_Map118, .LField_Map118_SCENE
.LField_Map118_CELL_PICTURES:
	.incbin "games/tla/src/map/map_118/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map118_SHAPES:
	.incbin "games/tla/src/map/map_118/shapes.table.lz"
	.balign 4, 0
.LField_Map118_GRID:
	.incbin "games/tla/src/map/map_118/grid.parts.lz"
	.balign 4, 0
.LField_Map118_SHAPE_GRID:
	.incbin "games/tla/src/map/map_118/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map118_ANIMATION:
	.incbin "games/tla/src/map/map_021/animation.script.lz"
	.balign 4, 0
.LField_Map118_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map118_SCENE:
	.incbin "games/tla/src/map/map_118/scene.parts.lz"
	.balign 4, 0
	.global Tileset_Set59Palette
Tileset_Set59Palette:
	.incbin "games/tla/src/graphics/tileset/set_59/palette.gbapal.lz"
	.byte 0xd5, 0x1b	@ PACKER: left in the buffer by Field_Map118+0x1a2
	.global Tileset_Set59TilesA
Tileset_Set59TilesA:
	.incbin "games/tla/src/graphics/tileset/set_59/tiles_a.4bpp.mtf"
	.byte 0x77, 0x46	@ PACKER: left in the buffer by Field_Map099+0x2ec2
	.global Tileset_Set59TilesB
Tileset_Set59TilesB:
	.incbin "games/tla/src/graphics/tileset/set_59/tiles_b.4bpp.mtf"
	.byte 0x4d, 0xd0, 0x25	@ PACKER: left in the buffer by Tileset_Set59TilesA+0x197d
	.global Tileset_Set59TilesC
Tileset_Set59TilesC:
	.incbin "games/tla/src/graphics/tileset/set_59/tiles_c.4bpp.mtf"
	.byte 0x38, 0xa4	@ PACKER: left in the buffer by Tileset_Set59TilesB+0x104a
	.global Tileset_Set59TilesD
Tileset_Set59TilesD:
	.incbin "games/tla/src/graphics/tileset/set_59/tiles_d.4bpp.mtf"
