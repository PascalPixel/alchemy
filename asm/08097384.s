@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08097384
	.thumb_func
Func_08097384:
	push	{r5, lr}
	ldr	r3, [pc, #208]
	movs	r2, #154
	ldr	r5, [r3, #20]
	lsls	r2, r2, #5
	ldr	r4, [r3, #0]
	adds	r0, r5, r2
	ldr	r2, [pc, #200]
	ldr	r3, [pc, #200]
	adds	r1, r4, r2
	ldr	r2, [pc, #200]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r2, [pc, #200]
	adds	r3, r4, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L0
	movs	r2, #224
	lsls	r2, r2, #4
	adds	r0, r5, r2
	ldr	r2, [pc, #184]
	ldr	r3, [pc, #172]
	adds	r1, r4, r2
	ldr	r2, [pc, #172]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L0:
	movs	r2, #224
	lsls	r2, r2, #4
	adds	r0, r5, r2
	movs	r2, #224
	lsls	r2, r2, #2
	adds	r1, r5, r2
	ldr	r3, [pc, #148]
	ldr	r2, [pc, #164]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #160]
	movs	r0, #164
	ldr	r5, [r3, #0]
	lsls	r0, r0, #1
	movs	r3, #7
	ands	r5, r3
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L1
	movs	r5, #0
.L1:
	ldr	r0, [pc, #144]
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L2
	movs	r5, #1
.L2:
	movs	r0, #165
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L3
	movs	r5, #2
.L3:
	ldr	r0, [pc, #120]
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L4
	movs	r5, #3
.L4:
	movs	r0, #166
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L5
	movs	r5, #4
.L5:
	ldr	r0, [pc, #100]
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L6
	movs	r5, #5
.L6:
	movs	r0, #167
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L7
	movs	r5, #6
.L7:
	ldr	r0, [pc, #76]
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L8
	movs	r5, #7
.L8:
	ldr	r3, [pc, #68]
	lsls	r2, r5, #2
	ldr	r0, [r3, r2]
	movs	r1, #1
	bl	Func_08091200
	movs	r0, #8
	bl	Func_08091254
	pop	{r5}
	pop	{r0}
	bx	r0
	.4byte 0x03001ebc
	.4byte 0x00000776
	.4byte 0x040000d4
	.4byte 0x84000150
	.4byte 0x00000cb8
	.4byte 0x00000236
	.4byte 0x840002a0
	.4byte 0x03001e40
	.4byte 0x00000149
	.4byte 0x0000014b
	.4byte 0x0000014d
	.4byte 0x0000014f
	.4byte 0x080a0108
