	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map179
Field_Map179:
	map_header 0, 0, 128, 128, 3, 3, 3, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 4, -4, 0x1f, 0x1f
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map179, .LField_Map179_CELL_PICTURES
	map_block Field_Map179, .LField_Map179_SHAPES
	map_block Field_Map179, .LField_Map179_GRID
	map_block Field_Map179, .LField_Map179_SHAPE_GRID
	map_block Field_Map179, .LField_Map179_ANIMATION
	map_block Field_Map179, .LField_Map179_BLEND
	map_block Field_Map179, .LField_Map179_SCENE
.LField_Map179_CELL_PICTURES:
	.incbin "games/tla/src/map/map_179/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map179_SHAPES:
	.incbin "games/tla/src/map/map_179/shapes.table.lz"
	.balign 4, 0
.LField_Map179_GRID:
	.incbin "games/tla/src/map/map_179/map.grid.lz"
	.balign 4, 0
.LField_Map179_SHAPE_GRID:
	.incbin "games/tla/src/map/map_179/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map179_ANIMATION:
	.incbin "games/tla/src/map/map_179/animation.script.lz"
	.balign 4, 0
.LField_Map179_BLEND:
	.incbin "games/tla/src/map/map_179/blend.script.lz"
	.balign 4, 0
.LField_Map179_SCENE:
	.incbin "games/tla/src/map/map_179/scene.parts.lz"
	.balign 4, 0
	.global Tileset_Set80Palette
Tileset_Set80Palette:
	.incbin "games/tla/src/graphics/tileset/set_80/palette.gbapal.lz"
	.byte 0x04, 0x08, 0xf0	@ PACKER: left in the buffer by Field_Map179+0xf1
	.global Tileset_Set80TilesA
Tileset_Set80TilesA:
	.incbin "games/tla/src/graphics/tileset/set_80/tiles_a.4bpp.mtf"
	.byte 0x24, 0x03, 0x8f	@ PACKER: left in the buffer by Field_Map179+0x10dd
	.global Tileset_Set80TilesB
Tileset_Set80TilesB:
	.incbin "games/tla/src/graphics/tileset/set_80/tiles_b.4bpp.mtf"
	.byte 0x00	@ PACKER: left in the buffer by Field_Map179+0x1ed7
	.global Tileset_Set80TilesC
Tileset_Set80TilesC:
	.incbin "games/tla/src/graphics/tileset/set_80/tiles_c.4bpp.mtf"
	.byte 0xf8	@ PACKER: left in the buffer by Tileset_Set80TilesB+0xfb7
	.global Tileset_Set80TilesD
Tileset_Set80TilesD:
	.incbin "games/tla/src/graphics/tileset/set_80/tiles_d.4bpp.mtf"
	.byte 0x24, 0x40	@ PACKER: left in the buffer by Field_Map179+0x1f46
