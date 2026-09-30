.syntax unified
	.thumb
	.global Func_08046260
	.thumb_func
Func_08046260:
	push {r5, lr}
	adds r5, r0, #0
	adds r0, r1, #0
	bl Text_FormatSignedDecimalToWork
	ldr r1, .L_08046280
	movs r2, #13
.L_0804626e:
	ldrb r3, [r1]
	subs r2, #1
	strh r3, [r5]
	adds r1, #1
	adds r5, #2
	cmp r2, #0
	bge .L_0804626e
	pop {r5, pc}
	.2byte 0x0000
.L_08046280:
	.4byte gNumberTextBuffer
