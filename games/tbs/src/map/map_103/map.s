	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map103
Field_Map103:
	map_header 0, 0, 45, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, -25, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map103, .LField_Map103_METATILES
	map_block Field_Map103, .LField_Map103_SHAPES
	map_block Field_Map103, .LField_Map103_GRID
	map_block Field_Map103, .LField_Map103_ANIMATION
	map_block Field_Map103
	map_block Field_Map103, .LField_Map103_MARKERS
.LField_Map103_METATILES:
	.incbin "map/map_103/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map103_SHAPES:
	.incbin "map/map_103/shapes.table.lz"
	.balign 4, 0
.LField_Map103_GRID:
	.incbin "map/map_103/grid.parts.lz"
	.balign 4, 0
.LField_Map103_ANIMATION:
	.incbin "map/map_103/animation.script.lz"
	.balign 4, 0
.LField_Map103_MARKERS:
	.incbin "map/map_000/markers.table"
	.balign 4, 0
	.global Tileset_Set56Palette
Tileset_Set56Palette:
	.incbin "graphics/tileset/set_56/palette.gbapal.lz"
	.byte 0xc5, 0x3e, 0x03	@ PACKER: left in the buffer by Field_Map103+0x1d1
	.global Tileset_Set56TilesA
Tileset_Set56TilesA:
	.incbin "graphics/tileset/set_56/tiles_a.4bpp.mtf"
	.byte 0xd1, 0x43	@ PACKER: left in the buffer by Tileset_Set54TilesB+0x208e
	.global Tileset_Set56TilesB
Tileset_Set56TilesB:
	.incbin "graphics/tileset/set_56/tiles_b.4bpp.mtf"
	.byte 0x79, 0x3a	@ PACKER: left in the buffer by Tileset_Set54TilesB+0x20fe
	.global Tileset_Set56TilesC
Tileset_Set56TilesC:
	.incbin "graphics/tileset/set_56/tiles_c.4bpp.mtf"
	.global Tileset_Set56TilesD
Tileset_Set56TilesD:
	.incbin "graphics/tileset/set_56/tiles_d.4bpp.mtf"
	.byte 0x08, 0x7e	@ PACKER: left in the buffer by Tileset_Set56TilesC+0x99a
