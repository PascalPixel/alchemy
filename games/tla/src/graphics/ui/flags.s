@ Two 16x16 sprites of 16 colours, one under the other: a red flag numbered
@ 1 and a blue flag numbered 2, stored unpacked. No loader of the resource
@ was found; the picture reads in the first row of the shared OBJ palette
@ (black, red, blue and white in colours 1, 2, 8 and 15), which it carries.
	.section .rodata
	.global Ui_Flags
Ui_Flags:
	.incbin "games/tla/src/graphics/ui/flags.4bpp16x16"
