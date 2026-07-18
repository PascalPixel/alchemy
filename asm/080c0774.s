@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080c0774
	.thumb_func
Func_080c0774:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #168]
	ldr	r6, [r3, #0]
	ldr	r3, [r6, #8]
	adds	r7, r0, #0
	adds	r5, r2, #0
	cmp	r3, #0
	bne.n	.L0
	ldr	r0, [pc, #156]
	ldr	r1, [pc, #160]
	bl	Func_080041d8
.L0:
	str	r7, [r6, #8]
	cmp	r7, #1
	bne.n	.L1
	ldr	r1, [pc, #152]
	ldr	r0, [pc, #152]
	ldrh	r3, [r0, #0]
	adds	r4, r3, #0
	strh	r0, [r0, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L2
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r1
	strh	r2, [r1, #0]
	ldr	r2, [pc, #132]
	adds	r3, #4
	stmia	r3!, {r2}
	ldr	r2, [pc, #128]
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L2:
	strh	r4, [r0, #0]
.L1:
	ldr	r3, [pc, #120]
	ldr	r0, [pc, #124]
	ldr	r1, [pc, #124]
	ldr	r2, [pc, #128]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r2, [pc, #124]
	ldr	r3, [pc, #128]
	ldrh	r2, [r2, #0]
	strh	r2, [r3, #0]
	cmp	r5, #128
	bne.n	.L3
	ldr	r3, [pc, #120]
	ldr	r2, [pc, #124]
	ldr	r0, [r3, #0]
	adds	r1, #32
	adds	r0, r0, r2
	ldr	r3, [pc, #88]
	ldr	r2, [pc, #116]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	b.n	.L4
.L3:
	cmp	r5, #0
	beq.n	.L4
	ldr	r3, [pc, #96]
	ldr	r2, [pc, #100]
	ldr	r3, [r3, #0]
	ldr	r7, [pc, #36]
	adds	r2, r2, r3
	ldr	r4, [pc, #100]
	mov	ip, r2
	movs	r6, #0
	movs	r0, #0
.L11:
	mov	r2, ip
	ldrh	r3, [r0, r2]
	movs	r1, #31
	ands	r1, r3
	lsls	r3, r3, #16
	lsrs	r2, r3, #21
	lsrs	r3, r3, #26
	ands	r2, r7
	ands	r3, r7
	cmp	r1, r5
	ble.n	.L5
	subs	r1, r1, r5
	b.n	.L6
	.4byte 0x0000001f
	.4byte 0x03001f00
	.4byte 0x080c0131
	.4byte 0x000004ff
	.4byte 0x02002090
	.4byte 0x04000208
	.4byte 0x00001f83
	.4byte 0x0400000a
	.4byte 0x040000d4
	.4byte 0x05000200
	.4byte 0x050000a0
	.4byte 0x80000010
	.4byte 0x050001e8
	.4byte 0x050000bc
	.4byte 0x03001e74
	.4byte 0x00000544
	.4byte 0x80000080
	.4byte 0x050000c0
.L5:
	movs	r1, #0
.L6:
	cmp	r2, r5
	ble.n	.L7
	subs	r2, r2, r5
	b.n	.L8
.L7:
	movs	r2, #0
.L8:
	cmp	r3, r5
	ble.n	.L9
	subs	r3, r3, r5
	b.n	.L10
.L9:
	movs	r3, #0
.L10:
	lsls	r3, r3, #10
	lsls	r2, r2, #5
	orrs	r3, r2
	orrs	r3, r1
	adds	r6, #1
	strh	r3, [r4, #0]
	adds	r0, #2
	adds	r4, #2
	cmp	r6, #128
	bne.n	.L11
.L4:
	ldr	r0, [pc, #16]
	bl	Func_080c0098
	ldr	r0, [pc, #12]
	bl	Func_080c00d8
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x06003800
	.4byte 0x0600f800
