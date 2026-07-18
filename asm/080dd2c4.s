@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080dd2c4
	.thumb_func
Func_080dd2c4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #56
	str	r1, [sp, #44]
	ldr	r3, [pc, #60]
	ldmia	r3!, {r1}
	ldr	r3, [r3, #0]
	str	r3, [sp, #40]
	ldr	r3, [pc, #56]
	mov	fp, r1
	add	r3, fp
	str	r0, [r3, #0]
	movs	r0, #1
	bl	Func_080cd594
	ldr	r2, [pc, #44]
	ldr	r3, [pc, #28]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #28]
	adds	r2, #48
	strh	r3, [r2, #0]
	ldr	r2, [sp, #44]
	cmp	r2, #1
	bne.n	.L0
	ldr	r0, [pc, #32]
	mov	r1, fp
	movs	r2, #1
	movs	r3, #1
	bl	Func_080e0524
	b.n	.L1
	.4byte 0x00000100
	.4byte 0x00000000
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x04000020
	.4byte 0x00000083
.L0:
	ldr	r0, [pc, #360]
	mov	r1, fp
	movs	r2, #1
	movs	r3, #1
	bl	Func_080e0524
.L1:
	ldr	r7, [pc, #352]
	add	r7, fp
	ldr	r3, [r7, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #1
	bne.n	.L2
	ldr	r2, [pc, #344]
	ldr	r3, [pc, #348]
	str	r3, [r2, #0]
.L2:
	movs	r6, #1
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	str	r6, [sp, #0]
	bl	Func_080ed408
	ldr	r5, [pc, #332]
	adds	r3, r5, #0
	adds	r3, #184
	ldr	r3, [r3, #0]
	movs	r1, #7
	str	r3, [sp, #48]
	movs	r2, #7
	movs	r3, #7
	movs	r0, #47
	str	r6, [sp, #0]
	bl	Func_080ed408
	adds	r5, #188
	ldr	r3, [r5, #0]
	mov	r5, sp
	adds	r5, #48
	str	r5, [sp, #28]
	str	r3, [r5, #4]
	ldr	r3, [r7, #0]
	ldr	r2, [pc, #296]
	ldr	r3, [r3, #24]
	ldrb	r3, [r2, r3]
	lsls	r3, r3, #2
	adds	r3, #56
	str	r3, [sp, #32]
	movs	r6, #0
	movs	r1, #1
	movs	r2, #128
	ldr	r3, [pc, #284]
	mov	r9, r6
	negs	r1, r1
	lsls	r2, r2, #3
.L3:
	movs	r0, #1
	add	r9, r0
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r9, r2
	bne.n	.L3
	movs	r6, #225
	movs	r1, #0
	lsls	r6, r6, #7
	ldr	r7, [pc, #260]
	mov	r9, r1
	add	r6, fp
.L5:
	bl	Func_08004458
	ldr	r3, [pc, #256]
	mov	r5, r9
	ldrb	r2, [r3, r5]
	movs	r3, #7
	ands	r3, r0
	adds	r2, r2, r3
	lsrs	r3, r5, #31
	add	r3, r9
	asrs	r3, r3, #1
	adds	r3, #108
	subs	r2, #4
	str	r3, [r6, #4]
	str	r2, [r6, #0]
	bl	Func_08004458
	movs	r5, #63
	ands	r5, r0
	adds	r5, #55
	str	r5, [r6, #16]
	mov	r0, r9
	movs	r1, #3
	bl	Func_080022fc
	ldrb	r3, [r7, r0]
	cmp	r3, r5
	bge.n	.L4
	str	r3, [r6, #16]
.L4:
	mov	r0, r9
	movs	r1, #1
	lsls	r3, r0, #2
	add	r9, r1
	adds	r3, #8
	mov	r2, r9
	str	r3, [r6, #24]
	adds	r6, #28
	cmp	r2, #16
	bne.n	.L5
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, fp
	movs	r3, #1
	str	r3, [r2, #0]
	ldr	r2, [pc, #176]
	movs	r3, #0
	add	r2, fp
	movs	r1, #144
	str	r3, [r2, #0]
	ldr	r0, [pc, #168]
	lsls	r1, r1, #3
	bl	Func_080041d8
	ldr	r5, [sp, #32]
	movs	r3, #0
	str	r3, [sp, #36]
	cmp	r5, #0
	bne.n	.L6
	b.n	.L7
.L6:
	ldr	r6, [sp, #32]
	ldr	r0, [sp, #32]
	subs	r5, #64
	subs	r6, #20
	subs	r0, #4
	str	r5, [sp, #24]
	str	r6, [sp, #20]
	str	r0, [sp, #16]
.L33:
	ldr	r1, [sp, #36]
	ldr	r2, [sp, #24]
	cmp	r1, r2
	bne.n	.L8
	movs	r0, #132
	bl	Func_080b50e8
.L8:
	ldr	r3, [sp, #36]
	ldr	r5, [sp, #20]
	cmp	r3, r5
	blt.n	.L9
	ldr	r6, [sp, #16]
	cmp	r3, r6
	blt.n	.L10
	b.n	.L11
.L10:
	ldr	r2, [pc, #108]
	ldr	r3, [pc, #52]
	strh	r3, [r2, #0]
	ldr	r2, [sp, #36]
	ldr	r0, [sp, #32]
	subs	r3, r0, r2
	ldr	r2, [pc, #48]
	ldr	r1, [pc, #96]
	subs	r3, #5
	orrs	r3, r2
	strh	r3, [r1, #0]
.L9:
	ldr	r3, [sp, #36]
	ldr	r5, [sp, #16]
	cmp	r3, r5
	blt.n	.L12
	b.n	.L11
.L12:
	ldr	r3, [pc, #36]
	add	r3, fp
	ldr	r3, [r3, #0]
	ldr	r0, [pc, #44]
	ldr	r3, [r3, #24]
	ldrb	r3, [r0, r3]
	movs	r6, #0
	mov	r9, r6
	cmp	r3, #0
	bne.n	.L13
	b.n	.L11
.L13:
	b.n	.L14
	movs	r0, r0
	.4byte 0x00003f44
	.4byte 0x00001000
	.4byte 0x00000084
	.4byte 0x00007828
	.4byte 0x04000028
	.4byte 0xffff9000
	.4byte 0x03001e50
	.4byte 0x080eeb5e
	.4byte 0x02010018
	.4byte 0x080eeb4b
	.4byte 0x080eeb61
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x04000050
	.4byte 0x04000052
.L14:
	movs	r2, #225
	movs	r1, #8
	lsls	r2, r2, #7
	add	r2, fp
	str	r1, [sp, #12]
	mov	sl, r2
.L29:
	mov	r5, r9
	lsls	r3, r5, #2
	ldr	r6, [sp, #36]
	adds	r3, #9
	cmp	r6, r3
	bne.n	.L15
	ldr	r2, [pc, #604]
	movs	r3, #2
	add	r2, fp
	str	r3, [r2, #0]
.L15:
	ldr	r0, [sp, #36]
	ldr	r1, [sp, #12]
	cmp	r0, r1
	ble.n	.L16
	mov	r0, r9
	movs	r1, #3
	bl	Func_080022fc
	ldr	r5, [sp, #12]
	ldr	r2, [sp, #36]
	mov	r6, sl
	subs	r3, r2, r5
	lsls	r5, r3, #3
	ldr	r3, [r6, #16]
	adds	r4, r0, #0
	cmp	r5, r3
	ble.n	.L17
	adds	r5, r3, #0
.L17:
	ldr	r0, [sp, #44]
	cmp	r0, #0
	bne.n	.L18
	ldr	r2, [pc, #560]
	lsls	r3, r4, #1
	mov	r1, r9
	movs	r0, #1
	ands	r0, r1
	ldrh	r1, [r2, r3]
	ldr	r3, [pc, #552]
	mov	r6, sl
	ldrb	r4, [r3, r4]
	ldr	r2, [r6, #0]
	lsrs	r3, r4, #1
	subs	r2, r2, r3
	ldr	r3, [r6, #4]
	str	r5, [sp, #4]
	subs	r3, r3, r5
	str	r4, [sp, #0]
	ldr	r5, [sp, #28]
	lsls	r0, r0, #2
	ldr	r4, [r0, r5]
	add	r1, fp
	ldr	r0, [sp, #40]
	bl	Func_080072f4
	b.n	.L16
.L18:
	ldr	r2, [pc, #520]
	movs	r3, #7
	mov	r6, r9
	ands	r3, r6
	ldrsb	r3, [r2, r3]
	cmp	r5, r3
	ble.n	.L19
	adds	r5, r3, #0
.L19:
	ldr	r2, [pc, #508]
	lsls	r3, r4, #1
	mov	r1, r9
	movs	r0, #1
	ands	r0, r1
	ldrh	r1, [r2, r3]
	ldr	r3, [pc, #500]
	mov	r6, sl
	ldrb	r4, [r3, r4]
	ldr	r2, [r6, #0]
	lsrs	r3, r4, #1
	subs	r2, r2, r3
	ldr	r3, [r6, #4]
	str	r5, [sp, #4]
	subs	r3, r3, r5
	str	r4, [sp, #0]
	ldr	r5, [sp, #28]
	lsls	r0, r0, #2
	ldr	r4, [r0, r5]
	add	r1, fp
	ldr	r0, [sp, #40]
	bl	Func_080072f4
.L16:
	ldr	r2, [pc, #468]
	add	r2, fp
	ldr	r3, [r2, #0]
	ldr	r3, [r3, #20]
	movs	r6, #0
	cmp	r3, #0
	beq.n	.L20
	ldr	r7, [sp, #12]
	movs	r0, #1
	mov	r1, r9
	ands	r1, r0
	adds	r7, #4
	mov	r8, r1
	adds	r5, r2, #0
	movs	r4, #36
.L23:
	ldr	r2, [sp, #36]
	cmp	r2, r7
	bne.n	.L21
	mov	r3, r8
	cmp	r3, #0
	bne.n	.L22
	movs	r0, #133
	str	r4, [sp, #8]
	bl	Func_080f9010
	ldr	r4, [sp, #8]
.L22:
	ldr	r3, [r5, #0]
	ldrsh	r0, [r3, r4]
	movs	r3, #3
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	adds	r3, r6, #0
	str	r4, [sp, #8]
	bl	Func_080d6888
	ldr	r4, [sp, #8]
.L21:
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #20]
	adds	r6, #1
	adds	r4, #2
	cmp	r6, r3
	bne.n	.L23
	b.n	.L24
.L20:
	ldr	r7, [sp, #12]
	adds	r7, #4
.L24:
	ldr	r2, [sp, #36]
	cmp	r2, r7
	beq.n	.L25
	ldr	r3, [sp, #12]
	adds	r3, #8
	cmp	r2, r3
	bne.n	.L26
.L25:
	ldr	r5, [pc, #368]
	movs	r6, #0
	movs	r7, #15
	b.n	.L27
.L28:
	adds	r5, #28
	adds	r6, #1
.L27:
	movs	r3, #128
	lsls	r3, r3, #2
	cmp	r6, r3
	beq.n	.L26
	movs	r0, #1
	ldr	r3, [r5, #24]
	negs	r0, r0
	cmp	r3, r0
	bne.n	.L28
	bl	Func_08004458
	mov	r1, sl
	ldr	r3, [r1, #0]
	ands	r0, r7
	adds	r0, r0, r3
	subs	r0, #8
	str	r0, [r5, #0]
	bl	Func_08004458
	ands	r0, r7
	adds	r0, #80
	movs	r3, #0
	str	r0, [r5, #4]
	str	r3, [r5, #24]
.L26:
	ldr	r2, [sp, #12]
	movs	r3, #28
	adds	r2, #4
	add	sl, r3
	ldr	r3, [pc, #296]
	str	r2, [sp, #12]
	add	r3, fp
	ldr	r3, [r3, #0]
	ldr	r6, [pc, #296]
	ldr	r3, [r3, #24]
	movs	r5, #1
	ldrb	r3, [r6, r3]
	add	r9, r5
	cmp	r9, r3
	beq.n	.L11
	b.n	.L29
.L11:
	ldr	r1, [pc, #276]
	movs	r0, #0
	mov	r9, r0
	mov	r8, r1
.L32:
	mov	r3, r8
	ldr	r2, [r3, #24]
	cmp	r2, #0
	blt.n	.L30
	lsrs	r3, r2, #31
	ldr	r5, [pc, #264]
	ldr	r6, [sp, #44]
	adds	r3, r2, r3
	asrs	r7, r3, #1
	mov	sl, r5
	cmp	r6, #0
	beq.n	.L31
	ldr	r0, [pc, #256]
	mov	sl, r0
.L31:
	ldr	r3, [pc, #256]
	ldr	r2, [pc, #256]
	ldrsb	r5, [r3, r7]
	ldr	r3, [pc, #256]
	lsls	r6, r7, #1
	mov	r0, r8
	ldrh	r1, [r2, r6]
	ldr	r2, [r0, #0]
	ldrb	r0, [r3, r7]
	lsls	r0, r0, #24
	asrs	r4, r0, #24
	mov	r3, r8
	lsrs	r0, r0, #31
	ldr	r3, [r3, #4]
	adds	r0, r4, r0
	asrs	r0, r0, #1
	add	r1, sl
	subs	r2, r2, r5
	subs	r3, r3, r0
	str	r5, [sp, #0]
	str	r4, [sp, #4]
	add	r1, fp
	ldr	r4, [sp, #48]
	ldr	r0, [sp, #40]
	bl	Func_080072f4
	ldr	r3, [pc, #212]
	ldrb	r0, [r3, r7]
	ldr	r5, [pc, #204]
	lsls	r0, r0, #24
	ldrh	r1, [r5, r6]
	asrs	r4, r0, #24
	mov	r6, r8
	lsrs	r0, r0, #31
	ldr	r3, [r6, #4]
	adds	r0, r4, r0
	ldr	r5, [pc, #184]
	asrs	r0, r0, #1
	subs	r3, r3, r0
	ldrsb	r0, [r5, r7]
	ldr	r2, [r6, #0]
	str	r0, [sp, #0]
	str	r4, [sp, #4]
	ldr	r6, [sp, #28]
	add	r1, sl
	add	r1, fp
	ldr	r4, [r6, #4]
	ldr	r0, [sp, #40]
	bl	Func_080072f4
	mov	r0, r8
	ldr	r3, [r0, #24]
	adds	r3, #1
	str	r3, [r0, #24]
	cmp	r3, #14
	bne.n	.L30
	movs	r3, #1
	negs	r3, r3
	str	r3, [r0, #24]
.L30:
	movs	r2, #1
	movs	r3, #128
	movs	r1, #28
	add	r9, r2
	lsls	r3, r3, #2
	add	r8, r1
	cmp	r9, r3
	bne.n	.L32
	movs	r0, #4
	movs	r1, #4
	bl	Func_080e155c
	bl	Func_080cd52c
	ldr	r2, [pc, #120]
	movs	r3, #1
	add	r2, fp
	str	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	ldr	r5, [sp, #36]
	ldr	r6, [sp, #32]
	adds	r5, #1
	str	r5, [sp, #36]
	cmp	r5, r6
	beq.n	.L7
	b.n	.L33
.L7:
	ldr	r0, [pc, #96]
	bl	Func_08004278
	movs	r0, #47
	bl	Func_08002dd8
	movs	r0, #46
	bl	Func_08002dd8
	bl	Func_080cdbc0
	add	sp, #56
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x000077a8
	.4byte 0x080eeb4e
	.4byte 0x080eeb48
	.4byte 0x080eeb71
	.4byte 0x080eeb58
	.4byte 0x080eeb54
	.4byte 0x00007828
	.4byte 0x02010000
	.4byte 0x080eeb5e
	.4byte 0x00001e59
	.4byte 0x00000aff
	.4byte 0x080eeb79
	.4byte 0x080eeb88
	.4byte 0x080eeb80
	.4byte 0x00007824
	.4byte 0x080cd261
