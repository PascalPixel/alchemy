@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080cf2b8
	.thumb_func
Func_080cf2b8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #260]
	adds	r6, r0, #0
	ldmia	r3!, {r0}
	ldr	r5, [pc, #256]
	ldr	r3, [r3, #0]
	mov	fp, r0
	sub	sp, #116
	add	r5, fp
	str	r3, [sp, #48]
	movs	r0, #0
	str	r6, [r5, #0]
	adds	r7, r1, #0
	bl	Func_080cd594
	ldr	r5, [r5, #0]
	ldr	r3, [r5, #28]
	cmp	r3, #1
	bne.n	.L0
	ldr	r2, [r5, #4]
	eors	r2, r3
	add	r3, sp, #64
	str	r3, [sp, #0]
	add	r3, sp, #60
	str	r3, [sp, #4]
	adds	r0, r6, #0
	adds	r1, r7, #0
	movs	r3, #0
	bl	Func_080de2f8
.L0:
	ldr	r0, [pc, #208]
	bl	Func_08002f40
	adds	r6, r0, #0
	movs	r0, #160
	movs	r2, #128
	adds	r1, r6, #0
	ldr	r5, [pc, #200]
	adds	r6, #128
	lsls	r0, r0, #19
	bl	Func_080072f8
	mov	r1, fp
	adds	r0, r6, #0
	bl	Func_08005340
	ldr	r0, [pc, #184]
	bl	Func_08002f40
	adds	r6, r0, #0
	movs	r0, #160
	adds	r1, r6, #0
	lsls	r0, r0, #19
	movs	r2, #128
	bl	Func_080072f8
	ldr	r1, [pc, #168]
	adds	r6, #128
	add	r1, fp
	adds	r0, r6, #0
	bl	Func_08005340
	cmp	r7, #0
	bne.n	.L1
	ldr	r0, [pc, #156]
	b.n	.L2
.L1:
	ldr	r0, [pc, #156]
.L2:
	bl	Func_08002f40
	adds	r6, r0, #0
	movs	r0, #160
	adds	r1, r6, #0
	ldr	r3, [pc, #128]
	movs	r2, #128
	lsls	r0, r0, #19
	bl	Func_080072f0
	ldr	r1, [pc, #140]
	adds	r6, #128
	add	r1, fp
	adds	r0, r6, #0
	bl	Func_08005340
	ldr	r5, [pc, #132]
	movs	r1, #0
	mov	r9, r1
.L3:
	movs	r3, #0
	str	r3, [r5, #0]
	movs	r3, #160
	lsls	r3, r3, #15
	str	r3, [r5, #4]
	bl	Func_08004458
	movs	r3, #32
	negs	r3, r3
	orrs	r3, r0
	lsls	r3, r3, #14
	str	r3, [r5, #8]
	bl	Func_08004458
	movs	r3, #255
	ands	r3, r0
	str	r3, [r5, #24]
	movs	r2, #1
	movs	r3, #128
	add	r9, r2
	lsls	r3, r3, #2
	adds	r5, #28
	cmp	r9, r3
	bne.n	.L3
	ldr	r2, [pc, #80]
	ldr	r3, [pc, #36]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #40]
	add	r3, fp
	ldr	r2, [r3, #0]
	ldr	r3, [r2, #20]
	cmp	r3, #1
	bne.n	.L4
	movs	r5, #36
	ldrsh	r0, [r2, r5]
	add	r5, sp, #104
	adds	r1, r5, #0
	bl	Func_080e396c
	ldr	r2, [r5, #0]
	movs	r3, #64
	subs	r3, r3, r2
	str	r3, [sp, #40]
	b.n	.L5
	.4byte 0x00000100
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x0000007b
	.4byte 0x03001388
	.4byte 0x000000b1
	.4byte 0x00002710
	.4byte 0x00000093
	.4byte 0x00000091
	.4byte 0x000065c0
	.4byte 0x02010000
	.4byte 0x04000020
.L4:
	ldr	r3, [r2, #4]
	movs	r0, #112
	negs	r0, r0
	str	r0, [sp, #40]
	cmp	r3, #1
	beq.n	.L5
	movs	r1, #0
	str	r1, [sp, #40]
.L5:
	ldr	r5, [sp, #40]
	ldr	r2, [pc, #464]
	lsls	r3, r5, #8
	str	r3, [r2, #0]
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, fp
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #452]
	movs	r3, #50
	add	r2, fp
	movs	r1, #144
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #444]
	bl	Func_080041d8
	movs	r0, #0
	ldr	r3, [pc, #440]
	str	r0, [sp, #44]
	add	r3, fp
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #20]
	movs	r1, #116
	lsls	r3, r3, #4
	negs	r1, r1
	cmp	r3, r1
	bne.n	.L6
	b.n	.L7
.L6:
	mov	r2, sp
	adds	r2, #52
	str	r2, [sp, #28]
.L34:
	ldr	r3, [pc, #416]
	ldr	r3, [r3, #0]
	str	r3, [sp, #36]
	ldr	r3, [sp, #44]
	cmp	r3, #64
	bne.n	.L8
	movs	r0, #212
	bl	Func_080f9010
.L8:
	ldr	r5, [sp, #44]
	cmp	r5, #80
	bne.n	.L9
	movs	r0, #0
	bl	Func_080b50e8
.L9:
	ldr	r7, [pc, #380]
	add	r7, fp
	ldr	r3, [r7, #0]
	ldr	r3, [r3, #28]
	mov	r8, r3
	cmp	r3, #1
	bne.n	.L10
	ldr	r0, [sp, #44]
	lsls	r5, r0, #11
	adds	r0, r5, #0
	bl	Func_08002322
	negs	r0, r0
	lsls	r3, r0, #2
	adds	r3, r3, r0
	ldr	r2, [sp, #64]
	lsls	r3, r3, #2
	ldr	r1, [sp, #40]
	asrs	r3, r3, #16
	adds	r3, r3, r2
	adds	r3, r3, r1
	subs	r3, #20
	adds	r0, r5, #0
	mov	sl, r3
	bl	Func_0800231c
	ldr	r3, [sp, #60]
	lsls	r0, r0, #2
	asrs	r0, r0, #16
	adds	r0, r0, r3
	ldr	r3, [r7, #0]
	adds	r6, r0, #0
	ldr	r0, [r3, #4]
	mov	r2, r8
	eors	r0, r2
	ldr	r1, [sp, #28]
	bl	Func_080cef64
	ldr	r3, [sp, #44]
	subs	r6, #24
	cmp	r3, #32
	ble.n	.L11
	lsls	r3, r3, #1
	subs	r3, r6, r3
	adds	r6, r3, #0
	adds	r6, #64
.L11:
	movs	r5, #40
	ldr	r7, [pc, #296]
	ldr	r0, [sp, #28]
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	add	r7, fp
	ldr	r4, [r0, #4]
	adds	r1, r7, #0
	ldr	r0, [sp, #48]
	mov	r2, sl
	adds	r3, r6, #0
	bl	Func_080072f4
	ldr	r1, [sp, #44]
	cmp	r1, #3
	bgt.n	.L12
	ldr	r2, [sp, #28]
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	ldr	r0, [sp, #48]
	ldr	r4, [r2, #4]
	adds	r1, r7, #0
	mov	r2, sl
	adds	r3, r6, #0
	bl	Func_080072f4
.L12:
	movs	r0, #47
	bl	Func_08002dd8
	movs	r0, #46
	bl	Func_08002dd8
.L10:
	movs	r6, #2
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	str	r6, [sp, #0]
	bl	Func_080ed408
	ldr	r5, [pc, #220]
	adds	r3, r5, #0
	adds	r3, #184
	ldr	r3, [r3, #0]
	movs	r1, #7
	str	r3, [sp, #52]
	movs	r2, #7
	movs	r3, #7
	movs	r0, #47
	str	r6, [sp, #0]
	bl	Func_080ed408
	adds	r5, #188
	ldr	r3, [r5, #0]
	ldr	r2, [pc, #180]
	ldr	r5, [sp, #28]
	mov	r1, fp
	str	r3, [r5, #4]
	ldr	r3, [r1, r2]
	ldr	r3, [r3, #20]
	movs	r0, #0
	mov	r8, r0
	cmp	r3, #0
	bne.n	.L13
	b.n	.L14
.L13:
	ldr	r3, [sp, #36]
	adds	r3, #12
	str	r3, [sp, #32]
	str	r0, [sp, #16]
	add	r7, sp, #68
.L21:
	mov	r5, fp
	mov	r0, r8
	ldr	r2, [r5, r2]
	lsls	r3, r0, #1
	adds	r3, #36
	ldrsh	r0, [r2, r3]
	bl	Func_080b5098
	mov	r2, r8
	lsls	r2, r2, #4
	ldr	r5, [r0, #0]
	mov	sl, r2
	bl	Func_080049ac
	ldr	r0, [sp, #36]
	ldr	r1, [sp, #32]
	bl	Func_080051d8
	ldr	r3, [r5, #8]
	str	r3, [r7, #0]
	movs	r3, #0
	str	r3, [r7, #4]
	ldr	r3, [r5, #16]
	adds	r0, r7, #0
	str	r3, [r7, #8]
	bl	Func_08004cb4
	ldr	r3, [sp, #44]
	cmp	r3, sl
	ble.n	.L15
	ldr	r0, [sp, #16]
	ldr	r1, [pc, #104]
	movs	r5, #0
	mov	r9, r5
	adds	r6, r0, r1
.L20:
	mov	r2, r9
	lsls	r3, r2, #3
	ldr	r5, [sp, #44]
	add	r3, sl
	cmp	r5, r3
	ble.n	.L16
	movs	r0, #128
	ldr	r3, [r6, #4]
	lsls	r0, r0, #12
	cmp	r3, r0
	ble.n	.L16
	add	r5, sp, #92
	adds	r1, r5, #0
	adds	r0, r6, #0
	bl	Func_080e3944
	ldr	r3, [r5, #0]
	ldr	r1, [sp, #40]
	adds	r3, r3, r1
	str	r3, [r5, #0]
	ldr	r0, [r6, #24]
	lsls	r0, r0, #10
	bl	Func_08002322
	movs	r3, #1
	mov	r2, r9
	lsls	r0, r0, #4
	ands	r3, r2
	asrs	r0, r0, #16
	cmp	r3, #0
	beq.n	.L17
	ldr	r3, [r5, #0]
	subs	r3, r3, r0
	b.n	.L18
	movs	r0, r0
	.4byte 0x04000028
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x00007828
	.4byte 0x03001e80
	.4byte 0x000065c0
	.4byte 0x03001e50
	.4byte 0x02010000
.L17:
	ldr	r3, [r5, #0]
	adds	r3, r3, r0
.L18:
	str	r3, [r5, #0]
	movs	r3, #1
	mov	r0, r9
	ldr	r1, [sp, #28]
	ands	r3, r0
	ldr	r2, [r6, #24]
	lsls	r3, r3, #2
	adds	r4, r3, r1
	ldr	r1, [pc, #596]
	cmp	r2, #0
	bge.n	.L19
	adds	r2, #7
.L19:
	movs	r3, #7
	asrs	r2, r2, #3
	ands	r2, r3
	ldrb	r3, [r1, r2]
	lsls	r1, r3, #3
	adds	r1, r1, r3
	ldr	r2, [pc, #580]
	lsls	r1, r1, #6
	movs	r0, #24
	add	r1, fp
	ldr	r3, [r5, #4]
	adds	r1, r1, r2
	ldr	r2, [r5, #0]
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	subs	r3, #12
	subs	r2, #12
	ldr	r4, [r4, #0]
	ldr	r0, [sp, #48]
	bl	Func_080072f4
	ldr	r5, [pc, #556]
	ldr	r3, [r6, #4]
	adds	r3, r3, r5
	str	r3, [r6, #4]
	ldr	r3, [r6, #24]
	adds	r3, #1
	str	r3, [r6, #24]
.L16:
	movs	r0, #1
	add	r9, r0
	mov	r1, r9
	adds	r6, #28
	cmp	r1, #8
	bne.n	.L20
.L15:
	ldr	r2, [sp, #16]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r2, r2, r3
	str	r2, [sp, #16]
	ldr	r2, [pc, #524]
	mov	r0, fp
	ldr	r3, [r0, r2]
	movs	r5, #1
	ldr	r3, [r3, #20]
	add	r8, r5
	cmp	r8, r3
	beq.n	.L14
	b.n	.L21
.L14:
	ldr	r2, [pc, #504]
	mov	r5, fp
	ldr	r3, [r5, r2]
	ldr	r3, [r3, #20]
	movs	r1, #0
	mov	r9, r1
	cmp	r3, #0
	bne.n	.L22
	b.n	.L23
.L22:
	movs	r0, #36
	movs	r1, #72
	str	r0, [sp, #20]
	str	r1, [sp, #24]
.L33:
	mov	r3, r9
	ldr	r5, [sp, #44]
	ldr	r0, [sp, #24]
	lsls	r3, r3, #4
	mov	r8, r3
	cmp	r5, r0
	bge.n	.L24
	b.n	.L25
.L24:
	mov	r1, fp
	adds	r5, r1, r2
	ldr	r3, [r5, #0]
	ldr	r2, [sp, #20]
	ldrsh	r0, [r3, r2]
	bl	Func_080b5098
	ldr	r6, [r0, #0]
	bl	Func_080049ac
	ldr	r0, [sp, #36]
	adds	r1, r0, #0
	adds	r1, #12
	bl	Func_080051d8
	ldr	r2, [sp, #44]
	ldr	r3, [sp, #24]
	cmp	r2, r3
	bne.n	.L26
	ldr	r3, [r5, #0]
	ldr	r1, [sp, #20]
	ldrsh	r0, [r3, r1]
	movs	r3, #0
	movs	r2, #1
	str	r3, [sp, #0]
	movs	r1, #1
	negs	r2, r2
	subs	r3, #1
	bl	Func_080d6888
.L26:
	ldr	r4, [sp, #24]
	ldr	r3, [sp, #44]
	adds	r4, #16
	cmp	r3, r4
	bne.n	.L27
	ldr	r3, [r5, #0]
	ldr	r5, [sp, #20]
	movs	r2, #1
	ldrsh	r0, [r3, r5]
	movs	r3, #0
	str	r3, [sp, #0]
	movs	r1, #0
	negs	r2, r2
	subs	r3, #1
	str	r4, [sp, #8]
	bl	Func_080d6888
	ldr	r4, [sp, #8]
.L27:
	ldr	r3, [r6, #8]
	add	r0, sp, #80
	str	r3, [r0, #0]
	movs	r3, #0
	str	r3, [r0, #4]
	ldr	r3, [r6, #16]
	add	r7, sp, #92
	str	r3, [r0, #8]
	adds	r1, r7, #0
	str	r4, [sp, #8]
	bl	Func_080e3944
	ldr	r3, [r7, #0]
	ldr	r5, [sp, #40]
	adds	r2, r3, r5
	str	r2, [r7, #0]
	mov	r3, r8
	ldr	r0, [sp, #44]
	adds	r3, #104
	ldr	r4, [sp, #8]
	cmp	r0, r3
	bge.n	.L25
	adds	r3, r0, #0
	cmp	r0, #0
	bge.n	.L28
	adds	r3, #3
.L28:
	asrs	r6, r3, #2
	ldr	r3, [sp, #44]
	movs	r1, #6
	mov	sl, r1
	cmp	r3, r4
	ble.n	.L29
	mov	r5, r8
	subs	r0, r3, r5
	movs	r1, #3
	subs	r0, #88
	str	r2, [sp, #12]
	bl	Func_080022ec
	mov	r1, sl
	subs	r1, r1, r0
	ldr	r2, [sp, #12]
	mov	sl, r1
.L29:
	cmp	r6, #2
	ble.n	.L30
	movs	r3, #1
	ands	r3, r6
	adds	r6, r3, #1
.L30:
	mov	r3, r8
	ldr	r5, [sp, #44]
	adds	r3, #100
	cmp	r5, r3
	bge.n	.L31
	ldr	r3, [pc, #264]
	ldr	r0, [pc, #268]
	ldrb	r4, [r3, r6]
	ldr	r3, [pc, #268]
	lsls	r5, r6, #1
	ldrh	r1, [r0, r5]
	ldrb	r0, [r3, r6]
	ldr	r3, [r7, #4]
	subs	r3, r3, r0
	subs	r2, r2, r4
	add	r1, fp
	adds	r3, #8
	str	r4, [sp, #0]
	str	r0, [sp, #4]
	ldr	r4, [sp, #52]
	ldr	r0, [sp, #48]
	bl	Func_080072f4
	ldr	r0, [pc, #232]
	ldr	r3, [pc, #236]
	ldrh	r1, [r0, r5]
	ldr	r5, [pc, #224]
	ldrb	r4, [r3, r6]
	ldrb	r0, [r5, r6]
	ldr	r3, [r7, #4]
	ldr	r2, [r7, #0]
	str	r0, [sp, #0]
	str	r4, [sp, #4]
	ldr	r0, [sp, #28]
	subs	r3, r3, r4
	add	r1, fp
	ldr	r4, [r0, #4]
	adds	r3, #8
	ldr	r0, [sp, #48]
	bl	Func_080072f4
.L31:
	ldr	r3, [r7, #4]
	movs	r1, #0
	mov	r8, r1
	cmp	r3, #0
	beq.n	.L25
	mov	r5, fp
	adds	r6, r7, #0
	adds	r5, #5
	movs	r7, #1
.L32:
	ldr	r2, [r6, #0]
	mov	r3, sl
	subs	r2, r2, r3
	str	r3, [sp, #0]
	ldr	r4, [sp, #52]
	mov	r3, r8
	str	r7, [sp, #4]
	ldr	r0, [sp, #48]
	adds	r1, r5, #0
	bl	Func_080072f4
	mov	r0, sl
	ldr	r2, [r6, #0]
	ldr	r1, [sp, #28]
	str	r0, [sp, #0]
	str	r7, [sp, #4]
	mov	r3, r8
	ldr	r4, [r1, #4]
	ldr	r0, [sp, #48]
	adds	r1, r5, #0
	bl	Func_080072f4
	movs	r2, #1
	ldr	r3, [r6, #4]
	add	r8, r2
	cmp	r8, r3
	bne.n	.L32
.L25:
	ldr	r3, [sp, #20]
	ldr	r5, [sp, #24]
	adds	r3, #2
	adds	r5, #16
	str	r3, [sp, #20]
	str	r5, [sp, #24]
	ldr	r2, [pc, #104]
	mov	r1, fp
	ldr	r3, [r1, r2]
	movs	r0, #1
	ldr	r3, [r3, #20]
	add	r9, r0
	cmp	r9, r3
	beq.n	.L23
	b.n	.L33
.L23:
	movs	r0, #47
	bl	Func_08002dd8
	movs	r0, #46
	bl	Func_08002dd8
	ldr	r2, [pc, #92]
	movs	r3, #1
	add	r2, fp
	str	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	ldr	r2, [sp, #44]
	ldr	r3, [pc, #60]
	adds	r2, #1
	str	r2, [sp, #44]
	add	r3, fp
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #20]
	lsls	r3, r3, #4
	adds	r3, #116
	cmp	r2, r3
	beq.n	.L7
	b.n	.L34
.L7:
	ldr	r0, [pc, #56]
	bl	Func_08004278
	bl	Func_080cdbc0
	add	sp, #116
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x080ee0a2
	.4byte 0x00002710
	.4byte 0xffff0000
	.4byte 0x00007828
	.4byte 0x080ee0b0
	.4byte 0x080ee0aa
	.4byte 0x080ee0b3
	.4byte 0x00007824
	.4byte 0x080cd261
