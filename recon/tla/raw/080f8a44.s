.syntax unified
	.thumb
	.global Func_080f8a44
	.thumb_func
Func_080f8a44:
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r6, [r3, #0]
	ldr	r3, [pc, #80]
	ldr	r5, [r6, #20]
	ldr	r4, [r3, #0]
	ldr	r6, [r6, #16]
	ldr	r2, [pc, #76]
	movs	r3, #7
	lsrs	r4, r4, #1
	ands	r4, r3
	ldrb	r2, [r2, r4]
	ldrh	r3, [r6, #12]
	mov	ip, r6
	adds	r2, r2, r0
	ldr	r6, [pc, #48]
	lsls	r3, r3, #3
	adds	r2, r2, r3
	ldr	r3, [pc, #44]
	adds	r2, #8
	strh	r2, [r5, #6]
	ands	r2, r6
	ldrh	r0, [r5, #22]
	ands	r2, r3
	ldr	r3, [pc, #44]
	ands	r3, r0
	orrs	r3, r2
	strh	r3, [r5, #22]
	ldr	r3, [pc, #40]
	mov	r0, ip
	ldrb	r3, [r3, r4]
	ldrh	r2, [r0, #14]
	adds	r3, r3, r1
	lsls	r2, r2, #3
	adds	r3, r3, r2
	adds	r3, #8
	strh	r3, [r5, #8]
	ands	r3, r6
	strb	r3, [r5, #20]
	b.n	.L_080f8ab0
	.4byte 0x0000ffff
	.4byte 0x000001ff
	.4byte 0x0300122c
	.4byte 0x081059dc
	.4byte 0xfffffe00
	.2byte 0x59e5
	.2byte 0x0810
.L_080f8ab0:
	pop	{r5, r6, pc}
	.align 2, 0
