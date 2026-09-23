.syntax unified
	.thumb
	.set sub_08013b30, 0x08013b30
	.set sub_081c11ac, 0x081c11ac
	.set sub_081c11ec, 0x081c11ec
	.set sub_081c1f3c, 0x081c1f3c
	.set sub_081c1fc0, 0x081c1fc0
	.set sub_081c2158, 0x081c2158
	.set sub_081c26f4, 0x081c26f4
	.set sub_081c2f68, 0x081c2f68
	.global Func_081c0c1c
	.thumb_func
Func_081c0c1c:
	push	{lr}
	bl	sub_081c1f3c
	ldr	r1, [pc, #56]
	ldr	r3, [pc, #64]
	ldr	r2, [pc, #56]
	strh	r1, [r3, #0]
	ldr	r3, [pc, #64]
	ldr	r0, [pc, #52]
	strb	r1, [r3, #0]
	ldr	r3, [pc, #60]
	strh	r2, [r3, #0]
	ldr	r3, [pc, #60]
	strh	r2, [r3, #0]
	ldr	r3, [pc, #60]
	strh	r0, [r3, #0]
	ldr	r3, [pc, #60]
	strh	r2, [r3, #0]
	ldr	r3, [pc, #60]
	strh	r2, [r3, #0]
	ldr	r3, [pc, #60]
	ldr	r2, [pc, #64]
	strh	r0, [r3, #0]
	ldr	r3, [pc, #64]
	strb	r1, [r3, #0]
	ldr	r3, [pc, #64]
	strb	r1, [r3, #0]
	ldr	r3, [pc, #64]
	strb	r1, [r3, #0]
	movs	r3, #7
	b.n	.L_081c0c98
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x00000100
	.4byte 0x00000004
	.4byte 0x02005830
	.4byte 0x02005800
	.4byte 0x02005838
	.4byte 0x02005808
	.4byte 0x02005810
	.4byte 0x02005834
	.4byte 0x0200583c
	.4byte 0x0200580c
	.4byte 0x02005820
	.4byte 0x02005814
	.4byte 0x02005840
	.2byte 0x5804
	.2byte 0x0200
.L_081c0c98:
	subs	r3, #1
	strh	r1, [r2, #0]
	adds	r2, #2
	cmp	r3, #0
	bge.n	.L_081c0c98
	movs	r0, #144
	lsls	r0, r0, #12
	bl	sub_081c11ec
	pop	{pc}
.L_081c0cac:
	movs	r0, #3
	bx	lr
	push	{r5, r6, r7, lr}
	adds	r6, r0, #0
	bl	sub_081c11ac
	ldr	r3, [pc, #268]
	movs	r5, #240
	ldrb	r3, [r3, #3]
	lsls	r5, r5, #8
	adds	r7, r3, #0
	ands	r5, r6
	cmp	r7, #0
	beq.n	.L_081c0cca
	b.n	.L_081c0e2c
.L_081c0cca:
	movs	r3, #240
	lsls	r3, r3, #4
	adds	r3, #255
	ands	r6, r3
	cmp	r6, #78
	bne.n	.L_081c0cf4
	ldr	r5, [pc, #244]
	ldrb	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_081c0ce0
	b.n	.L_081c0e2c
.L_081c0ce0:
	ldr	r0, [pc, #236]
	movs	r1, #7
	bl	sub_081c2158
	ldrb	r3, [r5, #0]
	adds	r3, #1
	strb	r3, [r5, #0]
	ldr	r3, [pc, #228]
	strh	r7, [r3, #0]
	b.n	.L_081c0e2c
.L_081c0cf4:
	movs	r3, #195
	lsls	r3, r3, #1
	cmp	r6, r3
	bne.n	.L_081c0d0e
	ldr	r5, [pc, #216]
	ldr	r0, [pc, #220]
	movs	r1, #3
	strh	r7, [r5, #6]
	bl	sub_081c2158
	strh	r7, [r5, #10]
	ldr	r0, [pc, #212]
	b.n	.L_081c0d1c
.L_081c0d0e:
	movs	r3, #140
	adds	r3, #255
	cmp	r6, r3
	bne.n	.L_081c0d24
	ldr	r3, [pc, #192]
	ldr	r0, [pc, #196]
	strh	r7, [r3, #10]
.L_081c0d1c:
	movs	r1, #3
	bl	sub_081c2158
	b.n	.L_081c0e2c
.L_081c0d24:
	movs	r3, #175
	lsls	r3, r3, #2
	cmp	r6, r3
	bge.n	.L_081c0d96
	cmp	r6, #99
	ble.n	.L_081c0d6e
	ldr	r7, [pc, #176]
	lsls	r4, r6, #3
	adds	r3, r4, #4
	ldrh	r2, [r7, r3]
	cmp	r2, #7
	bne.n	.L_081c0d58
	ldr	r1, [pc, #168]
.L_081c0d3e:
	lsls	r5, r2, #1
	adds	r3, r5, r2
	lsls	r3, r3, #2
	ldr	r3, [r1, r3]
	ldrb	r3, [r3, #4]
	cmp	r3, #0
	beq.n	.L_081c0d5c
	subs	r2, #1
	cmp	r2, #3
	bgt.n	.L_081c0d3e
	movs	r2, #7
	movs	r5, #14
	b.n	.L_081c0d5c
.L_081c0d58:
	ldr	r1, [pc, #140]
	lsls	r5, r2, #1
.L_081c0d5c:
	adds	r3, r5, r2
	lsls	r3, r3, #2
	ldr	r0, [r1, r3]
	ldr	r1, [r7, r4]
	bl	sub_081c26f4
	ldr	r3, [pc, #108]
	strh	r6, [r3, r5]
	b.n	.L_081c0e2c
.L_081c0d6e:
	cmp	r6, #79
	ble.n	.L_081c0d92
	movs	r2, #0
	ldr	r0, [pc, #88]
	movs	r1, #255
	bl	sub_081c2f68
	ldr	r3, [pc, #108]
	lsls	r0, r6, #16
	strh	r7, [r3, #0]
	ldr	r3, [pc, #108]
	lsrs	r0, r0, #16
	strh	r7, [r3, #0]
	bl	sub_081c1fc0
	ldr	r2, [pc, #100]
	movs	r3, #10
	b.n	.L_081c0e0e
.L_081c0d92:
	cmp	r6, #79
	beq.n	.L_081c0e2c
.L_081c0d96:
	ldr	r2, [pc, #60]
	ldrh	r3, [r2, #0]
	cmp	r6, r3
	beq.n	.L_081c0e2c
	strh	r6, [r2, #0]
	adds	r0, r6, #0
	bl	.L_081c0cac
	bl	sub_08013b30
	lsls	r0, r6, #16
	lsrs	r0, r0, #16
	bl	sub_081c1fc0
	movs	r3, #128
	lsls	r3, r3, #5
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_081c0df8
	ldr	r2, [pc, #48]
	ldr	r3, [pc, #4]
	b.n	.L_081c0dfc
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x03001138
	.4byte 0x02005814
	.4byte 0x02006a90
	.4byte 0x02005830
	.4byte 0x02005820
	.4byte 0x02006b60
	.4byte 0x02006990
	.4byte 0x081c4530
	.4byte 0x081c44d0
	.4byte 0x02005838
	.4byte 0x02005808
	.2byte 0x5800
	.2byte 0x0200
.L_081c0df8:
	ldr	r2, [pc, #32]
	ldr	r3, [pc, #24]
.L_081c0dfc:
	strh	r3, [r2, #0]
	ldr	r2, [pc, #32]
	ldr	r3, [pc, #16]
	strh	r3, [r2, #0]
	ldr	r2, [pc, #28]
	ldr	r3, [pc, #16]
	strh	r3, [r2, #0]
	ldr	r2, [pc, #28]
	movs	r3, #0
.L_081c0e0e:
	strb	r3, [r2, #0]
	b.n	.L_081c0e2c
	movs	r0, r0
	.4byte 0x00000100
	.4byte 0x00000004
	.4byte 0x02005808
	.4byte 0x02005838
	.4byte 0x02005810
	.2byte 0x5814
	.2byte 0x0200
.L_081c0e2c:
	pop	{r5, r6, r7, pc}
	.align 2, 0
