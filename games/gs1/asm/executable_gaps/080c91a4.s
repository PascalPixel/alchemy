.syntax unified
.text
	.thumb
	.global Fragment_080c91a4
	.thumb_func
Fragment_080c91a4:
	ldr	r3, [pc, #28]
	ldr	r2, [pc, #32]
	ldrh	r1, [r3, #10]
	ands	r2, r1
	strh	r2, [r3, #10]
	ldr	r2, [pc, #28]
	ldrh	r1, [r3, #10]
	ands	r2, r1
	strh	r2, [r3, #10]
	ldr	r0, [pc, #24]
	ldrh	r2, [r3, #10]
	ldr	r1, [pc, #24]
	ldr	r2, [pc, #24]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	bx	lr
	.4byte 0x040000b0
	.4byte 0x0000c5ff
	.4byte 0x00007fff
	.4byte 0x02010000
	.4byte 0x04000040
	.4byte 0xa2600001
