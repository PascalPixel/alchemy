.syntax unified
	.thumb
	.global Func_0815b24c
	.thumb_func
Func_0815b24c:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #12]
	movs r0, #0
	ldrb r3, [r2, #4]
	movs r1, #0
	b .L_0815b266
.L_0815b25c:
	adds r1, #1
	adds r2, #24
	cmp r1, #63
	bgt .L_0815b26c
	ldrb r3, [r2, #4]
.L_0815b266:
	cmp r3, #0
	bne .L_0815b25c
	adds r0, r2, #0
.L_0815b26c:
	ldr r2, .L_0815b288
	movs r3, #0
	strh r3, [r0]
	str r3, [r0, #12]
	str r3, [r0, #8]
	str r3, [r0, #16]
	movs r3, #1
	strb r2, [r0, #7]
	strb r2, [r0, #22]
	strb r2, [r0, #20]
	strb r3, [r0, #4]
	strb r2, [r0, #5]
	b .L_0815b28c
	.2byte 0x0000
.L_0815b288:
	.4byte 0x00000000
.L_0815b28c:
	pop {pc}
	.2byte 0x0000
