	.global Field_GondowanSettlement
Field_GondowanSettlement:
	map_header 0, 0, 60, 60, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 60, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GondowanSettlement, .LField_GondowanSettlement_CELL_TILES
	map_block Field_GondowanSettlement, .LField_GondowanSettlement_SHAPES
	map_block Field_GondowanSettlement, .LField_GondowanSettlement_GRID
	map_block Field_GondowanSettlement, .LField_GondowanSettlement_SHAPE_GRID
	map_block Field_GondowanSettlement, .LField_GondowanSettlement_ANIMATION
	map_block Field_GondowanSettlement, .LField_GondowanSettlement_BLEND
	map_block Field_GondowanSettlement, .LField_GondowanSettlement_SCENE
.LField_GondowanSettlement_CELL_TILES:
	.incbin "games/tla/src/map/gondowan_settlement/cell_tiles.delta0.lz"
	.balign 4, 0
.LField_GondowanSettlement_SHAPES:
	.incbin "games/tla/src/map/gondowan_settlement/shapes.table.lz"
	.balign 4, 0
.LField_GondowanSettlement_GRID:
	.incbin "games/tla/src/map/gondowan_settlement/map.grid.lz"
	.balign 4, 0
.LField_GondowanSettlement_SHAPE_GRID:
	.incbin "games/tla/src/map/gondowan_settlement/shape_grid.plane.lz"
	.balign 4, 0
.LField_GondowanSettlement_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_GondowanSettlement_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_GondowanSettlement_SCENE:
	.incbin "games/tla/src/map/gondowan_settlement/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteSettlements1
Tileset_PaletteSettlements1:
	.incbin "games/tla/src/graphics/tileset/palettes/gondowan_settlement.gbapal.lz"
	.byte 0x38, 0x10	@ PACKER: left in the buffer by Field_GondowanSettlement+0x14e
