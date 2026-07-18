@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080bb938
	.thumb_func
Func_080bb938:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #360]
	movs	r2, #215
	ldr	r3, [r3, #0]
	lsls	r2, r2, #3
	adds	r6, r3, r2
	ldr	r2, [pc, #352]
	adds	r3, r3, r2
	ldr	r3, [r3, #0]
	movs	r7, #0
	cmp	r7, r3
	blt.n	.L0
	b.n	.L1
.L0:
	ldrb	r3, [r6, r7]
	cmp	r3, #13
	bls.n	.L2
	b.n	.L3
.L2:
	ldr	r2, [pc, #336]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x080bb9b6
	.4byte 0x080bb9c4
	.4byte 0x080bb9d2
	.4byte 0x080bb9de
	.4byte 0x080bb9fa
	.4byte 0x080bba12
	.4byte 0x080bb9f0
	.4byte 0x080bba0c
	.4byte 0x080bba22
	.4byte 0x080bba42
	.4byte 0x080bba62
	.4byte 0x080bba70
	.4byte 0x080bb9aa
	.4byte 0x080bb99c
	lsls	r3, r7, #2
	adds	r3, #64
	ldr	r1, [r6, r3]
	adds	r0, r6, #0
	bl	Func_080bb928
	b.n	.L3
	lsls	r3, r7, #2
	adds	r3, #64
	ldr	r0, [r6, r3]
	bl	Func_080bb8e8
	b.n	.L3
	lsls	r3, r7, #2
	adds	r3, #64
	ldr	r0, [r6, r3]
	movs	r1, #1
	bl	Func_08015120
	b.n	.L3
	lsls	r3, r7, #2
	adds	r3, #64
	ldr	r0, [r6, r3]
	movs	r1, #5
	bl	Func_08015120
	b.n	.L3
	lsls	r3, r7, #2
	adds	r3, #64
	ldr	r0, [r6, r3]
	ldr	r3, [pc, #212]
	movs	r1, #2
	b.n	.L4
	lsls	r3, r7, #2
	adds	r3, #64
	ldr	r0, [r6, r3]
	ldr	r3, [pc, #204]
	movs	r1, #4
.L4:
	ands	r0, r3
	bl	Func_08015120
	b.n	.L3
	ldr	r3, [pc, #196]
	ldr	r2, [r3, #0]
	movs	r3, #1
	str	r3, [r2, #8]
	b.n	.L3
	lsls	r3, r7, #2
	adds	r3, #64
	ldr	r0, [r6, r3]
	cmp	r0, #0
	blt.n	.L5
	bl	Func_080151c8
.L5:
	bl	Func_080bb65c
.L6:
	bl	Func_08015118
	b.n	.L3
	lsls	r3, r7, #2
	adds	r3, #64
	ldr	r0, [r6, r3]
	cmp	r0, #0
	blt.n	.L6
	bl	Func_080151c8
	b.n	.L6
	movs	r2, #180
	lsls	r2, r2, #1
	adds	r3, r6, r2
	ldr	r0, [r3, #0]
	cmp	r0, #0
	ble.n	.L7
	bl	Func_080f9010
.L7:
	lsls	r3, r7, #2
	adds	r3, #64
	ldr	r0, [r6, r3]
	movs	r1, #0
	movs	r2, #0
	bl	Func_080babdc
	b.n	.L3
	movs	r2, #182
	lsls	r2, r2, #1
	lsls	r5, r7, #2
	adds	r5, #64
	adds	r3, r6, r2
	ldr	r1, [r3, #0]
	ldr	r0, [r6, r5]
	bl	Func_080c24f0
	ldr	r0, [r6, r5]
	bl	Func_080bb588
	ldr	r0, [r6, r5]
	bl	Func_080bace8
	b.n	.L3
	ldr	r3, [pc, #64]
	ldr	r3, [r3, #0]
	adds	r3, #65
	ldrb	r0, [r3, #0]
	bl	Func_08015130
	b.n	.L3
	lsls	r5, r7, #2
	adds	r5, #64
	ldr	r0, [r6, r5]
	bl	Func_080b7dd0
	adds	r1, r0, #0
	ldr	r0, [r6, r5]
	bl	Func_080b78e4
	ldr	r0, [r6, r5]
	bl	Func_080b7aac
.L3:
	movs	r2, #162
	lsls	r2, r2, #1
	adds	r3, r6, r2
	ldr	r3, [r3, #0]
	adds	r7, #1
	cmp	r7, r3
	bge.n	.L1
	b.n	.L0
.L1:
	bl	Func_080bdfec
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001e74
	.4byte 0x000007fc
	.4byte 0x080bb964
	.4byte 0x000001ff
	.4byte 0x00003fff
	.4byte 0x03001ee4
