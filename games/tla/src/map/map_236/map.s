	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map236
Field_Map236:
	map_header 0, 0, 128, 64, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 8, 8, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map236, .LField_Map236_CELL_PICTURES
	map_block Field_Map236, .LField_Map236_SHAPES
	map_block Field_Map236, .LField_Map236_GRID
	map_block Field_Map236, .LField_Map236_SHAPE_GRID
	map_block Field_Map236, .LField_Map236_ANIMATION
	map_block Field_Map236, .LField_Map236_BLEND
	map_block Field_Map236, .LField_Map236_SCENE
.LField_Map236_CELL_PICTURES:
	.incbin "games/tla/src/map/map_236/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map236_SHAPES:
	.incbin "games/tla/src/map/map_236/shapes.table.lz"
	.balign 4, 0
.LField_Map236_GRID:
	.incbin "games/tla/src/map/map_236/map.grid.lz"
	.balign 4, 0
.LField_Map236_SHAPE_GRID:
	.incbin "games/tla/src/map/map_236/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map236_ANIMATION:
	.incbin "games/tla/src/map/map_235/animation.script.lz"
	.balign 4, 0
.LField_Map236_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map236_SCENE:
	.incbin "games/tla/src/map/map_236/scene.parts.lz"
	.balign 4, 0
	.global Tileset_Set102Palette
Tileset_Set102Palette:
	.incbin "games/tla/src/graphics/tileset/set_102/palette.gbapal.lz"
	.byte 0xd4, 0x99, 0xc0	@ PACKER: left in the buffer by Field_Map236+0x17d
	.global Tileset_Set102TilesA
Tileset_Set102TilesA:
	.incbin "games/tla/src/graphics/tileset/set_102/tiles_a.4bpp.mtf"
	.global Tileset_Set102TilesB
Tileset_Set102TilesB:
	.incbin "games/tla/src/graphics/tileset/set_102/tiles_b.4bpp.mtf"
	.byte 0x46	@ PACKER: left in the buffer by Tileset_Set101TilesA+0x1edb
	.global Tileset_Set102TilesC
Tileset_Set102TilesC:
	.incbin "games/tla/src/graphics/tileset/set_102/tiles_c.4bpp.mtf"
	.byte 0x0b, 0x24	@ PACKER: left in the buffer by Tileset_Set102TilesB+0x1632
	.global Tileset_Set102TilesD
Tileset_Set102TilesD:
	.incbin "games/tla/src/graphics/tileset/set_102/tiles_d.4bpp.mtf"
	.byte 0x24	@ PACKER: left in the buffer by Tileset_Set102TilesC+0x13f
