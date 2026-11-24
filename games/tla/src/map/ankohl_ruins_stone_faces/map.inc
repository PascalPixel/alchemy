	.global Field_AnkohlRuinsStoneFaces
Field_AnkohlRuinsStoneFaces:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AnkohlRuinsStoneFaces, .LField_AnkohlRuinsStoneFaces_CELL_TILES
	map_block Field_AnkohlRuinsStoneFaces, .LField_AnkohlRuinsStoneFaces_SHAPES
	map_block Field_AnkohlRuinsStoneFaces, .LField_AnkohlRuinsStoneFaces_GRID
	map_block Field_AnkohlRuinsStoneFaces, .LField_AnkohlRuinsStoneFaces_SHAPE_GRID
	map_block Field_AnkohlRuinsStoneFaces, .LField_AnkohlRuinsStoneFaces_ANIMATION
	map_block Field_AnkohlRuinsStoneFaces, .LField_AnkohlRuinsStoneFaces_BLEND
	map_block Field_AnkohlRuinsStoneFaces, .LField_AnkohlRuinsStoneFaces_SCENE
.LField_AnkohlRuinsStoneFaces_CELL_TILES:
	.incbin "games/tla/src/map/ankohl_ruins_stone_faces/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_AnkohlRuinsStoneFaces_SHAPES:
	.incbin "games/tla/src/map/ankohl_ruins_stone_faces/shapes.table.lz"
	.balign 4, 0
.LField_AnkohlRuinsStoneFaces_GRID:
	.incbin "games/tla/src/map/ankohl_ruins_stone_faces/map.grid.lz"
	.balign 4, 0
.LField_AnkohlRuinsStoneFaces_SHAPE_GRID:
	.incbin "games/tla/src/map/ankohl_ruins_stone_faces/shape_grid.plane.lz"
	.balign 4, 0
.LField_AnkohlRuinsStoneFaces_ANIMATION:
	.incbin "games/tla/src/map/ankohl_ruins_stone_faces/animation.script.lz"
	.balign 4, 0
.LField_AnkohlRuinsStoneFaces_BLEND:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_ladders/blend.script.lz"
	.balign 4, 0
.LField_AnkohlRuinsStoneFaces_SCENE:
	.incbin "games/tla/src/map/ankohl_ruins_stone_faces/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteAnkohlRuins1
Tileset_PaletteAnkohlRuins1:
	.incbin "games/tla/src/graphics/tileset/palettes/ankohl_ruins_stone_faces.gbapal.lz"
	.byte 0x85, 0x1e	@ PACKER: left in the buffer by Field_AnkohlRuinsStoneFaces+0x12a
