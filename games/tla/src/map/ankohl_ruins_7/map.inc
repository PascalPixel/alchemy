	.global Field_AnkohlRuins7
Field_AnkohlRuins7:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AnkohlRuins7, .LField_AnkohlRuins7_CELL_PICTURES
	map_block Field_AnkohlRuins7, .LField_AnkohlRuins7_SHAPES
	map_block Field_AnkohlRuins7, .LField_AnkohlRuins7_GRID
	map_block Field_AnkohlRuins7, .LField_AnkohlRuins7_SHAPE_GRID
	map_block Field_AnkohlRuins7, .LField_AnkohlRuins7_ANIMATION
	map_block Field_AnkohlRuins7
	map_block Field_AnkohlRuins7, .LField_AnkohlRuins7_SCENE
.LField_AnkohlRuins7_CELL_PICTURES:
	.incbin "games/tla/src/map/ankohl_ruins_7/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_AnkohlRuins7_SHAPES:
	.incbin "games/tla/src/map/ankohl_ruins_7/shapes.table.lz"
	.balign 4, 0
.LField_AnkohlRuins7_GRID:
	.incbin "games/tla/src/map/ankohl_ruins_7/map.grid.lz"
	.balign 4, 0
.LField_AnkohlRuins7_SHAPE_GRID:
	.incbin "games/tla/src/map/ankohl_ruins_7/shape_grid.plane.lz"
	.balign 4, 0
.LField_AnkohlRuins7_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_2/animation.script.lz"
	.balign 4, 0
.LField_AnkohlRuins7_SCENE:
	.incbin "games/tla/src/map/ankohl_ruins_7/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteAnkohlRuins7A
Tileset_PaletteAnkohlRuins7A:
	.incbin "games/tla/src/graphics/tileset/palettes/ankohl_ruins_7_a.gbapal.lz"
	.byte 0x58, 0xf9	@ PACKER: left in the buffer by Field_AnkohlRuins7+0x102
	.global Tileset_PaletteAnkohlRuins7B
Tileset_PaletteAnkohlRuins7B:
	.incbin "games/tla/src/graphics/tileset/palettes/ankohl_ruins_7_b.gbapal.lz"
	.byte 0x05, 0x08	@ PACKER: unused post-stream residue; Decompress_TaggedLz uses decoded RAM and output length; writer unknown
	.global Tileset_PaletteAnkohlRuins7C
Tileset_PaletteAnkohlRuins7C:
	.incbin "games/tla/src/graphics/tileset/palettes/ankohl_ruins_7_c.gbapal.lz"
