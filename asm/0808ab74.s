@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0808ab74
	.thumb_func
Func_0808ab74:
	push	{r5, r6, lr}
	ldr	r5, [pc, #288]
	movs	r2, #224
	lsls	r2, r2, #1
	adds	r3, r5, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	ldr	r2, [pc, #280]
	lsls	r3, r3, #3
	adds	r3, r3, r2
	movs	r6, #2
	ldrsb	r6, [r3, r6]
	cmp	r1, #0
	bne.n	.L0
	movs	r5, #128
	lsls	r5, r5, #2
.L1:
	adds	r0, r5, #0
	bl	Func_080770d0
	ldr	r3, [pc, #260]
	adds	r5, #1
	cmp	r5, r3
	ble.n	.L1
	ldr	r3, [pc, #244]
	movs	r1, #230
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r6, r3
	beq.n	.L2
	movs	r5, #192
	lsls	r5, r5, #2
.L3:
	adds	r0, r5, #0
	bl	Func_080770d0
	ldr	r3, [pc, #228]
	adds	r5, #1
	cmp	r5, r3
	ble.n	.L3
	ldr	r0, [pc, #224]
	bl	Func_080770c8
	ldr	r5, [pc, #204]
	movs	r1, #142
	lsls	r1, r1, #2
	movs	r2, #0
	adds	r3, r5, r1
	subs	r1, #6
	str	r2, [r3, #0]
	movs	r0, #136
	adds	r3, r5, r1
	strh	r2, [r3, #0]
	lsls	r0, r0, #1
	bl	Func_080770d0
	ldr	r0, [pc, #196]
	bl	Func_080770d0
	movs	r0, #137
	lsls	r0, r0, #1
	bl	Func_080770d0
	ldr	r0, [pc, #188]
	bl	Func_080770d0
	movs	r2, #224
	lsls	r2, r2, #1
	adds	r3, r5, r2
	movs	r1, #144
	ldrh	r2, [r3, #0]
	lsls	r1, r1, #2
	adds	r3, r5, r1
	strh	r2, [r3, #0]
	movs	r2, #225
	lsls	r2, r2, #1
	adds	r3, r5, r2
	ldrh	r2, [r3, #0]
	adds	r1, #2
	adds	r3, r5, r1
	strh	r2, [r3, #0]
.L2:
	movs	r5, #128
.L4:
	adds	r0, r5, #0
	adds	r5, #1
	bl	Func_080770d0
	cmp	r5, #223
	ble.n	.L4
	movs	r0, #182
	lsls	r0, r0, #1
	bl	Func_080770d0
	movs	r0, #162
	lsls	r0, r0, #1
	bl	Func_080770d0
	ldr	r0, [pc, #124]
	bl	Func_080770d0
	ldr	r0, [pc, #124]
	bl	Func_080770d0
	movs	r0, #142
	lsls	r0, r0, #1
	bl	Func_080770d0
	ldr	r1, [pc, #76]
	ldr	r3, [pc, #112]
	adds	r2, r1, r3
	ldr	r3, [pc, #112]
	strh	r3, [r2, #0]
	adds	r5, r1, #0
.L0:
	movs	r1, #230
	lsls	r1, r1, #1
	adds	r3, r5, r1
	strh	r6, [r3, #0]
	movs	r2, #192
	movs	r3, #127
	lsls	r2, r2, #1
	ands	r6, r3
	adds	r0, r6, r2
	bl	Func_080770c8
	movs	r1, #224
	lsls	r1, r1, #1
	adds	r3, r5, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	ldr	r2, [pc, #36]
	lsls	r3, r3, #3
	adds	r3, r3, r2
	movs	r2, #3
	ldrsb	r2, [r3, r2]
	adds	r1, #126
	adds	r3, r5, r1
	strh	r2, [r3, #0]
	cmp	r2, #2
	bne.n	.L5
	ldr	r0, [pc, #44]
	bl	Func_080770c8
.L5:
	bl	Func_080772f0
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x02000240
	.4byte 0x0809f1a8
	.4byte 0x000002ff
	.4byte 0x000003ff
	.4byte 0x0000012f
	.4byte 0x00000111
	.4byte 0x00000113
	.4byte 0x00000161
	.4byte 0x00000123
	.4byte 0x0000021e
	.4byte 0x0000ffff
