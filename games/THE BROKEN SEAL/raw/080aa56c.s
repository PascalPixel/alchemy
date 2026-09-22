.syntax unified
	.thumb
	.set sub_08002304, 0x08002304
	.set sub_08002dd8, 0x08002dd8
	.set sub_08002df0, 0x08002df0
	.set sub_080030f8, 0x080030f8
	.set sub_080048b0, 0x080048b0
	.set sub_08004970, 0x08004970
	.set sub_080072f8, 0x080072f8
	.set sub_08015010, 0x08015010
	.set sub_08015278, 0x08015278
	.set sub_080152a8, 0x080152a8
	.set sub_080153e0, 0x080153e0
	.set sub_08015408, 0x08015408
	.set sub_08015410, 0x08015410
	.set sub_08015418, 0x08015418
	.set sub_08077010, 0x08077010
	.set sub_080770c0, 0x080770c0
	.set sub_08077158, 0x08077158
	.set sub_080771b0, 0x080771b0
	.set sub_080771b8, 0x080771b8
	.set sub_080771c0, 0x080771c0
	.set sub_080771c8, 0x080771c8
	.set sub_080771d0, 0x080771d0
	.set sub_0808a548, 0x0808a548
	.set sub_080a1050, 0x080a1050
	.set sub_080a1070, 0x080a1070
	.set sub_080a1090, 0x080a1090
	.set sub_080a2144, 0x080a2144
	.set sub_080a3354, 0x080a3354
	.set sub_080a34c0, 0x080a34c0
	.set sub_080aa544, 0x080aa544
	.set sub_080aad10, 0x080aad10
	.set sub_080aaf58, 0x080aaf58
	.set sub_080aafb8, 0x080aafb8
	.set sub_080ab314, 0x080ab314
	.set sub_080ab5e4, 0x080ab5e4
	.set sub_080ad508, 0x080ad508
	.set sub_080ad5b4, 0x080ad5b4
	.set sub_080ad658, 0x080ad658
	.set sub_080ad6d4, 0x080ad6d4
	.set sub_080ae2f4, 0x080ae2f4
	.set sub_080ae88c, 0x080ae88c
	.set sub_080ae8dc, 0x080ae8dc
	.set sub_080f9010, 0x080f9010
	.global Overlay_080aa56c
Overlay_080aa56c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r1, #167
	lsls	r1, r1, #4
	movs	r0, #55
	sub	sp, #4
	bl	sub_080048b0
	ldr	r3, [pc, #136]
	movs	r2, #131
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldrb	r2, [r3, #0]
	mov	r9, r2
	movs	r2, #2
	strb	r2, [r3, #0]
	ldr	r3, [pc, #124]
	movs	r2, #1
	ldr	r3, [r3, #0]
	mov	sl, r2
	mov	r2, sl
	movs	r1, #0
	strh	r2, [r3, #4]
	adds	r7, r0, #0
	movs	r2, #30
	movs	r3, #20
	movs	r0, #0
	bl	sub_08015408
	movs	r0, #1
	bl	sub_080030f8
	movs	r0, #0
	bl	sub_080a1090
	ldr	r0, [pc, #88]
	bl	sub_08004970
	movs	r2, #194
	lsls	r2, r2, #1
	adds	r3, r7, r2
	str	r0, [r3, #0]
	ldr	r6, [pc, #80]
	ldr	r3, [pc, #80]
	mov	r8, r0
	movs	r5, #0
	add	r3, r8
	add	r6, r8
	movs	r0, #183
	str	r5, [r3, #0]
	str	r5, [r6, #0]
	lsls	r0, r0, #1
	bl	sub_080770c0
	cmp	r0, #0
	beq.n	.L_080aa62c
	ldr	r0, [pc, #60]
	bl	sub_080770c0
	cmp	r0, #0
	bne.n	.L_080aa5fe
	ldr	r0, [pc, #52]
	bl	sub_080770c0
	cmp	r0, #0
	bne.n	.L_080aa5fa
	mov	r3, sl
	b.n	.L_080aa62a
.L_080aa5fa:
	movs	r3, #14
	b.n	.L_080aa62a
.L_080aa5fe:
	ldr	r0, [pc, #36]
	bl	sub_080770c0
	cmp	r0, #0
	bne.n	.L_080aa628
	movs	r3, #27
	b.n	.L_080aa62a
	.4byte 0x02000240
	.4byte 0x03001e68
	.4byte 0x00002130
	.4byte 0x0000212c
	.4byte 0x00002128
	.4byte 0x0000016f
	.2byte 0x0171
	.2byte 0x0000
.L_080aa628:
	movs	r3, #28
.L_080aa62a:
	str	r3, [r6, #0]
.L_080aa62c:
	bl	sub_080a1070
	movs	r0, #1
	bl	sub_080153e0
	ldr	r0, [pc, #140]
	bl	sub_08015418
	movs	r2, #130
	lsls	r2, r2, #2
	adds	r0, r7, r2
	bl	sub_08077158
	ldr	r2, [pc, #128]
	adds	r3, r7, r2
	strb	r0, [r3, #0]
	bl	sub_080ae88c
	movs	r1, #3
	movs	r2, #0
	movs	r3, #7
	movs	r0, #0
	bl	sub_080a3354
	movs	r0, #0
	bl	sub_080aa544
	movs	r0, #14
	bl	sub_080a2144
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #17
	movs	r3, #5
	movs	r0, #13
	bl	sub_08015010
	movs	r2, #188
	lsls	r2, r2, #1
	movs	r3, #134
	adds	r1, r7, r2
	lsls	r3, r3, #1
	ldr	r2, [pc, #60]
	adds	r4, r7, r3
	movs	r3, #255
	str	r0, [r4, #0]
	strh	r3, [r1, #0]
	strb	r2, [r7, #28]
	strb	r2, [r7, #29]
	movs	r2, #186
	lsls	r2, r2, #1
	movs	r5, #0
	adds	r3, r7, r2
	adds	r2, #2
	strh	r5, [r3, #0]
	adds	r3, r7, r2
	strh	r5, [r3, #0]
	movs	r1, #0
	ldr	r0, [r4, #0]
	bl	sub_080ad508
	bl	.L_080aa768
	bl	sub_080ad658
	bl	sub_080ae8dc
	movs	r0, #1
	bl	sub_080030f8
	bl	sub_080a34c0
	b.n	.L_080aa6cc
	.4byte 0x00000000
	.4byte 0x06002500
	.2byte 0x0219
	.2byte 0x0000
.L_080aa6cc:
	movs	r1, #0
	movs	r2, #30
	movs	r0, #0
	movs	r3, #20
	bl	sub_08015408
	ldr	r3, [pc, #116]
	ldr	r3, [r3, #0]
	strh	r5, [r3, #4]
	bl	sub_080152a8
	movs	r0, #0
	bl	sub_080153e0
	mov	r1, r8
	movs	r2, #128
	ldr	r5, [pc, #100]
	adds	r1, #168
	lsls	r2, r2, #6
	ldr	r0, [pc, #100]
	bl	sub_080072f8
	ldr	r1, [pc, #96]
	movs	r2, #128
	add	r1, r8
	ldr	r0, [pc, #96]
	bl	sub_080072f8
	movs	r0, #1
	bl	sub_080030f8
	bl	sub_080a1050
	movs	r1, #0
	movs	r0, #0
	movs	r2, #30
	movs	r3, #20
	bl	sub_08015410
	movs	r2, #194
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldr	r0, [r3, #0]
	bl	sub_08002df0
	movs	r0, #55
	bl	sub_08002dd8
	bl	sub_0808a548
	ldr	r3, [pc, #48]
	movs	r2, #131
	lsls	r2, r2, #2
	adds	r3, r3, r2
	mov	r2, r9
	movs	r0, #1
	strb	r2, [r3, #0]
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001e68
	.4byte 0x03001388
	.4byte 0x06004000
	.4byte 0x000020a8
	.4byte 0x05000080
	.2byte 0x0240
	.2byte 0x0200
.L_080aa768:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #792]
	ldr	r7, [r3, #0]
	movs	r4, #0
	ldr	r2, [r7, #20]
	mov	sl, r4
	movs	r3, #13
	strb	r3, [r2, #5]
	mov	r3, sl
	sub	sp, #8
	movs	r1, #0
	strh	r3, [r2, #12]
	str	r4, [sp, #0]
	mov	r8, r1
	bl	sub_080aad10
	movs	r0, #1
	bl	sub_080030f8
	ldr	r4, [sp, #0]
	movs	r5, #2
.L_080aa798:
	cmp	r5, #15
	bls.n	.L_080aa79e
	b.n	.L_080aac52
.L_080aa79e:
	ldr	r2, [pc, #752]
	lsls	r3, r5, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x080aa7e8
	.4byte 0x080aac56
	.4byte 0x080aa7fa
	.4byte 0x080aa8b0
	.4byte 0x080aabc0
	.4byte 0x080aabec
	.4byte 0x080aaaa4
	.4byte 0x080aa9be
	.4byte 0x080aa880
	.4byte 0x080aaaec
	.4byte 0x080aa83c
	.4byte 0x080aac06
	.4byte 0x080aaac0
	.4byte 0x080aa9da
	.4byte 0x080aab08
	.2byte 0xa86a
	.2byte 0x080a
	cmp	r4, #0
	blt.n	.L_080aa7ee
	b.n	.L_080aabbc
.L_080aa7ee:
	movs	r1, #1
	negs	r1, r1
	movs	r2, #1
	mov	sl, r1
	mov	r8, r2
	b.n	.L_080aabbc
	movs	r0, #0
	bl	sub_080aa544
	movs	r1, #0
	movs	r2, #200
	movs	r3, #0
	movs	r0, #1
	bl	sub_080ad5b4
	movs	r0, #0
	bl	sub_080ab5e4
	adds	r4, r0, #0
	movs	r5, #15
	cmp	r4, #10
	bne.n	.L_080aa81c
	b.n	.L_080aac56
.L_080aa81c:
	movs	r5, #0
	cmp	r4, #0
	bge.n	.L_080aa824
	b.n	.L_080aac56
.L_080aa824:
	movs	r1, #187
	movs	r3, #28
	ldrsb	r3, [r7, r3]
	lsls	r1, r1, #1
	adds	r2, r7, r1
	strh	r3, [r2, #0]
	movs	r5, #10
	cmp	r4, #7
	bne.n	.L_080aa838
	b.n	.L_080aac56
.L_080aa838:
	movs	r5, #3
	b.n	.L_080aac56
	movs	r3, #28
	ldrsb	r3, [r7, r3]
	movs	r2, #130
	lsls	r2, r2, #2
	lsls	r3, r3, #1
	adds	r3, r3, r2
	ldrh	r2, [r7, r3]
	ldr	r1, [pc, #584]
	str	r2, [r7, #8]
	ldrh	r2, [r7, r3]
	adds	r3, r7, r1
	strb	r2, [r3, #0]
	bl	sub_080ae2f4
	movs	r2, #2
	adds	r4, r0, #0
	negs	r2, r2
	cmp	r4, r2
	beq.n	.L_080aa864
	b.n	.L_080aabbc
.L_080aa864:
	movs	r3, #1
	mov	r8, r3
	b.n	.L_080aabbc
	bl	sub_080ab314
	movs	r1, #2
	adds	r4, r0, #0
	negs	r1, r1
	cmp	r4, r1
	beq.n	.L_080aa87a
	b.n	.L_080aabbc
.L_080aa87a:
	movs	r2, #1
	mov	r8, r2
	b.n	.L_080aabbc
	movs	r1, #134
	lsls	r1, r1, #2
	adds	r3, r7, r1
	ldrb	r3, [r3, #0]
	movs	r5, #0
	cmp	r3, #0
	bne.n	.L_080aa890
	b.n	.L_080aac56
.L_080aa890:
	movs	r0, #1
	bl	sub_080ab5e4
	movs	r2, #2
	adds	r4, r0, #0
	negs	r2, r2
	cmp	r4, r2
	bne.n	.L_080aa8a4
	movs	r3, #1
	mov	r8, r3
.L_080aa8a4:
	movs	r5, #4
	cmp	r4, #0
	bge.n	.L_080aa8ac
	b.n	.L_080aac56
.L_080aa8ac:
	movs	r5, #9
	b.n	.L_080aac56
	movs	r1, #194
	lsls	r1, r1, #1
	adds	r3, r7, r1
	ldr	r0, [r3, #0]
	bl	sub_080aafb8
	movs	r0, #8
	negs	r0, r0
	bl	sub_080aa544
	movs	r3, #28
	ldrsb	r3, [r7, r3]
	movs	r2, #130
	lsls	r2, r2, #2
	lsls	r3, r3, #1
	adds	r3, r3, r2
	ldrh	r2, [r7, r3]
	ldr	r1, [pc, #448]
	str	r2, [r7, #8]
	ldrh	r2, [r7, r3]
	adds	r3, r7, r1
	strb	r2, [r3, #0]
	movs	r3, #28
	ldrsb	r3, [r7, r3]
	lsls	r1, r3, #3
	subs	r1, r1, r3
	lsls	r1, r1, #3
	adds	r1, #48
	movs	r2, #54
	movs	r3, #0
	movs	r0, #0
	bl	sub_080ad5b4
	movs	r0, #1
	bl	sub_080ab5e4
	ldr	r2, [pc, #412]
	adds	r3, r7, r2
	ldrb	r3, [r3, #0]
	movs	r1, #0
	adds	r4, r0, #0
	cmp	r1, r3
	bge.n	.L_080aa91a
	adds	r0, r7, r2
	subs	r2, #213
.L_080aa90a:
	ldrh	r3, [r2, r7]
	adds	r3, #8
	strh	r3, [r2, r7]
	ldrb	r3, [r0, #0]
	adds	r1, #1
	adds	r2, #2
	cmp	r1, r3
	blt.n	.L_080aa90a
.L_080aa91a:
	movs	r3, #2
	negs	r3, r3
	cmp	r4, r3
	bne.n	.L_080aa926
	movs	r1, #1
	mov	r8, r1
.L_080aa926:
	cmp	r4, #0
	bge.n	.L_080aa92c
	b.n	.L_080aabbc
.L_080aa92c:
	subs	r3, r4, #3
	cmp	r3, #1
	bls.n	.L_080aa93a
	cmp	r4, #8
	beq.n	.L_080aa93a
	cmp	r4, #9
	bne.n	.L_080aa94e
.L_080aa93a:
	movs	r3, #29
	ldrsb	r3, [r7, r3]
	movs	r2, #130
	lsls	r2, r2, #2
	lsls	r3, r3, #1
	adds	r3, r3, r2
	ldr	r1, [pc, #340]
	ldrh	r2, [r7, r3]
	adds	r3, r7, r1
	strb	r2, [r3, #0]
.L_080aa94e:
	cmp	r4, #0
	bge.n	.L_080aa954
	b.n	.L_080aabbc
.L_080aa954:
	cmp	r4, #1
	bne.n	.L_080aa95c
	movs	r5, #5
	b.n	.L_080aac56
.L_080aa95c:
	cmp	r4, #2
	bne.n	.L_080aa964
	movs	r5, #6
	b.n	.L_080aac56
.L_080aa964:
	cmp	r4, #3
	bne.n	.L_080aa976
	movs	r3, #136
	lsls	r3, r3, #2
	adds	r2, r7, r3
	movs	r3, #2
	strh	r3, [r2, #0]
	movs	r5, #7
	b.n	.L_080aac56
.L_080aa976:
	cmp	r4, #4
	bne.n	.L_080aa988
	movs	r1, #136
	lsls	r1, r1, #2
	adds	r2, r7, r1
	movs	r3, #2
	strh	r3, [r2, #0]
	movs	r5, #9
	b.n	.L_080aac56
.L_080aa988:
	cmp	r4, #5
	bne.n	.L_080aa990
	movs	r5, #11
	b.n	.L_080aac56
.L_080aa990:
	cmp	r4, #6
	bne.n	.L_080aa998
	movs	r5, #12
	b.n	.L_080aac56
.L_080aa998:
	cmp	r4, #8
	bne.n	.L_080aa9aa
	movs	r3, #136
	lsls	r3, r3, #2
	adds	r2, r7, r3
	movs	r3, #2
	strh	r3, [r2, #0]
	movs	r5, #13
	b.n	.L_080aac56
.L_080aa9aa:
	cmp	r4, #9
	beq.n	.L_080aa9b0
	b.n	.L_080aac56
.L_080aa9b0:
	movs	r1, #136
	lsls	r1, r1, #2
	adds	r2, r7, r1
	movs	r3, #2
	strh	r3, [r2, #0]
	movs	r5, #14
	b.n	.L_080aac56
	movs	r0, #1
	bl	sub_080ad6d4
	movs	r2, #2
	adds	r4, r0, #0
	negs	r2, r2
	cmp	r4, r2
	bne.n	.L_080aa9d2
	movs	r3, #1
	mov	r8, r3
.L_080aa9d2:
	movs	r5, #3
	cmp	r4, #0
	bge.n	.L_080aa9da
	b.n	.L_080aac56
.L_080aa9da:
	movs	r0, #126
	bl	sub_080f9010
	ldr	r2, [pc, #188]
	ldr	r1, [pc, #176]
	adds	r3, r7, r2
	subs	r2, #2
	adds	r6, r7, r1
	ldrb	r1, [r3, #0]
	adds	r3, r7, r2
	ldrb	r2, [r3, #0]
	ldr	r3, [pc, #168]
	adds	r5, r7, r3
	ldrb	r3, [r5, #0]
	ldrb	r0, [r6, #0]
	bl	sub_080771d0
	adds	r4, r0, #0
	ldrb	r0, [r6, #0]
	str	r4, [sp, #0]
	bl	sub_08077010
	ldrb	r0, [r5, #0]
	bl	sub_08077010
	ldr	r2, [r7, #20]
	movs	r3, #13
	strb	r3, [r2, #5]
	ldr	r0, [r7, #48]
	bl	sub_08015278
	movs	r1, #194
	lsls	r1, r1, #1
	adds	r3, r7, r1
	ldr	r0, [r3, #0]
	bl	sub_080aaf58
	movs	r2, #187
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldrh	r0, [r3, #0]
	movs	r1, #10
	bl	sub_08002304
	movs	r1, #188
	movs	r3, #0
	lsls	r1, r1, #1
	mov	ip, r3
	adds	r3, r7, r1
	lsls	r0, r0, #16
	ldrb	r6, [r3, #0]
	lsrs	r0, r0, #16
	movs	r1, #0
	adds	r5, r0, #0
	adds	r5, #160
	ldr	r4, [sp, #0]
	b.n	.L_080aaa4e
.L_080aaa4c:
	adds	r1, #1
.L_080aaa4e:
	movs	r2, #194
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldr	r2, [r3, #0]
	ldrsb	r3, [r2, r5]
	cmp	r1, r3
	bge.n	.L_080aaa6e
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #1
	adds	r3, r3, r1
	lsls	r3, r3, #1
	ldrb	r3, [r2, r3]
	cmp	r6, r3
	bne.n	.L_080aaa4c
	mov	ip, r1
.L_080aaa6e:
	mov	r1, ip
	lsls	r3, r1, #2
	add	r3, ip
	movs	r1, #186
	lsls	r1, r1, #1
	lsls	r3, r3, #1
	adds	r3, r0, r3
	adds	r2, r7, r1
	strh	r3, [r2, #0]
	ldr	r2, [r7, #20]
	movs	r3, #1
	strb	r3, [r2, #5]
	movs	r5, #0
	b.n	.L_080aac56
	movs	r0, r0
	.4byte 0x03001f2c
	.4byte 0x080aa7a8
	.4byte 0x0000021a
	.4byte 0x00000219
	.4byte 0x0000021b
	.2byte 0x0256
	.2byte 0x0000
	movs	r0, #2
	bl	sub_080ad6d4
	movs	r2, #2
	adds	r4, r0, #0
	negs	r2, r2
	cmp	r4, r2
	bne.n	.L_080aaab8
	movs	r3, #1
	mov	r8, r3
.L_080aaab8:
	movs	r5, #3
	cmp	r4, #0
	bge.n	.L_080aaac0
	b.n	.L_080aac56
.L_080aaac0:
	movs	r0, #175
	bl	sub_080f9010
	ldr	r2, [pc, #424]
	ldr	r1, [pc, #424]
	adds	r3, r7, r2
	subs	r2, #2
	adds	r6, r7, r1
	adds	r5, r7, r2
	ldrb	r1, [r3, #0]
	ldrb	r2, [r5, #0]
	ldrb	r0, [r6, #0]
	str	r3, [sp, #4]
	bl	sub_080771b8
	ldr	r3, [sp, #4]
	ldrb	r2, [r5, #0]
	ldrb	r1, [r3, #0]
	ldrb	r0, [r6, #0]
	bl	sub_080771c8
	b.n	.L_080aac30
	movs	r0, #0
	bl	sub_080ad6d4
	movs	r3, #2
	adds	r4, r0, #0
	negs	r3, r3
	cmp	r4, r3
	bne.n	.L_080aab00
	movs	r1, #1
	mov	r8, r1
.L_080aab00:
	movs	r5, #3
	cmp	r4, #0
	bge.n	.L_080aab08
	b.n	.L_080aac56
.L_080aab08:
	movs	r0, #126
	bl	sub_080f9010
	ldr	r2, [pc, #356]
	ldr	r1, [pc, #348]
	adds	r6, r7, r2
	adds	r3, r7, r1
	adds	r2, #58
	ldrb	r1, [r3, #0]
	adds	r3, r7, r2
	ldrb	r2, [r3, #0]
	ldr	r3, [pc, #344]
	adds	r5, r7, r3
	ldrb	r3, [r5, #0]
	ldrb	r0, [r6, #0]
	bl	sub_080771d0
	ldr	r1, [pc, #336]
	ldr	r2, [pc, #336]
	adds	r3, r7, r1
	ldrb	r1, [r3, #0]
	adds	r3, r7, r2
	ldrb	r2, [r3, #0]
	ldrb	r0, [r5, #0]
	ldrb	r3, [r6, #0]
	bl	sub_080771d0
	adds	r4, r0, #0
	ldrb	r0, [r6, #0]
	str	r4, [sp, #0]
	bl	sub_08077010
	ldrb	r0, [r5, #0]
	bl	sub_08077010
	movs	r1, #194
	lsls	r1, r1, #1
	adds	r3, r7, r1
	ldr	r0, [r3, #0]
	bl	sub_080aaf58
	movs	r2, #187
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldrh	r0, [r3, #0]
	movs	r1, #10
	bl	sub_08002304
	movs	r1, #188
	movs	r3, #0
	lsls	r1, r1, #1
	mov	ip, r3
	adds	r3, r7, r1
	lsls	r0, r0, #16
	ldrb	r6, [r3, #0]
	lsrs	r0, r0, #16
	movs	r1, #0
	adds	r5, r0, #0
	adds	r5, #160
	ldr	r4, [sp, #0]
	b.n	.L_080aab84
.L_080aab82:
	adds	r1, #1
.L_080aab84:
	movs	r2, #194
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldr	r2, [r3, #0]
	ldrsb	r3, [r2, r5]
	cmp	r1, r3
	bge.n	.L_080aaba4
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #1
	adds	r3, r3, r1
	lsls	r3, r3, #1
	ldrb	r3, [r2, r3]
	cmp	r6, r3
	bne.n	.L_080aab82
	mov	ip, r1
.L_080aaba4:
	mov	r1, ip
	lsls	r3, r1, #2
	add	r3, ip
	movs	r1, #186
	lsls	r1, r1, #1
	lsls	r3, r3, #1
	adds	r3, r0, r3
	adds	r2, r7, r1
	strh	r3, [r2, #0]
	ldr	r2, [r7, #20]
	movs	r3, #1
	strb	r3, [r2, #5]
.L_080aabbc:
	movs	r5, #2
	b.n	.L_080aac56
	movs	r2, #1
	negs	r2, r2
	cmp	r4, r2
	bne.n	.L_080aabcc
	mov	sl, r4
	b.n	.L_080aabbc
.L_080aabcc:
	movs	r1, #136
	lsls	r1, r1, #2
	adds	r3, r7, r1
	ldrh	r2, [r3, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080aabe0
	movs	r5, #8
	b.n	.L_080aac56
.L_080aabe0:
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080aac56
	movs	r5, #7
	b.n	.L_080aac56
	movs	r0, #3
	bl	sub_080ad6d4
	movs	r2, #2
	adds	r4, r0, #0
	negs	r2, r2
	cmp	r4, r2
	bne.n	.L_080aac00
	movs	r3, #1
	mov	r8, r3
.L_080aac00:
	movs	r5, #3
	cmp	r4, #0
	blt.n	.L_080aac56
	movs	r0, #139
	bl	sub_080f9010
	ldr	r2, [pc, #96]
	ldr	r1, [pc, #100]
	adds	r3, r7, r2
	subs	r2, #2
	adds	r6, r7, r1
	adds	r5, r7, r2
	ldrb	r1, [r3, #0]
	ldrb	r2, [r5, #0]
	ldrb	r0, [r6, #0]
	str	r3, [sp, #4]
	bl	sub_080771b0
	ldr	r3, [sp, #4]
	ldrb	r2, [r5, #0]
	ldrb	r1, [r3, #0]
	ldrb	r0, [r6, #0]
	bl	sub_080771c0
.L_080aac30:
	adds	r4, r0, #0
	ldrb	r0, [r6, #0]
	str	r4, [sp, #0]
	bl	sub_08077010
	ldr	r2, [r7, #20]
	movs	r3, #13
	strb	r3, [r2, #5]
	ldr	r0, [r7, #48]
	bl	sub_08015278
	ldr	r2, [r7, #20]
	movs	r3, #1
	strb	r3, [r2, #5]
	movs	r5, #2
	ldr	r4, [sp, #0]
	b.n	.L_080aac56
.L_080aac52:
	movs	r3, #1
	mov	r8, r3
.L_080aac56:
	mov	r1, r8
	cmp	r1, #0
	bne.n	.L_080aac5e
	b.n	.L_080aa798
.L_080aac5e:
	mov	r0, sl
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x00000256
	.4byte 0x0000021a
	.4byte 0x0000021b
	.4byte 0x00000257
	.4byte 0x00000255
