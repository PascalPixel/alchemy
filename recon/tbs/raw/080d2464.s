.syntax unified
	.thumb
	.set sub_0800231c, 0x0800231c
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
	.set sub_080072fc, 0x080072fc
	.set sub_08009020, 0x08009020
	.set sub_08009038, 0x08009038
	.set sub_080b5088, 0x080b5088
	.set sub_080b50e8, 0x080b50e8
	.set sub_080cd52c, 0x080cd52c
	.set sub_080cd594, 0x080cd594
	.set sub_080cdbc0, 0x080cdbc0
	.set sub_080d6750, 0x080d6750
	.set sub_080d6888, 0x080d6888
	.set sub_080dbb24, 0x080dbb24
	.set sub_080e155c, 0x080e155c
	.set sub_080e396c, 0x080e396c
	.set sub_080e6d3c, 0x080e6d3c
	.set sub_080e6eac, 0x080e6eac
	.set sub_080e727c, 0x080e727c
	.set sub_080ed408, 0x080ed408
	.set sub_080f9010, 0x080f9010
	.global BattleEffect_RunPaletteParticles
	.global Func_080d2464
	.thumb_func
BattleEffect_RunPaletteParticles:
Func_080d2464:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #72
	str	r1, [sp, #56]
	ldr	r5, [pc, #104]
	ldr	r1, [r5, #0]
	adds	r3, r5, #0
	str	r1, [sp, #52]
	subs	r3, #112
	ldr	r3, [r3, #0]
	str	r3, [sp, #48]
	subs	r3, r5, #4
	ldr	r3, [r3, #0]
	ldr	r7, [pc, #88]
	mov	sl, r3
	ldr	r2, [r5, #4]
	add	r7, sl
	str	r2, [sp, #32]
	str	r0, [r7, #0]
	movs	r0, #0
	bl	sub_080cd594
	ldr	r2, [pc, #76]
	ldr	r3, [pc, #56]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #56]
	subs	r2, #70
	strh	r3, [r2, #0]
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	mov	r8, r3
	movs	r0, #46
	movs	r3, #3
	bl	sub_080ed408
	ldr	r4, [r5, #24]
	movs	r3, #3
	movs	r1, #7
	movs	r2, #7
	movs	r0, #47
	str	r4, [sp, #40]
	str	r3, [sp, #0]
	bl	sub_080ed408
	ldr	r5, [r5, #28]
	ldr	r0, [pc, #28]
	str	r5, [sp, #44]
	bl	sub_08002f40
	adds	r5, r0, #0
	b.n	.L_080d24f0
	.4byte 0x00001010
	.4byte 0x00000784
	.4byte 0x03001ef0
	.4byte 0x00007828
	.4byte 0x04000052
	.2byte 0x007d
	.2byte 0x0000
.L_080d24f0:
	movs	r0, #160
	movs	r2, #128
	adds	r1, r5, #0
	ldr	r6, [pc, #848]
	adds	r5, #128
	lsls	r0, r0, #19
	bl	sub_080072fc
	mov	r1, sl
	adds	r0, r5, #0
	bl	sub_08005340
	ldr	r0, [pc, #832]
	bl	sub_08002f40
	movs	r1, #192
	adds	r5, r0, #0
	adds	r5, #128
	lsls	r1, r1, #6
	add	r1, sl
	adds	r0, r5, #0
	bl	sub_08005340
	ldr	r0, [pc, #816]
	bl	sub_08002f40
	ldr	r1, [sp, #32]
	bl	sub_08005340
	ldr	r0, [sp, #56]
	cmp	r0, #1
	bne.n	.L_080d2542
	ldr	r0, [pc, #800]
	bl	sub_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	lsls	r0, r0, #19
	movs	r2, #128
	bl	sub_080072fc
.L_080d2542:
	movs	r3, #239
	lsls	r3, r3, #7
	ldr	r2, [pc, #784]
	add	r3, sl
	mov	r1, r8
	str	r1, [r3, #0]
	add	r2, sl
	movs	r3, #75
	movs	r1, #144
	str	r3, [r2, #0]
	ldr	r0, [pc, #772]
	lsls	r1, r1, #3
	bl	sub_080041d8
	ldr	r3, [sp, #56]
	movs	r2, #1
	str	r2, [sp, #20]
	cmp	r3, #0
	beq.n	.L_080d257a
	ldr	r3, [r7, #0]
	movs	r4, #1
	ldr	r3, [r3, #4]
	negs	r4, r4
	str	r4, [sp, #20]
	cmp	r3, #1
	beq.n	.L_080d257a
	movs	r6, #1
	str	r6, [sp, #20]
.L_080d257a:
	ldr	r0, [sp, #56]
	cmp	r0, #1
	bne.n	.L_080d25ae
	ldr	r5, [pc, #732]
	add	r5, sl
	ldr	r3, [r5, #0]
	add	r6, sp, #60
	ldr	r0, [r3, #8]
	adds	r1, r6, #0
	bl	sub_080e396c
	ldr	r3, [r6, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r6, #0]
	movs	r3, #66
	str	r3, [r6, #4]
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #1
	bne.n	.L_080d25aa
	movs	r3, #76
	b.n	.L_080d25ac
.L_080d25aa:
	movs	r3, #44
.L_080d25ac:
	str	r3, [r6, #0]
.L_080d25ae:
	ldr	r2, [pc, #692]
	movs	r1, #212
	lsls	r1, r1, #16
	ldr	r3, [pc, #688]
	str	r2, [sp, #28]
	str	r1, [sp, #24]
	movs	r2, #1
	movs	r7, #0
	negs	r2, r2
	add	r3, sl
.L_080d25c2:
	adds	r7, #1
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r7, #64
	bne.n	.L_080d25c2
	ldr	r5, [pc, #668]
	movs	r7, #0
	movs	r6, #127
	add	r5, sl
.L_080d25d4:
	ldr	r3, [sp, #20]
	cmp	r3, #1
	bne.n	.L_080d25e4
	bl	sub_08004458
	ands	r0, r6
	adds	r0, #128
	b.n	.L_080d25ec
.L_080d25e4:
	bl	sub_08004458
	ands	r0, r6
	subs	r0, #128
.L_080d25ec:
	str	r0, [r5, #0]
	bl	sub_08004458
	movs	r3, #7
	ands	r3, r0
	subs	r3, #72
	str	r3, [r5, #4]
	bl	sub_08004458
	movs	r3, #31
	ands	r3, r0
	negs	r3, r3
	adds	r7, #1
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r7, #16
	bne.n	.L_080d25d4
	movs	r1, #1
	movs	r2, #128
	ldr	r3, [pc, #604]
	movs	r7, #0
	negs	r1, r1
	lsls	r2, r2, #2
.L_080d261a:
	adds	r7, #1
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r7, r2
	bne.n	.L_080d261a
	ldr	r4, [sp, #56]
	cmp	r4, #0
	bne.n	.L_080d2644
	ldr	r3, [pc, #564]
	add	r3, sl
	ldr	r0, [r3, #0]
	bl	sub_080d6750
	movs	r0, #1
	bl	sub_080030f8
	ldr	r1, [pc, #568]
	movs	r0, #8
	movs	r2, #2
	bl	sub_080dbb24
.L_080d2644:
	ldr	r0, [sp, #48]
	movs	r6, #0
	adds	r0, #12
	str	r6, [sp, #36]
	str	r0, [sp, #12]
.L_080d264e:
	ldr	r3, [pc, #552]
	ldr	r3, [r3, #0]
	movs	r2, #3
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080d26da
	ldr	r1, [sp, #36]
	cmp	r1, #48
	ble.n	.L_080d26da
	cmp	r1, #159
	bgt.n	.L_080d26da
	ldr	r2, [sp, #56]
	cmp	r2, #0
	bne.n	.L_080d269a
	ldr	r3, [pc, #528]
	add	r3, sl
	ldr	r0, [r3, #0]
	movs	r1, #8
	bl	sub_08009020
	ldr	r3, [pc, #520]
	add	r3, sl
	ldr	r0, [r3, #0]
	movs	r1, #9
	bl	sub_08009020
	ldr	r3, [pc, #512]
	add	r3, sl
	ldr	r0, [r3, #0]
	movs	r1, #10
	bl	sub_08009020
	ldr	r3, [pc, #504]
	add	r3, sl
	ldr	r0, [r3, #0]
	movs	r1, #11
	bl	sub_08009020
.L_080d269a:
	ldr	r3, [pc, #452]
	mov	r4, sl
	ldr	r3, [r4, r3]
	ldr	r3, [r3, #20]
	movs	r7, #0
	cmp	r3, #0
	beq.n	.L_080d26d6
	ldr	r5, [pc, #436]
	movs	r6, #36
	add	r5, sl
.L_080d26ae:
	ldr	r3, [r5, #0]
	ldrsh	r0, [r3, r6]
	movs	r3, #0
	str	r3, [sp, #0]
	movs	r2, #5
	movs	r1, #10
	subs	r3, #1
	bl	sub_080d6888
	ldr	r3, [r5, #0]
	movs	r1, #4
	ldrsh	r0, [r3, r6]
	bl	sub_080b5088
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #20]
	adds	r7, #1
	adds	r6, #2
	cmp	r7, r3
	bne.n	.L_080d26ae
.L_080d26d6:
	movs	r3, #160
	str	r3, [sp, #36]
.L_080d26da:
	bl	sub_080049ac
	ldr	r0, [sp, #48]
	ldr	r1, [sp, #12]
	bl	sub_080051d8
	ldr	r4, [sp, #36]
	cmp	r4, #178
	bne.n	.L_080d26f2
	movs	r0, #134
	bl	sub_080b50e8
.L_080d26f2:
	ldr	r6, [sp, #36]
	cmp	r6, #128
	bne.n	.L_080d2700
	ldr	r2, [pc, #348]
	movs	r3, #50
	add	r2, sl
	str	r3, [r2, #0]
.L_080d2700:
	ldr	r0, [sp, #36]
	cmp	r0, #176
	bne.n	.L_080d2730
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, sl
	movs	r3, #3
	str	r3, [r2, #0]
	ldr	r2, [pc, #324]
	ldr	r3, [pc, #376]
	add	r2, sl
	str	r3, [r2, #0]
	ldr	r0, [pc, #304]
	bl	sub_08002f40
	adds	r5, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #292]
	lsls	r0, r0, #19
	adds	r1, r5, #0
	movs	r2, #128
	bl	sub_080072f0
	b.n	.L_080d275e
.L_080d2730:
	ldr	r3, [sp, #36]
	subs	r3, #160
	cmp	r3, #15
	bhi.n	.L_080d275e
	movs	r3, #239
	lsls	r3, r3, #7
	add	r3, sl
	movs	r2, #1
	str	r2, [r3, #0]
	ldr	r2, [pc, #276]
	ldr	r3, [pc, #328]
	add	r2, sl
	str	r3, [r2, #0]
	ldr	r1, [sp, #36]
	cmp	r1, #173
	ble.n	.L_080d2754
	ldr	r3, [pc, #320]
	str	r3, [r2, #0]
.L_080d2754:
	movs	r0, #2
	movs	r1, #2
	movs	r2, #2
	bl	sub_080e727c
.L_080d275e:
	ldr	r3, [sp, #36]
	subs	r3, #33
	cmp	r3, #142
	bhi.n	.L_080d27e6
	ldr	r4, [sp, #36]
	movs	r2, #0
	movs	r3, #1
	mov	r8, r2
	str	r3, [sp, #16]
	cmp	r4, #103
	ble.n	.L_080d2778
	movs	r6, #8
	str	r6, [sp, #16]
.L_080d2778:
	ldr	r1, [pc, #284]
	movs	r0, #127
	ldr	r6, [pc, #284]
	movs	r7, #0
	mov	fp, r0
	mov	r9, r1
.L_080d2784:
	movs	r2, #1
	ldr	r3, [r6, #24]
	negs	r2, r2
	cmp	r3, r2
	bne.n	.L_080d27da
	bl	sub_08004458
	movs	r3, #255
	ands	r3, r0
	subs	r3, #32
	lsls	r3, r3, #16
	str	r3, [r6, #0]
	movs	r3, #224
	lsls	r3, r3, #15
	str	r3, [r6, #4]
	bl	sub_08004458
	movs	r5, #3
	mov	r4, r9
	mov	r3, fp
	ands	r5, r7
	ands	r0, r3
	ldrb	r3, [r4, r5]
	adds	r0, r0, r3
	lsls	r0, r0, #9
	str	r0, [r6, #12]
	bl	sub_08004458
	mov	r2, r9
	ldrb	r3, [r2, r5]
	mov	r1, fp
	ands	r0, r1
	adds	r0, r0, r3
	negs	r0, r0
	movs	r3, #0
	lsls	r0, r0, #11
	str	r3, [r6, #24]
	str	r0, [r6, #16]
	movs	r3, #1
	ldr	r4, [sp, #16]
	add	r8, r3
	cmp	r8, r4
	beq.n	.L_080d27e6
.L_080d27da:
	movs	r0, #128
	adds	r7, #1
	lsls	r0, r0, #2
	adds	r6, #28
	cmp	r7, r0
	bne.n	.L_080d2784
.L_080d27e6:
	ldr	r3, [sp, #36]
	subs	r3, #41
	cmp	r3, #86
	bls.n	.L_080d27f0
	b.n	.L_080d28f8
.L_080d27f0:
	ldr	r2, [sp, #36]
	movs	r3, #1
	movs	r1, #0
	ands	r3, r2
	mov	fp, r1
	cmp	r3, #0
	beq.n	.L_080d28f8
	movs	r3, #60
	ldr	r5, [pc, #156]
	add	r3, sp
	movs	r7, #0
	mov	r9, r3
	add	r5, sl
.L_080d280a:
	movs	r4, #1
	ldr	r3, [r5, #24]
	negs	r4, r4
	cmp	r3, r4
	bne.n	.L_080d28f0
	bl	sub_08004458
	movs	r3, #255
	ands	r3, r0
	adds	r3, #128
	mov	r8, r3
	bl	sub_08004458
	ldr	r3, [pc, #124]
	ldr	r1, [sp, #56]
	ands	r3, r0
	ldr	r0, [pc, #124]
	adds	r6, r3, r0
	cmp	r1, #0
	bne.n	.L_080d28ac
	bl	sub_08004458
	movs	r2, #7
	ands	r0, r2
	adds	r0, #78
	movs	r3, #140
	lsls	r0, r0, #16
	lsls	r3, r3, #15
	str	r0, [r5, #0]
	b.n	.L_080d28c4
	movs	r0, r0
	.4byte 0x03001388
	.4byte 0x000000b4
	.4byte 0x00000073
	.4byte 0x000000c4
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x00007828
	.4byte 0xffc40000
	.4byte 0x00007098
	.4byte 0x00007320
	.4byte 0x02010018
	.4byte 0x00000179
	.4byte 0x03001b04
	.4byte 0x000077d8
	.4byte 0x000077dc
	.4byte 0x000077e4
	.4byte 0x000077e8
	.4byte 0x02020202
	.4byte 0x10101010
	.4byte 0x3f3f3f3f
	.4byte 0x080ee184
	.4byte 0x02010000
	.4byte 0x000074e0
	.4byte 0x00001fff
	.2byte 0x4e20
	.2byte 0x0000
.L_080d28ac:
	bl	sub_08004458
	movs	r3, #7
	mov	r4, r9
	ands	r0, r3
	ldr	r3, [r4, #0]
	adds	r0, r0, r3
	subs	r0, #8
	lsls	r0, r0, #16
	str	r0, [r5, #0]
	ldr	r3, [r4, #4]
	lsls	r3, r3, #16
.L_080d28c4:
	str	r3, [r5, #4]
	adds	r0, r6, #0
	bl	sub_08002322
	mov	r3, r8
	muls	r3, r0
	asrs	r3, r3, #9
	str	r3, [r5, #12]
	adds	r0, r6, #0
	bl	sub_0800231c
	mov	r3, r8
	muls	r3, r0
	movs	r6, #1
	asrs	r3, r3, #9
	add	fp, r6
	str	r3, [r5, #16]
	mov	r0, fp
	movs	r3, #0
	str	r3, [r5, #24]
	cmp	r0, #1
	beq.n	.L_080d28f8
.L_080d28f0:
	adds	r7, #1
	adds	r5, #28
	cmp	r7, #24
	bne.n	.L_080d280a
.L_080d28f8:
	ldr	r1, [sp, #36]
	cmp	r1, #48
	bne.n	.L_080d2904
	movs	r0, #141
	bl	sub_080f9010
.L_080d2904:
	ldr	r2, [sp, #36]
	cmp	r2, #128
	bne.n	.L_080d2910
	movs	r0, #145
	bl	sub_080f9010
.L_080d2910:
	ldr	r3, [sp, #36]
	subs	r3, #129
	cmp	r3, #46
	bhi.n	.L_080d299e
	movs	r4, #60
	movs	r5, #225
	movs	r3, #0
	add	r4, sp
	lsls	r5, r5, #7
	mov	fp, r3
	movs	r7, #0
	mov	r9, r4
	add	r5, sl
.L_080d292a:
	movs	r6, #1
	ldr	r3, [r5, #24]
	negs	r6, r6
	cmp	r3, r6
	bne.n	.L_080d2996
	bl	sub_08004458
	movs	r3, #255
	ands	r3, r0
	adds	r3, #128
	mov	r8, r3
	bl	sub_08004458
	ldr	r3, [pc, #788]
	ldr	r1, [sp, #56]
	ands	r3, r0
	ldr	r0, [pc, #788]
	adds	r6, r3, r0
	cmp	r1, #0
	bne.n	.L_080d295e
	movs	r3, #136
	lsls	r3, r3, #15
	str	r3, [r5, #0]
	movs	r3, #128
	lsls	r3, r3, #15
	b.n	.L_080d296a
.L_080d295e:
	mov	r2, r9
	ldr	r3, [r2, #0]
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	ldr	r3, [r2, #4]
	lsls	r3, r3, #16
.L_080d296a:
	str	r3, [r5, #4]
	adds	r0, r6, #0
	bl	sub_08002322
	mov	r3, r8
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r5, #12]
	adds	r0, r6, #0
	bl	sub_0800231c
	mov	r3, r8
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r5, #16]
	movs	r3, #0
	str	r3, [r5, #24]
	movs	r3, #1
	add	fp, r3
	mov	r4, fp
	cmp	r4, #1
	beq.n	.L_080d299e
.L_080d2996:
	adds	r7, #1
	adds	r5, #28
	cmp	r7, #24
	bne.n	.L_080d292a
.L_080d299e:
	ldr	r6, [sp, #36]
	cmp	r6, #175
	bgt.n	.L_080d2a02
	movs	r5, #225
	lsls	r5, r5, #7
	movs	r7, #0
	add	r5, sl
.L_080d29ac:
	ldr	r1, [r5, #24]
	cmp	r1, #0
	blt.n	.L_080d29fa
	movs	r0, #2
	ldrsh	r2, [r5, r0]
	movs	r4, #6
	ldrsh	r3, [r5, r4]
	movs	r0, #32
	asrs	r1, r1, #2
	str	r0, [sp, #0]
	lsls	r1, r1, #11
	movs	r0, #64
	subs	r2, #16
	subs	r3, #32
	str	r0, [sp, #4]
	add	r1, sl
	ldr	r0, [sp, #52]
	ldr	r6, [sp, #40]
	bl	sub_080072fc
	ldr	r3, [r5, #12]
	ldr	r0, [sp, #20]
	adds	r2, r0, #0
	muls	r2, r3
	ldr	r3, [r5, #0]
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r2, [r5, #16]
	ldr	r3, [r5, #4]
	adds	r3, r3, r2
	str	r3, [r5, #4]
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
	cmp	r3, #24
	bne.n	.L_080d29fa
	movs	r3, #1
	negs	r3, r3
	str	r3, [r5, #24]
.L_080d29fa:
	adds	r7, #1
	adds	r5, #28
	cmp	r7, #24
	bne.n	.L_080d29ac
.L_080d2a02:
	ldr	r5, [pc, #608]
	movs	r7, #0
	movs	r6, #1
	add	r5, sl
.L_080d2a0a:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	blt.n	.L_080d2a56
	movs	r4, #6
	ldrsh	r3, [r5, r4]
	movs	r1, #2
	ldrsh	r2, [r5, r1]
	movs	r1, #2
	subs	r3, #1
	str	r1, [sp, #4]
	str	r6, [sp, #0]
	ldr	r1, [pc, #580]
	ldr	r0, [sp, #52]
	ldr	r4, [sp, #44]
	bl	sub_080072f4
	ldr	r3, [r5, #12]
	ldr	r0, [sp, #20]
	adds	r2, r0, #0
	muls	r2, r3
	ldr	r3, [r5, #0]
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r2, [r5, #16]
	ldr	r3, [r5, #4]
	adds	r3, r3, r2
	str	r3, [r5, #4]
	ldr	r1, [pc, #552]
	ldr	r3, [r5, #24]
	adds	r2, r2, r1
	adds	r3, #1
	str	r2, [r5, #16]
	str	r3, [r5, #24]
	cmp	r3, #48
	bne.n	.L_080d2a56
	movs	r3, #1
	negs	r3, r3
	str	r3, [r5, #24]
.L_080d2a56:
	adds	r7, #1
	adds	r5, #28
	cmp	r7, #24
	bne.n	.L_080d2a0a
	ldr	r2, [sp, #36]
	cmp	r2, #175
	bgt.n	.L_080d2af6
	ldr	r5, [pc, #520]
	movs	r7, #0
.L_080d2a68:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	blt.n	.L_080d2aea
	ldr	r2, [pc, #516]
	movs	r3, #3
	ands	r3, r7
	ldrb	r0, [r2, r3]
	ldr	r2, [pc, #512]
	lsls	r4, r0, #1
	subs	r3, r4, #2
	ldrh	r1, [r2, r3]
	ldr	r3, [sp, #32]
	movs	r6, #2
	ldrsh	r2, [r5, r6]
	adds	r1, r3, r1
	lsrs	r3, r0, #1
	subs	r2, r2, r3
	movs	r6, #6
	ldrsh	r3, [r5, r6]
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #52]
	ldr	r4, [sp, #40]
	bl	sub_080072f4
	ldr	r1, [r5, #12]
	ldr	r6, [sp, #20]
	adds	r2, r6, #0
	muls	r2, r1
	ldr	r3, [r5, #0]
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r2, [r5, #16]
	ldr	r3, [r5, #4]
	adds	r3, r3, r2
	str	r3, [r5, #4]
	ldr	r0, [sp, #36]
	cmp	r0, #128
	ble.n	.L_080d2ad0
	movs	r3, #1
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L_080d2ac8
	ldr	r2, [pc, #440]
	adds	r3, r1, r2
	str	r3, [r5, #12]
	b.n	.L_080d2ad6
.L_080d2ac8:
	ldr	r4, [pc, #436]
	adds	r3, r1, r4
	str	r3, [r5, #12]
	b.n	.L_080d2ad6
.L_080d2ad0:
	ldr	r6, [pc, #408]
	adds	r3, r2, r6
	str	r3, [r5, #16]
.L_080d2ad6:
	ldr	r3, [r5, #24]
	movs	r0, #128
	adds	r3, #1
	lsls	r0, r0, #1
	str	r3, [r5, #24]
	cmp	r3, r0
	bne.n	.L_080d2aea
	movs	r3, #1
	negs	r3, r3
	str	r3, [r5, #24]
.L_080d2aea:
	movs	r1, #128
	adds	r7, #1
	lsls	r1, r1, #2
	adds	r5, #28
	cmp	r7, r1
	bne.n	.L_080d2a68
.L_080d2af6:
	ldr	r2, [sp, #36]
	cmp	r2, #128
	bne.n	.L_080d2b04
	ldr	r2, [pc, #388]
	movs	r3, #48
	add	r2, sl
	str	r3, [r2, #0]
.L_080d2b04:
	ldr	r3, [sp, #56]
	cmp	r3, #0
	bne.n	.L_080d2b18
	ldr	r4, [sp, #36]
	cmp	r4, #48
	bne.n	.L_080d2b18
	ldr	r2, [pc, #368]
	movs	r3, #8
	add	r2, sl
	str	r3, [r2, #0]
.L_080d2b18:
	ldr	r3, [sp, #36]
	subs	r3, #40
	cmp	r3, #7
	bhi.n	.L_080d2b32
	ldr	r6, [sp, #24]
	ldr	r1, [sp, #28]
	ldr	r0, [pc, #352]
	movs	r2, #128
	lsls	r2, r2, #13
	adds	r0, r6, r0
	adds	r2, r1, r2
	str	r0, [sp, #24]
	str	r2, [sp, #28]
.L_080d2b32:
	ldr	r3, [sp, #56]
	cmp	r3, #0
	bne.n	.L_080d2bae
	ldr	r4, [sp, #36]
	cmp	r4, #128
	bne.n	.L_080d2b6e
	ldr	r3, [pc, #332]
	add	r3, sl
	ldr	r0, [r3, #0]
	movs	r1, #8
	bl	sub_08009020
	ldr	r3, [pc, #324]
	add	r3, sl
	ldr	r0, [r3, #0]
	movs	r1, #9
	bl	sub_08009020
	ldr	r3, [pc, #316]
	add	r3, sl
	ldr	r0, [r3, #0]
	movs	r1, #10
	bl	sub_08009020
	ldr	r3, [pc, #308]
	add	r3, sl
	ldr	r0, [r3, #0]
	movs	r1, #11
	bl	sub_08009020
.L_080d2b6e:
	ldr	r6, [sp, #36]
	cmp	r6, #176
	bne.n	.L_080d2ba4
	ldr	r3, [pc, #276]
	add	r3, sl
	ldr	r0, [r3, #0]
	movs	r1, #0
	bl	sub_08009020
	ldr	r3, [pc, #268]
	add	r3, sl
	ldr	r0, [r3, #0]
	movs	r1, #1
	bl	sub_08009020
	ldr	r3, [pc, #260]
	add	r3, sl
	ldr	r0, [r3, #0]
	movs	r1, #3
	bl	sub_08009020
	ldr	r3, [pc, #252]
	add	r3, sl
	ldr	r0, [r3, #0]
	movs	r1, #4
	bl	sub_08009020
.L_080d2ba4:
	movs	r0, #3
	ldr	r1, [sp, #24]
	ldr	r2, [sp, #28]
	bl	sub_080e6d3c
.L_080d2bae:
	ldr	r0, [sp, #36]
	cmp	r0, #138
	bne.n	.L_080d2bf0
	ldr	r3, [pc, #228]
	mov	r1, sl
	ldr	r3, [r1, r3]
	ldr	r3, [r3, #20]
	movs	r7, #0
	cmp	r3, #0
	beq.n	.L_080d2bf0
	ldr	r5, [pc, #216]
	movs	r6, #36
	add	r5, sl
.L_080d2bc8:
	ldr	r3, [r5, #0]
	ldrsh	r0, [r3, r6]
	movs	r3, #0
	str	r3, [sp, #0]
	movs	r1, #10
	movs	r2, #5
	subs	r3, #1
	bl	sub_080d6888
	ldr	r3, [r5, #0]
	movs	r1, #4
	ldrsh	r0, [r3, r6]
	bl	sub_080b5088
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #20]
	adds	r7, #1
	adds	r6, #2
	cmp	r7, r3
	bne.n	.L_080d2bc8
.L_080d2bf0:
	ldr	r6, [sp, #36]
	cmp	r6, #175
	ble.n	.L_080d2bf8
	b.n	.L_080d2d16
.L_080d2bf8:
	ldr	r5, [pc, #164]
	movs	r0, #0
	movs	r1, #20
	movs	r2, #5
	movs	r7, #0
	mov	fp, r0
	mov	r8, r1
	mov	r9, r2
	add	r5, sl
.L_080d2c0a:
	ldr	r6, [r5, #4]
	cmp	r6, #55
	ble.n	.L_080d2cb4
	ldr	r3, [r5, #24]
	cmp	r3, #11
	bhi.n	.L_080d2c4c
	lsrs	r4, r3, #31
	adds	r4, r3, r4
	asrs	r4, r4, #1
	ldr	r2, [pc, #132]
	lsls	r3, r4, #1
	ldrh	r1, [r2, r3]
	movs	r3, #192
	lsls	r3, r3, #6
	add	r1, sl
	adds	r1, r1, r3
	ldr	r3, [pc, #124]
	ldrb	r0, [r3, r4]
	ldr	r2, [r5, #0]
	lsrs	r3, r0, #1
	subs	r2, r2, r3
	ldr	r3, [pc, #116]
	ldrb	r3, [r3, r4]
	str	r0, [sp, #0]
	ldr	r0, [pc, #116]
	ldrb	r0, [r0, r4]
	adds	r3, r6, r3
	str	r0, [sp, #4]
	ldr	r4, [sp, #40]
	ldr	r0, [sp, #52]
	bl	sub_080072f4
	ldr	r3, [r5, #24]
.L_080d2c4c:
	adds	r3, #1
	str	r3, [r5, #24]
	cmp	r3, #12
	bne.n	.L_080d2d0c
	mov	r6, fp
	str	r6, [r5, #24]
	b.n	.L_080d2d0c
	movs	r0, r0
	.4byte 0x00001fff
	.4byte 0xffffb1e0
	.4byte 0x000074e0
	.4byte 0x080ee188
	.4byte 0xfffffc00
	.4byte 0x02010000
	.4byte 0x080ee18a
	.4byte 0x080ede48
	.4byte 0xffff8000
	.4byte 0xffffe000
	.4byte 0x000077a8
	.4byte 0xfff80000
	.4byte 0x000077d8
	.4byte 0x000077dc
	.4byte 0x000077e4
	.4byte 0x000077e8
	.4byte 0x00007828
	.4byte 0x00007320
	.4byte 0x080ee1a0
	.4byte 0x080ee18e
	.4byte 0x080ee19a
	.2byte 0xe194
	.2byte 0x080e
.L_080d2cb4:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	bne.n	.L_080d2d08
	ldr	r0, [sp, #20]
	lsls	r3, r0, #1
	ldr	r2, [r5, #0]
	adds	r3, r3, r0
	lsls	r3, r3, #1
	subs	r2, r2, r3
	adds	r3, r6, #6
	str	r2, [r5, #0]
	str	r3, [r5, #4]
	ldr	r1, [sp, #36]
	movs	r4, #10
	cmp	r1, #47
	bgt.n	.L_080d2ce2
	cmp	r3, #55
	ble.n	.L_080d2ce2
	movs	r0, #136
	str	r4, [sp, #8]
	bl	sub_080f9010
	ldr	r4, [sp, #8]
.L_080d2ce2:
	ldr	r2, [pc, #164]
	mov	r3, r8
	subs	r3, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #32]
	adds	r1, r2, r1
	ldr	r2, [r5, #0]
	mov	r3, r9
	subs	r2, r2, r3
	ldr	r3, [r5, #4]
	str	r4, [sp, #0]
	mov	r4, r8
	adds	r3, #30
	str	r4, [sp, #4]
	ldr	r0, [sp, #52]
	ldr	r6, [sp, #40]
	bl	sub_080072fc
	b.n	.L_080d2d0c
.L_080d2d08:
	adds	r3, #1
	str	r3, [r5, #24]
.L_080d2d0c:
	adds	r7, #1
	adds	r5, #28
	cmp	r7, #16
	beq.n	.L_080d2d16
	b.n	.L_080d2c0a
.L_080d2d16:
	movs	r0, #8
	movs	r1, #8
	bl	sub_080e155c
	bl	sub_080cd52c
	ldr	r2, [pc, #104]
	movs	r3, #1
	add	r2, sl
	movs	r0, #1
	str	r3, [r2, #0]
	bl	sub_080030f8
	ldr	r0, [sp, #36]
	adds	r0, #1
	str	r0, [sp, #36]
	cmp	r0, #208
	beq.n	.L_080d2d3c
	b.n	.L_080d264e
.L_080d2d3c:
	ldr	r0, [pc, #80]
	bl	sub_08004278
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	ldr	r1, [sp, #56]
	cmp	r1, #0
	bne.n	.L_080d2d70
	movs	r0, #3
	ldr	r1, [sp, #24]
	ldr	r2, [sp, #28]
	bl	sub_080e6eac
	ldr	r5, [pc, #52]
	movs	r7, #0
	add	r5, sl
.L_080d2d64:
	ldmia	r5!, {r0}
	adds	r7, #1
	bl	sub_08009038
	cmp	r7, #8
	bne.n	.L_080d2d64
.L_080d2d70:
	bl	sub_080cdbc0
	add	sp, #72
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x080ede48
	.4byte 0x00007824
	.4byte 0x080cd261
	.4byte 0x000077d8
