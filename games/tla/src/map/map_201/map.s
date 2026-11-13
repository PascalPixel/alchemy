	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map201
Field_Map201:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map201, .LField_Map201_CELL_PICTURES
	map_block Field_Map201, .LField_Map201_SHAPES
	map_block Field_Map201, .LField_Map201_GRID
	map_block Field_Map201, .LField_Map201_SHAPE_GRID
	map_block Field_Map201, .LField_Map201_ANIMATION
	map_block Field_Map201, .LField_Map201_BLEND
	map_block Field_Map201, .LField_Map201_SCENE
.LField_Map201_CELL_PICTURES:
	.incbin "games/tla/src/map/map_201/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map201_SHAPES:
	.incbin "games/tla/src/map/map_072/shapes.table.lz"
	.balign 4, 0
.LField_Map201_GRID:
	.incbin "games/tla/src/map/map_201/map.grid.lz"
	.balign 4, 0
.LField_Map201_SHAPE_GRID:
	.incbin "games/tla/src/map/map_201/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map201_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map201_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map201_SCENE:
	.incbin "games/tla/src/map/map_201/scene.parts.lz"
	.balign 4, 0
	.global Tileset_Set87Palette
Tileset_Set87Palette:
	.incbin "games/tla/src/graphics/tileset/set_87/palette.gbapal.lz"
	.byte 0x09, 0x12, 0xe0	@ PACKER: left in the buffer by Field_Map201+0x14d
	.global Tileset_Set87TilesA
Tileset_Set87TilesA:
	.incbin "games/tla/src/graphics/tileset/set_87/tiles_a.4bpp.mtf"
	.global Tileset_Set87TilesB
Tileset_Set87TilesB:
	.incbin "games/tla/src/graphics/tileset/set_87/tiles_b.4bpp.mtf"
	.byte 0x2b	@ PACKER: left in the buffer by Tileset_Set87TilesA+0x1bcf
	.global Tileset_Set87TilesC
Tileset_Set87TilesC:
	.incbin "games/tla/src/graphics/tileset/set_87/tiles_c.4bpp.mtf"
	.global Tileset_Set87TilesD
Tileset_Set87TilesD:
	.incbin "games/tla/src/graphics/tileset/set_87/tiles_d.4bpp.mtf"
	.byte 0x38, 0x5a, 0x61	@ PACKER: left in the buffer by Tileset_Set87TilesC+0xb29
