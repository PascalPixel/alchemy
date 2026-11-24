	.global Field_AnkohlRuinsEntrance
Field_AnkohlRuinsEntrance:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AnkohlRuinsEntrance, .LField_AnkohlRuinsEntrance_CELL_TILES
	map_block Field_AnkohlRuinsEntrance, .LField_AnkohlRuinsEntrance_SHAPES
	map_block Field_AnkohlRuinsEntrance, .LField_AnkohlRuinsEntrance_GRID
	map_block Field_AnkohlRuinsEntrance, .LField_AnkohlRuinsEntrance_SHAPE_GRID
	map_block Field_AnkohlRuinsEntrance, .LField_AnkohlRuinsEntrance_ANIMATION
	map_block Field_AnkohlRuinsEntrance
	map_block Field_AnkohlRuinsEntrance, .LField_AnkohlRuinsEntrance_SCENE
.LField_AnkohlRuinsEntrance_CELL_TILES:
	.incbin "games/tla/src/map/ankohl_ruins_entrance/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_AnkohlRuinsEntrance_SHAPES:
	.incbin "games/tla/src/map/ankohl_ruins_entrance/shapes.table.lz"
	.balign 4, 0
.LField_AnkohlRuinsEntrance_GRID:
	.incbin "games/tla/src/map/ankohl_ruins_entrance/map.grid.lz"
	.balign 4, 0
.LField_AnkohlRuinsEntrance_SHAPE_GRID:
	.incbin "games/tla/src/map/ankohl_ruins_entrance/shape_grid.plane.lz"
	.balign 4, 0
.LField_AnkohlRuinsEntrance_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_AnkohlRuinsEntrance_SCENE:
	.incbin "games/tla/src/map/ankohl_ruins_entrance/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteAnkohlRuinsEntrance
Tileset_PaletteAnkohlRuinsEntrance:
	.incbin "games/tla/src/graphics/tileset/palettes/ankohl_ruins_entrance.gbapal.lz"
	.byte 0x58, 0xf9	@ PACKER: left in the buffer by Field_AnkohlRuinsEntrance+0x102
	.global Tileset_PaletteShrineOfTheSeaGodRightProng
Tileset_PaletteShrineOfTheSeaGodRightProng:
	.incbin "games/tla/src/graphics/tileset/palettes/shrine_of_the_sea_god_right_prong.gbapal.lz"
	.byte 0x05, 0x08	@ PACKER: unused post-stream residue; Decompress_TaggedLz uses decoded RAM and output length; writer unknown
	.global Tileset_PaletteLemurianShipTiller
Tileset_PaletteLemurianShipTiller:
	.incbin "games/tla/src/graphics/tileset/palettes/lemurian_ship_tiller.gbapal.lz"
