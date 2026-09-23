.syntax unified
	.thumb
	.set sub_080132d0, 0x080132d0
	.set sub_080133c4, 0x080133c4
	.set sub_08013438, 0x08013438
	.set sub_08013560, 0x08013560
	.set sub_08014368, 0x08014368
	.set sub_080144c0, 0x080144c0
	.set sub_08014b70, 0x08014b70
	.set sub_08014bac, 0x08014bac
	.set sub_08014c6c, 0x08014c6c
	.set sub_080c8008, 0x080c8008
	.set sub_081c0008, 0x081c0008
	.global Func_0801319c
	.thumb_func
Func_0801319c:
	push	{r5, lr}
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #176
	ldrh	r1, [r2, #10]
	movs	r3, #197
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r1
	strh	r3, [r2, #10]
	movs	r3, #254
	ldrh	r1, [r2, #10]
	lsls	r3, r3, #7
	adds	r3, #255
	ands	r3, r1
	strh	r3, [r2, #10]
	sub	sp, #4
	ldrh	r3, [r2, #10]
	movs	r2, #128
	ldr	r3, [pc, #128]
	lsls	r2, r2, #7
	adds	r2, #20
	strh	r2, [r3, #0]
	mov	r0, sp
	movs	r3, #128
	movs	r5, #0
	lsls	r3, r3, #19
	movs	r1, #192
	str	r5, [r0, #0]
	adds	r3, #212
	lsls	r1, r1, #18
	ldr	r2, [pc, #108]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	bl	sub_08014c6c
	bl	sub_080133c4
	ldr	r3, [pc, #96]
	str	r5, [r3, #0]
	ldr	r3, [pc, #96]
	strb	r5, [r3, #0]
	ldr	r3, [pc, #96]
	strb	r5, [r3, #0]
	ldr	r3, [pc, #96]
	strb	r5, [r3, #0]
	bl	sub_080132d0
	bl	sub_08014bac
	bl	sub_08014b70
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	movs	r0, #0
	ldr	r2, [pc, #76]
	movs	r1, #1
	bl	sub_08013438
	ldr	r2, [pc, #72]
	movs	r0, #13
	movs	r1, #1
	bl	sub_08013438
	movs	r2, #192
	ldr	r3, [pc, #64]
	lsls	r2, r2, #8
	adds	r2, #15
	strh	r2, [r3, #0]
	bl	sub_081c0008
	bl	sub_08014368
	bl	sub_080144c0
	ldr	r3, [pc, #48]
	ldr	r2, [pc, #48]
	str	r5, [r3, #0]
	movs	r3, #1
	b.n	.L_08013270
	.4byte 0x00000140
	.4byte 0x04000204
	.4byte 0x85001e00
	.4byte 0x020038e0
	.4byte 0x03001110
	.4byte 0x03001238
	.4byte 0x0300123c
	.4byte 0x0801399d
	.4byte 0x030007ac
	.4byte 0x04000132
	.4byte 0x03007800
	.2byte 0x120c
	.2byte 0x0300
.L_08013270:
	strb	r3, [r2, #0]
	ldr	r2, [pc, #36]
	movs	r3, #128
	lsls	r3, r3, #1
	ldr	r1, [pc, #24]
	strh	r3, [r2, #2]
	ldr	r3, [pc, #28]
	strh	r5, [r2, #0]
	strb	r1, [r3, #0]
	movs	r0, #10
	bl	sub_08013560
	movs	r0, #0
	bl	sub_080c8008
	add	sp, #4
	b.n	.L_080132a0
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x03001248
	.2byte 0x1180
	.2byte 0x0300
.L_080132a0:
	pop	{r5, pc}
	.align 2, 0
