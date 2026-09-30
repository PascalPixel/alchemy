.syntax unified
	.thumb
	.global Random16
	.thumb_func
Random16:
	ldr r1, .L_08014894
	ldr r3, .L_08014898
	ldr r2, [r1]
	adds r0, r2, #0
	muls r0, r3
	movs r3, #192
	lsls r3, r3, #6
	adds r3, #57
	adds r0, r0, r3
	str r0, [r1]
	lsls r0, r0, #8
	lsrs r0, r0, #16
	bx lr
	.2byte 0x0000
.L_08014894:
	.4byte Data_030011bc
.L_08014898:
	.4byte 0x41c64e6d
