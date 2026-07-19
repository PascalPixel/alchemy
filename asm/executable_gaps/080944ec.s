.syntax unified
.text
	.thumb
	.global Fragment_080944ec
	.thumb_func
Fragment_080944ec:
	ldr	r3, [pc, #60]
	movs	r2, #240
	ldr	r0, [r3, #0]
	lsls	r2, r2, #4
	adds	r3, r0, r2
	ldrb	r2, [r3, #0]
	lsls	r3, r2, #4
	subs	r3, r3, r2
	lsls	r3, r3, #7
	adds	r0, r0, r3
	ldr	r3, [pc, #44]
	ldr	r2, [pc, #48]
	ldrh	r4, [r3, #10]
	ands	r2, r4
	strh	r2, [r3, #10]
	ldr	r2, [pc, #44]
	ldrh	r4, [r3, #10]
	ands	r2, r4
	strh	r2, [r3, #10]
	ldr	r1, [pc, #40]
	ldrh	r2, [r3, #10]
	ldmia	r0!, {r2}
	str	r2, [r1, #0]
	ldmia	r0!, {r2}
	str	r2, [r1, #0]
	ldmia	r0!, {r2}
	str	r2, [r1, #0]
	ldr	r2, [pc, #28]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	bx	lr
	.inst.n 0x0000
	.4byte 0x03001ed8
	.4byte 0x040000b0
	.4byte 0x0000c5ff
	.4byte 0x00007fff
	.4byte 0x04000014
	.4byte 0xa6600003
