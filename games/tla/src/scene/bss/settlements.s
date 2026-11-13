.syntax unified
	.thumb
	.section .bss,"aw",%nobits
	.global gSettlements_WaterColors
gSettlements_WaterColors:
	.space 0x00000020
	.global gSettlements_WaterColorsSaved
gSettlements_WaterColorsSaved:
	.space 0x00000060
	.global gSettlements_WaterfallLine
gSettlements_WaterfallLine:
	.space 0x00000004
	.global gSettlements_WaterfallScrollAbove
gSettlements_WaterfallScrollAbove:
	.space 0x00000002
	.global gSettlements_WaterfallScrollBelow
gSettlements_WaterfallScrollBelow:
	.space 0x00000002
	.global gSettlements_SlideStopped
gSettlements_SlideStopped:
