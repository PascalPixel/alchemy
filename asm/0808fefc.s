@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0808fefc
	.thumb_func
Func_0808fefc:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #255
	mov	sl, r1
	asrs	r2, r0, #8
	ldr	r1, [pc, #492]
	adds	r6, r3, #0
	ands	r2, r3
	ldr	r7, [r1, #0]
	ands	r6, r0
	cmp	r2, #4
	bls.n	.L0
	b.n	.L1
.L0:
	lsls	r3, r2, #2
	ldr	r2, [pc, #480]
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x0808ff38
	.4byte 0x0808ff4c
	.4byte 0x0808ffa2
	.4byte 0x0809003c
	.4byte 0x080900c0
	movs	r0, #0
	bl	Func_08003b70
	mov	r0, sl
	bl	Func_08003bb4
	movs	r0, #1
	bl	Func_080030f8
	b.n	.L1
	movs	r3, #160
	lsls	r3, r3, #19
	movs	r0, #128
	ldrh	r1, [r3, #0]
	lsls	r0, r0, #8
	bl	Func_08091220
	mov	r0, sl
	bl	Func_08091254
	movs	r0, #1
	bl	Func_080030f8
	ldr	r1, [pc, #412]
	ldr	r4, [pc, #412]
	ldrh	r3, [r4, #0]
	adds	r5, r3, #0
	strh	r4, [r4, #0]
	ldrh	r3, [r1, #0]
	cmp	r3, #31
	bgt.n	.L2
	lsls	r2, r3, #1
	adds	r2, r2, r3
	adds	r3, #1
	strh	r3, [r1, #0]
	movs	r0, #128
	lsls	r0, r0, #19
	lsls	r2, r2, #2
	ldrh	r3, [r7, #20]
	adds	r2, r2, r1
	ldrh	r1, [r0, #0]
	adds	r2, #4
	orrs	r3, r1
	stmia	r2!, {r3}
	movs	r3, #128
	stmia	r2!, {r0}
	lsls	r3, r3, #10
	str	r3, [r2, #0]
.L2:
	strh	r5, [r4, #0]
	movs	r0, #0
	bl	Func_08091240
	b.n	.L3
	bl	Func_0808fecc
	movs	r1, #165
	adds	r5, r0, #0
	lsls	r1, r1, #3
	movs	r2, #0
	adds	r3, r5, r1
	mov	r8, r2
	adds	r1, #2
	strh	r6, [r3, #0]
	mov	r2, r8
	adds	r3, r5, r1
	strh	r2, [r3, #0]
	ldr	r3, [pc, #332]
	adds	r1, #12
	adds	r2, r5, r3
	movs	r3, #63
	strh	r3, [r2, #0]
	adds	r2, r5, r1
	movs	r3, #1
	movs	r1, #200
	strh	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #316]
	bl	Func_080041d8
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #312]
	bl	Func_080041d8
	movs	r0, #1
	bl	Func_080030f8
	ldr	r1, [pc, #284]
	ldr	r4, [pc, #284]
	ldrh	r3, [r4, #0]
	adds	r6, r3, #0
	strh	r4, [r4, #0]
	ldrh	r3, [r1, #0]
	cmp	r3, #31
	bgt.n	.L4
	lsls	r2, r3, #1
	adds	r2, r2, r3
	adds	r3, #1
	strh	r3, [r1, #0]
	movs	r0, #128
	lsls	r0, r0, #19
	lsls	r2, r2, #2
	ldrh	r3, [r7, #20]
	adds	r2, r2, r1
	ldrh	r1, [r0, #0]
	adds	r2, #4
	orrs	r3, r1
	stmia	r2!, {r3}
	movs	r3, #128
	stmia	r2!, {r0}
	lsls	r3, r3, #10
	str	r3, [r2, #0]
.L4:
	strh	r6, [r4, #0]
	ldr	r2, [pc, #252]
	adds	r3, r5, r2
	mov	r1, r8
	strb	r1, [r3, #0]
	ldr	r3, [pc, #248]
	ldr	r1, [pc, #248]
	adds	r2, r5, r3
	movs	r3, #32
	strb	r3, [r2, #0]
	adds	r3, r5, r1
	mov	r2, sl
	adds	r1, #1
	strb	r2, [r3, #0]
	adds	r3, r5, r1
	mov	r2, r8
	strb	r2, [r3, #0]
	b.n	.L3
	bl	Func_0808fecc
	movs	r1, #165
	adds	r5, r0, #0
	lsls	r1, r1, #3
	adds	r3, r5, r1
	ldr	r2, [pc, #216]
	movs	r1, #32
	mov	r8, r1
	strh	r6, [r3, #0]
	adds	r3, r5, r2
	mov	r2, r8
	strh	r2, [r3, #0]
	movs	r0, #15
	bl	Func_080907b0
	movs	r0, #1
	bl	Func_080030f8
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #192]
	bl	Func_080041d8
	ldr	r1, [pc, #148]
	ldr	r4, [pc, #152]
	ldrh	r3, [r4, #0]
	adds	r6, r3, #0
	strh	r4, [r4, #0]
	ldrh	r3, [r1, #0]
	cmp	r3, #31
	bgt.n	.L5
	lsls	r2, r3, #1
	adds	r2, r2, r3
	adds	r3, #1
	strh	r3, [r1, #0]
	movs	r0, #128
	lsls	r0, r0, #19
	lsls	r2, r2, #2
	ldrh	r3, [r7, #20]
	adds	r2, r2, r1
	ldrh	r1, [r0, #0]
	adds	r2, #4
	orrs	r3, r1
	stmia	r2!, {r3}
	movs	r3, #128
	stmia	r2!, {r0}
	lsls	r3, r3, #10
	str	r3, [r2, #0]
.L5:
	strh	r6, [r4, #0]
	ldr	r1, [pc, #116]
	movs	r2, #0
	adds	r3, r5, r1
	adds	r1, #1
	strb	r2, [r3, #0]
	adds	r3, r5, r1
	mov	r1, r8
	strb	r1, [r3, #0]
	ldr	r1, [pc, #108]
	adds	r3, r5, r1
	mov	r1, sl
	strb	r1, [r3, #0]
	ldr	r1, [pc, #112]
	adds	r3, r5, r1
	strb	r2, [r3, #0]
	b.n	.L3
	ldr	r7, [r1, #0]
	bl	Func_0808fecc
	movs	r3, #128
	lsls	r3, r3, #1
	ldr	r1, [pc, #40]
	adds	r2, r7, r3
	ldr	r3, [pc, #40]
	mov	r8, r1
	movs	r1, #129
	mov	r9, r3
	lsls	r1, r1, #1
	movs	r3, #80
	strh	r3, [r2, #0]
	adds	r2, r7, r1
	adds	r5, r0, #0
	strh	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	cmp	r6, #0
	bne.n	.L6
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #60]
	b.n	.L7
	.4byte 0x00000000
	.4byte 0x00000050
	.4byte 0x03001e70
	.4byte 0x0808ff24
	.4byte 0x02002090
	.4byte 0x04000208
	.4byte 0x00000534
	.4byte 0x0808f52d
	.4byte 0x0808f499
	.4byte 0x0000053a
	.4byte 0x0000053b
	.4byte 0x0000053c
	.4byte 0x0000052a
	.4byte 0x08090659
	.4byte 0x0000053d
	.4byte 0x080903bd
.L6:
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #112]
.L7:
	bl	Func_080041d8
	ldr	r2, [pc, #112]
	movs	r1, #0
	movs	r0, #1
	bl	Func_0800307c
	ldr	r2, [pc, #104]
	mov	r1, r9
	adds	r3, r5, r2
	adds	r2, #1
	strb	r1, [r3, #0]
	adds	r3, r5, r2
	mov	r1, r8
	adds	r2, #1
	strb	r1, [r3, #0]
	adds	r3, r5, r2
	mov	r1, sl
	adds	r2, #1
	strb	r1, [r3, #0]
	adds	r3, r5, r2
	mov	r1, r8
	strb	r1, [r3, #0]
.L1:
	ldr	r1, [pc, #76]
	ldr	r4, [pc, #80]
	ldrh	r3, [r4, #0]
	adds	r5, r3, #0
	strh	r4, [r4, #0]
	ldrh	r3, [r1, #0]
	cmp	r3, #31
	bgt.n	.L8
	lsls	r2, r3, #1
	adds	r2, r2, r3
	adds	r3, #1
	strh	r3, [r1, #0]
	movs	r0, #128
	lsls	r0, r0, #19
	lsls	r2, r2, #2
	ldrh	r3, [r7, #20]
	adds	r2, r2, r1
	ldrh	r1, [r0, #0]
	adds	r2, #4
	orrs	r3, r1
	stmia	r2!, {r3}
	movs	r3, #128
	stmia	r2!, {r0}
	lsls	r3, r3, #10
	str	r3, [r2, #0]
.L8:
	strh	r5, [r4, #0]
.L3:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x08090489
	.4byte 0x08090585
	.4byte 0x0000053a
	.4byte 0x02002090
	.4byte 0x04000208
