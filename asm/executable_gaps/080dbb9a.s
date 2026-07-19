.syntax unified
.text
	.thumb
	.global Fragment_080dbb9a
	.thumb_func
Fragment_080dbb9a:
	movs	r0, r0
	ldr	r3, [pc, #36]
	ldr	r0, [r3, #0]
	ldr	r3, [pc, #36]
	ldr	r2, [pc, #40]
	ldrh	r1, [r3, #10]
	ands	r2, r1
	strh	r2, [r3, #10]
	ldr	r2, [pc, #36]
	ldrh	r1, [r3, #10]
	ands	r2, r1
	strh	r2, [r3, #10]
	ldrh	r2, [r3, #10]
	movs	r2, #211
	lsls	r2, r2, #7
	adds	r0, r0, r2
	ldr	r1, [pc, #24]
	ldr	r2, [pc, #24]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	bx	lr
	.4byte 0x03001eec
	.4byte 0x040000b0
	.4byte 0x0000c5ff
	.4byte 0x00007fff
	.4byte 0x04000028
	.4byte 0xa6600001
