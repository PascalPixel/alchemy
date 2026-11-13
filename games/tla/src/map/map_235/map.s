	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map235
Field_Map235:
	map_header 0, 0, 128, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map235, .LField_Map235_CELL_PICTURES
	map_block Field_Map235, .LField_Map235_SHAPES
	map_block Field_Map235, .LField_Map235_GRID
	map_block Field_Map235, .LField_Map235_SHAPE_GRID
	map_block Field_Map235, .LField_Map235_ANIMATION
	map_block Field_Map235, .LField_Map235_BLEND
	map_block Field_Map235, .LField_Map235_SCENE
.LField_Map235_CELL_PICTURES:
	.incbin "games/tla/src/map/map_235/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Map235_SHAPES:
	.incbin "games/tla/src/map/map_235/shapes.table.lz"
	.balign 4, 0
.LField_Map235_GRID:
	.incbin "games/tla/src/map/map_235/grid.parts.lz"
	.balign 4, 0
.LField_Map235_SHAPE_GRID:
	.incbin "games/tla/src/map/map_235/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map235_ANIMATION:
	.incbin "games/tla/src/map/map_235/animation.script.lz"
	.balign 4, 0
.LField_Map235_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map235_SCENE:
	.incbin "games/tla/src/map/map_235/scene.parts.lz"
	.global Tileset_Set101Palette
Tileset_Set101Palette:
	.incbin "games/tla/src/graphics/tileset/set_101/palette.gbapal.lz"
	.global Tileset_Set101TilesA
Tileset_Set101TilesA:
	.incbin "games/tla/src/graphics/tileset/set_101/tiles_a.4bpp.mtf"
	.byte 0x5b	@ PACKER: left in the buffer by Tileset_Set100TilesA+0x1fd7
	.global Tileset_Set101TilesB
Tileset_Set101TilesB:
	.incbin "games/tla/src/graphics/tileset/set_101/tiles_b.4bpp.mtf"
	.byte 0x16	@ PACKER: left in the buffer by Tileset_Set101TilesA+0x107f
	.global Tileset_Set101TilesC
Tileset_Set101TilesC:
	.incbin "games/tla/src/graphics/tileset/set_101/tiles_c.4bpp.mtf"
	.byte 0x10, 0x00, 0x00	@ PACKER: unused post-stream residue; Resource_DecodeType2 uses decoded RAM and output length; writer unknown
	.global Tileset_Set101TilesD
Tileset_Set101TilesD:
	.incbin "games/tla/src/graphics/tileset/set_101/tiles_d.4bpp.mtf"
	.byte 0x02, 0x09	@ PACKER: left in the buffer by Tileset_Set101TilesC+0xb6e
