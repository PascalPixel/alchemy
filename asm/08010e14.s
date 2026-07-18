@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08010e14
	.thumb_func
Func_08010e14:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r5, r0, #0
	movs	r0, #128
	movs	r2, #0
	lsls	r0, r0, #2
	sub	sp, #4
	adds	r6, r1, #0
	mov	r9, r2
	bl	Func_08004938
	ldr	r3, [pc, #356]
	mov	sl, r0
	ldr	r7, [r3, #0]
	cmp	r5, #0
	bge.n	.L0
	ldr	r3, [pc, #352]
	adds	r5, r5, r3
.L0:
	asrs	r5, r5, #21
	movs	r2, #31
	adds	r1, r6, #0
	ands	r5, r2
	cmp	r1, #0
	bge.n	.L1
	ldr	r3, [pc, #336]
	adds	r1, r1, r3
.L1:
	asrs	r3, r1, #21
	ands	r3, r2
	lsls	r3, r3, #5
	adds	r3, r5, r3
	ldr	r2, [pc, #328]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldr	r3, [r3, #0]
	lsls	r3, r3, #1
	lsrs	r3, r3, #25
	cmp	r3, #21
	bne.n	.L2
	movs	r3, #1
	mov	r9, r3
.L2:
	mov	r2, r9
	lsls	r6, r2, #1
	ldr	r3, [pc, #308]
	add	r6, r9
	movs	r2, #142
	lsls	r2, r2, #1
	lsls	r6, r6, #3
	movs	r5, #160
	adds	r6, r6, r3
	lsls	r5, r5, #19
	adds	r3, r7, r2
	str	r6, [r3, #0]
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	ldr	r0, [r6, #0]
	mov	r8, r3
	bl	Func_08002f40
	mov	r1, sl
	bl	Func_080053e8
	mov	r3, r8
	mov	r2, sl
	strh	r3, [r2, #0]
	mov	r0, sl
	ldr	r3, [pc, #264]
	adds	r1, r5, #0
	ldr	r2, [pc, #264]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r5, [pc, #264]
	ldr	r0, [r6, #4]
	bl	Func_08002f40
	adds	r1, r5, #0
	bl	Func_08005340
	ldr	r3, [pc, #240]
	adds	r0, r5, #0
	ldr	r1, [pc, #248]
	ldr	r2, [pc, #252]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r5, [pc, #248]
	ldr	r0, [r6, #8]
	bl	Func_08002f40
	adds	r1, r5, #0
	bl	Func_08005340
	ldr	r3, [pc, #216]
	adds	r0, r5, #0
	ldr	r1, [pc, #236]
	ldr	r2, [pc, #224]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r5, [pc, #232]
	ldr	r0, [r6, #12]
	bl	Func_08002f40
	adds	r1, r5, #0
	bl	Func_08005340
	ldr	r3, [pc, #188]
	adds	r0, r5, #0
	ldr	r1, [pc, #216]
	ldr	r2, [pc, #200]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r5, [pc, #212]
	ldr	r0, [r6, #16]
	bl	Func_08002f40
	adds	r1, r5, #0
	bl	Func_08005340
	ldr	r3, [pc, #164]
	adds	r0, r5, #0
	ldr	r1, [pc, #200]
	ldr	r2, [pc, #172]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r0, [r6, #20]
	bl	Func_08002f40
	ldr	r1, [pc, #188]
	bl	Func_08005340
	ldr	r3, [pc, #188]
	mov	r0, sp
	str	r3, [r0, #0]
	ldr	r1, [pc, #184]
	ldr	r3, [pc, #132]
	ldr	r2, [pc, #184]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r1, [pc, #184]
	ldr	r2, [pc, #184]
	ldr	r4, [pc, #188]
	movs	r0, #0
.L4:
	movs	r3, #14
.L3:
	subs	r3, #1
	stmia	r1!, {r2}
	adds	r2, r2, r4
	cmp	r3, #0
	bge.n	.L3
	adds	r0, #1
	adds	r1, #4
	cmp	r0, #19
	ble.n	.L4
	mov	r3, r9
	cmp	r3, #1
	bne.n	.L5
	movs	r3, #166
	lsls	r3, r3, #1
	adds	r2, r7, r3
	subs	r3, #66
	strh	r3, [r2, #0]
	adds	r3, #68
	adds	r2, r7, r3
	subs	r3, #67
	strh	r3, [r2, #0]
	adds	r3, #69
	adds	r2, r7, r3
	subs	r3, #68
	strh	r3, [r2, #0]
	adds	r3, #96
	adds	r2, r7, r3
	subs	r3, #82
	strh	r3, [r2, #0]
	adds	r3, #84
	adds	r2, r7, r3
	subs	r3, #83
	strh	r3, [r2, #0]
	adds	r3, #85
	adds	r2, r7, r3
	subs	r3, #84
	strh	r3, [r2, #0]
	bl	Func_080113e4
.L5:
	mov	r0, sl
	bl	Func_08002df0
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001e70
	.4byte 0x001fffff
	.4byte 0x02020000
	.4byte 0x080132cc
	.4byte 0x040000d4
	.4byte 0x84000070
	.4byte 0x02038000
	.4byte 0x06008000
	.4byte 0x84000800
	.4byte 0x0203a000
	.4byte 0x0600a000
	.4byte 0x0203c000
	.4byte 0x0600c000
	.4byte 0x0203e000
	.4byte 0x0600e000
	.4byte 0x02028000
	.4byte 0xf07ff07f
	.4byte 0x06002800
	.4byte 0x85000180
	.4byte 0x06003000
	.4byte 0x01a901a8
	.4byte 0x00020002
