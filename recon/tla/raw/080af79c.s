.syntax unified
	.thumb
	.global Owner_GetRecordStride180
	.thumb_func
Owner_GetRecordStride180:
	movs r3, #180
	ldr r2, .L_080af7a8
	muls r0, r3
	adds r0, r0, r2
	bx lr
	.2byte 0x0000
.L_080af7a8:
	.4byte Data_080c0f4c
