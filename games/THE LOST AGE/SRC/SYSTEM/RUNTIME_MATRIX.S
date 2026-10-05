.syntax unified
	.section .data,"aw",%progbits
	.arm
	.global gTransform
gTransform:
	.4byte	0x00010000
	.4byte	0x00000000
	.4byte	0x00000000
	.4byte	0
	.4byte	0x00010000
	.4byte	0
	.4byte	0
	.4byte	0
	.4byte	0x00010000
	.global gTransformTranslation
gTransformTranslation:
	.4byte	0x00000000
	.4byte	0x00000000
	.4byte	0x00000000
