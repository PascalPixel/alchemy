	.global Field_AnkohlRuins1
Field_AnkohlRuins1:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AnkohlRuins1, .LField_AnkohlRuins1_CELL_PICTURES
	map_block Field_AnkohlRuins1, .LField_AnkohlRuins1_SHAPES
	map_block Field_AnkohlRuins1, .LField_AnkohlRuins1_GRID
	map_block Field_AnkohlRuins1, .LField_AnkohlRuins1_SHAPE_GRID
	map_block Field_AnkohlRuins1, .LField_AnkohlRuins1_ANIMATION
	map_block Field_AnkohlRuins1, .LField_AnkohlRuins1_BLEND
	map_block Field_AnkohlRuins1, .LField_AnkohlRuins1_SCENE
.LField_AnkohlRuins1_CELL_PICTURES:
	.incbin "games/tla/src/map/ankohl_ruins_1/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_AnkohlRuins1_SHAPES:
	.incbin "games/tla/src/map/ankohl_ruins_1/shapes.table.lz"
	.balign 4, 0
.LField_AnkohlRuins1_GRID:
	.incbin "games/tla/src/map/ankohl_ruins_1/map.grid.lz"
	.balign 4, 0
.LField_AnkohlRuins1_SHAPE_GRID:
	.incbin "games/tla/src/map/ankohl_ruins_1/shape_grid.plane.lz"
	.balign 4, 0
.LField_AnkohlRuins1_ANIMATION:
	.incbin "games/tla/src/map/ankohl_ruins_1/animation.script.lz"
	.balign 4, 0
.LField_AnkohlRuins1_BLEND:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_10/blend.script.lz"
	.balign 4, 0
.LField_AnkohlRuins1_SCENE:
	.incbin "games/tla/src/map/ankohl_ruins_1/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteAnkohlRuins1
Tileset_PaletteAnkohlRuins1:
	.incbin "games/tla/src/graphics/tileset/palettes/ankohl_ruins_1.gbapal.lz"
	.byte 0x85, 0x1e	@ PACKER: left in the buffer by Field_AnkohlRuins1+0x12a
