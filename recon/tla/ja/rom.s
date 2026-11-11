	.section .rom.000423b8, "ax"
	.balign 4
	.global Text_DrawPrefixedNumberAtOffset
	.type Text_DrawPrefixedNumberAtOffset, %function
	.thumb_func
Text_DrawPrefixedNumberAtOffset:
	.incbin "baserom.gba", 0x000423b8, 0x000000a0
