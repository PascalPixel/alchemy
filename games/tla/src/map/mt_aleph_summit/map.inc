	.global Field_MtAlephSummit
Field_MtAlephSummit:
	map_header 0, 0, 128, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 8, 8, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MtAlephSummit, .LField_MtAlephSummit_CELL_PICTURES
	map_block Field_MtAlephSummit, .LField_MtAlephSummit_SHAPES
	map_block Field_MtAlephSummit, .LField_MtAlephSummit_GRID
	map_block Field_MtAlephSummit, .LField_MtAlephSummit_SHAPE_GRID
	map_block Field_MtAlephSummit, .LField_MtAlephSummit_ANIMATION
	map_block Field_MtAlephSummit, .LField_MtAlephSummit_BLEND
	map_block Field_MtAlephSummit, .LField_MtAlephSummit_SCENE
.LField_MtAlephSummit_CELL_PICTURES:
	.incbin "games/tla/src/map/mt_aleph_summit/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_MtAlephSummit_SHAPES:
	.incbin "games/tla/src/map/map_179/shapes.table.lz"
	.balign 4, 0
.LField_MtAlephSummit_GRID:
	.incbin "games/tla/src/map/mt_aleph_summit/map.grid.lz"
	.balign 4, 0
.LField_MtAlephSummit_SHAPE_GRID:
	.incbin "games/tla/src/map/mt_aleph_summit/shape_grid.plane.lz"
	.balign 4, 0
.LField_MtAlephSummit_ANIMATION:
	.incbin "games/tla/src/map/mt_aleph_summit/animation.script.lz"
	.balign 4, 0
.LField_MtAlephSummit_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_MtAlephSummit_SCENE:
	.incbin "games/tla/src/map/map_236/scene.scene.lz"
	.balign 4, 0
	.global Tileset_MtAlephSummitPalette
Tileset_MtAlephSummitPalette:
	.incbin "games/tla/src/graphics/tileset/mt_aleph_summit/palette.gbapal.lz"
	.global Tileset_MtAlephSummitTilesA
Tileset_MtAlephSummitTilesA:
	.incbin "games/tla/src/graphics/tileset/mt_aleph_summit/tiles_a.4bpp.mtf"
	.byte 0x0e, 0x59, 0x80	@ PACKER: left in the buffer by Tileset_Set117TilesD+0x167d
	.global Tileset_MtAlephSummitTilesB
Tileset_MtAlephSummitTilesB:
	.incbin "games/tla/src/graphics/tileset/mt_aleph_summit/tiles_b.4bpp.mtf"
	.byte 0x0c, 0xea, 0x4c	@ PACKER: left in the buffer by Tileset_MtAlephSummitTilesA+0x115d
	.global Tileset_MtAlephSummitTilesC
Tileset_MtAlephSummitTilesC:
	.incbin "games/tla/src/graphics/tileset/mt_aleph_summit/tiles_c.4bpp.mtf"
	.byte 0x56	@ PACKER: left in the buffer by Tileset_MtAlephSummitTilesB+0x111b
	.global Tileset_MtAlephSummitTilesD
Tileset_MtAlephSummitTilesD:
	.incbin "games/tla/src/graphics/tileset/mt_aleph_summit/tiles_d.4bpp.mtf"
	.byte 0x3e, 0x7f	@ PACKER: left in the buffer by Tileset_MtAlephSummitTilesC+0x206
