	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map148
Field_Map148:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map148, .LField_Map148_CELL_PICTURES
	map_block Field_Map148, .LField_Map148_SHAPES
	map_block Field_Map148, .LField_Map148_GRID
	map_block Field_Map148, .LField_Map148_SHAPE_GRID
	map_block Field_Map148, .LField_Map148_ANIMATION
	map_block Field_Map148, .LField_Map148_BLEND
	map_block Field_Map148, .LField_Map148_SCENE
.LField_Map148_CELL_PICTURES:
	.incbin "games/tla/src/map/map_148/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Map148_SHAPES:
	.incbin "games/tla/src/map/map_148/shapes.table.lz"
	.balign 4, 0
.LField_Map148_GRID:
	.incbin "games/tla/src/map/map_148/grid.parts.lz"
	.balign 4, 0
.LField_Map148_SHAPE_GRID:
	.incbin "games/tla/src/map/map_148/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map148_ANIMATION:
	.incbin "games/tla/src/map/map_148/animation.script.lz"
	.balign 4, 0
.LField_Map148_BLEND:
	.incbin "games/tla/src/map/map_148/blend.script.lz"
	.balign 4, 0
.LField_Map148_SCENE:
	.incbin "games/tla/src/map/map_148/scene.parts.lz"
	.balign 4, 0
	.global Tileset_Set71Palette
Tileset_Set71Palette:
	.incbin "games/tla/src/graphics/tileset/set_71/palette.gbapal.lz"
	.byte 0x55, 0x71, 0xd4	@ PACKER: left in the buffer by Field_Map148+0x161
	.global Tileset_Set71TilesA
Tileset_Set71TilesA:
	.incbin "games/tla/src/graphics/tileset/set_71/tiles_a.4bpp.mtf"
	.byte 0xd1, 0x92, 0x40	@ PACKER: left in the buffer by Field_Map148+0x26dd
	.global Tileset_Set71TilesB
Tileset_Set71TilesB:
	.incbin "games/tla/src/graphics/tileset/set_71/tiles_b.4bpp.mtf"
	.global Tileset_Set71TilesC
Tileset_Set71TilesC:
	.incbin "games/tla/src/graphics/tileset/set_71/tiles_c.4bpp.mtf"
	.byte 0x7c, 0x3c, 0x07	@ PACKER: left in the buffer by Tileset_Set71TilesB+0x118d
	.global Tileset_Set71TilesD
Tileset_Set71TilesD:
	.incbin "games/tla/src/graphics/tileset/set_71/tiles_d.4bpp.mtf"
	.byte 0x8c, 0x03	@ PACKER: left in the buffer by Tileset_Set71TilesB+0x1572
