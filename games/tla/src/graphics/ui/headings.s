@ Eighteen headings of 26x5 tiles of 16 colours, drawn in colours 0 and 1,
@ each packed on its own as a palette-LZ stream after a table of their
@ offsets. The first seven carry a title in Japanese over a rule, with the
@ title in roman letters under it: Gimikku Test, Shinden, Rounge, Markyury,
@ Takara Jima, Takara Doukutsu and Kaizoku Ship; the other eleven are the
@ rule alone. No loader of the resource was found, so the palette is a
@ reconstruction choice: the pictures carry the window palette
@ (gUi_WindowPalette), white on window blue.
	.section .rodata
	.global Ui_Headings
Ui_Headings:
	.4byte .LUi_Heading00 - Ui_Headings
	.4byte .LUi_Heading01 - Ui_Headings
	.4byte .LUi_Heading02 - Ui_Headings
	.4byte .LUi_Heading03 - Ui_Headings
	.4byte .LUi_Heading04 - Ui_Headings
	.4byte .LUi_Heading05 - Ui_Headings
	.4byte .LUi_Heading06 - Ui_Headings
	.4byte .LUi_Heading07 - Ui_Headings
	.4byte .LUi_Heading08 - Ui_Headings
	.4byte .LUi_Heading09 - Ui_Headings
	.4byte .LUi_Heading10 - Ui_Headings
	.4byte .LUi_Heading11 - Ui_Headings
	.4byte .LUi_Heading12 - Ui_Headings
	.4byte .LUi_Heading13 - Ui_Headings
	.4byte .LUi_Heading14 - Ui_Headings
	.4byte .LUi_Heading15 - Ui_Headings
	.4byte .LUi_Heading16 - Ui_Headings
	.4byte .LUi_Heading17 - Ui_Headings
.LUi_Heading00:
	.incbin "games/tla/src/graphics/ui/headings/heading_00.4bpp.plz"
	.balign 4, 0
.LUi_Heading01:
	.incbin "games/tla/src/graphics/ui/headings/heading_01.4bpp.plz"
	.balign 4, 0
.LUi_Heading02:
	.incbin "games/tla/src/graphics/ui/headings/heading_02.4bpp.plz"
	.balign 4, 0
.LUi_Heading03:
	.incbin "games/tla/src/graphics/ui/headings/heading_03.4bpp.plz"
	.balign 4, 0
.LUi_Heading04:
	.incbin "games/tla/src/graphics/ui/headings/heading_04.4bpp.plz"
	.balign 4, 0
.LUi_Heading05:
	.incbin "games/tla/src/graphics/ui/headings/heading_05.4bpp.plz"
	.balign 4, 0
.LUi_Heading06:
	.incbin "games/tla/src/graphics/ui/headings/heading_06.4bpp.plz"
	.balign 4, 0
.LUi_Heading07:
	.incbin "games/tla/src/graphics/ui/headings/heading_blank.4bpp.plz"
.LUi_Heading08:
	.incbin "games/tla/src/graphics/ui/headings/heading_blank.4bpp.plz"
.LUi_Heading09:
	.incbin "games/tla/src/graphics/ui/headings/heading_blank.4bpp.plz"
.LUi_Heading10:
	.incbin "games/tla/src/graphics/ui/headings/heading_blank.4bpp.plz"
.LUi_Heading11:
	.incbin "games/tla/src/graphics/ui/headings/heading_blank.4bpp.plz"
.LUi_Heading12:
	.incbin "games/tla/src/graphics/ui/headings/heading_blank.4bpp.plz"
.LUi_Heading13:
	.incbin "games/tla/src/graphics/ui/headings/heading_blank.4bpp.plz"
.LUi_Heading14:
	.incbin "games/tla/src/graphics/ui/headings/heading_blank.4bpp.plz"
.LUi_Heading15:
	.incbin "games/tla/src/graphics/ui/headings/heading_blank.4bpp.plz"
.LUi_Heading16:
	.incbin "games/tla/src/graphics/ui/headings/heading_blank.4bpp.plz"
.LUi_Heading17:
	.incbin "games/tla/src/graphics/ui/headings/heading_blank.4bpp.plz"
