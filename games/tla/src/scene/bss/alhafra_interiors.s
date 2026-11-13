.syntax unified
	.thumb
	.section .bss,"aw",%nobits
	.balign 16
	.global gAlhafraInteriors_SecondGuestSprite
gAlhafraInteriors_SecondGuestSprite:
	.space 0x00000030
	.global gAlhafraInteriors_FirstGuestSprite
gAlhafraInteriors_FirstGuestSprite:
