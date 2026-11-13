	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_AltinPeakHydrosStatue
Field_AltinPeakHydrosStatue:
	map_header 0, 0, 100, 68, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 68, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_AltinPeakHydrosStatue, .LField_AltinPeakHydrosStatue_CELL_PICTURES
	map_offset Field_AltinPeakHydrosStatue, .LField_AltinPeakHydrosStatue_SHAPES
	map_offset Field_AltinPeakHydrosStatue, .LField_AltinPeakHydrosStatue_GRID
	map_offset Field_AltinPeakHydrosStatue, .LField_AltinPeakHydrosStatue_ANIMATION
	map_offset Field_AltinPeakHydrosStatue, .LField_AltinPeakHydrosStatue_BLEND
	map_offset Field_AltinPeakHydrosStatue, .LField_AltinPeakHydrosStatue_MARKERS
.LField_AltinPeakHydrosStatue_CELL_PICTURES:
	.incbin "games/tbs/src/map/altin_peak_hydros_statue/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_AltinPeakHydrosStatue_SHAPES:
	.incbin "games/tbs/src/map/altin_peak_hydros_statue/shapes.table.lz"
	.balign 4, 0
.LField_AltinPeakHydrosStatue_GRID:
	.incbin "games/tbs/src/map/altin_peak_hydros_statue/map.grid.lz"
	.balign 4, 0
.LField_AltinPeakHydrosStatue_ANIMATION:
	.incbin "games/tbs/src/map/altin_peak_hydros_statue/tileset_anims.script.lz"
	.balign 4, 0
.LField_AltinPeakHydrosStatue_BLEND:
	.incbin "games/tbs/src/map/altin_peak_hydros_statue/blend.script.lz"
	.balign 4, 0
.LField_AltinPeakHydrosStatue_MARKERS:
	treasure_spot 20, 4, 100
	treasure_spots_end
	.balign 4, 0
	.global Tileset_PaletteAltinPeakHydrosStatue
Tileset_PaletteAltinPeakHydrosStatue:
	.incbin "games/tbs/src/graphics/tileset/palettes/altin_peak_hydros_statue.gbapal.lz"
	.byte 0x2a, 0x02, 0x98	@ PACKER: left in the buffer by Field_AltinPeakHydrosStatue+0x15d
