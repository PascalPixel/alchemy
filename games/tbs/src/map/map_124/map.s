	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map124
Field_Map124:
	map_header 0, 0, 70, 34, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map124, .LField_Map124_METATILES
	map_block Field_Map124, .LField_Map124_SHAPES
	map_block Field_Map124, .LField_Map124_GRID
	map_block Field_Map124, .LField_Map124_ANIMATION
	map_block Field_Map124, .LField_Map124_BLEND
	map_block Field_Map124, .LField_Map124_MARKERS
.LField_Map124_METATILES:
	.incbin "games/tbs/src/map/map_124/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map124_SHAPES:
	.incbin "games/common/src/map/map_001/shapes.table.lz"
	.balign 4, 0
.LField_Map124_GRID:
	.incbin "games/tbs/src/map/map_124/grid.parts.lz"
	.balign 4, 0
.LField_Map124_ANIMATION:
	.incbin "games/tbs/src/map/map_124/animation.script.lz"
	.balign 4, 0
.LField_Map124_BLEND:
	.incbin "games/tbs/src/map/map_124/blend.script.lz"
	.balign 4, 0
.LField_Map124_MARKERS:
	.incbin "games/tbs/src/map/map_000/markers.table"
	.balign 4, 0
	.global Tileset_Set59Palette
Tileset_Set59Palette:
	.incbin "games/tbs/src/graphics/tileset/set_59/palette.gbapal.lz"
	.byte 0x8e, 0x89	@ PACKER: left in the buffer by Field_Map124+0x166
	.global Tileset_Set59TilesA
Tileset_Set59TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_59/tiles_a.4bpp.mtf"
	.byte 0xe0	@ PACKER: left in the buffer by Field_Map123+0x2a13
	.global Tileset_Set59TilesB
Tileset_Set59TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_59/tiles_b.4bpp.mtf"
	.byte 0x8c, 0xba	@ PACKER: left in the buffer by Tileset_Set59TilesA+0x1c3e
	.global Tileset_Set59TilesC
Tileset_Set59TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_59/tiles_c.4bpp.mtf"
	.byte 0xc6	@ PACKER: left in the buffer by Tileset_Set59TilesB+0x4bb
	.global Tileset_Set59TilesD
Tileset_Set59TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_59/tiles_d.4bpp.mtf"
	.byte 0x58	@ PACKER: left in the buffer by Tileset_Set59TilesB+0x1377
