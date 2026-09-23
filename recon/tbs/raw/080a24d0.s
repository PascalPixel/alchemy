.syntax unified
	.thumb
	.set sub_08002dd8, 0x08002dd8
	.set sub_08002df0, 0x08002df0
	.set sub_080030f8, 0x080030f8
	.set sub_080048b0, 0x080048b0
	.set sub_08004938, 0x08004938
	.set sub_08004970, 0x08004970
	.set sub_080072f0, 0x080072f0
	.set sub_080072f8, 0x080072f8
	.set sub_080072fc, 0x080072fc
	.set sub_08007310, 0x08007310
	.set sub_08015010, 0x08015010
	.set sub_08015068, 0x08015068
	.set sub_08015080, 0x08015080
	.set sub_08015270, 0x08015270
	.set sub_08015278, 0x08015278
	.set sub_08015298, 0x08015298
	.set sub_080152a8, 0x080152a8
	.set sub_080153e0, 0x080153e0
	.set sub_08015408, 0x08015408
	.set sub_08015410, 0x08015410
	.set sub_08015418, 0x08015418
	.set sub_08077008, 0x08077008
	.set sub_08077010, 0x08077010
	.set sub_08077018, 0x08077018
	.set sub_08077028, 0x08077028
	.set sub_08077050, 0x08077050
	.set sub_08077058, 0x08077058
	.set sub_080770c0, 0x080770c0
	.set sub_08077158, 0x08077158
	.set sub_08077240, 0x08077240
	.set sub_080772c0, 0x080772c0
	.set sub_0808a490, 0x0808a490
	.set sub_0808a548, 0x0808a548
	.set sub_080a1050, 0x080a1050
	.set sub_080a1070, 0x080a1070
	.set sub_080a1090, 0x080a1090
	.set sub_080a112c, 0x080a112c
	.set sub_080a17c4, 0x080a17c4
	.set sub_080a1d08, 0x080a1d08
	.set sub_080a2144, 0x080a2144
	.set sub_080a23c0, 0x080a23c0
	.set sub_080a2474, 0x080a2474
	.set sub_080a2490, 0x080a2490
	.set sub_080a32b8, 0x080a32b8
	.set sub_080a3354, 0x080a3354
	.set sub_080a345c, 0x080a345c
	.set sub_080a3480, 0x080a3480
	.set sub_080a34c0, 0x080a34c0
	.set sub_080a355c, 0x080a355c
	.set sub_080a38d0, 0x080a38d0
	.set sub_080a3ce4, 0x080a3ce4
	.set sub_080a3cf8, 0x080a3cf8
	.set sub_080a3d6c, 0x080a3d6c
	.set sub_080a3d9c, 0x080a3d9c
	.set sub_080a3ddc, 0x080a3ddc
	.set sub_080a3e28, 0x080a3e28
	.set sub_080a3e88, 0x080a3e88
	.set sub_080a3ef0, 0x080a3ef0
	.set sub_080a414c, 0x080a414c
	.set sub_080a46b4, 0x080a46b4
	.set sub_080a4754, 0x080a4754
	.set sub_080a4800, 0x080a4800
	.set sub_080a4e20, 0x080a4e20
	.set sub_080a4e44, 0x080a4e44
	.set sub_080a4e68, 0x080a4e68
	.set sub_080a4e90, 0x080a4e90
	.set sub_080a4ee0, 0x080a4ee0
	.set sub_080a4f08, 0x080a4f08
	.set sub_080a51d0, 0x080a51d0
	.set sub_080a524c, 0x080a524c
	.set sub_080a5388, 0x080a5388
	.set sub_080a5534, 0x080a5534
	.set sub_080a5788, 0x080a5788
	.set sub_080ae8dc, 0x080ae8dc
	.set sub_080f9010, 0x080f9010
	.global RunAssetSelectionScreen
	.global Func_080a24d0
	.thumb_func
RunAssetSelectionScreen:
Func_080a24d0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #128
	lsls	r1, r1, #6
	mov	r9, r1
	mov	r0, r9
	sub	sp, #16
	bl	sub_08004970
	movs	r1, #167
	adds	r7, r0, #0
	lsls	r1, r1, #4
	movs	r0, #55
	bl	sub_080048b0
	ldr	r2, [pc, #280]
	mov	r8, r2
	ldr	r2, [r2, #0]
	movs	r3, #1
	movs	r1, #0
	adds	r5, r0, #0
	strh	r3, [r2, #4]
	movs	r0, #0
	movs	r2, #30
	movs	r3, #20
	bl	sub_08015408
	movs	r0, #1
	bl	sub_080030f8
	movs	r0, #0
	bl	sub_080a1090
	movs	r3, #130
	lsls	r3, r3, #2
	adds	r0, r5, r3
	bl	sub_08077158
	ldr	r1, [pc, #240]
	adds	r3, r5, r1
	movs	r2, #0
	movs	r1, #3
	strb	r0, [r3, #0]
	movs	r3, #7
	movs	r0, #0
	bl	sub_080a3354
	bl	sub_080a5534
	movs	r0, #14
	bl	sub_080a2144
	ldr	r0, [pc, #216]
	bl	sub_08015418
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #17
	movs	r3, #3
	movs	r0, #13
	bl	sub_08015010
	movs	r2, #134
	lsls	r2, r2, #1
	adds	r3, r5, r2
	str	r0, [r3, #0]
	bl	sub_080a1070
	ldr	r3, [pc, #184]
	ldr	r1, [pc, #188]
	mov	fp, r3
	mov	r2, r9
	adds	r0, r7, #0
	bl	sub_08007310
	ldr	r3, [pc, #180]
	mov	r1, r9
	ldr	r2, [pc, #180]
	ldr	r0, [pc, #168]
	bl	sub_080072f0
	movs	r0, #1
	bl	sub_080153e0
	bl	sub_080a2474
	add	r1, sp, #8
	add	r0, sp, #12
	add	r2, sp, #4
	bl	.L_080a2680
	mov	sl, r0
	bl	sub_080a2490
	mov	r1, sl
	cmp	r1, #1
	bne.n	.L_080a25c2
	mov	r2, r8
	ldr	r0, [r2, #84]
	ldr	r1, [sp, #12]
	ldr	r3, [sp, #4]
	ldr	r2, [pc, #136]
	lsls	r1, r1, #10
	ands	r3, r2
	subs	r2, #127
	orrs	r1, r3
	adds	r3, r0, r2
	strh	r1, [r3, #0]
	movs	r1, #186
	lsls	r1, r1, #1
	adds	r3, r5, r1
	ldrh	r3, [r3, #0]
	adds	r1, #38
	adds	r2, r0, r1
	strh	r3, [r2, #0]
.L_080a25c2:
	mov	r6, r8
	ldr	r0, [r5, #36]
	adds	r6, #36
	bl	sub_08015278
	ldr	r5, [pc, #100]
	ldr	r2, [r6, #0]
	ldr	r3, [pc, #60]
	strb	r3, [r2, r5]
	bl	sub_080a34c0
	movs	r1, #0
	movs	r2, #30
	movs	r3, #20
	movs	r0, #0
	bl	sub_08015408
	bl	sub_080ae8dc
	movs	r0, #55
	bl	sub_08002dd8
	mov	r3, r8
	ldr	r2, [r3, #0]
	movs	r3, #0
	strh	r3, [r2, #4]
	bl	sub_080152a8
	movs	r0, #0
	bl	sub_080153e0
	mov	r2, r9
	adds	r1, r7, #0
	ldr	r0, [pc, #28]
	bl	sub_08007310
	ldr	r3, [r6, #0]
	b.n	.L_080a2638
	movs	r0, r0
	.4byte 0x00000001
	.4byte 0x03001e68
	.4byte 0x00000219
	.4byte 0x06002500
	.4byte 0x03001388
	.4byte 0x06004000
	.4byte 0x03000168
	.4byte 0x33333333
	.4byte 0x000001ff
	.2byte 0x0ea6
	.2byte 0x0000
.L_080a2638:
	movs	r1, #0
	adds	r3, r3, r5
	strb	r1, [r3, #0]
	adds	r0, r7, #0
	bl	sub_08002df0
	movs	r0, #1
	bl	sub_080030f8
	bl	sub_080a1050
	movs	r0, #1
	bl	sub_080030f8
	movs	r0, #0
	movs	r1, #0
	movs	r2, #30
	movs	r3, #20
	bl	sub_08015410
	ldr	r3, [r6, #0]
	movs	r2, #0
	adds	r3, r3, r5
	strb	r2, [r3, #0]
	bl	sub_0808a548
	mov	r0, sl
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
.L_080a2680:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #40
	str	r1, [sp, #32]
	movs	r1, #0
	str	r0, [sp, #36]
	str	r2, [sp, #28]
	str	r1, [sp, #24]
	str	r1, [sp, #20]
	str	r1, [sp, #16]
	ldr	r3, [pc, #860]
	ldr	r3, [r3, #0]
	mov	r8, r1
	mov	r9, r3
	bl	.L_080a3252
.L_080a26aa:
	mov	r2, r8
	cmp	r2, #12
	bls.n	.L_080a26b4
	bl	.L_080a324e
.L_080a26b4:
	lsls	r3, r2, #2
	ldr	r2, [pc, #840]
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r6, #240
	lsrs	r2, r1, #32
	movs	r7, #80
	lsrs	r2, r1, #32
	cmp	r0, #216
	lsrs	r2, r1, #32
	adds	r0, #142
	lsrs	r2, r1, #32
	cmp	r3, #142
	lsrs	r2, r1, #32
	cmp	r2, #48
	lsrs	r2, r1, #32
	cmp	r1, #162
	lsrs	r2, r1, #32
	cmp	r5, #204
	lsrs	r2, r1, #32
	adds	r2, #82
	lsrs	r2, r1, #32
	movs	r7, #178
	lsrs	r2, r1, #32
	adds	r1, #252
	lsrs	r2, r1, #32
	adds	r1, #98
	lsrs	r2, r1, #32
	adds	r2, #60
	lsrs	r2, r1, #32
	movs	r2, #186
	lsls	r2, r2, #1
	add	r2, r9
	movs	r3, #0
	strh	r3, [r2, #0]
	bl	sub_080a4ee0
	bl	sub_080a4e44
	movs	r3, #135
	lsls	r3, r3, #2
	add	r3, r9
	ldr	r2, [r3, #0]
	movs	r3, #13
	strb	r3, [r2, #5]
	ldr	r1, [pc, #756]
	movs	r0, #0
	bl	sub_080a3cf8
	mov	r3, r9
	ldr	r0, [r3, #44]
	bl	sub_08015270
	mov	r1, r9
	ldr	r0, [r1, #44]
	bl	sub_080a23c0
	movs	r0, #0
	bl	sub_080a355c
	movs	r3, #1
	adds	r7, r0, #0
	negs	r3, r3
	cmp	r7, r3
	bne.n	.L_080a2740
	movs	r2, #0
	str	r3, [sp, #16]
	movs	r3, #1
	str	r2, [sp, #20]
	str	r3, [sp, #24]
.L_080a2740:
	mov	r1, r9
	ldr	r0, [r1, #44]
	bl	sub_08015270
	bl	sub_080a345c
	bl	.L_080a31f6
	ldr	r3, [pc, #692]
	add	r3, r9
	ldrb	r0, [r3, #0]
	bl	sub_080a3d6c
	movs	r3, #0
	mov	r8, r3
	cmp	r0, #0
	bne.n	.L_080a2766
	bl	.L_080a3252
.L_080a2766:
	bl	sub_080a4ee0
	bl	sub_080a4e44
	movs	r3, #135
	lsls	r3, r3, #2
	add	r3, r9
	ldr	r2, [r3, #0]
	movs	r3, #13
	strb	r3, [r2, #5]
	mov	r1, r9
	ldr	r2, [r1, #20]
	movs	r3, #1
	strb	r3, [r2, #5]
	ldr	r1, [pc, #648]
	movs	r0, #0
	bl	sub_080a3cf8
	movs	r0, #0
	bl	sub_080a5788
	movs	r3, #1
	movs	r2, #0
	negs	r3, r3
	str	r0, [sp, #20]
	mov	r8, r2
	cmp	r0, r3
	bne.n	.L_080a27a2
	bl	.L_080a3252
.L_080a27a2:
	ldr	r2, [pc, #620]
	movs	r3, #255
	add	r2, r9
	movs	r1, #9
	strb	r3, [r2, #0]
	mov	r8, r1
	bl	.L_080a3252
	bl	sub_080a414c
	movs	r5, #1
	adds	r7, r0, #0
	negs	r5, r5
	cmp	r7, r5
	bne.n	.L_080a27cc
	movs	r2, #1
	ldr	r3, [pc, #592]
	mov	r8, r2
	add	r3, r9
	mov	r1, r8
	strh	r1, [r3, #0]
.L_080a27cc:
	cmp	r7, #0
	bne.n	.L_080a28a8
	movs	r2, #188
	lsls	r2, r2, #1
	add	r2, r9
	ldrh	r3, [r2, #0]
	ldr	r0, [pc, #572]
	ands	r0, r3
	mov	sl, r2
	bl	sub_0808a490
	cmp	r0, #0
	beq.n	.L_080a280c
	movs	r3, #1
	str	r3, [sp, #24]
	ldr	r3, [pc, #540]
	add	r3, r9
	ldrb	r3, [r3, #0]
	ldr	r1, [sp, #36]
	str	r3, [r1, #0]
	ldr	r2, [sp, #32]
	str	r7, [r2, #0]
	mov	r3, sl
	ldrh	r2, [r3, #0]
	ldr	r3, [pc, #536]
	ldr	r1, [sp, #28]
	ands	r3, r2
	movs	r2, #1
	str	r3, [r1, #0]
	str	r2, [sp, #16]
	bl	.L_080a3252
.L_080a280c:
	ldr	r3, [pc, #504]
	mov	r2, sl
	add	r3, r9
	ldrb	r0, [r3, #0]
	ldrh	r1, [r2, #0]
	mov	fp, r3
	bl	sub_080a46b4
	adds	r6, r0, #0
	cmp	r6, #1
	bne.n	.L_080a2826
	movs	r3, #2
	mov	r8, r3
.L_080a2826:
	cmp	r6, #2
	bne.n	.L_080a287c
	bl	sub_080a32b8
	mov	r1, r9
	ldr	r0, [r1, #44]
	bl	sub_08015278
	ldr	r3, [pc, #484]
	add	r3, r9
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	ldr	r3, [pc, #480]
	adds	r2, r5, #0
	adds	r0, r0, r3
	movs	r1, #0
	bl	sub_080a1d08
	mov	r3, r9
	ldr	r2, [r3, #20]
	movs	r5, #228
	movs	r3, #13
	mov	r1, fp
	lsls	r5, r5, #1
	strb	r3, [r2, #5]
	ldrb	r0, [r1, #0]
	add	r5, r9
	bl	sub_08077008
	movs	r2, #0
	adds	r1, r5, #0
	bl	sub_080a3ddc
	movs	r3, #134
	lsls	r3, r3, #2
	add	r3, r9
	strb	r0, [r3, #0]
	movs	r1, #0
	adds	r0, r5, #0
	bl	sub_080a3e28
	movs	r2, #0
	mov	r8, r2
.L_080a287c:
	adds	r3, r6, #1
	cmp	r3, #1
	bhi.n	.L_080a28a8
	movs	r3, #1
	str	r3, [sp, #24]
	mov	r1, fp
	ldrb	r3, [r1, #0]
	ldr	r2, [sp, #36]
	str	r3, [r2, #0]
	ldr	r3, [pc, #404]
	add	r3, r9
	ldrb	r3, [r3, #0]
	ldr	r1, [sp, #32]
	str	r3, [r1, #0]
	mov	r3, sl
	ldrh	r2, [r3, #0]
	ldr	r3, [pc, #376]
	ldr	r1, [sp, #28]
	ands	r3, r2
	movs	r2, #1
	str	r3, [r1, #0]
	str	r2, [sp, #16]
.L_080a28a8:
	cmp	r7, #1
	bne.n	.L_080a28b0
	movs	r3, #3
	mov	r8, r3
.L_080a28b0:
	cmp	r7, #3
	bne.n	.L_080a28b8
	movs	r1, #6
	mov	r8, r1
.L_080a28b8:
	cmp	r7, #5
	bne.n	.L_080a28c0
	movs	r2, #5
	mov	r8, r2
.L_080a28c0:
	cmp	r7, #4
	bne.n	.L_080a28c8
	movs	r3, #11
	mov	r8, r3
.L_080a28c8:
	cmp	r7, #2
	beq.n	.L_080a28d0
	bl	.L_080a3252
.L_080a28d0:
	movs	r1, #10
	mov	r8, r1
	bl	.L_080a3252
	movs	r5, #134
	lsls	r5, r5, #1
	add	r5, r9
	bl	sub_080a345c
	bl	sub_080a4e68
	bl	sub_080a4e20
	ldr	r0, [r5, #0]
	bl	sub_08015270
	bl	sub_080a51d0
	ldr	r1, [r5, #0]
	ldr	r0, [pc, #304]
	movs	r2, #16
	movs	r3, #16
	bl	sub_08015080
	movs	r0, #0
	bl	sub_080a38d0
	movs	r5, #1
	negs	r5, r5
	cmp	r0, r5
	beq.n	.L_080a29f2
	movs	r3, #188
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r3, [r3, #0]
	ldr	r0, [pc, #256]
	ands	r0, r3
	movs	r7, #0
	bl	sub_080a3ce4
	cmp	r0, #0
	beq.n	.L_080a2926
	movs	r7, #8
.L_080a2926:
	bl	sub_080a32b8
	ldr	r3, [pc, #248]
	mov	r2, r9
	add	r3, r9
	adds	r6, r0, #0
	ldrb	r1, [r3, #0]
	ldr	r0, [r2, #36]
	adds	r3, r7, #0
	movs	r2, #0
	bl	sub_080a112c
	cmp	r6, r5
	beq.n	.L_080a296e
	mov	r3, r9
	ldr	r0, [r3, #44]
	bl	sub_08015278
	ldr	r3, [pc, #208]
	add	r3, r9
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	ldr	r3, [pc, #204]
	movs	r1, #0
	adds	r0, r0, r3
	adds	r2, r5, #0
	bl	sub_080a1d08
	mov	r3, r9
	ldr	r2, [r3, #20]
	movs	r3, #13
	strb	r3, [r2, #5]
	bl	sub_080a4754
	movs	r1, #1
	mov	r8, r1
.L_080a296e:
	ldr	r3, [pc, #152]
	movs	r5, #228
	add	r3, r9
	lsls	r5, r5, #1
	ldrb	r0, [r3, #0]
	add	r5, r9
	bl	sub_08077008
	movs	r2, #0
	adds	r1, r5, #0
	bl	sub_080a3ddc
	movs	r3, #134
	lsls	r3, r3, #2
	add	r3, r9
	strb	r0, [r3, #0]
	movs	r1, #0
	adds	r0, r5, #0
	bl	sub_080a3e28
	ldr	r2, [pc, #124]
	movs	r3, #1
	add	r2, r9
	strh	r3, [r2, #0]
	bl	.L_080a3252
	movs	r5, #134
	lsls	r5, r5, #1
	add	r5, r9
	bl	sub_080a4e68
	bl	sub_080a4e20
	ldr	r0, [r5, #0]
	bl	sub_08015270
	bl	sub_080a51d0
	ldr	r1, [r5, #0]
	movs	r3, #16
	ldr	r0, [pc, #108]
	movs	r2, #16
	bl	sub_08015080
	movs	r0, #1
	bl	sub_080a38d0
	movs	r1, #1
	movs	r3, #4
	negs	r1, r1
	mov	r8, r3
	cmp	r0, r1
	beq.n	.L_080a29dc
	bl	.L_080a3252
.L_080a29dc:
	ldr	r3, [pc, #40]
	movs	r2, #186
	add	r3, r9
	ldrb	r3, [r3, #0]
	lsls	r2, r2, #1
	add	r2, r9
	ldrh	r1, [r2, #0]
	adds	r0, r3, #0
	movs	r2, #0
	bl	sub_080a3ef0
.L_080a29f2:
	movs	r2, #9
	mov	r8, r2
	bl	.L_080a3252
	movs	r0, r0
	.4byte 0x03001f2c
	.4byte 0x080a26bc
	.4byte 0x00000ad8
	.4byte 0x0000021a
	.4byte 0x00000ad9
	.4byte 0x0000025d
	.4byte 0x00000222
	.4byte 0x000001ff
	.4byte 0x0000025a
	.4byte 0x00000bef
	.4byte 0x0000021b
	.4byte 0x00000adb
	.2byte 0x0adc
	.2byte 0x0000
	movs	r5, #188
	lsls	r5, r5, #1
	add	r5, r9
	bl	sub_080a345c
	ldrh	r3, [r5, #0]
	ldr	r0, [pc, #872]
	ands	r0, r3
	bl	sub_08077018
	ldrb	r2, [r0, #3]
	movs	r3, #16
	ands	r3, r2
	movs	r6, #0
	cmp	r3, #0
	beq.n	.L_080a2a6a
	ldrh	r3, [r5, #0]
	lsrs	r3, r3, #11
	adds	r5, r3, #1
	cmp	r5, #1
	ble.n	.L_080a2a6a
	bl	sub_080a51d0
	movs	r0, #0
	adds	r1, r5, #0
	movs	r2, #1
	bl	sub_080a4f08
	adds	r6, r0, #0
.L_080a2a6a:
	movs	r1, #1
	movs	r3, #9
	negs	r1, r1
	mov	r8, r3
	cmp	r6, r1
	bne.n	.L_080a2a7a
	bl	.L_080a3252
.L_080a2a7a:
	ldr	r2, [pc, #816]
	movs	r3, #0
	add	r2, r9
	strb	r3, [r2, #0]
	movs	r3, #188
	ldr	r2, [pc, #800]
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r3, [r3, #0]
	mov	r8, r2
	movs	r5, #135
	mov	r1, r8
	lsls	r5, r5, #2
	add	r5, r9
	ands	r1, r3
	lsls	r3, r6, #11
	orrs	r1, r3
	ldr	r3, [r5, #0]
	movs	r0, #2
	ldrb	r2, [r3, #14]
	movs	r3, #0
	bl	sub_08015298
	ldr	r2, [r5, #0]
	movs	r3, #1
	strb	r3, [r2, #5]
	ldr	r2, [r5, #0]
	movs	r3, #120
	strh	r3, [r2, #6]
	ldr	r2, [r5, #0]
	movs	r3, #28
	strh	r3, [r2, #8]
	ldr	r0, [r5, #0]
	bl	sub_080a17c4
	mov	r3, r9
	ldr	r0, [r3, #52]
	movs	r3, #96
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #72
	movs	r3, #120
	bl	sub_08015068
	movs	r3, #134
	lsls	r3, r3, #1
	add	r3, r9
	ldr	r0, [r3, #0]
	bl	sub_08015270
	ldr	r0, [sp, #20]
	bl	sub_080a524c
	cmp	r0, #0
	bne.n	.L_080a2b6e
	ldr	r3, [pc, #708]
	add	r3, r9
	ldrb	r7, [r3, #0]
	adds	r0, r7, #0
	bl	sub_08077008
	adds	r0, r6, #1
	cmp	r0, #0
	ble.n	.L_080a2b24
	adds	r5, r0, #0
	mov	r6, r8
.L_080a2afe:
	movs	r3, #186
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r1, [r3, #0]
	adds	r0, r7, #0
	bl	sub_08077058
	movs	r3, #188
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r3, [r3, #0]
	adds	r0, r6, #0
	ands	r0, r3
	movs	r1, #1
	subs	r5, #1
	bl	sub_08077240
	cmp	r5, #0
	bne.n	.L_080a2afe
.L_080a2b24:
	adds	r0, r7, #0
	bl	sub_08077010
	bl	sub_080a4e44
	ldr	r3, [pc, #640]
	add	r3, r9
	ldrb	r0, [r3, #0]
	movs	r1, #0
	bl	sub_080a3e88
	movs	r3, #135
	lsls	r3, r3, #2
	add	r3, r9
	ldr	r3, [r3, #0]
	movs	r2, #13
	strb	r2, [r3, #5]
	mov	r1, r9
	ldr	r3, [r1, #20]
	movs	r0, #1
	strb	r2, [r3, #5]
	bl	sub_080030f8
	mov	r2, r9
	ldr	r0, [r2, #44]
	bl	sub_08015278
	movs	r2, #13
	ldr	r0, [pc, #596]
	movs	r1, #14
	bl	sub_080a1d08
	ldr	r2, [pc, #592]
	movs	r3, #1
	add	r2, r9
	strh	r3, [r2, #0]
	b.n	.L_080a2b70
.L_080a2b6e:
	movs	r3, #9
.L_080a2b70:
	mov	r8, r3
	ldr	r3, [pc, #572]
	add	r3, r9
	ldrb	r0, [r3, #0]
	bl	sub_08077010
	movs	r3, #135
	lsls	r3, r3, #2
	add	r3, r9
	ldr	r2, [r3, #0]
	movs	r3, #13
	strb	r3, [r2, #5]
	bl	sub_0808a548
	b.n	.L_080a3252
	movs	r5, #188
	lsls	r5, r5, #1
	add	r5, r9
	ldr	r7, [pc, #528]
	ldrh	r3, [r5, #0]
	adds	r0, r7, #0
	movs	r1, #0
	ands	r0, r3
	mov	sl, r1
	bl	sub_08077018
	ldrb	r2, [r0, #3]
	movs	r3, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080a2c6a
	ldr	r6, [pc, #508]
	ldrh	r3, [r5, #0]
	add	r6, r9
	adds	r1, r7, #0
	ldrb	r0, [r6, #0]
	ands	r1, r3
	bl	sub_080a3d9c
	adds	r5, r0, #0
	cmp	r5, #30
	bne.n	.L_080a2bc8
	movs	r2, #1
	mov	sl, r2
.L_080a2bc8:
	ldrb	r0, [r6, #0]
	bl	sub_080a3d6c
	cmp	r0, #15
	bne.n	.L_080a2bd6
	cmp	r5, #0
	beq.n	.L_080a2c8a
.L_080a2bd6:
	movs	r3, #188
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r3, [r3, #0]
	mov	r1, sl
	lsrs	r3, r3, #11
	adds	r3, #1
	cmp	r1, #0
	bne.n	.L_080a2cb4
	lsls	r2, r3, #24
	asrs	r1, r2, #24
	adds	r3, r5, r1
	cmp	r3, #30
	ble.n	.L_080a2bf6
	movs	r3, #30
	subs	r1, r3, r5
.L_080a2bf6:
	movs	r3, #128
	lsls	r3, r3, #17
	cmp	r2, r3
	ble.n	.L_080a2c0a
	movs	r0, #0
	movs	r2, #0
	bl	sub_080a4f08
	adds	r6, r0, #0
	b.n	.L_080a2c0c
.L_080a2c0a:
	movs	r6, #0
.L_080a2c0c:
	movs	r1, #1
	negs	r1, r1
	cmp	r6, r1
	bne.n	.L_080a2c16
	b.n	.L_080a2df0
.L_080a2c16:
	movs	r7, #0
	adds	r6, #1
	cmp	r7, r6
	bge.n	.L_080a2cb4
	ldr	r3, [pc, #412]
	mov	fp, r3
.L_080a2c22:
	ldr	r3, [pc, #392]
	add	r3, r9
	ldrb	r0, [r3, #0]
	movs	r3, #188
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r3, [r3, #0]
	mov	r1, fp
	ands	r1, r3
	bl	sub_08077028
	movs	r1, #1
	adds	r5, r0, #0
	negs	r1, r1
	cmp	r5, r1
	beq.n	.L_080a2c5e
	ldr	r3, [pc, #364]
	add	r3, r9
	ldrb	r0, [r3, #0]
	movs	r3, #186
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r1, [r3, #0]
	bl	sub_08077058
	movs	r3, #187
	lsls	r3, r3, #1
	add	r3, r9
	strh	r5, [r3, #0]
	b.n	.L_080a2c62
.L_080a2c5e:
	movs	r2, #1
	mov	sl, r2
.L_080a2c62:
	adds	r7, #1
	cmp	r7, r6
	blt.n	.L_080a2c22
	b.n	.L_080a2cb4
.L_080a2c6a:
	ldr	r3, [pc, #320]
	add	r3, r9
	ldrb	r0, [r3, #0]
	movs	r3, #188
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r3, [r3, #0]
	ldr	r1, [pc, #320]
	ands	r1, r3
	bl	sub_08077028
	movs	r5, #1
	adds	r6, r0, #0
	negs	r5, r5
	cmp	r6, r5
	bne.n	.L_080a2c90
.L_080a2c8a:
	movs	r3, #7
	mov	r8, r3
	b.n	.L_080a3252
.L_080a2c90:
	movs	r3, #187
	lsls	r3, r3, #1
	add	r3, r9
	strh	r6, [r3, #0]
	ldr	r3, [pc, #276]
	add	r3, r9
	ldrb	r0, [r3, #0]
	movs	r3, #186
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r1, [r3, #0]
	bl	sub_08077058
	adds	r6, r0, #0
	cmp	r6, r5
	bne.n	.L_080a2cb4
	movs	r1, #1
	mov	sl, r1
.L_080a2cb4:
	ldr	r5, [pc, #248]
	ldr	r7, [pc, #244]
	add	r5, r9
	ldrb	r0, [r5, #0]
	add	r7, r9
	bl	sub_08077010
	ldrb	r0, [r7, #0]
	bl	sub_08077010
	ldrb	r0, [r5, #0]
	bl	sub_080772c0
	ldrb	r0, [r7, #0]
	bl	sub_080772c0
	mov	r2, sl
	movs	r6, #1
	cmp	r2, #0
	bne.n	.L_080a2d0a
	ldrb	r3, [r7, #0]
	movs	r2, #188
	strb	r3, [r5, #0]
	lsls	r2, r2, #1
	add	r2, r9
	ldrh	r1, [r2, #0]
	ldr	r3, [pc, #188]
	ands	r3, r1
	strh	r3, [r2, #0]
	bl	sub_080a4e90
	movs	r3, #134
	lsls	r3, r3, #1
	add	r3, r9
	ldr	r0, [r3, #0]
	bl	sub_08015270
	bl	sub_080a51d0
	movs	r0, #0
	bl	sub_080a5388
	adds	r6, r0, #0
.L_080a2d0a:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_080770c0
	cmp	r0, #0
	beq.n	.L_080a2d18
	b.n	.L_080a3252
.L_080a2d18:
	movs	r1, #1
	ldrb	r0, [r7, #0]
	bl	sub_080a3e88
	mov	r3, r9
	ldr	r2, [r3, #20]
	movs	r3, #13
	strb	r3, [r2, #5]
	movs	r0, #1
	bl	sub_080030f8
	mov	r1, sl
	cmp	r1, #1
	bne.n	.L_080a2d40
	mov	r2, r9
	ldr	r0, [r2, #44]
	bl	sub_08015278
	ldr	r0, [pc, #128]
	b.n	.L_080a2d4e
.L_080a2d40:
	mov	r3, r9
	ldr	r0, [r3, #44]
	bl	sub_08015278
	cmp	r6, #1
	bne.n	.L_080a2d58
	ldr	r0, [pc, #116]
.L_080a2d4e:
	movs	r1, #15
	movs	r2, #14
	bl	sub_080a1d08
	b.n	.L_080a3084
.L_080a2d58:
	movs	r2, #187
	ldrb	r3, [r7, #0]
	lsls	r2, r2, #1
	add	r2, r9
	adds	r0, r3, #0
	ldrh	r1, [r2, #0]
	movs	r2, #0
	bl	sub_080a3ef0
	ldr	r5, [pc, #92]
	movs	r2, #14
	adds	r0, r5, #0
	movs	r1, #15
	bl	sub_080a1d08
	movs	r3, #188
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r0, [r3, #0]
	bl	sub_08077018
	ldrb	r2, [r0, #3]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080a2d8e
	b.n	.L_080a3084
.L_080a2d8e:
	movs	r0, #103
	bl	sub_080f9010
	mov	r1, r9
	ldr	r0, [r1, #44]
	bl	sub_08015278
	adds	r0, r5, #7
	movs	r1, #14
	movs	r2, #14
	bl	sub_080a1d08
	b.n	.L_080a3084
	.4byte 0x000001ff
	.4byte 0x0000021b
	.4byte 0x0000021a
	.4byte 0x00000b7d
	.4byte 0x00000222
	.4byte 0x000005ff
	.4byte 0x00000b85
	.4byte 0x00000b7f
	.2byte 0x0b7c
	.2byte 0x0000
	movs	r3, #0
	mov	sl, r3
	bl	sub_080a4ee0
	bl	sub_080a4e44
	ldr	r1, [pc, #108]
	movs	r0, #0
	bl	sub_080a3cf8
	movs	r0, #1
	bl	sub_080a5788
	movs	r1, #1
	negs	r1, r1
	str	r0, [sp, #20]
	cmp	r0, r1
	bne.n	.L_080a2dfc
.L_080a2df0:
	movs	r2, #6
	mov	r8, r2
	b.n	.L_080a3252
.L_080a2df6:
	movs	r3, #1
	mov	sl, r3
	b.n	.L_080a2eea
.L_080a2dfc:
	ldr	r3, [pc, #76]
	add	r3, r9
	ldrb	r0, [r3, #0]
	bl	sub_08077008
	ldr	r3, [pc, #72]
	str	r0, [sp, #12]
	add	r3, r9
	ldrb	r0, [r3, #0]
	bl	sub_08077008
	movs	r5, #166
	lsls	r5, r5, #1
	str	r0, [sp, #8]
	adds	r0, r5, #0
	bl	sub_08004938
	mov	fp, r0
	adds	r0, r5, #0
	bl	sub_08004938
	adds	r2, r5, #0
	ldr	r1, [sp, #12]
	mov	r8, r0
	ldr	r6, [pc, #36]
	mov	r0, fp
	bl	sub_080072fc
	adds	r2, r5, #0
	mov	r0, r8
	ldr	r1, [sp, #8]
	bl	sub_080072fc
	adds	r5, #206
	movs	r7, #0
	add	r5, r9
	b.n	.L_080a2e5e
	movs	r0, r0
	.4byte 0x00000add
	.4byte 0x0000021a
	.4byte 0x0000021b
	.2byte 0x1388
	.2byte 0x0300
.L_080a2e58:
	adds	r3, r7, #1
	lsls	r3, r3, #24
	lsrs	r7, r3, #24
.L_080a2e5e:
	cmp	r7, #29
	bhi.n	.L_080a2e82
	movs	r3, #186
	lsls	r3, r3, #1
	add	r3, r9
	ldrb	r0, [r5, #0]
	ldrh	r1, [r3, #0]
	bl	sub_08077058
	adds	r6, r0, #0
	cmp	r6, #2
	beq.n	.L_080a2e82
	movs	r1, #1
	negs	r1, r1
	cmp	r6, r1
	bne.n	.L_080a2e58
	movs	r2, #1
	mov	sl, r2
.L_080a2e82:
	adds	r3, r7, #1
	lsls	r3, r3, #24
	lsrs	r7, r3, #24
	movs	r5, #0
.L_080a2e8a:
	movs	r3, #189
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r2, [r3, #0]
	ldr	r3, [pc, #48]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080a2eb0
	ldr	r0, [pc, #44]
	ands	r0, r2
	bl	sub_08077018
	ldrb	r2, [r0, #3]
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080a2eb0
	movs	r3, #1
	mov	sl, r3
.L_080a2eb0:
	ldr	r3, [pc, #24]
	add	r3, r9
	ldrb	r0, [r3, #0]
	movs	r3, #187
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r1, [r3, #0]
	bl	sub_08077058
	b.n	.L_080a2ed0
	.4byte 0x00000200
	.4byte 0x000001ff
	.2byte 0x021b
	.2byte 0x0000
.L_080a2ed0:
	adds	r6, r0, #0
	cmp	r6, #2
	beq.n	.L_080a2eea
	movs	r1, #1
	negs	r1, r1
	cmp	r6, r1
	bne.n	.L_080a2ee0
	b.n	.L_080a2df6
.L_080a2ee0:
	adds	r3, r5, #1
	lsls	r3, r3, #24
	lsrs	r5, r3, #24
	cmp	r5, #29
	bls.n	.L_080a2e8a
.L_080a2eea:
	adds	r3, r5, #1
	lsls	r3, r3, #24
	ldr	r2, [pc, #4]
	lsrs	r5, r3, #24
	b.n	.L_080a2f08
	.2byte 0x05ff
	.2byte 0x0000
.L_080a2ef8:
	movs	r3, #187
	lsls	r3, r3, #1
	add	r3, r9
	strh	r6, [r3, #0]
	adds	r3, r7, #0
	adds	r3, #255
	lsls	r3, r3, #24
	lsrs	r7, r3, #24
.L_080a2f08:
	cmp	r7, #0
	beq.n	.L_080a2f34
	ldr	r3, [pc, #44]
	add	r3, r9
	ldrb	r0, [r3, #0]
	movs	r3, #188
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r3, [r3, #0]
	adds	r1, r2, #0
	ands	r1, r3
	str	r2, [sp, #4]
	bl	sub_08077028
	movs	r3, #1
	adds	r6, r0, #0
	negs	r3, r3
	ldr	r2, [sp, #4]
	cmp	r6, r3
	bne.n	.L_080a2ef8
	movs	r1, #1
	mov	sl, r1
.L_080a2f34:
	ldr	r7, [pc, #0]
	b.n	.L_080a2f50
	.4byte 0x000005ff
	.2byte 0x021b
	.2byte 0x0000
.L_080a2f40:
	movs	r3, #186
	lsls	r3, r3, #1
	add	r3, r9
	strh	r6, [r3, #0]
	adds	r3, r5, #0
	adds	r3, #255
	lsls	r3, r3, #24
	lsrs	r5, r3, #24
.L_080a2f50:
	cmp	r5, #0
	beq.n	.L_080a2f78
	ldr	r3, [pc, #824]
	add	r3, r9
	ldrb	r0, [r3, #0]
	movs	r3, #189
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r3, [r3, #0]
	adds	r1, r7, #0
	ands	r1, r3
	bl	sub_08077028
	movs	r2, #1
	adds	r6, r0, #0
	negs	r2, r2
	cmp	r6, r2
	bne.n	.L_080a2f40
	movs	r3, #1
	mov	sl, r3
.L_080a2f78:
	movs	r0, #1
	bl	sub_080030f8
	mov	r1, sl
	cmp	r1, #1
	bne.n	.L_080a2fac
	movs	r2, #166
	ldr	r0, [sp, #12]
	ldr	r5, [pc, #776]
	mov	r1, fp
	lsls	r2, r2, #1
	bl	sub_080072f8
	movs	r2, #166
	mov	r1, r8
	ldr	r0, [sp, #8]
	lsls	r2, r2, #1
	bl	sub_080072f8
	mov	r2, r9
	ldr	r0, [r2, #44]
	bl	sub_08015278
	ldr	r0, [pc, #752]
	movs	r1, #15
	b.n	.L_080a3066
.L_080a2fac:
	ldr	r5, [pc, #736]
	ldr	r7, [pc, #748]
	add	r5, r9
	ldrb	r0, [r5, #0]
	add	r7, r9
	bl	sub_08077010
	ldrb	r0, [r7, #0]
	bl	sub_08077010
	ldrb	r0, [r5, #0]
	bl	sub_080772c0
	ldrb	r0, [r7, #0]
	bl	sub_080772c0
	bl	sub_080a4e68
	bl	sub_080a4e90
	bl	sub_080a3480
	movs	r3, #134
	lsls	r3, r3, #1
	add	r3, r9
	ldr	r0, [r3, #0]
	bl	sub_08015270
	ldrb	r3, [r7, #0]
	strb	r3, [r5, #0]
	movs	r5, #188
	lsls	r5, r5, #1
	add	r5, r9
	ldrh	r2, [r5, #0]
	ldr	r3, [pc, #684]
	ands	r3, r2
	strh	r3, [r5, #0]
	bl	sub_080a51d0
	movs	r0, #0
	bl	sub_080a5388
	adds	r6, r0, #0
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_080770c0
	cmp	r0, #0
	bne.n	.L_080a3078
	mov	r3, r9
	ldr	r0, [r3, #44]
	bl	sub_08015278
	bl	sub_080a4e20
	ldrb	r0, [r7, #0]
	movs	r1, #1
	bl	sub_080a3e88
	cmp	r6, #0
	bne.n	.L_080a306e
	movs	r2, #187
	ldrb	r3, [r7, #0]
	lsls	r2, r2, #1
	add	r2, r9
	adds	r0, r3, #0
	ldrh	r1, [r2, #0]
	movs	r2, #0
	bl	sub_080a3ef0
	ldr	r6, [pc, #616]
	movs	r2, #14
	adds	r0, r6, #0
	movs	r1, #15
	bl	sub_080a1d08
	ldrh	r0, [r5, #0]
	bl	sub_08077018
	ldrb	r2, [r0, #3]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080a3078
	movs	r0, #103
	bl	sub_080f9010
	mov	r1, r9
	ldr	r0, [r1, #44]
	bl	sub_08015278
	adds	r0, r6, #7
	movs	r1, #14
.L_080a3066:
	movs	r2, #14
	bl	sub_080a1d08
	b.n	.L_080a3078
.L_080a306e:
	ldr	r0, [pc, #568]
	movs	r1, #15
	movs	r2, #14
	bl	sub_080a1d08
.L_080a3078:
	mov	r0, r8
	bl	sub_08002df0
	mov	r0, fp
	bl	sub_08002df0
.L_080a3084:
	bl	sub_0808a548
	movs	r2, #0
	mov	r8, r2
	b.n	.L_080a3252
	ldr	r7, [pc, #512]
	movs	r3, #186
	lsls	r3, r3, #1
	add	r3, r9
	add	r7, r9
	ldrh	r1, [r3, #0]
	ldrb	r0, [r7, #0]
	mov	sl, r3
	bl	sub_08077050
	movs	r5, #1
	movs	r1, #1
	adds	r6, r0, #0
	negs	r5, r5
	mov	r8, r1
	cmp	r6, r5
	bne.n	.L_080a30b2
	b.n	.L_080a3252
.L_080a30b2:
	movs	r2, #2
	negs	r2, r2
	cmp	r6, r2
	bne.n	.L_080a30d2
	mov	r3, r9
	ldr	r0, [r3, #44]
	bl	sub_08015278
	movs	r1, #0
	ldr	r0, [pc, #484]
	adds	r2, r5, #0
	bl	sub_080a1d08
	movs	r1, #1
	mov	r8, r1
	b.n	.L_080a3252
.L_080a30d2:
	ldrb	r0, [r7, #0]
	bl	sub_08077010
	ldrb	r0, [r7, #0]
	bl	sub_080772c0
	mov	r3, r9
	ldr	r2, [r3, #20]
	movs	r5, #228
	movs	r3, #13
	lsls	r5, r5, #1
	strb	r3, [r2, #5]
	add	r5, r9
	ldrb	r0, [r7, #0]
	bl	sub_08077008
	movs	r2, #0
	adds	r1, r5, #0
	bl	sub_080a3ddc
	movs	r3, #134
	lsls	r3, r3, #2
	add	r3, r9
	strb	r0, [r3, #0]
	movs	r1, #0
	adds	r0, r5, #0
	bl	sub_080a3e28
	movs	r0, #1
	bl	sub_080030f8
	ldrb	r3, [r7, #0]
	mov	r2, sl
	ldrh	r1, [r2, #0]
	adds	r0, r3, #0
	movs	r2, #0
	bl	sub_080a3ef0
	mov	r3, r9
	ldr	r0, [r3, #44]
	bl	sub_08015278
	ldr	r5, [pc, #380]
	movs	r2, #8
	adds	r0, r5, #0
	movs	r1, #15
	bl	sub_080a1d08
	movs	r3, #188
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r0, [r3, #0]
	bl	sub_08077018
	ldrb	r2, [r0, #3]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080a31f6
	movs	r0, #103
	bl	sub_080f9010
	mov	r1, r9
	ldr	r0, [r1, #44]
	bl	sub_08015278
	adds	r0, r5, #7
	movs	r1, #14
	movs	r2, #8
	bl	sub_080a1d08
	b.n	.L_080a31f6
	ldr	r6, [pc, #300]
	add	r6, r9
	ldrb	r0, [r6, #0]
	bl	sub_08077008
	movs	r3, #186
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r2, [r3, #0]
	lsls	r2, r2, #1
	adds	r2, #216
	ldrh	r1, [r0, r2]
	mov	r8, r3
	ldr	r3, [pc, #304]
	ands	r3, r1
	strh	r3, [r0, r2]
	ldrb	r0, [r6, #0]
	bl	sub_08077010
	ldrb	r0, [r6, #0]
	bl	sub_080772c0
	mov	r1, r9
	ldr	r2, [r1, #20]
	movs	r3, #0
	movs	r5, #228
	mov	sl, r3
	lsls	r5, r5, #1
	movs	r3, #13
	strb	r3, [r2, #5]
	add	r5, r9
	ldrb	r0, [r6, #0]
	bl	sub_08077008
	movs	r2, #0
	adds	r1, r5, #0
	bl	sub_080a3ddc
	movs	r3, #134
	lsls	r3, r3, #2
	add	r3, r9
	strb	r0, [r3, #0]
	movs	r1, #0
	adds	r0, r5, #0
	movs	r5, #151
	bl	sub_080a3e28
	lsls	r5, r5, #2
	movs	r0, #1
	bl	sub_080030f8
	add	r5, r9
	movs	r3, #1
	strb	r3, [r5, #0]
	ldrb	r3, [r6, #0]
	mov	r2, r8
	ldrh	r1, [r2, #0]
	adds	r0, r3, #0
	movs	r2, #0
	bl	sub_080a3ef0
	mov	r3, sl
	strb	r3, [r5, #0]
	mov	r1, r9
	ldr	r0, [r1, #44]
	bl	sub_08015278
	movs	r2, #8
	ldr	r0, [pc, #200]
	movs	r1, #14
	bl	sub_080a1d08
	bl	sub_0808a548
.L_080a31f6:
	movs	r2, #1
	mov	r8, r2
	b.n	.L_080a3252
	mov	r3, r9
	ldr	r2, [r3, #20]
	movs	r3, #13
	strb	r3, [r2, #5]
	movs	r3, #188
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r0, [r3, #0]
	bl	sub_080a4800
	mov	r1, r9
	ldr	r0, [r1, #36]
	bl	sub_08015270
	ldr	r3, [pc, #116]
	movs	r2, #186
	add	r3, r9
	ldrb	r3, [r3, #0]
	lsls	r2, r2, #1
	add	r2, r9
	ldrh	r1, [r2, #0]
	adds	r0, r3, #0
	movs	r2, #0
	bl	sub_080a3ef0
	mov	r3, r9
	ldr	r2, [r3, #20]
	movs	r1, #9
	movs	r3, #1
	strb	r3, [r2, #5]
	mov	r8, r1
	b.n	.L_080a3252
	movs	r0, #0
	movs	r1, #30
	movs	r2, #0
	bl	sub_080a4f08
	movs	r3, #1
	adds	r6, r0, #0
	mov	r8, r3
	b.n	.L_080a3252
.L_080a324e:
	movs	r1, #1
	str	r1, [sp, #24]
.L_080a3252:
	ldr	r2, [sp, #24]
	cmp	r2, #0
	bne.n	.L_080a3268
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_080770c0
	cmp	r0, #0
	bne.n	.L_080a3268
	bl	.L_080a26aa
.L_080a3268:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	sub_080770c0
	cmp	r0, #0
	beq.n	.L_080a327a
	movs	r3, #1
	negs	r3, r3
	str	r3, [sp, #16]
.L_080a327a:
	ldr	r0, [sp, #16]
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x0000021a
	.4byte 0x03001388
	.4byte 0x00000b84
	.4byte 0x0000021b
	.4byte 0x000001ff
	.4byte 0x00000b7c
	.4byte 0x00000b81
	.4byte 0x00000b82
	.4byte 0x0000fdff
	.4byte 0x00000b80
