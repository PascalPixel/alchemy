	.global Field_Map072
Field_Map072:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map072, .LField_Map072_CELL_PICTURES
	map_block Field_Map072, .LField_Map072_SHAPES
	map_block Field_Map072, .LField_Map072_GRID
	map_block Field_Map072, .LField_Map072_SHAPE_GRID
	map_block Field_Map072, .LField_Map072_ANIMATION
	map_block Field_Map072
	map_block Field_Map072, .LField_Map072_SCENE
.LField_Map072_CELL_PICTURES:
	.incbin "games/tla/src/map/map_072/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map072_SHAPES:
	.incbin "games/tla/src/map/map_072/shapes.table.lz"
	.balign 4, 0
.LField_Map072_GRID:
	.incbin "games/tla/src/map/map_072/map.grid.lz"
	.balign 4, 0
.LField_Map072_SHAPE_GRID:
	.incbin "games/tla/src/map/map_072/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map072_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map072_SCENE:
	.incbin "games/tla/src/map/map_072/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Set36Palette
Tileset_Set36Palette:
	.incbin "games/tla/src/graphics/tileset/set_36/palette.gbapal.lz"
	.byte 0x1b	@ PACKER: left in the buffer by Field_Map072+0x14f
	.global Tileset_Set36TilesA
Tileset_Set36TilesA:
	.incbin "games/tla/src/graphics/tileset/set_36/tiles_a.4bpp.mtf"
	.byte 0x05, 0x08	@ PACKER: unused post-stream residue; Resource_DecodeType2 uses decoded RAM and output length; writer unknown
