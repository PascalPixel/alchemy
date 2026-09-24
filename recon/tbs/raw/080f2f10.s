.syntax unified
	.thumb
	.global TitlePalette_UpdateFade
	.global Func_080f2f10
	.thumb_func
TitlePalette_UpdateFade:
Func_080f2f10:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #76]
	movs	r1, #224
	ldr	r5, [r3, #0]
	ldr	r2, [pc, #72]
	lsls	r1, r1, #5
	adds	r4, r5, r1
	adds	r1, r5, r2
	movs	r3, #0
	ldrsb	r3, [r1, r3]
	cmp	r3, #0
	bne.n	.L_080f2f2a
	b.n	.L_080f3060
.L_080f2f2a:
	adds	r2, #1
	adds	r3, r5, r2
	ldrb	r2, [r3, #0]
	adds	r2, #1
	strb	r2, [r3, #0]
	lsls	r2, r2, #24
	movs	r3, #0
	ldrsb	r3, [r1, r3]
	asrs	r2, r2, #24
	cmp	r2, r3
	bge.n	.L_080f2f6c
	movs	r3, #128
	lsls	r3, r3, #3
	ldr	r6, [pc, #32]
	adds	r1, r5, r3
	movs	r0, #0
.L_080f2f4a:
	ldrh	r3, [r1, #0]
	ldrh	r2, [r4, #0]
	adds	r0, #1
	adds	r3, r3, r2
	strh	r3, [r1, #0]
	adds	r4, #2
	adds	r1, #2
	cmp	r0, r6
	ble.n	.L_080f2f4a
	b.n	.L_080f2f88
	movs	r0, r0
	.4byte 0x03001ed0
	.4byte 0x00003001
	.2byte 0x05ff
	.2byte 0x0000
.L_080f2f6c:
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #5
	lsls	r2, r2, #3
	adds	r0, r5, r1
	ldr	r3, [pc, #68]
	adds	r1, r5, r2
	ldr	r2, [pc, #68]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #64]
	adds	r2, r5, r3
	movs	r3, #0
	strb	r3, [r2, #0]
.L_080f2f88:
	movs	r1, #192
	lsls	r1, r1, #6
	adds	r3, r5, r1
	ldrb	r2, [r3, #0]
	movs	r3, #1
	eors	r3, r2
	lsls	r3, r3, #10
	movs	r2, #160
	adds	r3, r5, r3
	lsls	r2, r2, #6
	adds	r4, r3, r2
	movs	r3, #248
	movs	r2, #128
	lsls	r3, r3, #7
	movs	r0, #128
	lsls	r2, r2, #3
	ldr	r7, [pc, #8]
	ldr	r6, [pc, #12]
	mov	ip, r3
	lsls	r0, r0, #2
	adds	r1, r5, r2
	b.n	.L_080f2fc8
	.4byte 0x000003e0
	.4byte 0x0000001f
	.4byte 0x040000d4
	.4byte 0x84000300
	.2byte 0x3001
	.2byte 0x0000
.L_080f2fc8:
	ldrh	r3, [r1, #0]
	mov	r2, ip
	ands	r2, r3
	ldrh	r3, [r1, #2]
	lsls	r3, r3, #16
	asrs	r3, r3, #21
	ands	r3, r7
	orrs	r2, r3
	ldrh	r3, [r1, #4]
	lsls	r3, r3, #16
	asrs	r3, r3, #26
	ands	r3, r6
	orrs	r2, r3
	subs	r0, #1
	strh	r2, [r4, #0]
	adds	r1, #6
	adds	r4, #2
	cmp	r0, #0
	bne.n	.L_080f2fc8
	movs	r3, #192
	lsls	r3, r3, #6
	adds	r1, r5, r3
	ldrb	r3, [r1, #0]
	movs	r2, #1
	eors	r3, r2
	strb	r3, [r1, #0]
	ldrb	r3, [r1, #0]
	movs	r1, #160
	lsls	r3, r3, #10
	adds	r0, r5, r3
	lsls	r1, r1, #6
	ldr	r5, [pc, #96]
	adds	r6, r0, r1
	ldr	r4, [pc, #96]
	ldrh	r3, [r4, #0]
	adds	r1, r3, #0
	strh	r4, [r4, #0]
	ldrh	r2, [r5, #0]
	cmp	r2, #31
	bgt.n	.L_080f3032
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, r3, r5
	adds	r3, #4
	adds	r2, #1
	stmia	r3!, {r6}
	strh	r2, [r5, #0]
	movs	r2, #160
	lsls	r2, r2, #19
	stmia	r3!, {r2}
	ldr	r2, [pc, #64]
	str	r2, [r3, #0]
.L_080f3032:
	strh	r1, [r4, #0]
	ldrh	r3, [r4, #0]
	adds	r6, r3, #0
	strh	r4, [r4, #0]
	ldrh	r2, [r5, #0]
	cmp	r2, #31
	bgt.n	.L_080f305e
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	movs	r1, #168
	adds	r2, #1
	adds	r3, r3, r5
	lsls	r1, r1, #6
	adds	r3, #4
	strh	r2, [r5, #0]
	adds	r2, r0, r1
	stmia	r3!, {r2}
	ldr	r2, [pc, #28]
	stmia	r3!, {r2}
	ldr	r2, [pc, #20]
	str	r2, [r3, #0]
.L_080f305e:
	strh	r6, [r4, #0]
.L_080f3060:
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02002090
	.4byte 0x04000208
	.4byte 0x84000080
	.4byte 0x05000200
