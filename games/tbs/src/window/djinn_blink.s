@ The Djinn menu's blinking tile, 4 bits a pixel.
	.section .rodata
	.balign 4
	.global gDjinnMenu_BlinkTile
gDjinnMenu_BlinkTile:
	.incbin "games/tbs/src/window/djinn_blink.4bpp"
