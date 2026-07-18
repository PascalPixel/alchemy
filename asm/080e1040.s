@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080e1040
	.thumb_func
Func_080e1040:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r2, [pc, #92]
	adds	r3, r2, #0
	adds	r6, r0, #0
	ldmia	r3!, {r0}
	ldr	r3, [r3, #0]
	sub	sp, #72
	str	r3, [sp, #48]
	subs	r2, #108
	ldr	r5, [pc, #80]
	ldr	r2, [r2, #0]
	mov	r9, r0
	add	r5, r9
	str	r2, [sp, #28]
	movs	r0, #0
	str	r6, [r5, #0]
	bl	Func_080cd594
	ldr	r3, [r5, #0]
	ldr	r2, [r3, #4]
	add	r3, sp, #56
	str	r3, [sp, #0]
	add	r3, sp, #52
	str	r3, [sp, #4]
	adds	r0, r6, #0
	movs	r3, #2
	movs	r1, #0
	bl	Func_080de2f8
	ldr	r3, [pc, #32]
	ldr	r2, [pc, #40]
	strh	r3, [r2, #0]
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #1
	bne.n	.L0
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r0, #46
	movs	r1, #7
	movs	r2, #7
	movs	r3, #7
	bl	Func_080ed408
	b.n	.L1
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x04000052
.L0:
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r0, #46
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	bl	Func_080ed408
.L1:
	ldr	r3, [pc, #724]
	adds	r3, #184
	ldr	r3, [r3, #0]
	ldr	r0, [pc, #724]
	str	r3, [sp, #32]
	mov	r1, r9
	movs	r2, #1
	movs	r3, #0
	bl	Func_080e0524
	ldr	r1, [pc, #712]
	ldr	r0, [pc, #716]
	add	r1, r9
	movs	r2, #1
	movs	r3, #1
	bl	Func_080e0524
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, r9
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #696]
	ldr	r5, [pc, #700]
	add	r2, r9
	movs	r3, #75
	movs	r1, #144
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	add	r5, r9
	ldr	r0, [pc, #688]
	bl	Func_080041d8
	ldr	r3, [r5, #0]
	ldr	r0, [r3, #8]
	bl	Func_080b5098
	ldr	r3, [r5, #0]
	ldr	r6, [r0, #0]
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	bl	Func_080b5098
	ldr	r0, [r0, #0]
	movs	r2, #0
	movs	r5, #225
	lsls	r5, r5, #7
	str	r0, [sp, #24]
	str	r2, [sp, #44]
	movs	r7, #0
	add	r5, r9
.L3:
	ldr	r3, [r6, #8]
	str	r3, [r5, #0]
	movs	r3, #132
	lsls	r3, r3, #15
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	asrs	r3, r7, #5
	str	r3, [r5, #12]
	bl	Func_08004458
	movs	r3, #127
	ands	r3, r0
	subs	r3, #64
	lsls	r3, r3, #16
	asrs	r3, r3, #6
	str	r3, [r5, #16]
	bl	Func_08004458
	movs	r3, #255
	ands	r3, r0
	subs	r3, #127
	lsls	r3, r3, #16
	asrs	r3, r3, #5
	str	r3, [r5, #20]
	ldr	r3, [r5, #0]
	cmp	r3, #0
	ble.n	.L2
	ldr	r3, [r5, #12]
	negs	r3, r3
	str	r3, [r5, #12]
.L2:
	movs	r3, #1
	str	r3, [r5, #24]
	ldr	r4, [sp, #44]
	movs	r3, #160
	lsls	r3, r3, #15
	adds	r4, #1
	adds	r7, r7, r3
	adds	r5, #28
	str	r4, [sp, #44]
	cmp	r4, #8
	bne.n	.L3
	ldr	r0, [sp, #28]
	movs	r5, #0
	adds	r0, #12
	str	r5, [sp, #40]
	str	r0, [sp, #16]
.L30:
	ldr	r1, [sp, #40]
	cmp	r1, #16
	ble.n	.L4
	ldr	r0, [pc, #528]
	bl	Func_080e46f0
.L4:
	ldr	r6, [pc, #536]
	add	r6, r9
	ldr	r3, [r6, #0]
	ldr	r3, [r3, #28]
	cmp	r3, #1
	bne.n	.L5
	ldr	r2, [sp, #40]
	lsls	r5, r2, #11
	adds	r0, r5, #0
	bl	Func_08002322
	ldr	r3, [sp, #56]
	negs	r0, r0
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	lsls	r0, r0, #2
	asrs	r3, r3, #1
	asrs	r0, r0, #16
	adds	r0, r0, r3
	adds	r7, r0, #0
	adds	r0, r5, #0
	bl	Func_0800231c
	ldr	r3, [sp, #52]
	lsls	r0, r0, #1
	asrs	r0, r0, #16
	adds	r0, r0, r3
	ldr	r3, [sp, #40]
	adds	r5, r0, #0
	subs	r7, #10
	subs	r5, #22
	cmp	r3, #16
	ble.n	.L6
	lsls	r3, r3, #1
	subs	r3, r5, r3
	adds	r5, r3, #0
	adds	r5, #32
.L6:
	ldr	r3, [r6, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #1
	bne.n	.L7
	movs	r3, #3
	str	r3, [sp, #0]
	movs	r0, #47
	movs	r1, #7
	movs	r2, #7
	movs	r3, #7
	bl	Func_080ed408
	b.n	.L8
.L7:
	movs	r3, #3
	movs	r0, #47
	movs	r1, #7
	movs	r2, #7
	str	r3, [sp, #0]
	bl	Func_080ed408
.L8:
	ldr	r4, [sp, #40]
	cmp	r4, #3
	bgt.n	.L9
	movs	r3, #20
	str	r3, [sp, #0]
	movs	r3, #40
	str	r3, [sp, #4]
	ldr	r0, [pc, #416]
	ldr	r1, [pc, #396]
	ldr	r4, [r0, #0]
	add	r1, r9
	ldr	r0, [sp, #48]
	adds	r2, r7, #0
	adds	r3, r5, #0
	bl	Func_080072f4
.L9:
	movs	r0, #47
	bl	Func_08002dd8
	movs	r3, #20
	ldr	r1, [pc, #372]
	str	r3, [sp, #0]
	movs	r3, #40
	str	r3, [sp, #4]
	ldr	r0, [sp, #48]
	add	r1, r9
	adds	r2, r7, #0
	adds	r3, r5, #0
	ldr	r4, [sp, #32]
	bl	Func_080072f4
.L5:
	ldr	r5, [sp, #40]
	movs	r3, #1
	ands	r3, r5
	cmp	r3, #0
	bne.n	.L10
	movs	r0, #0
	movs	r5, #232
	lsls	r5, r5, #7
	str	r0, [sp, #44]
	ldr	r6, [pc, #356]
	add	r5, r9
.L11:
	bl	Func_08004458
	movs	r1, #6
	bl	Func_08002304
	adds	r0, #3
	str	r0, [r5, #12]
	bl	Func_08004458
	movs	r3, #3
	ands	r3, r0
	ldrb	r3, [r6, r3]
	str	r3, [r5, #16]
	ldr	r1, [sp, #44]
	adds	r1, #1
	adds	r5, #28
	str	r1, [sp, #44]
	cmp	r1, #32
	bne.n	.L11
.L10:
	bl	Func_080049ac
	ldr	r0, [sp, #28]
	ldr	r1, [sp, #16]
	bl	Func_080051d8
	movs	r6, #225
	movs	r2, #0
	mov	r3, r9
	lsls	r6, r6, #7
	str	r2, [sp, #44]
	str	r2, [sp, #12]
	str	r3, [sp, #8]
	add	r6, r9
.L28:
	ldr	r3, [r6, #24]
	cmp	r3, #1
	beq.n	.L12
	b.n	.L13
.L12:
	ldr	r4, [sp, #12]
	ldr	r5, [sp, #40]
	str	r4, [sp, #20]
	cmp	r5, r4
	bgt.n	.L14
	b.n	.L15
.L14:
	add	r5, sp, #60
	adds	r1, r5, #0
	adds	r0, r6, #0
	bl	Func_080e3944
	ldr	r3, [r5, #0]
	asrs	r3, r3, #1
	str	r3, [r5, #0]
	subs	r3, #12
	mov	sl, r3
	ldr	r3, [r5, #4]
	subs	r3, #24
	mov	r8, r3
	movs	r3, #24
	str	r3, [sp, #0]
	movs	r3, #48
	str	r3, [sp, #4]
	ldr	r5, [sp, #32]
	mov	r3, r8
	ldr	r0, [sp, #48]
	mov	r1, r9
	mov	r2, sl
	bl	Func_080072f8
	ldr	r0, [sp, #40]
	movs	r3, #3
	ands	r3, r0
	cmp	r3, #1
	bgt.n	.L16
	ldr	r3, [pc, #212]
	ldr	r4, [pc, #212]
	ldrh	r1, [r3, #2]
	ldr	r3, [pc, #212]
	ldrb	r2, [r3, #1]
	ldrb	r3, [r4, #1]
	ldr	r4, [pc, #212]
	ldrb	r0, [r4, #1]
	str	r0, [sp, #0]
	ldr	r0, [pc, #208]
	ldrb	r0, [r0, #1]
	add	r1, r9
	str	r0, [sp, #4]
	add	r2, sl
	add	r3, r8
	ldr	r0, [sp, #48]
	bl	Func_080072f8
	b.n	.L17
.L16:
	ldr	r3, [pc, #172]
	ldr	r0, [pc, #176]
	ldrh	r1, [r3, #4]
	ldr	r4, [pc, #180]
	ldr	r3, [pc, #172]
	ldrb	r2, [r3, #2]
	ldrb	r3, [r0, #2]
	ldrb	r0, [r4, #2]
	str	r0, [sp, #0]
	ldr	r0, [pc, #172]
	ldrb	r0, [r0, #2]
	add	r1, r9
	str	r0, [sp, #4]
	add	r2, sl
	add	r3, r8
	ldr	r0, [sp, #48]
	bl	Func_080072f8
.L17:
	ldr	r0, [sp, #8]
	movs	r1, #232
	movs	r5, #0
	lsls	r1, r1, #7
	mov	fp, r5
	adds	r7, r0, r1
.L22:
	movs	r2, #2
	ldr	r3, [r7, #16]
	movs	r1, #7
	str	r2, [sp, #0]
	movs	r0, #47
	movs	r2, #7
	bl	Func_080ed408
	ldr	r2, [pc, #100]
	ldr	r1, [r7, #16]
	ldr	r2, [r2, #0]
	movs	r3, #4
	ands	r3, r1
	str	r2, [sp, #36]
	cmp	r3, #0
	beq.n	.L18
	ldr	r0, [r7, #12]
	ldr	r4, [pc, #104]
	ldrb	r3, [r4, r0]
	mov	r5, sl
	subs	r3, r5, r3
	ldr	r5, [pc, #92]
	ldrb	r2, [r5, r0]
	subs	r3, r3, r2
	adds	r3, #24
	b.n	.L19
.L18:
	ldr	r0, [r7, #12]
	ldr	r2, [pc, #80]
	ldrb	r3, [r2, r0]
	ldr	r4, [pc, #80]
	add	r3, sl
.L19:
	mov	ip, r3
	movs	r3, #8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L20
	ldr	r5, [pc, #68]
	ldrb	r3, [r5, r0]
	ldr	r5, [pc, #52]
	mov	r1, r8
	ldrb	r2, [r5, r0]
	subs	r3, r1, r3
	subs	r3, r3, r2
	adds	r5, r3, #0
	adds	r5, #48
	b.n	.L21
	.4byte 0x03001e50
	.4byte 0x000000a7
	.4byte 0x000065c0
	.4byte 0x00000094
	.4byte 0x00007784
	.4byte 0x00007828
	.4byte 0x080cd261
	.4byte 0x03001f0c
	.4byte 0x080eec70
	.4byte 0x080eec86
	.4byte 0x080eeca1
	.4byte 0x080eec98
	.4byte 0x080eec74
	.4byte 0x080eec7d
.L20:
	ldr	r1, [pc, #352]
	ldrb	r3, [r1, r0]
	mov	r2, r8
	adds	r5, r2, r3
.L21:
	ldr	r2, [pc, #348]
	lsls	r3, r0, #1
	ldrh	r1, [r2, r3]
	ldrb	r3, [r4, r0]
	str	r3, [sp, #0]
	ldr	r3, [r7, #12]
	ldr	r4, [pc, #340]
	ldrb	r3, [r4, r3]
	add	r1, r9
	str	r3, [sp, #4]
	ldr	r0, [sp, #48]
	adds	r3, r5, #0
	mov	r2, ip
	ldr	r5, [sp, #36]
	bl	Func_080072f8
	movs	r0, #47
	bl	Func_08002dd8
	movs	r0, #1
	add	fp, r0
	mov	r1, fp
	adds	r7, #28
	cmp	r1, #4
	bne.n	.L22
	ldr	r3, [r6, #0]
	ldr	r2, [r6, #12]
	adds	r3, r3, r2
	str	r3, [r6, #0]
	ldr	r2, [r6, #16]
	ldr	r3, [r6, #4]
	adds	r3, r3, r2
	str	r3, [r6, #4]
	ldr	r2, [r6, #20]
	ldr	r3, [r6, #8]
	adds	r3, r3, r2
	str	r3, [r6, #8]
.L15:
	ldr	r3, [sp, #12]
	ldr	r2, [sp, #40]
	adds	r3, #16
	cmp	r2, r3
	ble.n	.L13
	ldr	r4, [sp, #24]
	ldr	r2, [r6, #0]
	ldr	r3, [r4, #8]
	subs	r3, r3, r2
	ldr	r2, [r6, #12]
	asrs	r3, r3, #8
	adds	r1, r2, r3
	ldr	r2, [r6, #4]
	movs	r3, #160
	lsls	r3, r3, #13
	subs	r3, r3, r2
	ldr	r2, [r6, #16]
	asrs	r3, r3, #8
	adds	r0, r2, r3
	str	r1, [r6, #12]
	str	r0, [r6, #16]
	ldr	r2, [r6, #8]
	ldr	r3, [r4, #16]
	subs	r3, r3, r2
	ldr	r2, [r6, #20]
	asrs	r3, r3, #8
	adds	r4, r2, r3
	str	r4, [r6, #20]
	ldr	r3, [sp, #20]
	ldr	r5, [sp, #40]
	adds	r3, #85
	cmp	r5, r3
	bge.n	.L23
	lsls	r3, r1, #4
	subs	r3, r3, r1
	lsls	r2, r3, #2
	cmp	r2, #0
	bge.n	.L24
	adds	r2, #63
.L24:
	asrs	r3, r2, #6
	str	r3, [r6, #12]
	lsls	r3, r0, #4
	subs	r3, r3, r0
	lsls	r2, r3, #2
	cmp	r2, #0
	bge.n	.L25
	adds	r2, #63
.L25:
	asrs	r3, r2, #6
	str	r3, [r6, #16]
	lsls	r3, r4, #4
	subs	r3, r3, r4
	lsls	r2, r3, #2
	cmp	r2, #0
	bge.n	.L26
	adds	r2, #63
.L26:
	asrs	r3, r2, #6
	str	r3, [r6, #20]
.L23:
	ldr	r3, [r6, #4]
	ldr	r0, [pc, #168]
	cmp	r3, r0
	bgt.n	.L13
	ldr	r2, [pc, #164]
	movs	r3, #8
	add	r2, r9
	str	r3, [r2, #0]
	movs	r3, #0
	str	r3, [r6, #24]
	movs	r0, #134
	bl	Func_080f9010
	ldr	r5, [pc, #152]
	add	r5, r9
	ldr	r3, [r5, #0]
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	movs	r3, #4
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	movs	r3, #0
	bl	Func_080d6888
	ldr	r3, [r5, #0]
	movs	r1, #4
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	bl	Func_080b5088
.L13:
	ldr	r3, [sp, #12]
	ldr	r4, [sp, #8]
	ldr	r5, [sp, #44]
	adds	r3, #2
	adds	r4, #112
	adds	r5, #1
	str	r3, [sp, #12]
	adds	r6, #28
	str	r4, [sp, #8]
	str	r5, [sp, #44]
	cmp	r5, #6
	beq.n	.L27
	b.n	.L28
.L27:
	movs	r0, #16
	movs	r1, #16
	bl	Func_080e155c
	bl	Func_080cd52c
	ldr	r2, [pc, #80]
	movs	r3, #1
	add	r2, r9
	movs	r0, #1
	str	r3, [r2, #0]
	bl	Func_080030f8
	ldr	r0, [sp, #40]
	adds	r0, #1
	str	r0, [sp, #40]
	cmp	r0, #96
	beq.n	.L29
	b.n	.L30
.L29:
	ldr	r0, [pc, #60]
	bl	Func_08004278
	movs	r0, #46
	bl	Func_08002dd8
	bl	Func_080cdbc0
	add	sp, #72
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x080eeca1
	.4byte 0x080eec86
	.4byte 0x080eec7d
	.4byte 0x0013ffff
	.4byte 0x000077a8
	.4byte 0x00007828
	.4byte 0x00007824
	.4byte 0x080cd261
