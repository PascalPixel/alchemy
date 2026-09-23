.syntax unified
	.thumb
	.set sub_08002f10, 0x08002f10
	.set sub_0800300c, 0x0800300c
	.set sub_0800307c, 0x0800307c
	.set sub_080030f8, 0x080030f8
	.set sub_0800403c, 0x0800403c
	.set sub_080040e8, 0x080040e8
	.set sub_08004760, 0x08004760
	.set sub_0800479c, 0x0800479c
	.set sub_08004858, 0x08004858
	.set sub_0808a008, 0x0808a008
	.set sub_080f9008, 0x080f9008
	.global Func_08002e00
	.thumb_func
Func_08002e00:
	push	{r5, lr}
	ldr	r2, [pc, #144]
	ldr	r3, [pc, #144]
	ldrh	r1, [r2, #10]
	ands	r3, r1
	strh	r3, [r2, #10]
	ldr	r3, [pc, #140]
	ldrh	r1, [r2, #10]
	ands	r3, r1
	strh	r3, [r2, #10]
	ldrh	r3, [r2, #10]
	sub	sp, #4
	ldr	r2, [pc, #132]
	ldr	r3, [pc, #136]
	strh	r2, [r3, #0]
	mov	r0, sp
	movs	r5, #0
	movs	r1, #192
	str	r5, [r0, #0]
	ldr	r3, [pc, #128]
	lsls	r1, r1, #18
	ldr	r2, [pc, #128]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	bl	sub_08004858
	bl	sub_0800300c
	ldr	r3, [pc, #116]
	str	r5, [r3, #0]
	ldr	r3, [pc, #116]
	strb	r5, [r3, #0]
	ldr	r3, [pc, #116]
	strb	r5, [r3, #0]
	ldr	r3, [pc, #116]
	strb	r5, [r3, #0]
	bl	sub_08002f10
	bl	sub_0800479c
	bl	sub_08004760
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	movs	r0, #0
	ldr	r2, [pc, #96]
	movs	r1, #1
	bl	sub_0800307c
	ldr	r2, [pc, #92]
	ldr	r3, [pc, #92]
	strh	r2, [r3, #0]
	bl	sub_080f9008
	bl	sub_0800403c
	bl	sub_080040e8
	ldr	r3, [pc, #80]
	ldr	r2, [pc, #84]
	str	r5, [r3, #0]
	movs	r3, #1
	strb	r3, [r2, #0]
	ldr	r3, [pc, #80]
	movs	r0, #10
	strb	r5, [r3, #0]
	bl	sub_080030f8
	movs	r0, #0
	b.n	.L_08002ed8
	.4byte 0x00000140
	.4byte 0x040000b0
	.4byte 0x0000c5ff
	.4byte 0x00007fff
	.4byte 0x00004014
	.4byte 0x04000204
	.4byte 0x040000d4
	.4byte 0x85001e00
	.4byte 0x02002090
	.4byte 0x03001ac4
	.4byte 0x03001f54
	.4byte 0x03001f58
	.4byte 0x08003651
	.4byte 0x0000c00f
	.4byte 0x04000132
	.4byte 0x03007800
	.4byte 0x03001d18
	.2byte 0x1ca0
	.2byte 0x0300
.L_08002ed8:
	bl	sub_0808a008
	add	sp, #4
	pop	{r5}
	pop	{r0}
	bx	r0
