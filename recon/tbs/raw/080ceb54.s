.syntax unified
	.thumb
	.set sub_080022fc, 0x080022fc
	.set sub_08002322, 0x08002322
	.set sub_08002dd8, 0x08002dd8
	.set sub_08002f40, 0x08002f40
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_080049ac, 0x080049ac
	.set sub_080051d8, 0x080051d8
	.set sub_08005340, 0x08005340
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.set sub_080b5070, 0x080b5070
	.set sub_080b5098, 0x080b5098
	.set sub_080b50e8, 0x080b50e8
	.set sub_080cd52c, 0x080cd52c
	.set sub_080cd594, 0x080cd594
	.set sub_080cdbc0, 0x080cdbc0
	.set sub_080cef64, 0x080cef64
	.set sub_080d6888, 0x080d6888
	.set sub_080e38b8, 0x080e38b8
	.set sub_080e3944, 0x080e3944
	.set sub_080f9010, 0x080f9010
	.global Func_080ceb54
	.thumb_func
Func_080ceb54:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #84
	ldr	r2, [pc, #520]
	str	r1, [sp, #60]
	adds	r3, r2, #0
	ldmia	r3!, {r1}
	ldr	r3, [r3, #0]
	str	r3, [sp, #56]
	subs	r2, #108
	ldr	r3, [pc, #512]
	ldr	r2, [r2, #0]
	mov	sl, r1
	add	r3, sl
	str	r2, [sp, #44]
	str	r0, [r3, #0]
	movs	r0, #1
	bl	sub_080cd594
	ldr	r0, [pc, #496]
	bl	sub_08002f40
	mov	r1, sl
	bl	sub_08005340
	ldr	r2, [sp, #60]
	cmp	r2, #0
	bne.n	.L_080ceb9a
	ldr	r0, [pc, #484]
	b.n	.L_080ceba6
.L_080ceb9a:
	ldr	r3, [sp, #60]
	cmp	r3, #1
	bne.n	.L_080ceba4
	ldr	r0, [pc, #476]
	b.n	.L_080ceba6
.L_080ceba4:
	ldr	r0, [pc, #476]
.L_080ceba6:
	bl	sub_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	movs	r2, #128
	ldr	r3, [pc, #468]
	lsls	r0, r0, #19
	bl	sub_080072f0
	ldr	r3, [pc, #440]
	add	r3, sl
	ldr	r3, [r3, #0]
	add	r1, sp, #64
	ldr	r0, [r3, #4]
	bl	sub_080cef64
	movs	r1, #1
	movs	r2, #128
	ldr	r3, [pc, #448]
	movs	r7, #0
	negs	r1, r1
	lsls	r2, r2, #3
.L_080cebd2:
	adds	r7, #1
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r7, r2
	bne.n	.L_080cebd2
	movs	r0, #0
	str	r0, [sp, #48]
	ldr	r2, [pc, #400]
	mov	r1, sl
	ldr	r3, [r1, r2]
	ldr	r3, [r3, #20]
	cmp	r3, #0
	beq.n	.L_080cec6c
	movs	r3, #255
	mov	r9, r3
	mov	fp, r0
.L_080cebf2:
	mov	r0, sl
	adds	r5, r0, r2
	ldr	r3, [r5, #0]
	ldr	r0, [r3, #8]
	bl	sub_080b5098
	ldr	r3, [r5, #0]
	ldr	r6, [r0, #0]
	ldr	r0, [r3, #8]
	bl	sub_080b5070
	ldr	r5, [pc, #388]
	mov	r8, r0
	movs	r7, #0
	add	r5, fp
.L_080cec10:
	ldr	r3, [r6, #8]
	mov	r1, r8
	str	r1, [r5, #4]
	str	r3, [r5, #0]
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	bl	sub_08004458
	mov	r2, r9
	ands	r0, r2
	subs	r0, #128
	lsls	r0, r0, #10
	str	r0, [r5, #12]
	bl	sub_08004458
	mov	r3, r9
	ands	r0, r3
	subs	r0, #128
	lsls	r0, r0, #10
	str	r0, [r5, #16]
	bl	sub_08004458
	mov	r1, r9
	ands	r0, r1
	subs	r0, #128
	lsls	r0, r0, #10
	movs	r3, #0
	adds	r7, #1
	str	r0, [r5, #20]
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r7, #128
	bne.n	.L_080cec10
	ldr	r3, [sp, #48]
	movs	r2, #224
	lsls	r2, r2, #4
	adds	r3, #1
	str	r3, [sp, #48]
	add	fp, r2
	ldr	r2, [pc, #276]
	mov	r0, sl
	ldr	r3, [r0, r2]
	ldr	r1, [sp, #48]
	ldr	r3, [r3, #20]
	cmp	r1, r3
	bne.n	.L_080cebf2
.L_080cec6c:
	movs	r5, #144
	lsls	r5, r5, #3
	adds	r1, r5, #0
	ldr	r0, [pc, #288]
	bl	sub_080041d8
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, sl
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #276]
	movs	r3, #75
	add	r2, sl
	str	r3, [r2, #0]
	adds	r1, r5, #0
	ldr	r0, [pc, #268]
	bl	sub_080041d8
	movs	r0, #146
	bl	sub_080f9010
	ldr	r3, [sp, #60]
	movs	r2, #0
	lsls	r3, r3, #1
	str	r2, [sp, #52]
	str	r3, [sp, #28]
	ldr	r2, [pc, #252]
	adds	r3, #1
	ldrb	r1, [r2, r3]
	ldr	r3, [pc, #200]
	add	r3, sl
	ldr	r3, [r3, #0]
	ldr	r2, [r3, #20]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #2
	cmn	r1, r3
	bne.n	.L_080cecbc
	b.n	.L_080cef14
.L_080cecbc:
	ldr	r0, [sp, #44]
	adds	r0, #12
	str	r0, [sp, #24]
.L_080cecc2:
	ldr	r1, [sp, #52]
	cmp	r1, #80
	bne.n	.L_080cecdc
	ldr	r2, [sp, #60]
	cmp	r2, #0
	bne.n	.L_080cecd6
	movs	r0, #134
	bl	sub_080b50e8
	b.n	.L_080cecdc
.L_080cecd6:
	movs	r0, #133
	bl	sub_080b50e8
.L_080cecdc:
	bl	sub_080049ac
	ldr	r0, [sp, #44]
	ldr	r1, [sp, #24]
	bl	sub_080051d8
	movs	r6, #211
	ldr	r0, [sp, #52]
	lsls	r6, r6, #7
	movs	r3, #128
	add	r6, sl
	movs	r7, #0
	lsls	r3, r3, #13
	lsls	r5, r0, #10
.L_080cecf8:
	adds	r0, r5, #0
	str	r3, [sp, #8]
	bl	sub_08002322
	ldr	r3, [sp, #8]
	lsls	r0, r0, #4
	subs	r0, r3, r0
	movs	r1, #128
	asrs	r0, r0, #10
	lsls	r1, r1, #3
	adds	r7, #1
	stmia	r6!, {r0}
	adds	r5, r5, r1
	cmp	r7, #160
	bne.n	.L_080cecf8
	movs	r2, #0
	str	r2, [sp, #48]
	ldr	r2, [pc, #88]
	mov	r0, sl
	ldr	r3, [r0, r2]
	ldr	r3, [r3, #20]
	cmp	r3, #0
	bne.n	.L_080ced28
	b.n	.L_080ceedc
.L_080ced28:
	movs	r1, #0
	movs	r3, #36
	str	r1, [sp, #20]
	str	r3, [sp, #16]
	str	r1, [sp, #12]
	mov	fp, r1
.L_080ced34:
	mov	r0, sl
	adds	r5, r0, r2
	ldr	r1, [sp, #16]
	ldr	r3, [r5, #0]
	ldrsh	r0, [r3, r1]
	bl	sub_080b5098
	ldr	r3, [r5, #0]
	ldr	r0, [r0, #0]
	ldr	r1, [sp, #16]
	mov	r9, r0
	ldrsh	r0, [r3, r1]
	bl	sub_080b5070
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	asrs	r0, r0, #1
	str	r0, [sp, #40]
	mov	r3, fp
	ldr	r0, [sp, #52]
	adds	r3, #71
	cmp	r0, r3
	bne.n	.L_080cedaa
	ldr	r1, [sp, #60]
	cmp	r1, #0
	bne.n	.L_080ceda4
	movs	r0, #134
	bl	sub_080f9010
	b.n	.L_080cedaa
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x00000069
	.4byte 0x000000bb
	.4byte 0x0000008d
	.4byte 0x00000091
	.4byte 0x03001388
	.4byte 0x02010018
	.4byte 0x02010000
	.4byte 0x080dbb9d
	.4byte 0x00007784
	.4byte 0x080cd261
	.2byte 0xe090
	.2byte 0x080e
.L_080ceda4:
	movs	r0, #133
	bl	sub_080f9010
.L_080cedaa:
	mov	r3, fp
	ldr	r2, [sp, #52]
	adds	r3, #70
	cmp	r2, r3
	bne.n	.L_080cedcc
	ldr	r3, [pc, #396]
	add	r3, sl
	ldr	r3, [r3, #0]
	ldr	r1, [sp, #16]
	ldrsh	r0, [r3, r1]
	movs	r3, #26
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	ldr	r3, [sp, #48]
	bl	sub_080d6888
.L_080cedcc:
	ldr	r3, [sp, #52]
	cmp	r3, fp
	ble.n	.L_080ceeac
	ldr	r2, [pc, #372]
	ldr	r0, [sp, #28]
	ldrb	r3, [r2, r0]
	movs	r7, #0
	cmp	r3, #0
	beq.n	.L_080ceeac
	ldr	r1, [sp, #20]
	add	r3, sp, #72
	str	r1, [sp, #36]
	mov	r8, r3
	mov	r0, fp
	ldr	r1, [sp, #12]
	ldr	r3, [pc, #352]
	str	r0, [sp, #32]
	adds	r6, r1, r3
.L_080cedf0:
	ldr	r0, [sp, #36]
	lsls	r3, r0, #1
	adds	r3, r3, r7
	ldr	r1, [sp, #52]
	lsls	r3, r3, #1
	cmp	r1, r3
	ble.n	.L_080ceea0
	ldr	r3, [r6, #24]
	cmp	r3, #0
	blt.n	.L_080ceea0
	mov	r1, r8
	adds	r0, r6, #0
	bl	sub_080e3944
	mov	r2, r8
	ldr	r5, [r2, #0]
	asrs	r5, r5, #1
	str	r5, [r2, #0]
	movs	r1, #3
	adds	r0, r7, #0
	bl	sub_080022fc
	lsls	r1, r0, #2
	adds	r1, r1, r0
	mov	r0, r8
	ldr	r3, [r0, #4]
	movs	r2, #20
	str	r2, [sp, #0]
	lsls	r1, r1, #7
	movs	r2, #32
	subs	r5, #10
	subs	r3, #16
	str	r2, [sp, #4]
	add	r1, sl
	ldr	r4, [sp, #64]
	ldr	r0, [sp, #56]
	adds	r2, r5, #0
	bl	sub_080072f4
	movs	r1, #62
	movs	r2, #0
	adds	r0, r6, #0
	bl	sub_080e38b8
	ldr	r1, [sp, #32]
	ldr	r2, [sp, #52]
	adds	r3, r1, r7
	adds	r3, #30
	cmp	r2, r3
	ble.n	.L_080cee9e
	mov	r3, r9
	ldr	r0, [r3, #8]
	ldr	r3, [r6, #0]
	mov	r2, r9
	ldr	r1, [r2, #12]
	subs	r0, r0, r3
	ldr	r3, [sp, #40]
	adds	r1, r1, r3
	ldr	r3, [r6, #4]
	ldr	r2, [r2, #16]
	subs	r1, r1, r3
	ldr	r3, [r6, #8]
	subs	r2, r2, r3
	ldr	r3, [r6, #12]
	asrs	r0, r0, #9
	adds	r3, r3, r0
	str	r3, [r6, #12]
	ldr	r3, [r6, #16]
	asrs	r1, r1, #9
	adds	r3, r3, r1
	ldr	r1, [pc, #208]
	str	r3, [r6, #16]
	ldr	r3, [r6, #20]
	asrs	r2, r2, #9
	adds	r0, r0, r1
	ldr	r1, [pc, #204]
	adds	r3, r3, r2
	str	r3, [r6, #20]
	cmp	r0, r1
	bhi.n	.L_080cee9e
	ldr	r0, [pc, #188]
	adds	r3, r2, r0
	cmp	r3, r1
	bhi.n	.L_080cee9e
	movs	r3, #1
	negs	r3, r3
	str	r3, [r6, #24]
.L_080cee9e:
	ldr	r2, [pc, #168]
.L_080ceea0:
	ldr	r1, [sp, #28]
	ldrb	r3, [r2, r1]
	adds	r7, #1
	adds	r6, #28
	cmp	r7, r3
	bne.n	.L_080cedf0
.L_080ceeac:
	ldr	r2, [sp, #20]
	ldr	r3, [sp, #16]
	adds	r2, #5
	adds	r3, #2
	str	r2, [sp, #20]
	str	r3, [sp, #16]
	ldr	r1, [sp, #12]
	ldr	r3, [sp, #48]
	movs	r2, #224
	lsls	r2, r2, #4
	adds	r1, r1, r2
	adds	r3, #1
	str	r1, [sp, #12]
	str	r3, [sp, #48]
	movs	r0, #20
	ldr	r2, [pc, #120]
	add	fp, r0
	mov	r0, sl
	ldr	r3, [r0, r2]
	ldr	r1, [sp, #48]
	ldr	r3, [r3, #20]
	cmp	r1, r3
	beq.n	.L_080ceedc
	b.n	.L_080ced34
.L_080ceedc:
	bl	sub_080cd52c
	ldr	r2, [pc, #116]
	movs	r3, #1
	add	r2, sl
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r2, [sp, #52]
	ldr	r3, [sp, #28]
	adds	r2, #1
	str	r2, [sp, #52]
	ldr	r2, [pc, #80]
	adds	r3, #1
	ldrb	r1, [r2, r3]
	ldr	r3, [pc, #68]
	add	r3, sl
	ldr	r3, [r3, #0]
	ldr	r2, [r3, #20]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r1, r1, r3
	ldr	r3, [sp, #52]
	cmp	r3, r1
	beq.n	.L_080cef14
	b.n	.L_080cecc2
.L_080cef14:
	ldr	r0, [pc, #68]
	bl	sub_08004278
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	ldr	r0, [pc, #56]
	bl	sub_08004278
	bl	sub_080cdbc0
	add	sp, #84
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x00007828
	.4byte 0x080ee090
	.4byte 0x02010000
	.4byte 0x00000fff
	.4byte 0x00001ffe
	.4byte 0x00007824
	.4byte 0x080cd261
	.4byte 0x080dbb9d
