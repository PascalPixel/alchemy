.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002064, 0x08002064
	.set sub_08014274, 0x08014274
	.set sub_08014878, 0x08014878
	.set sub_08016ce4, 0x08016ce4
	.set sub_08020058, 0x08020058
	.set sub_08020090, 0x08020090
	.set sub_080200c0, 0x080200c0
	.set sub_080200c8, 0x080200c8
	.set sub_080200d8, 0x080200d8
	.set sub_080200e8, 0x080200e8
	.set sub_080201c0, 0x080201c0
	.set sub_08020218, 0x08020218
	.set sub_080ca6a4, 0x080ca6a4
	.set sub_080cad64, 0x080cad64
	.set sub_080cad84, 0x080cad84
	.set sub_080cc994, 0x080cc994
	.set sub_080ccd48, 0x080ccd48
	.set sub_080d4b0c, 0x080d4b0c
	.global Overlay_080ca6e8
Overlay_080ca6e8:
.L_080ca6e8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	mov	r8, r0
	mov	sl, r3
	ldr	r3, [r3, #0]
	movs	r0, #0
	mov	r9, r1
	sub	sp, #20
	mov	fp, r0
	movs	r1, #0
	cmp	r3, r8
	beq.n	.L_080ca734
	cmp	r3, #0
	bne.n	.L_080ca71c
	mov	r1, r8
	mov	r2, sl
	str	r1, [r2, #0]
	b.n	.L_080ca734
.L_080ca71c:
	adds	r1, #1
	cmp	r1, #3
	bgt.n	.L_080ca734
	lsls	r2, r1, #2
	mov	r0, sl
	ldr	r3, [r0, r2]
	cmp	r3, r8
	beq.n	.L_080ca734
	cmp	r3, #0
	bne.n	.L_080ca71c
	mov	r1, r8
	str	r1, [r0, r2]
.L_080ca734:
	mov	r3, r8
	ldrh	r2, [r3, #0]
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	movs	r1, #1
	b.n	.L_080ca9a4
.L_080ca740:
	lsls	r3, r2, #16
	asrs	r3, r3, #16
	cmp	r3, #7
	bgt.n	.L_080ca74c
	mov	fp, r3
	b.n	.L_080ca75c
.L_080ca74c:
	movs	r0, #156
	lsls	r0, r0, #6
	adds	r0, #5
	cmp	r3, r0
	bgt.n	.L_080ca75c
	movs	r1, #1
	mov	fp, r9
	add	r9, r1
.L_080ca75c:
	mov	r3, r8
	movs	r2, #2
	ldrsh	r7, [r3, r2]
	adds	r0, r7, #0
	bl	sub_080ccd48
	cmp	r0, #0
	bne.n	.L_080ca76e
	b.n	.L_080ca994
.L_080ca76e:
	adds	r5, r7, #0
	subs	r5, #48
	cmp	r5, #79
	bhi.n	.L_080ca7d4
	movs	r1, #20
	adds	r0, r5, #0
	bl	sub_08002054
	movs	r1, #20
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl	sub_08002064
	cmp	r0, #19
	bne.n	.L_080ca7b6
	movs	r0, #200
	ldr	r2, [pc, #560]
	lsls	r0, r0, #5
	adds	r0, #80
	adds	r3, r6, r0
	ldrsb	r0, [r2, r3]
	cmp	r0, #0
	bne.n	.L_080ca79e
	b.n	.L_080ca994
.L_080ca79e:
	lsls	r3, r6, #2
	adds	r3, r3, r6
	lsls	r3, r3, #2
	adds	r3, r3, r0
	adds	r7, r3, #0
	adds	r7, #47
	adds	r0, r7, #0
	bl	sub_080ccd48
	cmp	r0, #0
	bne.n	.L_080ca7b6
	b.n	.L_080ca994
.L_080ca7b6:
	movs	r3, #197
	lsls	r3, r3, #1
	add	r3, sl
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #3
	beq.n	.L_080ca7d4
	adds	r0, r7, #0
	adds	r0, #80
	bl	sub_080ccd48
	cmp	r0, #0
	bne.n	.L_080ca7d4
	b.n	.L_080ca994
.L_080ca7d4:
	mov	r2, r8
	movs	r1, #0
	ldrsh	r0, [r2, r1]
	bl	sub_080ca6a4
	adds	r7, r0, #0
	mov	r0, fp
	bl	sub_080cad84
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_080ca870
	mov	r3, r8
	ldrb	r2, [r3, #23]
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080ca814
	add	r5, sp, #8
	str	r6, [r5, #0]
	str	r6, [r5, #4]
	str	r6, [r5, #8]
	mov	r1, r8
	ldr	r0, [r1, #8]
	adds	r1, r5, #0
	bl	.L_080ca9cc
	ldr	r1, [r5, #0]
	ldr	r3, [r5, #8]
	adds	r0, r7, #0
	movs	r2, #0
	b.n	.L_080ca820
.L_080ca814:
	mov	r2, r8
	mov	r0, r8
	ldr	r1, [r2, #8]
	ldr	r3, [r0, #16]
	ldr	r2, [r2, #12]
	adds	r0, r7, #0
.L_080ca820:
	bl	sub_080200c0
	adds	r6, r0, #0
	mov	r1, r8
	ldrb	r2, [r1, #23]
	movs	r3, #1
	ands	r3, r2
	movs	r1, #1
	cmp	r3, #0
	beq.n	.L_080ca892
	mov	r0, fp
	subs	r0, #1
	str	r1, [sp, #4]
	bl	sub_080cad84
	adds	r3, r0, #0
	adds	r3, #84
	ldrb	r3, [r3, #0]
	ldr	r1, [sp, #4]
	cmp	r3, #1
	bne.n	.L_080ca892
	adds	r3, r6, #0
	adds	r3, #84
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L_080ca892
	ldr	r7, [r0, #80]
	ldrb	r3, [r7, #17]
	ldrb	r5, [r7, #16]
	orrs	r3, r1
	strb	r3, [r7, #17]
	ldr	r7, [r6, #80]
	ldrb	r3, [r7, #17]
	ldrb	r0, [r7, #16]
	orrs	r3, r1
	strb	r3, [r7, #17]
	bl	sub_08014274
	strb	r5, [r7, #16]
	b.n	.L_080ca892
.L_080ca870:
	mov	r0, r8
	movs	r2, #0
	ldrsh	r3, [r0, r2]
	ldr	r2, [pc, #332]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r2, r2, r1
	ldr	r2, [r2, #0]
	cmp	r3, r2
	bne.n	.L_080ca886
	b.n	.L_080ca994
.L_080ca886:
	ldr	r1, [r0, #8]
	ldr	r2, [r0, #12]
	ldr	r3, [r0, #16]
	adds	r0, r6, #0
	bl	sub_080200e8
.L_080ca892:
	cmp	r6, #0
	beq.n	.L_080ca98a
	adds	r0, r6, #0
	movs	r1, #1
	ldr	r7, [r6, #80]
	bl	sub_08020090
	adds	r3, r6, #0
	adds	r3, #84
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L_080ca8e0
	cmp	r7, #0
	beq.n	.L_080ca8e0
	ldrb	r3, [r7, #27]
	movs	r1, #0
	cmp	r1, r3
	bge.n	.L_080ca8dc
	adds	r2, r7, #0
	adds	r2, #40
.L_080ca8ba:
	ldmia	r2!, {r5}
	cmp	r5, #0
	beq.n	.L_080ca8d6
	str	r1, [sp, #4]
	str	r2, [sp, #0]
	bl	sub_08014878
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsrs	r3, r3, #15
	strh	r3, [r5, #2]
	ldr	r2, [sp, #0]
	ldrb	r3, [r7, #27]
	ldr	r1, [sp, #4]
.L_080ca8d6:
	adds	r1, #1
	cmp	r1, r3
	blt.n	.L_080ca8ba
.L_080ca8dc:
	movs	r3, #1
	strb	r3, [r7, #25]
.L_080ca8e0:
	mov	r2, r8
	ldrh	r3, [r2, #20]
	adds	r2, r6, #0
	strh	r3, [r6, #6]
	adds	r2, #89
	movs	r3, #1
	strb	r3, [r2, #0]
	mov	r3, r8
	ldr	r1, [r3, #4]
	adds	r0, r6, #0
	bl	sub_080d4b0c
	ldr	r2, [r6, #8]
	cmp	r2, #0
	bge.n	.L_080ca906
	movs	r0, #255
	lsls	r0, r0, #8
	adds	r0, #255
	adds	r2, r2, r0
.L_080ca906:
	adds	r3, r6, #0
	adds	r3, #100
	asrs	r2, r2, #16
	strh	r2, [r3, #0]
	ldr	r3, [r6, #16]
	cmp	r3, #0
	bge.n	.L_080ca91c
	movs	r1, #255
	lsls	r1, r1, #8
	adds	r1, #255
	adds	r3, r3, r1
.L_080ca91c:
	adds	r2, r6, #0
	asrs	r3, r3, #16
	adds	r2, #102
	strh	r3, [r2, #0]
	ldr	r3, [r6, #12]
	cmp	r3, #0
	beq.n	.L_080ca93a
	subs	r2, #17
	movs	r3, #4
	strb	r3, [r2, #0]
	movs	r2, #128
	ldr	r3, [r6, #12]
	lsls	r2, r2, #8
	adds	r3, r3, r2
	str	r3, [r6, #12]
.L_080ca93a:
	movs	r3, #197
	lsls	r3, r3, #1
	add	r3, sl
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #3
	bne.n	.L_080ca970
	adds	r1, r6, #0
	adds	r1, #85
	ldrb	r3, [r1, #0]
	movs	r2, #254
	ands	r2, r3
	strb	r2, [r1, #0]
	movs	r0, #33
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080ca982
	movs	r1, #192
	ldr	r0, [r7, #12]
	ldr	r3, [pc, #96]
	lsls	r1, r1, #8
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x60f8
	b.n	.L_080ca982
.L_080ca970:
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #16]
	movs	r0, #0
	bl	sub_080201c0
	ldr	r3, [r6, #12]
	str	r0, [r6, #20]
	adds	r3, r3, r0
	str	r3, [r6, #12]
.L_080ca982:
	adds	r2, r6, #0
	adds	r2, #35
	movs	r3, #1
	strb	r3, [r2, #0]
.L_080ca98a:
	mov	r0, fp
	lsls	r3, r0, #2
	adds	r3, #20
	mov	r1, sl
	str	r6, [r1, r3]
.L_080ca994:
	movs	r2, #24
	add	r8, r2
	mov	r0, r8
	ldrh	r3, [r0, #0]
	movs	r1, #1
	adds	r2, r3, #0
	lsls	r3, r2, #16
	asrs	r3, r3, #16
.L_080ca9a4:
	negs	r1, r1
	cmp	r3, r1
	beq.n	.L_080ca9b2
	mov	r3, r9
	cmp	r3, #63
	bgt.n	.L_080ca9b2
	b.n	.L_080ca740
.L_080ca9b2:
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02001000
	.4byte 0x02000240
	.2byte 0x021c
	.2byte 0x0300
.L_080ca9cc:
	push	{r5, r6, lr}
	ldr	r3, [pc, #88]
	sub	sp, #4
	str	r3, [sp, #0]
	adds	r6, r1, #0
	ldrb	r2, [r3, #0]
	movs	r1, #1
	adds	r3, #1
	lsls	r2, r2, #24
	str	r3, [sp, #0]
	negs	r1, r1
	asrs	r3, r2, #24
	cmp	r3, r1
	beq.n	.L_080caa1e
	mov	r5, sp
	mov	ip, r1
.L_080ca9ec:
	asrs	r3, r2, #24
	cmp	r3, r0
	bne.n	.L_080caa0a
	adds	r0, r5, #0
	bl	sub_080cc994
	lsls	r0, r0, #16
	str	r0, [r6, #0]
	adds	r0, r5, #0
	bl	sub_080cc994
	lsls	r0, r0, #16
	str	r0, [r6, #8]
	movs	r0, #0
	b.n	.L_080caa22
.L_080caa0a:
	ldr	r3, [sp, #0]
	adds	r2, r3, #4
	str	r2, [sp, #0]
	adds	r3, #5
	ldrb	r2, [r2, #0]
	str	r3, [sp, #0]
	lsls	r2, r2, #24
	asrs	r3, r2, #24
	cmp	r3, ip
	bne.n	.L_080ca9ec
.L_080caa1e:
	movs	r0, #1
	negs	r0, r0
.L_080caa22:
	add	sp, #4
	pop	{r5, r6, pc}
	movs	r0, r0
	.2byte 0xe008
	.2byte 0x0202
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r5, r0, #0
	ldr	r6, [r3, #108]
	bl	sub_080cad84
	cmp	r0, #0
	beq.n	.L_080caa4a
	bl	sub_080200c8
	lsls	r3, r5, #2
	adds	r3, #20
	movs	r2, #0
	str	r2, [r6, r3]
.L_080caa4a:
	pop	{r5, r6, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	mov	r9, r0
	mov	r8, r3
	ldr	r3, [pc, #576]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r3, r0
	movs	r5, #246
	ldr	r6, [r3, #0]
	lsls	r5, r5, #1
	ldr	r3, [pc, #568]
	add	r5, r8
	adds	r2, r5, #0
	ldmia	r3!, {r1, r4, r7}
	stmia	r2!, {r1, r4, r7}
	ldmia	r3!, {r0, r1, r4}
	stmia	r2!, {r0, r1, r4}
	movs	r2, #129
	lsls	r2, r2, #2
	add	r2, r8
	ldmia	r3!, {r0, r1, r7}
	stmia	r2!, {r0, r1, r7}
	ldmia	r3!, {r0, r4, r7}
	stmia	r2!, {r0, r4, r7}
	mov	r3, r8
	movs	r2, #0
	adds	r3, #12
	mov	ip, r8
.L_080caa96:
	str	r2, [r3, #0]
	subs	r3, #4
	cmp	r3, ip
	bge.n	.L_080caa96
	bl	sub_080cad64
	movs	r3, #255
	lsls	r3, r3, #8
	ldr	r7, [pc, #512]
	adds	r3, #255
	strh	r3, [r5, #2]
	strh	r6, [r5, #0]
	movs	r2, #254
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldr	r3, [r3, #0]
	movs	r1, #0
	str	r3, [r5, #8]
	str	r1, [r5, #12]
	movs	r4, #129
	lsls	r4, r4, #2
	adds	r3, r7, r4
	ldr	r3, [r3, #0]
	movs	r0, #130
	str	r3, [r5, #16]
	lsls	r0, r0, #2
	adds	r3, r7, r0
	ldr	r3, [r3, #0]
	mov	sl, r1
	strh	r3, [r5, #20]
	adds	r1, r6, #0
	adds	r0, r5, #0
	bl	.L_080ca6e8
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r1, r1, r7
	ldr	r4, [r1, #0]
	mov	fp, r1
	cmp	r4, #0
	beq.n	.L_080caaf8
	lsls	r6, r6, #2
	adds	r3, r6, #0
	adds	r3, #20
	mov	r2, r8
	ldr	r5, [r2, r3]
	str	r4, [r5, #12]
	str	r4, [r5, #20]
	b.n	.L_080caafa
.L_080caaf8:
	lsls	r6, r6, #2
.L_080caafa:
	mov	r0, r9
	movs	r1, #8
	bl	.L_080ca6e8
	adds	r3, r6, #0
	adds	r3, #20
	mov	r4, r8
	movs	r0, #131
	ldr	r5, [r4, r3]
	lsls	r0, r0, #2
	adds	r3, r7, r0
	ldrh	r3, [r3, #0]
	adds	r2, r5, #0
	adds	r2, #34
	strb	r3, [r2, #0]
	adds	r3, r5, #0
	adds	r3, #100
	mov	r1, sl
	strh	r1, [r3, #0]
	mov	r4, sl
	adds	r3, #2
	strh	r4, [r3, #0]
	movs	r3, #197
	lsls	r3, r3, #1
	add	r3, r8
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #3
	bne.n	.L_080cab5e
	adds	r0, #106
	adds	r3, r7, r0
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	beq.n	.L_080cab50
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #18
	adds	r2, r7, r3
	movs	r3, #7
	strb	r3, [r2, #0]
	b.n	.L_080cac18
.L_080cab50:
	movs	r4, #128
	lsls	r4, r4, #2
	adds	r4, #18
	adds	r2, r7, r4
	movs	r3, #1
	strb	r3, [r2, #0]
	b.n	.L_080cac18
.L_080cab5e:
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #118
	adds	r3, r7, r0
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	beq.n	.L_080cab7c
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #18
	adds	r2, r7, r3
	movs	r3, #6
	strb	r3, [r2, #0]
	b.n	.L_080cac18
.L_080cab7c:
	movs	r4, #128
	lsls	r4, r4, #2
	adds	r4, #18
	adds	r6, r7, r4
	movs	r7, #0
	strb	r7, [r6, #0]
	ldrb	r2, [r2, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r4, [r3, #32]
	lsls	r3, r2, #3
	subs	r3, r3, r2
	movs	r1, #156
	lsls	r3, r3, #3
	lsls	r1, r1, #1
	adds	r0, r3, r1
	ldr	r1, [r5, #8]
	adds	r3, r1, #0
	cmp	r1, #0
	bge.n	.L_080caba8
	ldr	r2, [pc, #264]
	adds	r3, r1, r2
.L_080caba8:
	asrs	r2, r3, #20
	ldr	r3, [r5, #16]
	cmp	r3, #0
	bge.n	.L_080cabb4
	ldr	r7, [pc, #252]
	adds	r3, r3, r7
.L_080cabb4:
	asrs	r3, r3, #20
	lsls	r3, r3, #7
	adds	r3, r2, r3
	ldr	r2, [r4, r0]
	lsls	r3, r3, #2
	mov	r7, fp
	adds	r2, r2, r3
	ldr	r0, [pc, #240]
	ldr	r3, [r7, #0]
	adds	r4, r2, r0
	cmp	r3, #0
	beq.n	.L_080cac1a
	ldrb	r2, [r2, #3]
	movs	r0, #64
	adds	r3, r0, #0
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080cac1a
	ldrb	r2, [r4, #3]
	adds	r3, r0, #0
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080cac1a
	movs	r3, #2
	strb	r3, [r6, #0]
	ldr	r0, [pc, #208]
	ldr	r2, [r5, #16]
	ldr	r1, [r5, #8]
	adds	r2, r2, r0
	movs	r0, #0
	bl	sub_080201c0
	ldr	r1, [pc, #196]
	ldr	r3, [r5, #12]
	adds	r0, r0, r1
	adds	r3, r3, r0
	str	r3, [r5, #12]
	str	r3, [r5, #20]
	adds	r3, r5, #0
	adds	r3, #85
	movs	r2, #0
	strb	r2, [r3, #0]
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_08020218
	movs	r1, #12
	adds	r0, r5, #0
	bl	sub_08020090
.L_080cac18:
	ldr	r1, [r5, #8]
.L_080cac1a:
	ldr	r3, [pc, #140]
	movs	r4, #128
	lsls	r4, r4, #2
	adds	r4, #118
	adds	r3, r3, r4
	movs	r7, #0
	ldrsh	r3, [r3, r7]
	cmp	r3, #0
	beq.n	.L_080cac4c
	mov	r0, r8
	ldr	r2, [r0, #52]
	str	r1, [r2, #8]
	movs	r1, #0
	ldr	r3, [r5, #12]
	str	r3, [r2, #12]
	ldr	r3, [r5, #16]
	str	r3, [r2, #16]
	ldr	r3, [r5, #20]
	str	r3, [r2, #20]
	ldrh	r3, [r5, #6]
	strh	r3, [r2, #6]
	str	r1, [r5, #8]
	str	r1, [r5, #16]
	adds	r5, r2, #0
	ldr	r1, [r5, #8]
.L_080cac4c:
	movs	r0, #128
	ldr	r2, [r5, #12]
	ldr	r3, [r5, #16]
	lsls	r0, r0, #8
	bl	sub_080200c0
	ldr	r3, [r5, #20]
	adds	r6, r0, #0
	str	r3, [r6, #20]
	adds	r1, r5, #0
	bl	sub_080200d8
	movs	r3, #197
	lsls	r3, r3, #1
	add	r3, r8
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #3
	bne.n	.L_080cac86
	movs	r1, #133
	ldr	r0, [r5, #80]
	lsls	r1, r1, #1
	bl	sub_08020058
	movs	r3, #15
	strb	r3, [r0, #5]
	movs	r3, #9
	strb	r3, [r0, #6]
.L_080cac86:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #32]
	adds	r3, r6, #0
	adds	r3, #8
	str	r3, [r2, #0]
	movs	r3, #230
	lsls	r3, r3, #1
	add	r3, r8
	str	r6, [r3, #0]
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x02000240
	.4byte 0x080f21d4
	.4byte 0x000fffff
	.4byte 0xfffffe00
	.4byte 0xfff00000
	.2byte 0x0000
	.2byte 0xffe0
	.2byte 0xb500
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r0, #7
	adds	r1, r3, #0
	movs	r2, #8
	adds	r1, #52
.L_080cacd0:
	ldmia	r1!, {r3}
	cmp	r3, #0
	beq.n	.L_080cacd8
	adds	r0, r2, #0
.L_080cacd8:
	adds	r2, #1
	cmp	r2, #63
	ble.n	.L_080cacd0
	adds	r0, #1
	cmp	r0, #64
	bne.n	.L_080cace8
	movs	r0, #1
	negs	r0, r0
.L_080cace8:
	pop	{pc}
