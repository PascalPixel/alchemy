	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map097
Field_Map097:
	map_header 0, 0, 0, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 192, 64, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map097, .LField_Map097_CELL_PICTURES
	map_block Field_Map097, .LField_Map097_SHAPES
	map_block Field_Map097, .LField_Map097_GRID
	map_block Field_Map097, .LField_Map097_SHAPE_GRID
	map_block Field_Map097, .LField_Map097_ANIMATION
	map_block Field_Map097, .LField_Map097_BLEND
	map_block Field_Map097, .LField_Map097_SCENE
.LField_Map097_CELL_PICTURES:
	.incbin "games/tla/src/map/map_097/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Map097_SHAPES:
	.incbin "games/tla/src/map/map_097/shapes.table.lz"
	.balign 4, 0
.LField_Map097_GRID:
	.incbin "games/tla/src/map/map_097/map.grid.lz"
	.balign 4, 0
.LField_Map097_SHAPE_GRID:
	.incbin "games/tla/src/map/map_097/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map097_ANIMATION:
	.incbin "games/tla/src/map/map_097/animation.script.lz"
	.balign 4, 0
.LField_Map097_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map097_SCENE:
	.incbin "games/tla/src/map/map_097/scene.scene.lz"
	.global Tileset_Set48Palette
Tileset_Set48Palette:
	.incbin "games/tla/src/graphics/tileset/set_48/palette.gbapal.lz"
	.byte 0xe3, 0xe0, 0x4b	@ PACKER: left in the buffer by Field_Map097+0x121
	.global Tileset_Set48TilesA
Tileset_Set48TilesA:
	.incbin "games/tla/src/graphics/tileset/set_48/tiles_a.4bpp.mtf"
	.global Tileset_Set48TilesB
Tileset_Set48TilesB:
	.incbin "games/tla/src/graphics/tileset/set_48/tiles_b.4bpp.mtf"
	.byte 0xd6	@ PACKER: left in the buffer by Tileset_Set48TilesA+0x13cf
	.global Tileset_Set48TilesC
Tileset_Set48TilesC:
	.incbin "games/tla/src/graphics/tileset/set_48/tiles_c.4bpp.mtf"
	.byte 0x8f, 0xa4	@ PACKER: left in the buffer by Tileset_Set48TilesB+0xdf2
	.global Tileset_Set48TilesD
Tileset_Set48TilesD:
	.incbin "games/tla/src/graphics/tileset/set_48/tiles_d.4bpp.mtf"
	.byte 0x50, 0x4e	@ PACKER: left in the buffer by Tileset_Set48TilesC+0xd76
