	.global Field_GabombaCatacombsTeleporters
Field_GabombaCatacombsTeleporters:
	map_header 0, 0, 90, 100, 3, 3, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GabombaCatacombsTeleporters, .LField_GabombaCatacombsTeleporters_CELL_TILES
	map_block Field_GabombaCatacombsTeleporters, .LField_GabombaCatacombsTeleporters_SHAPES
	map_block Field_GabombaCatacombsTeleporters, .LField_GabombaCatacombsTeleporters_GRID
	map_block Field_GabombaCatacombsTeleporters, .LField_GabombaCatacombsTeleporters_SHAPE_GRID
	map_block Field_GabombaCatacombsTeleporters, .LField_GabombaCatacombsTeleporters_ANIMATION
	map_block Field_GabombaCatacombsTeleporters, .LField_GabombaCatacombsTeleporters_BLEND
	map_block Field_GabombaCatacombsTeleporters, .LField_GabombaCatacombsTeleporters_SCENE
.LField_GabombaCatacombsTeleporters_CELL_TILES:
	.incbin "games/tla/src/map/gabomba_catacombs_teleporters/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_GabombaCatacombsTeleporters_SHAPES:
	.incbin "games/tla/src/map/gabomba_catacombs_teleporters/shapes.table.lz"
	.balign 4, 0
.LField_GabombaCatacombsTeleporters_GRID:
	.incbin "games/tla/src/map/gabomba_catacombs_teleporters/map.grid.lz"
	.balign 4, 0
.LField_GabombaCatacombsTeleporters_SHAPE_GRID:
	.incbin "games/tla/src/map/gabomba_catacombs_teleporters/shape_grid.plane.lz"
	.balign 4, 0
.LField_GabombaCatacombsTeleporters_ANIMATION:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_sea_gods_tear/animation.script.lz"
	.balign 4, 0
.LField_GabombaCatacombsTeleporters_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_GabombaCatacombsTeleporters_SCENE:
	.incbin "games/tla/src/map/gabomba_catacombs_teleporters/scene.scene.lz"
	.balign 4, 0
	.global Tileset_GabombaCatacombsTeleportersPalette
Tileset_GabombaCatacombsTeleportersPalette:
	.incbin "games/tla/src/graphics/tileset/gabomba_catacombs_teleporters/palette.gbapal.lz"
	.byte 0xd5, 0x1b	@ PACKER: left in the buffer by Field_GabombaCatacombs1+0x1a2
	.global Tileset_GabombaCatacombsTeleportersTilesA
Tileset_GabombaCatacombsTeleportersTilesA:
	.incbin "games/tla/src/graphics/tileset/gabomba_catacombs_teleporters/tiles_a.4bpp.mtf"
	.byte 0x77, 0x46	@ PACKER: left in the buffer by Field_KibomboMountainsWaft+0x2ec2
	.global Tileset_GabombaCatacombsTeleportersTilesB
Tileset_GabombaCatacombsTeleportersTilesB:
	.incbin "games/tla/src/graphics/tileset/gabomba_catacombs_teleporters/tiles_b.4bpp.mtf"
	.byte 0x4d, 0xd0, 0x25	@ PACKER: left in the buffer by Tileset_GabombaCatacombsTeleportersTilesA+0x197d
	.global Tileset_GabombaCatacombsTeleportersTilesC
Tileset_GabombaCatacombsTeleportersTilesC:
	.incbin "games/tla/src/graphics/tileset/gabomba_catacombs_teleporters/tiles_c.4bpp.mtf"
	.byte 0x38, 0xa4	@ PACKER: left in the buffer by Tileset_GabombaCatacombsTeleportersTilesB+0x104a
	.global Tileset_GabombaCatacombsTeleportersTilesD
Tileset_GabombaCatacombsTeleportersTilesD:
	.incbin "games/tla/src/graphics/tileset/gabomba_catacombs_teleporters/tiles_d.4bpp.mtf"
