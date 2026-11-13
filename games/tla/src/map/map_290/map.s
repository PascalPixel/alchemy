	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map290
Field_Map290:
	map_header 0, 0, 100, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map290, .LField_Map290_CELL_PICTURES
	map_block Field_Map290, .LField_Map290_SHAPES
	map_block Field_Map290, .LField_Map290_GRID
	map_block Field_Map290, .LField_Map290_SHAPE_GRID
	map_block Field_Map290, .LField_Map290_ANIMATION
	map_block Field_Map290, .LField_Map290_BLEND
	map_block Field_Map290, .LField_Map290_SCENE
.LField_Map290_CELL_PICTURES:
	.incbin "games/tla/src/map/map_290/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map290_SHAPES:
	.incbin "games/tla/src/map/map_290/shapes.table.lz"
	.balign 4, 0
.LField_Map290_GRID:
	.incbin "games/tla/src/map/map_290/map.grid.lz"
	.balign 4, 0
.LField_Map290_SHAPE_GRID:
	.incbin "games/tla/src/map/map_290/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map290_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map290_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map290_SCENE:
	.incbin "games/tla/src/map/map_290/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Set119Palette
Tileset_Set119Palette:
	.incbin "games/tla/src/graphics/tileset/set_119/palette.gbapal.lz"
	.global Tileset_Set119TilesA
Tileset_Set119TilesA:
	.incbin "games/tla/src/graphics/tileset/set_119/tiles_a.4bpp.mtf"
	.byte 0x5a, 0x33, 0xa4	@ PACKER: left in the buffer by Tileset_Set117TilesA+0x2651
	.global Tileset_Set119TilesB
Tileset_Set119TilesB:
	.incbin "games/tla/src/graphics/tileset/set_119/tiles_b.4bpp.mtf"
	.byte 0x0f, 0xd8	@ PACKER: left in the buffer by Tileset_Set119TilesA+0x1772
	.global Tileset_Set119TilesC
Tileset_Set119TilesC:
	.incbin "games/tla/src/graphics/tileset/set_119/tiles_c.4bpp.mtf"
	.byte 0x06	@ PACKER: left in the buffer by Tileset_Set119TilesB+0x12bb
	.global Tileset_Set119TilesD
Tileset_Set119TilesD:
	.incbin "games/tla/src/graphics/tileset/set_119/tiles_d.4bpp.mtf"
	.byte 0xe5, 0x3d	@ PACKER: left in the buffer by Tileset_Set119TilesC+0x842
