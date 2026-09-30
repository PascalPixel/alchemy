.syntax unified
	.thumb
	.balign 4
	.global Func_081c117c
	.thumb_func
Func_081c117c:
	ldr	r3, [pc, #4]
	ldrh	r0, [r3, #0]
	bx	lr
	movs	r0, r0
	.2byte 0x5830
	.2byte 0x0200
	.global Func_081c1188
	.thumb_func
Func_081c1188:
	movs	r3, #7
	ands	r3, r0
	movs	r2, #1
	lsls	r2, r3
	movs	r3, #192
	lsls	r3, r3, #2
	adds	r3, #255
	ands	r3, r0
	asrs	r0, r3, #3
	ldr	r3, [pc, #12]
	ldrb	r3, [r3, r0]
	ands	r3, r2
	negs	r0, r3
	orrs	r0, r3
	lsrs	r0, r0, #31
	bx	lr
	.4byte 0x02002f20