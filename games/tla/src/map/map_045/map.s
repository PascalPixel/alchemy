	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map045
Field_Map045:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map045, .LField_Map045_CELL_PICTURES
	map_block Field_Map045, .LField_Map045_SHAPES
	map_block Field_Map045, .LField_Map045_GRID
	map_block Field_Map045, .LField_Map045_SHAPE_GRID
	map_block Field_Map045, .LField_Map045_ANIMATION
	map_block Field_Map045, .LField_Map045_BLEND
	map_block Field_Map045, .LField_Map045_SCENE
.LField_Map045_CELL_PICTURES:
	.incbin "games/tla/src/map/map_045/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map045_SHAPES:
	.incbin "games/tla/src/map/map_045/shapes.table.lz"
	.balign 4, 0
.LField_Map045_GRID:
	.incbin "games/tla/src/map/map_045/map.grid.lz"
	.balign 4, 0
.LField_Map045_SHAPE_GRID:
	.incbin "games/tla/src/map/map_045/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map045_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map045_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map045_SCENE:
	.incbin "games/tla/src/map/map_045/scene.parts.lz"
	.global Tileset_Set28Palette
Tileset_Set28Palette:
	.incbin "games/tla/src/graphics/tileset/set_28/palette.gbapal.lz"
	.byte 0x2d, 0x40, 0xff	@ PACKER: left in the buffer by Field_Map045+0x169
	.global Tileset_Set28TilesA
Tileset_Set28TilesA:
	.incbin "games/tla/src/graphics/tileset/set_28/tiles_a.4bpp.mtf"
	.byte 0x04	@ PACKER: left in the buffer by Field_Map041+0x25af
	.global Tileset_Set28TilesB
Tileset_Set28TilesB:
	.incbin "games/tla/src/graphics/tileset/set_28/tiles_b.4bpp.mtf"
	.byte 0xce, 0x80	@ PACKER: left in the buffer by Tileset_Set28TilesA+0x1cbe
	.global Tileset_Set28TilesC
Tileset_Set28TilesC:
	.incbin "games/tla/src/graphics/tileset/set_28/tiles_c.4bpp.mtf"
	.byte 0x20, 0xdb	@ PACKER: left in the buffer by Tileset_Set28TilesB+0x1aca
	.global Tileset_Set28TilesD
Tileset_Set28TilesD:
	.incbin "games/tla/src/graphics/tileset/set_28/tiles_d.4bpp.mtf"
