@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080a2680
	.thumb_func
Func_080a2680:
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
	bl	.L0
.L71:
	mov	r2, r8
	cmp	r2, #12
	bls.n	.L1
	bl	.L2
.L1:
	lsls	r3, r2, #2
	ldr	r2, [pc, #840]
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x080a26f0
	.4byte 0x080a2750
	.4byte 0x080a28d8
	.4byte 0x080a308e
	.4byte 0x080a2b8e
	.4byte 0x080a2a30
	.4byte 0x080a29a2
	.4byte 0x080a2dcc
	.4byte 0x080a3252
	.4byte 0x080a27b2
	.4byte 0x080a31fc
	.4byte 0x080a3162
	.4byte 0x080a323c
	movs	r2, #186
	lsls	r2, r2, #1
	add	r2, r9
	movs	r3, #0
	strh	r3, [r2, #0]
	bl	Func_080a4ee0
	bl	Func_080a4e44
	movs	r3, #135
	lsls	r3, r3, #2
	add	r3, r9
	ldr	r2, [r3, #0]
	movs	r3, #13
	strb	r3, [r2, #5]
	ldr	r1, [pc, #756]
	movs	r0, #0
	bl	Func_080a3cf8
	mov	r3, r9
	ldr	r0, [r3, #44]
	bl	Func_08015270
	mov	r1, r9
	ldr	r0, [r1, #44]
	bl	Func_080a23c0
	movs	r0, #0
	bl	Func_080a355c
	movs	r3, #1
	adds	r7, r0, #0
	negs	r3, r3
	cmp	r7, r3
	bne.n	.L3
	movs	r2, #0
	str	r3, [sp, #16]
	movs	r3, #1
	str	r2, [sp, #20]
	str	r3, [sp, #24]
.L3:
	mov	r1, r9
	ldr	r0, [r1, #44]
	bl	Func_08015270
	bl	Func_080a345c
	bl	.L4
	ldr	r3, [pc, #692]
	add	r3, r9
	ldrb	r0, [r3, #0]
	bl	Func_080a3d6c
	movs	r3, #0
	mov	r8, r3
	cmp	r0, #0
	bne.n	.L5
	bl	.L0
.L5:
	bl	Func_080a4ee0
	bl	Func_080a4e44
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
	bl	Func_080a3cf8
	movs	r0, #0
	bl	Func_080a5788
	movs	r3, #1
	movs	r2, #0
	negs	r3, r3
	str	r0, [sp, #20]
	mov	r8, r2
	cmp	r0, r3
	bne.n	.L6
	bl	.L0
.L6:
	ldr	r2, [pc, #620]
	movs	r3, #255
	add	r2, r9
	movs	r1, #9
	strb	r3, [r2, #0]
	mov	r8, r1
	bl	.L0
	bl	Func_080a414c
	movs	r5, #1
	adds	r7, r0, #0
	negs	r5, r5
	cmp	r7, r5
	bne.n	.L7
	movs	r2, #1
	ldr	r3, [pc, #592]
	mov	r8, r2
	add	r3, r9
	mov	r1, r8
	strh	r1, [r3, #0]
.L7:
	cmp	r7, #0
	bne.n	.L8
	movs	r2, #188
	lsls	r2, r2, #1
	add	r2, r9
	ldrh	r3, [r2, #0]
	ldr	r0, [pc, #572]
	ands	r0, r3
	mov	sl, r2
	bl	Func_0808a490
	cmp	r0, #0
	beq.n	.L9
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
	bl	.L0
.L9:
	ldr	r3, [pc, #504]
	mov	r2, sl
	add	r3, r9
	ldrb	r0, [r3, #0]
	ldrh	r1, [r2, #0]
	mov	fp, r3
	bl	Func_080a46b4
	adds	r6, r0, #0
	cmp	r6, #1
	bne.n	.L10
	movs	r3, #2
	mov	r8, r3
.L10:
	cmp	r6, #2
	bne.n	.L11
	bl	Func_080a32b8
	mov	r1, r9
	ldr	r0, [r1, #44]
	bl	Func_08015278
	ldr	r3, [pc, #484]
	add	r3, r9
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	ldr	r3, [pc, #480]
	adds	r2, r5, #0
	adds	r0, r0, r3
	movs	r1, #0
	bl	Func_080a1d08
	mov	r3, r9
	ldr	r2, [r3, #20]
	movs	r5, #228
	movs	r3, #13
	mov	r1, fp
	lsls	r5, r5, #1
	strb	r3, [r2, #5]
	ldrb	r0, [r1, #0]
	add	r5, r9
	bl	Func_08077008
	movs	r2, #0
	adds	r1, r5, #0
	bl	Func_080a3ddc
	movs	r3, #134
	lsls	r3, r3, #2
	add	r3, r9
	strb	r0, [r3, #0]
	movs	r1, #0
	adds	r0, r5, #0
	bl	Func_080a3e28
	movs	r2, #0
	mov	r8, r2
.L11:
	adds	r3, r6, #1
	cmp	r3, #1
	bhi.n	.L8
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
.L8:
	cmp	r7, #1
	bne.n	.L12
	movs	r3, #3
	mov	r8, r3
.L12:
	cmp	r7, #3
	bne.n	.L13
	movs	r1, #6
	mov	r8, r1
.L13:
	cmp	r7, #5
	bne.n	.L14
	movs	r2, #5
	mov	r8, r2
.L14:
	cmp	r7, #4
	bne.n	.L15
	movs	r3, #11
	mov	r8, r3
.L15:
	cmp	r7, #2
	beq.n	.L16
	bl	.L0
.L16:
	movs	r1, #10
	mov	r8, r1
	bl	.L0
	movs	r5, #134
	lsls	r5, r5, #1
	add	r5, r9
	bl	Func_080a345c
	bl	Func_080a4e68
	bl	Func_080a4e20
	ldr	r0, [r5, #0]
	bl	Func_08015270
	bl	Func_080a51d0
	ldr	r1, [r5, #0]
	ldr	r0, [pc, #304]
	movs	r2, #16
	movs	r3, #16
	bl	Func_08015080
	movs	r0, #0
	bl	Func_080a38d0
	movs	r5, #1
	negs	r5, r5
	cmp	r0, r5
	beq.n	.L17
	movs	r3, #188
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r3, [r3, #0]
	ldr	r0, [pc, #256]
	ands	r0, r3
	movs	r7, #0
	bl	Func_080a3ce4
	cmp	r0, #0
	beq.n	.L18
	movs	r7, #8
.L18:
	bl	Func_080a32b8
	ldr	r3, [pc, #248]
	mov	r2, r9
	add	r3, r9
	adds	r6, r0, #0
	ldrb	r1, [r3, #0]
	ldr	r0, [r2, #36]
	adds	r3, r7, #0
	movs	r2, #0
	bl	Func_080a112c
	cmp	r6, r5
	beq.n	.L19
	mov	r3, r9
	ldr	r0, [r3, #44]
	bl	Func_08015278
	ldr	r3, [pc, #208]
	add	r3, r9
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	ldr	r3, [pc, #204]
	movs	r1, #0
	adds	r0, r0, r3
	adds	r2, r5, #0
	bl	Func_080a1d08
	mov	r3, r9
	ldr	r2, [r3, #20]
	movs	r3, #13
	strb	r3, [r2, #5]
	bl	Func_080a4754
	movs	r1, #1
	mov	r8, r1
.L19:
	ldr	r3, [pc, #152]
	movs	r5, #228
	add	r3, r9
	lsls	r5, r5, #1
	ldrb	r0, [r3, #0]
	add	r5, r9
	bl	Func_08077008
	movs	r2, #0
	adds	r1, r5, #0
	bl	Func_080a3ddc
	movs	r3, #134
	lsls	r3, r3, #2
	add	r3, r9
	strb	r0, [r3, #0]
	movs	r1, #0
	adds	r0, r5, #0
	bl	Func_080a3e28
	ldr	r2, [pc, #124]
	movs	r3, #1
	add	r2, r9
	strh	r3, [r2, #0]
	bl	.L0
	movs	r5, #134
	lsls	r5, r5, #1
	add	r5, r9
	bl	Func_080a4e68
	bl	Func_080a4e20
	ldr	r0, [r5, #0]
	bl	Func_08015270
	bl	Func_080a51d0
	ldr	r1, [r5, #0]
	movs	r3, #16
	ldr	r0, [pc, #108]
	movs	r2, #16
	bl	Func_08015080
	movs	r0, #1
	bl	Func_080a38d0
	movs	r1, #1
	movs	r3, #4
	negs	r1, r1
	mov	r8, r3
	cmp	r0, r1
	beq.n	.L20
	bl	.L0
.L20:
	ldr	r3, [pc, #40]
	movs	r2, #186
	add	r3, r9
	ldrb	r3, [r3, #0]
	lsls	r2, r2, #1
	add	r2, r9
	ldrh	r1, [r2, #0]
	adds	r0, r3, #0
	movs	r2, #0
	bl	Func_080a3ef0
.L17:
	movs	r2, #9
	mov	r8, r2
	bl	.L0
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
	.4byte 0x00000adc
	movs	r5, #188
	lsls	r5, r5, #1
	add	r5, r9
	bl	Func_080a345c
	ldrh	r3, [r5, #0]
	ldr	r0, [pc, #872]
	ands	r0, r3
	bl	Func_08077018
	ldrb	r2, [r0, #3]
	movs	r3, #16
	ands	r3, r2
	movs	r6, #0
	cmp	r3, #0
	beq.n	.L21
	ldrh	r3, [r5, #0]
	lsrs	r3, r3, #11
	adds	r5, r3, #1
	cmp	r5, #1
	ble.n	.L21
	bl	Func_080a51d0
	movs	r0, #0
	adds	r1, r5, #0
	movs	r2, #1
	bl	Func_080a4f08
	adds	r6, r0, #0
.L21:
	movs	r1, #1
	movs	r3, #9
	negs	r1, r1
	mov	r8, r3
	cmp	r6, r1
	bne.n	.L22
	bl	.L0
.L22:
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
	bl	Func_08015298
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
	bl	Func_080a17c4
	mov	r3, r9
	ldr	r0, [r3, #52]
	movs	r3, #96
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #72
	movs	r3, #120
	bl	Func_08015068
	movs	r3, #134
	lsls	r3, r3, #1
	add	r3, r9
	ldr	r0, [r3, #0]
	bl	Func_08015270
	ldr	r0, [sp, #20]
	bl	Func_080a524c
	cmp	r0, #0
	bne.n	.L23
	ldr	r3, [pc, #708]
	add	r3, r9
	ldrb	r7, [r3, #0]
	adds	r0, r7, #0
	bl	Func_08077008
	adds	r0, r6, #1
	cmp	r0, #0
	ble.n	.L24
	adds	r5, r0, #0
	mov	r6, r8
.L25:
	movs	r3, #186
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r1, [r3, #0]
	adds	r0, r7, #0
	bl	Func_08077058
	movs	r3, #188
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r3, [r3, #0]
	adds	r0, r6, #0
	ands	r0, r3
	movs	r1, #1
	subs	r5, #1
	bl	Func_08077240
	cmp	r5, #0
	bne.n	.L25
.L24:
	adds	r0, r7, #0
	bl	Func_08077010
	bl	Func_080a4e44
	ldr	r3, [pc, #640]
	add	r3, r9
	ldrb	r0, [r3, #0]
	movs	r1, #0
	bl	Func_080a3e88
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
	bl	Func_080030f8
	mov	r2, r9
	ldr	r0, [r2, #44]
	bl	Func_08015278
	movs	r2, #13
	ldr	r0, [pc, #596]
	movs	r1, #14
	bl	Func_080a1d08
	ldr	r2, [pc, #592]
	movs	r3, #1
	add	r2, r9
	strh	r3, [r2, #0]
	b.n	.L26
.L23:
	movs	r3, #9
.L26:
	mov	r8, r3
	ldr	r3, [pc, #572]
	add	r3, r9
	ldrb	r0, [r3, #0]
	bl	Func_08077010
	movs	r3, #135
	lsls	r3, r3, #2
	add	r3, r9
	ldr	r2, [r3, #0]
	movs	r3, #13
	strb	r3, [r2, #5]
	bl	Func_0808a548
	b.n	.L0
	movs	r5, #188
	lsls	r5, r5, #1
	add	r5, r9
	ldr	r7, [pc, #528]
	ldrh	r3, [r5, #0]
	adds	r0, r7, #0
	movs	r1, #0
	ands	r0, r3
	mov	sl, r1
	bl	Func_08077018
	ldrb	r2, [r0, #3]
	movs	r3, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L27
	ldr	r6, [pc, #508]
	ldrh	r3, [r5, #0]
	add	r6, r9
	adds	r1, r7, #0
	ldrb	r0, [r6, #0]
	ands	r1, r3
	bl	Func_080a3d9c
	adds	r5, r0, #0
	cmp	r5, #30
	bne.n	.L28
	movs	r2, #1
	mov	sl, r2
.L28:
	ldrb	r0, [r6, #0]
	bl	Func_080a3d6c
	cmp	r0, #15
	bne.n	.L29
	cmp	r5, #0
	beq.n	.L30
.L29:
	movs	r3, #188
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r3, [r3, #0]
	mov	r1, sl
	lsrs	r3, r3, #11
	adds	r3, #1
	cmp	r1, #0
	bne.n	.L31
	lsls	r2, r3, #24
	asrs	r1, r2, #24
	adds	r3, r5, r1
	cmp	r3, #30
	ble.n	.L32
	movs	r3, #30
	subs	r1, r3, r5
.L32:
	movs	r3, #128
	lsls	r3, r3, #17
	cmp	r2, r3
	ble.n	.L33
	movs	r0, #0
	movs	r2, #0
	bl	Func_080a4f08
	adds	r6, r0, #0
	b.n	.L34
.L33:
	movs	r6, #0
.L34:
	movs	r1, #1
	negs	r1, r1
	cmp	r6, r1
	bne.n	.L35
	b.n	.L36
.L35:
	movs	r7, #0
	adds	r6, #1
	cmp	r7, r6
	bge.n	.L31
	ldr	r3, [pc, #412]
	mov	fp, r3
.L39:
	ldr	r3, [pc, #392]
	add	r3, r9
	ldrb	r0, [r3, #0]
	movs	r3, #188
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r3, [r3, #0]
	mov	r1, fp
	ands	r1, r3
	bl	Func_08077028
	movs	r1, #1
	adds	r5, r0, #0
	negs	r1, r1
	cmp	r5, r1
	beq.n	.L37
	ldr	r3, [pc, #364]
	add	r3, r9
	ldrb	r0, [r3, #0]
	movs	r3, #186
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r1, [r3, #0]
	bl	Func_08077058
	movs	r3, #187
	lsls	r3, r3, #1
	add	r3, r9
	strh	r5, [r3, #0]
	b.n	.L38
.L37:
	movs	r2, #1
	mov	sl, r2
.L38:
	adds	r7, #1
	cmp	r7, r6
	blt.n	.L39
	b.n	.L31
.L27:
	ldr	r3, [pc, #320]
	add	r3, r9
	ldrb	r0, [r3, #0]
	movs	r3, #188
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r3, [r3, #0]
	ldr	r1, [pc, #320]
	ands	r1, r3
	bl	Func_08077028
	movs	r5, #1
	adds	r6, r0, #0
	negs	r5, r5
	cmp	r6, r5
	bne.n	.L40
.L30:
	movs	r3, #7
	mov	r8, r3
	b.n	.L0
.L40:
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
	bl	Func_08077058
	adds	r6, r0, #0
	cmp	r6, r5
	bne.n	.L31
	movs	r1, #1
	mov	sl, r1
.L31:
	ldr	r5, [pc, #248]
	ldr	r7, [pc, #244]
	add	r5, r9
	ldrb	r0, [r5, #0]
	add	r7, r9
	bl	Func_08077010
	ldrb	r0, [r7, #0]
	bl	Func_08077010
	ldrb	r0, [r5, #0]
	bl	Func_080772c0
	ldrb	r0, [r7, #0]
	bl	Func_080772c0
	mov	r2, sl
	movs	r6, #1
	cmp	r2, #0
	bne.n	.L41
	ldrb	r3, [r7, #0]
	movs	r2, #188
	strb	r3, [r5, #0]
	lsls	r2, r2, #1
	add	r2, r9
	ldrh	r1, [r2, #0]
	ldr	r3, [pc, #188]
	ands	r3, r1
	strh	r3, [r2, #0]
	bl	Func_080a4e90
	movs	r3, #134
	lsls	r3, r3, #1
	add	r3, r9
	ldr	r0, [r3, #0]
	bl	Func_08015270
	bl	Func_080a51d0
	movs	r0, #0
	bl	Func_080a5388
	adds	r6, r0, #0
.L41:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L42
	b.n	.L0
.L42:
	movs	r1, #1
	ldrb	r0, [r7, #0]
	bl	Func_080a3e88
	mov	r3, r9
	ldr	r2, [r3, #20]
	movs	r3, #13
	strb	r3, [r2, #5]
	movs	r0, #1
	bl	Func_080030f8
	mov	r1, sl
	cmp	r1, #1
	bne.n	.L43
	mov	r2, r9
	ldr	r0, [r2, #44]
	bl	Func_08015278
	ldr	r0, [pc, #128]
	b.n	.L44
.L43:
	mov	r3, r9
	ldr	r0, [r3, #44]
	bl	Func_08015278
	cmp	r6, #1
	bne.n	.L45
	ldr	r0, [pc, #116]
.L44:
	movs	r1, #15
	movs	r2, #14
	bl	Func_080a1d08
	b.n	.L46
.L45:
	movs	r2, #187
	ldrb	r3, [r7, #0]
	lsls	r2, r2, #1
	add	r2, r9
	adds	r0, r3, #0
	ldrh	r1, [r2, #0]
	movs	r2, #0
	bl	Func_080a3ef0
	ldr	r5, [pc, #92]
	movs	r2, #14
	adds	r0, r5, #0
	movs	r1, #15
	bl	Func_080a1d08
	movs	r3, #188
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r0, [r3, #0]
	bl	Func_08077018
	ldrb	r2, [r0, #3]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L47
	b.n	.L46
.L47:
	movs	r0, #103
	bl	Func_080f9010
	mov	r1, r9
	ldr	r0, [r1, #44]
	bl	Func_08015278
	adds	r0, r5, #7
	movs	r1, #14
	movs	r2, #14
	bl	Func_080a1d08
	b.n	.L46
	.4byte 0x000001ff
	.4byte 0x0000021b
	.4byte 0x0000021a
	.4byte 0x00000b7d
	.4byte 0x00000222
	.4byte 0x000005ff
	.4byte 0x00000b85
	.4byte 0x00000b7f
	.4byte 0x00000b7c
	movs	r3, #0
	mov	sl, r3
	bl	Func_080a4ee0
	bl	Func_080a4e44
	ldr	r1, [pc, #108]
	movs	r0, #0
	bl	Func_080a3cf8
	movs	r0, #1
	bl	Func_080a5788
	movs	r1, #1
	negs	r1, r1
	str	r0, [sp, #20]
	cmp	r0, r1
	bne.n	.L48
.L36:
	movs	r2, #6
	mov	r8, r2
	b.n	.L0
.L56:
	movs	r3, #1
	mov	sl, r3
	b.n	.L49
.L48:
	ldr	r3, [pc, #76]
	add	r3, r9
	ldrb	r0, [r3, #0]
	bl	Func_08077008
	ldr	r3, [pc, #72]
	str	r0, [sp, #12]
	add	r3, r9
	ldrb	r0, [r3, #0]
	bl	Func_08077008
	movs	r5, #166
	lsls	r5, r5, #1
	str	r0, [sp, #8]
	adds	r0, r5, #0
	bl	Func_08004938
	mov	fp, r0
	adds	r0, r5, #0
	bl	Func_08004938
	adds	r2, r5, #0
	ldr	r1, [sp, #12]
	mov	r8, r0
	ldr	r6, [pc, #36]
	mov	r0, fp
	bl	Func_080072fc
	adds	r2, r5, #0
	mov	r0, r8
	ldr	r1, [sp, #8]
	bl	Func_080072fc
	adds	r5, #206
	movs	r7, #0
	add	r5, r9
	b.n	.L50
	movs	r0, r0
	.4byte 0x00000add
	.4byte 0x0000021a
	.4byte 0x0000021b
	.4byte 0x03001388
.L52:
	adds	r3, r7, #1
	lsls	r3, r3, #24
	lsrs	r7, r3, #24
.L50:
	cmp	r7, #29
	bhi.n	.L51
	movs	r3, #186
	lsls	r3, r3, #1
	add	r3, r9
	ldrb	r0, [r5, #0]
	ldrh	r1, [r3, #0]
	bl	Func_08077058
	adds	r6, r0, #0
	cmp	r6, #2
	beq.n	.L51
	movs	r1, #1
	negs	r1, r1
	cmp	r6, r1
	bne.n	.L52
	movs	r2, #1
	mov	sl, r2
.L51:
	adds	r3, r7, #1
	lsls	r3, r3, #24
	lsrs	r7, r3, #24
	movs	r5, #0
.L57:
	movs	r3, #189
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r2, [r3, #0]
	ldr	r3, [pc, #48]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L53
	ldr	r0, [pc, #44]
	ands	r0, r2
	bl	Func_08077018
	ldrb	r2, [r0, #3]
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L53
	movs	r3, #1
	mov	sl, r3
.L53:
	ldr	r3, [pc, #24]
	add	r3, r9
	ldrb	r0, [r3, #0]
	movs	r3, #187
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r1, [r3, #0]
	bl	Func_08077058
	b.n	.L54
	.4byte 0x00000200
	.4byte 0x000001ff
	.4byte 0x0000021b
.L54:
	adds	r6, r0, #0
	cmp	r6, #2
	beq.n	.L49
	movs	r1, #1
	negs	r1, r1
	cmp	r6, r1
	bne.n	.L55
	b.n	.L56
.L55:
	adds	r3, r5, #1
	lsls	r3, r3, #24
	lsrs	r5, r3, #24
	cmp	r5, #29
	bls.n	.L57
.L49:
	adds	r3, r5, #1
	lsls	r3, r3, #24
	ldr	r2, [pc, #4]
	lsrs	r5, r3, #24
	b.n	.L58
	.4byte 0x000005ff
.L60:
	movs	r3, #187
	lsls	r3, r3, #1
	add	r3, r9
	strh	r6, [r3, #0]
	adds	r3, r7, #0
	adds	r3, #255
	lsls	r3, r3, #24
	lsrs	r7, r3, #24
.L58:
	cmp	r7, #0
	beq.n	.L59
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
	bl	Func_08077028
	movs	r3, #1
	adds	r6, r0, #0
	negs	r3, r3
	ldr	r2, [sp, #4]
	cmp	r6, r3
	bne.n	.L60
	movs	r1, #1
	mov	sl, r1
.L59:
	ldr	r7, [pc, #0]
	b.n	.L61
	.4byte 0x000005ff
	.4byte 0x0000021b
.L63:
	movs	r3, #186
	lsls	r3, r3, #1
	add	r3, r9
	strh	r6, [r3, #0]
	adds	r3, r5, #0
	adds	r3, #255
	lsls	r3, r3, #24
	lsrs	r5, r3, #24
.L61:
	cmp	r5, #0
	beq.n	.L62
	ldr	r3, [pc, #824]
	add	r3, r9
	ldrb	r0, [r3, #0]
	movs	r3, #189
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r3, [r3, #0]
	adds	r1, r7, #0
	ands	r1, r3
	bl	Func_08077028
	movs	r2, #1
	adds	r6, r0, #0
	negs	r2, r2
	cmp	r6, r2
	bne.n	.L63
	movs	r3, #1
	mov	sl, r3
.L62:
	movs	r0, #1
	bl	Func_080030f8
	mov	r1, sl
	cmp	r1, #1
	bne.n	.L64
	movs	r2, #166
	ldr	r0, [sp, #12]
	ldr	r5, [pc, #776]
	mov	r1, fp
	lsls	r2, r2, #1
	bl	Func_080072f8
	movs	r2, #166
	mov	r1, r8
	ldr	r0, [sp, #8]
	lsls	r2, r2, #1
	bl	Func_080072f8
	mov	r2, r9
	ldr	r0, [r2, #44]
	bl	Func_08015278
	ldr	r0, [pc, #752]
	movs	r1, #15
	b.n	.L65
.L64:
	ldr	r5, [pc, #736]
	ldr	r7, [pc, #748]
	add	r5, r9
	ldrb	r0, [r5, #0]
	add	r7, r9
	bl	Func_08077010
	ldrb	r0, [r7, #0]
	bl	Func_08077010
	ldrb	r0, [r5, #0]
	bl	Func_080772c0
	ldrb	r0, [r7, #0]
	bl	Func_080772c0
	bl	Func_080a4e68
	bl	Func_080a4e90
	bl	Func_080a3480
	movs	r3, #134
	lsls	r3, r3, #1
	add	r3, r9
	ldr	r0, [r3, #0]
	bl	Func_08015270
	ldrb	r3, [r7, #0]
	strb	r3, [r5, #0]
	movs	r5, #188
	lsls	r5, r5, #1
	add	r5, r9
	ldrh	r2, [r5, #0]
	ldr	r3, [pc, #684]
	ands	r3, r2
	strh	r3, [r5, #0]
	bl	Func_080a51d0
	movs	r0, #0
	bl	Func_080a5388
	adds	r6, r0, #0
	movs	r0, #168
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	bne.n	.L66
	mov	r3, r9
	ldr	r0, [r3, #44]
	bl	Func_08015278
	bl	Func_080a4e20
	ldrb	r0, [r7, #0]
	movs	r1, #1
	bl	Func_080a3e88
	cmp	r6, #0
	bne.n	.L67
	movs	r2, #187
	ldrb	r3, [r7, #0]
	lsls	r2, r2, #1
	add	r2, r9
	adds	r0, r3, #0
	ldrh	r1, [r2, #0]
	movs	r2, #0
	bl	Func_080a3ef0
	ldr	r6, [pc, #616]
	movs	r2, #14
	adds	r0, r6, #0
	movs	r1, #15
	bl	Func_080a1d08
	ldrh	r0, [r5, #0]
	bl	Func_08077018
	ldrb	r2, [r0, #3]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L66
	movs	r0, #103
	bl	Func_080f9010
	mov	r1, r9
	ldr	r0, [r1, #44]
	bl	Func_08015278
	adds	r0, r6, #7
	movs	r1, #14
.L65:
	movs	r2, #14
	bl	Func_080a1d08
	b.n	.L66
.L67:
	ldr	r0, [pc, #568]
	movs	r1, #15
	movs	r2, #14
	bl	Func_080a1d08
.L66:
	mov	r0, r8
	bl	Func_08002df0
	mov	r0, fp
	bl	Func_08002df0
.L46:
	bl	Func_0808a548
	movs	r2, #0
	mov	r8, r2
	b.n	.L0
	ldr	r7, [pc, #512]
	movs	r3, #186
	lsls	r3, r3, #1
	add	r3, r9
	add	r7, r9
	ldrh	r1, [r3, #0]
	ldrb	r0, [r7, #0]
	mov	sl, r3
	bl	Func_08077050
	movs	r5, #1
	movs	r1, #1
	adds	r6, r0, #0
	negs	r5, r5
	mov	r8, r1
	cmp	r6, r5
	bne.n	.L68
	b.n	.L0
.L68:
	movs	r2, #2
	negs	r2, r2
	cmp	r6, r2
	bne.n	.L69
	mov	r3, r9
	ldr	r0, [r3, #44]
	bl	Func_08015278
	movs	r1, #0
	ldr	r0, [pc, #484]
	adds	r2, r5, #0
	bl	Func_080a1d08
	movs	r1, #1
	mov	r8, r1
	b.n	.L0
.L69:
	ldrb	r0, [r7, #0]
	bl	Func_08077010
	ldrb	r0, [r7, #0]
	bl	Func_080772c0
	mov	r3, r9
	ldr	r2, [r3, #20]
	movs	r5, #228
	movs	r3, #13
	lsls	r5, r5, #1
	strb	r3, [r2, #5]
	add	r5, r9
	ldrb	r0, [r7, #0]
	bl	Func_08077008
	movs	r2, #0
	adds	r1, r5, #0
	bl	Func_080a3ddc
	movs	r3, #134
	lsls	r3, r3, #2
	add	r3, r9
	strb	r0, [r3, #0]
	movs	r1, #0
	adds	r0, r5, #0
	bl	Func_080a3e28
	movs	r0, #1
	bl	Func_080030f8
	ldrb	r3, [r7, #0]
	mov	r2, sl
	ldrh	r1, [r2, #0]
	adds	r0, r3, #0
	movs	r2, #0
	bl	Func_080a3ef0
	mov	r3, r9
	ldr	r0, [r3, #44]
	bl	Func_08015278
	ldr	r5, [pc, #380]
	movs	r2, #8
	adds	r0, r5, #0
	movs	r1, #15
	bl	Func_080a1d08
	movs	r3, #188
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r0, [r3, #0]
	bl	Func_08077018
	ldrb	r2, [r0, #3]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L4
	movs	r0, #103
	bl	Func_080f9010
	mov	r1, r9
	ldr	r0, [r1, #44]
	bl	Func_08015278
	adds	r0, r5, #7
	movs	r1, #14
	movs	r2, #8
	bl	Func_080a1d08
	b.n	.L4
	ldr	r6, [pc, #300]
	add	r6, r9
	ldrb	r0, [r6, #0]
	bl	Func_08077008
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
	bl	Func_08077010
	ldrb	r0, [r6, #0]
	bl	Func_080772c0
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
	bl	Func_08077008
	movs	r2, #0
	adds	r1, r5, #0
	bl	Func_080a3ddc
	movs	r3, #134
	lsls	r3, r3, #2
	add	r3, r9
	strb	r0, [r3, #0]
	movs	r1, #0
	adds	r0, r5, #0
	movs	r5, #151
	bl	Func_080a3e28
	lsls	r5, r5, #2
	movs	r0, #1
	bl	Func_080030f8
	add	r5, r9
	movs	r3, #1
	strb	r3, [r5, #0]
	ldrb	r3, [r6, #0]
	mov	r2, r8
	ldrh	r1, [r2, #0]
	adds	r0, r3, #0
	movs	r2, #0
	bl	Func_080a3ef0
	mov	r3, sl
	strb	r3, [r5, #0]
	mov	r1, r9
	ldr	r0, [r1, #44]
	bl	Func_08015278
	movs	r2, #8
	ldr	r0, [pc, #200]
	movs	r1, #14
	bl	Func_080a1d08
	bl	Func_0808a548
.L4:
	movs	r2, #1
	mov	r8, r2
	b.n	.L0
	mov	r3, r9
	ldr	r2, [r3, #20]
	movs	r3, #13
	strb	r3, [r2, #5]
	movs	r3, #188
	lsls	r3, r3, #1
	add	r3, r9
	ldrh	r0, [r3, #0]
	bl	Func_080a4800
	mov	r1, r9
	ldr	r0, [r1, #36]
	bl	Func_08015270
	ldr	r3, [pc, #116]
	movs	r2, #186
	add	r3, r9
	ldrb	r3, [r3, #0]
	lsls	r2, r2, #1
	add	r2, r9
	ldrh	r1, [r2, #0]
	adds	r0, r3, #0
	movs	r2, #0
	bl	Func_080a3ef0
	mov	r3, r9
	ldr	r2, [r3, #20]
	movs	r1, #9
	movs	r3, #1
	strb	r3, [r2, #5]
	mov	r8, r1
	b.n	.L0
	movs	r0, #0
	movs	r1, #30
	movs	r2, #0
	bl	Func_080a4f08
	movs	r3, #1
	adds	r6, r0, #0
	mov	r8, r3
	b.n	.L0
.L2:
	movs	r1, #1
	str	r1, [sp, #24]
.L0:
	ldr	r2, [sp, #24]
	cmp	r2, #0
	bne.n	.L70
	movs	r0, #168
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	bne.n	.L70
	bl	.L71
.L70:
	movs	r0, #168
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L72
	movs	r3, #1
	negs	r3, r3
	str	r3, [sp, #16]
.L72:
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
