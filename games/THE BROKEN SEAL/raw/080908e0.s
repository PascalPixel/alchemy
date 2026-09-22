.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_080072f0, 0x080072f0
	.set sub_080072fc, 0x080072fc
	.set sub_080770c0, 0x080770c0
	.set sub_08091294, 0x08091294
	.set sub_080912a8, 0x080912a8
	.global Overlay_080908e0
Overlay_080908e0:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #88]
	movs	r1, #196
	ldr	r6, [r3, #0]
	movs	r0, #169
	lsls	r1, r1, #5
	lsls	r0, r0, #1
	adds	r5, r6, r1
	bl	sub_080770c0
	cmp	r0, #0
	beq.n	.L_080908fa
	b.n	.L_08090a44
.L_080908fa:
	ldr	r2, [pc, #68]
	adds	r1, r6, r2
	movs	r3, #0
	ldrsb	r3, [r1, r3]
	cmp	r3, #0
	bne.n	.L_08090908
	b.n	.L_08090a44
.L_08090908:
	adds	r2, #1
	adds	r3, r6, r2
	ldrb	r2, [r3, #0]
	adds	r2, #1
	strb	r2, [r3, #0]
	lsls	r2, r2, #24
	movs	r3, #0
	ldrsb	r3, [r1, r3]
	asrs	r2, r2, #24
	cmp	r2, r3
	bge.n	.L_08090948
	movs	r3, #224
	lsls	r3, r3, #2
	ldr	r4, [pc, #32]
	adds	r1, r6, r3
	movs	r0, #0
.L_08090928:
	ldrh	r3, [r1, #0]
	ldrh	r2, [r5, #0]
	adds	r0, #1
	adds	r3, r3, r2
	strh	r3, [r1, #0]
	adds	r5, #2
	adds	r1, #2
	cmp	r0, r4
	ble.n	.L_08090928
	b.n	.L_08090966
	.4byte 0x03001ed0
	.4byte 0x00002a01
	.2byte 0x053f
	.2byte 0x0000
.L_08090948:
	movs	r1, #224
	movs	r2, #224
	lsls	r1, r1, #2
	lsls	r2, r2, #4
	adds	r0, r6, r1
	adds	r1, r6, r2
	movs	r2, #168
	ldr	r3, [pc, #72]
	lsls	r2, r2, #4
	bl	sub_080072f0
	ldr	r3, [pc, #68]
	adds	r2, r6, r3
	movs	r3, #0
	strb	r3, [r2, #0]
.L_08090966:
	movs	r1, #168
	lsls	r1, r1, #6
	adds	r3, r6, r1
	ldrb	r3, [r3, #0]
	movs	r2, #1
	eors	r2, r3
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #7
	movs	r2, #140
	adds	r3, r6, r3
	lsls	r2, r2, #6
	adds	r4, r3, r2
	movs	r3, #248
	movs	r2, #224
	lsls	r3, r3, #7
	movs	r0, #224
	lsls	r2, r2, #2
	ldr	r7, [pc, #12]
	ldr	r5, [pc, #12]
	mov	ip, r3
	lsls	r0, r0, #1
	adds	r1, r6, r2
	b.n	.L_080909a8
	movs	r0, r0
	.4byte 0x000003e0
	.4byte 0x0000001f
	.4byte 0x03001388
	.2byte 0x2a01
	.2byte 0x0000
.L_080909a8:
	ldrh	r3, [r1, #0]
	mov	r2, ip
	ands	r2, r3
	ldrh	r3, [r1, #2]
	lsls	r3, r3, #16
	asrs	r3, r3, #21
	ands	r3, r7
	orrs	r2, r3
	ldrh	r3, [r1, #4]
	lsls	r3, r3, #16
	asrs	r3, r3, #26
	ands	r3, r5
	orrs	r2, r3
	subs	r0, #1
	strh	r2, [r4, #0]
	adds	r1, #6
	adds	r4, #2
	cmp	r0, #0
	bne.n	.L_080909a8
	movs	r3, #168
	lsls	r3, r3, #6
	adds	r1, r6, r3
	ldrb	r3, [r1, #0]
	movs	r2, #1
	eors	r3, r2
	strb	r3, [r1, #0]
	ldrb	r2, [r1, #0]
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #7
	movs	r1, #140
	adds	r0, r6, r3
	lsls	r1, r1, #6
	ldr	r5, [pc, #96]
	adds	r6, r0, r1
	ldr	r4, [pc, #96]
	ldrh	r3, [r4, #0]
	adds	r1, r3, #0
	strh	r4, [r4, #0]
	ldrh	r2, [r5, #0]
	cmp	r2, #31
	bgt.n	.L_08090a16
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, r3, r5
	adds	r3, #4
	adds	r2, #1
	stmia	r3!, {r6}
	strh	r2, [r5, #0]
	movs	r2, #160
	lsls	r2, r2, #19
	stmia	r3!, {r2}
	ldr	r2, [pc, #64]
	str	r2, [r3, #0]
.L_08090a16:
	strh	r1, [r4, #0]
	ldrh	r3, [r4, #0]
	adds	r6, r3, #0
	strh	r4, [r4, #0]
	ldrh	r2, [r5, #0]
	cmp	r2, #31
	bgt.n	.L_08090a42
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	movs	r1, #147
	adds	r2, #1
	adds	r3, r3, r5
	lsls	r1, r1, #6
	adds	r3, #4
	strh	r2, [r5, #0]
	adds	r2, r0, r1
	stmia	r3!, {r2}
	ldr	r2, [pc, #28]
	stmia	r3!, {r2}
	ldr	r2, [pc, #20]
	str	r2, [r3, #0]
.L_08090a42:
	strh	r6, [r4, #0]
.L_08090a44:
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02002090
	.4byte 0x04000208
	.4byte 0x84000070
	.2byte 0x0200
	.2byte 0x0500
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	sl, r1
	mov	r8, r2
	movs	r1, #224
	adds	r2, r3, #0
	movs	r3, #128
	sub	sp, #40
	lsls	r1, r1, #1
	lsls	r3, r3, #8
	str	r1, [sp, #36]
	cmp	r0, r3
	bne.n	.L_08090a86
	movs	r3, #160
	lsls	r3, r3, #19
	ldrh	r0, [r3, #0]
.L_08090a86:
	cmp	r2, #1
	bne.n	.L_08090a90
	movs	r1, #224
	str	r1, [sp, #36]
	b.n	.L_08090aa4
.L_08090a90:
	cmp	r2, #2
	bne.n	.L_08090aa4
	movs	r3, #168
	lsls	r3, r3, #3
	add	r8, r3
	movs	r1, #224
	movs	r3, #224
	lsls	r1, r1, #1
	str	r3, [sp, #36]
	add	sl, r1
.L_08090aa4:
	movs	r1, #128
	lsls	r1, r1, #8
	cmp	r0, r1
	bcs.n	.L_08090afe
	ldr	r2, [pc, #56]
	adds	r3, r0, #0
	ands	r3, r2
	mov	r2, r8
	strh	r3, [r2, #0]
	movs	r3, #2
	ldr	r2, [pc, #48]
	add	r8, r3
	adds	r3, r0, #0
	ands	r3, r2
	mov	r1, r8
	lsls	r3, r3, #5
	strh	r3, [r1, #0]
	ldr	r3, [pc, #40]
	movs	r2, #2
	add	r8, r2
	ands	r0, r3
	lsls	r3, r0, #10
	mov	r1, r8
	strh	r3, [r1, #0]
	ldr	r3, [sp, #36]
	subs	r3, #1
	add	r8, r2
	lsls	r2, r3, #1
	adds	r2, r2, r3
	lsls	r2, r2, #1
	movs	r4, #128
	lsls	r4, r4, #24
	b.n	.L_08090af4
	movs	r0, r0
	.4byte 0x00007c00
	.4byte 0x000003e0
	.2byte 0x001f
	.2byte 0x0000
.L_08090af4:
	mov	r0, r8
	lsrs	r2, r2, #1
	ldr	r3, [pc, #844]
	subs	r0, #6
	b.n	.L_08091156
.L_08090afe:
	movs	r3, #128
	lsls	r3, r3, #13
	cmp	r0, r3
	bcc.n	.L_08090b08
	b.n	.L_08090f3e
.L_08090b08:
	ldr	r1, [pc, #832]
	adds	r0, r0, r1
	cmp	r0, #6
	bls.n	.L_08090b12
	b.n	.L_08090ef0
.L_08090b12:
	ldr	r2, [pc, #828]
	lsls	r3, r0, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x08090b38
	.4byte 0x08090b86
	.4byte 0x08090c22
	.4byte 0x08090cb4
	.4byte 0x08090d4e
	.4byte 0x08090dd0
	.2byte 0x0e64
	.2byte 0x0809
	movs	r2, #0
	ldr	r3, [sp, #36]
	mov	r9, r2
	cmp	r9, r3
	bcc.n	.L_08090b44
	b.n	.L_0809115e
.L_08090b44:
	ldr	r6, [pc, #780]
	mov	r5, r8
.L_08090b48:
	mov	r1, sl
	ldrh	r4, [r1, #0]
	movs	r3, #248
	lsls	r0, r4, #11
	lsls	r3, r3, #8
	movs	r2, #2
	ands	r0, r3
	movs	r3, #248
	lsls	r3, r3, #9
	add	sl, r2
	lsls	r2, r4, #7
	ands	r2, r3
	movs	r3, #248
	lsls	r3, r3, #7
	ands	r3, r4
	adds	r0, r0, r2
	adds	r0, r0, r3
	movs	r1, #7
	bl	sub_080072fc
	adds	r4, r0, #0
	strh	r4, [r5, #0]
	strh	r4, [r5, #2]
	strh	r4, [r5, #4]
	movs	r3, #1
	ldr	r1, [sp, #36]
	add	r9, r3
	adds	r5, #6
	cmp	r9, r1
	bcc.n	.L_08090b48
	b.n	.L_0809115e
	movs	r2, #0
	ldr	r3, [sp, #36]
	mov	r9, r2
	cmp	r9, r3
	bcc.n	.L_08090b92
	b.n	.L_0809115e
.L_08090b92:
	movs	r1, #31
	ldr	r2, [pc, #704]
	mov	fp, r1
.L_08090b98:
	mov	r3, sl
	ldrh	r4, [r3, #0]
	movs	r1, #2
	adds	r6, r4, #0
	mov	r3, fp
	lsrs	r0, r4, #5
	ands	r6, r3
	ands	r0, r3
	add	sl, r1
	lsrs	r3, r4, #10
	mov	r1, fp
	ands	r3, r1
	adds	r0, r6, r0
	adds	r0, r0, r3
	str	r2, [sp, #0]
	ldr	r3, [pc, #668]
	movs	r1, #10
	bl	sub_080072f0
	adds	r4, r0, #0
	lsls	r3, r4, #2
	adds	r6, r3, #5
	lsls	r3, r4, #1
	adds	r3, r3, r4
	adds	r5, r3, #5
	adds	r7, r5, #0
	ldr	r2, [sp, #0]
	cmp	r6, #7
	bgt.n	.L_08090bd4
	movs	r6, #8
.L_08090bd4:
	cmp	r5, #7
	bgt.n	.L_08090be0
	movs	r7, #8
	cmp	r5, #7
	bgt.n	.L_08090be0
	movs	r5, #8
.L_08090be0:
	cmp	r6, #28
	ble.n	.L_08090be6
	movs	r6, #28
.L_08090be6:
	cmp	r7, #28
	ble.n	.L_08090bec
	movs	r7, #28
.L_08090bec:
	cmp	r5, #28
	ble.n	.L_08090bf2
	movs	r5, #28
.L_08090bf2:
	lsls	r3, r5, #1
	ldrh	r3, [r2, r3]
	mov	r1, r8
	strh	r3, [r1, #0]
	movs	r3, #2
	add	r8, r3
	lsls	r3, r7, #1
	ldrh	r3, [r2, r3]
	mov	r1, r8
	strh	r3, [r1, #0]
	movs	r3, #2
	add	r8, r3
	lsls	r3, r6, #1
	ldrh	r3, [r2, r3]
	mov	r1, r8
	strh	r3, [r1, #0]
	movs	r3, #2
	add	r8, r3
	movs	r1, #1
	ldr	r3, [sp, #36]
	add	r9, r1
	cmp	r9, r3
	bcc.n	.L_08090b98
	b.n	.L_0809115e
	movs	r1, #0
	ldr	r2, [sp, #36]
	mov	r9, r1
	cmp	r9, r2
	bcc.n	.L_08090c2e
	b.n	.L_0809115e
.L_08090c2e:
	movs	r3, #31
	mov	fp, r3
.L_08090c32:
	mov	r1, sl
	ldrh	r4, [r1, #0]
	mov	r3, fp
	adds	r6, r4, #0
	ands	r6, r3
	lsrs	r7, r4, #5
	lsrs	r5, r4, #10
	ands	r7, r3
	ands	r5, r3
	adds	r3, r6, #0
	orrs	r3, r7
	movs	r2, #2
	orrs	r3, r5
	add	sl, r2
	cmp	r3, #0
	beq.n	.L_08090c7e
	lsrs	r3, r6, #1
	movs	r1, #3
	adds	r0, r7, #0
	subs	r6, r6, r3
	bl	sub_080022ec
	adds	r6, #10
	subs	r7, r7, r0
	adds	r0, r6, #0
	bl	sub_08091294
	adds	r7, #8
	adds	r6, r0, #0
	adds	r0, r7, #0
	bl	sub_08091294
	subs	r5, #7
	adds	r7, r0, #0
	adds	r0, r5, #0
	bl	sub_08091294
	adds	r5, r0, #0
.L_08090c7e:
	ldr	r2, [pc, #476]
	lsls	r3, r5, #1
	ldrh	r3, [r2, r3]
	mov	r1, r8
	movs	r2, #2
	strh	r3, [r1, #0]
	add	r8, r2
	ldr	r2, [pc, #456]
	lsls	r3, r7, #1
	ldrh	r3, [r2, r3]
	mov	r1, r8
	movs	r2, #2
	strh	r3, [r1, #0]
	add	r8, r2
	ldr	r2, [pc, #452]
	lsls	r3, r6, #1
	ldrh	r3, [r2, r3]
	mov	r1, r8
	strh	r3, [r1, #0]
	ldr	r1, [sp, #36]
	movs	r3, #1
	movs	r2, #2
	add	r9, r3
	add	r8, r2
	cmp	r9, r1
	bcc.n	.L_08090c32
	b.n	.L_0809115e
	movs	r2, #0
	ldr	r3, [sp, #36]
	mov	r9, r2
	cmp	r9, r3
	bcc.n	.L_08090cc0
	b.n	.L_0809115e
.L_08090cc0:
	ldr	r1, [pc, #412]
	mov	fp, r1
.L_08090cc4:
	mov	r2, sl
	ldrh	r4, [r2, #0]
	movs	r1, #31
	adds	r6, r4, #0
	movs	r3, #2
	lsrs	r7, r4, #5
	lsrs	r5, r4, #10
	ands	r6, r1
	add	sl, r3
	ands	r7, r1
	ands	r5, r1
	cmp	r6, #9
	bgt.n	.L_08090ce0
	movs	r6, #10
.L_08090ce0:
	cmp	r7, #15
	bgt.n	.L_08090ce6
	movs	r7, #16
.L_08090ce6:
	cmp	r5, #15
	bgt.n	.L_08090cec
	movs	r5, #16
.L_08090cec:
	cmp	r6, #28
	ble.n	.L_08090cf2
	movs	r6, #28
.L_08090cf2:
	cmp	r7, #24
	ble.n	.L_08090cf8
	movs	r7, #24
.L_08090cf8:
	cmp	r5, #26
	ble.n	.L_08090cfe
	movs	r5, #26
.L_08090cfe:
	adds	r0, r6, #0
	bl	sub_08091294
	adds	r7, #2
	adds	r6, r0, #0
	adds	r0, r7, #0
	bl	sub_08091294
	adds	r5, #2
	adds	r7, r0, #0
	adds	r0, r5, #0
	bl	sub_08091294
	adds	r5, r0, #0
	mov	r2, fp
	lsls	r3, r5, #1
	ldrh	r3, [r2, r3]
	mov	r1, r8
	strh	r3, [r1, #0]
	movs	r2, #2
	mov	r1, fp
	lsls	r3, r7, #1
	ldrh	r3, [r1, r3]
	add	r8, r2
	mov	r2, r8
	strh	r3, [r2, #0]
	movs	r3, #2
	add	r8, r3
	lsls	r3, r6, #1
	ldrh	r3, [r1, r3]
	mov	r1, r8
	strh	r3, [r1, #0]
	ldr	r1, [sp, #36]
	movs	r3, #1
	movs	r2, #2
	add	r9, r3
	add	r8, r2
	cmp	r9, r1
	bcc.n	.L_08090cc4
	b.n	.L_0809115e
	movs	r2, #0
	ldr	r3, [sp, #36]
	mov	r9, r2
	cmp	r9, r3
	bcc.n	.L_08090d5a
	b.n	.L_0809115e
.L_08090d5a:
	ldr	r1, [pc, #256]
	mov	fp, r1
.L_08090d5e:
	mov	r2, sl
	ldrh	r4, [r2, #0]
	movs	r1, #31
	adds	r6, r4, #0
	lsrs	r7, r4, #5
	lsrs	r5, r4, #10
	ands	r6, r1
	ands	r7, r1
	ands	r5, r1
	adds	r0, r6, r7
	movs	r1, #3
	movs	r3, #2
	adds	r0, r0, r5
	add	sl, r3
	bl	sub_080022ec
	bl	sub_08091294
	asrs	r3, r6, #1
	adds	r6, r3, r0
	asrs	r3, r7, #1
	adds	r7, r3, r0
	asrs	r3, r5, #1
	adds	r5, r3, r0
	adds	r0, r6, #0
	bl	sub_08091294
	adds	r6, r0, #0
	adds	r0, r7, #0
	bl	sub_08091294
	adds	r7, r0, #0
	adds	r0, r5, #0
	bl	sub_08091294
	adds	r5, r0, #0
	mov	r2, fp
	lsls	r3, r5, #1
	ldrh	r3, [r2, r3]
	mov	r1, r8
	strh	r3, [r1, #0]
	lsls	r3, r7, #1
	ldrh	r3, [r2, r3]
	mov	r2, r8
	strh	r3, [r2, #2]
	mov	r1, fp
	lsls	r3, r6, #1
	ldrh	r3, [r1, r3]
	strh	r3, [r2, #4]
	movs	r1, #1
	ldr	r2, [sp, #36]
	movs	r3, #6
	add	r9, r1
	add	r8, r3
	cmp	r9, r2
	bcc.n	.L_08090d5e
	b.n	.L_0809115e
	movs	r3, #0
	ldr	r1, [sp, #36]
	mov	r9, r3
	cmp	r9, r1
	bcc.n	.L_08090ddc
	b.n	.L_0809115e
.L_08090ddc:
	movs	r2, #31
	mov	fp, r2
.L_08090de0:
	mov	r3, sl
	ldrh	r4, [r3, #0]
	mov	r2, fp
	lsrs	r7, r4, #5
	lsrs	r5, r4, #10
	adds	r6, r4, #0
	ands	r7, r2
	ands	r5, r2
	ands	r6, r2
	asrs	r3, r7, #3
	asrs	r2, r5, #3
	adds	r3, r3, r2
	adds	r6, r6, r3
	movs	r1, #2
	adds	r0, r6, #0
	add	sl, r1
	bl	sub_08091294
	movs	r1, #3
	adds	r6, r0, #0
	adds	r0, r7, #0
	bl	sub_080022ec
	movs	r1, #3
	subs	r7, r7, r0
	adds	r0, r5, #0
	bl	sub_080022ec
	ldr	r1, [pc, #68]
	subs	r5, r5, r0
	lsls	r3, r5, #1
	ldrh	r3, [r1, r3]
	mov	r2, r8
	strh	r3, [r2, #0]
	lsls	r3, r7, #1
	ldrh	r3, [r1, r3]
	mov	r1, r8
	strh	r3, [r1, #2]
	ldr	r2, [pc, #40]
	lsls	r3, r6, #1
	ldrh	r3, [r2, r3]
	mov	r2, r8
	strh	r3, [r2, #4]
	movs	r1, #1
	ldr	r2, [sp, #36]
	movs	r3, #6
	add	r9, r1
	add	r8, r3
	cmp	r9, r2
	bcc.n	.L_08090de0
	b.n	.L_0809115e
	movs	r0, r0
	.4byte 0x040000d4
	.4byte 0xfffeffff
	.4byte 0x08090b1c
	.4byte 0x03000380
	.4byte 0x0809e96e
	.4byte 0x0809e9ae
	.2byte 0xe92e
	.2byte 0x0809
	movs	r3, #0
	ldr	r1, [sp, #36]
	mov	r9, r3
	cmp	r9, r1
	bcc.n	.L_08090e70
	b.n	.L_0809115e
.L_08090e70:
	movs	r2, #31
	mov	fp, r2
.L_08090e74:
	mov	r3, sl
	ldrh	r4, [r3, #0]
	mov	r2, fp
	adds	r6, r4, #0
	lsrs	r7, r4, #5
	ands	r6, r2
	ands	r7, r2
	movs	r1, #2
	lsrs	r3, r6, #1
	lsrs	r5, r4, #10
	adds	r0, r7, #0
	add	sl, r1
	movs	r1, #3
	ands	r5, r2
	subs	r6, r6, r3
	bl	sub_080022ec
	adds	r6, #6
	subs	r7, r7, r0
	adds	r0, r6, #0
	bl	sub_08091294
	adds	r7, #4
	adds	r6, r0, #0
	adds	r0, r7, #0
	bl	sub_08091294
	subs	r5, #6
	adds	r7, r0, #0
	adds	r0, r5, #0
	bl	sub_08091294
	ldr	r2, [pc, #44]
	adds	r5, r0, #0
	lsls	r3, r5, #1
	ldrh	r3, [r2, r3]
	mov	r1, r8
	strh	r3, [r1, #0]
	ldr	r2, [pc, #36]
	lsls	r3, r7, #1
	ldrh	r3, [r2, r3]
	mov	r2, r8
	strh	r3, [r2, #2]
	ldr	r2, [pc, #32]
	lsls	r3, r6, #1
	ldrh	r3, [r2, r3]
	strh	r3, [r1, #4]
	ldr	r1, [sp, #36]
	movs	r3, #1
	movs	r2, #6
	add	r9, r3
	add	r8, r2
	cmp	r9, r1
	bcc.n	.L_08090e74
	b.n	.L_0809115e
	movs	r0, r0
	.4byte 0x0809e9ae
	.4byte 0x0809e96e
	.2byte 0xe92e
	.2byte 0x0809
.L_08090ef0:
	movs	r2, #0
	ldr	r3, [sp, #36]
	mov	r9, r2
	cmp	r9, r3
	bcc.n	.L_08090efc
	b.n	.L_0809115e
.L_08090efc:
	ldr	r5, [pc, #8]
	ldr	r0, [pc, #12]
	ldr	r2, [pc, #12]
	mov	r1, r8
	b.n	.L_08090f14
	movs	r0, r0
	.4byte 0x00007c00
	.4byte 0x000003e0
	.2byte 0x001f
	.2byte 0x0000
.L_08090f14:
	mov	r3, sl
	ldrh	r4, [r3, #0]
	movs	r3, #2
	add	sl, r3
	adds	r3, r4, #0
	ands	r3, r5
	strh	r3, [r1, #0]
	adds	r3, r4, #0
	ands	r3, r0
	lsls	r3, r3, #5
	ands	r4, r2
	strh	r3, [r1, #2]
	lsls	r3, r4, #10
	strh	r3, [r1, #4]
	movs	r3, #1
	add	r9, r3
	ldr	r3, [sp, #36]
	adds	r1, #6
	cmp	r9, r3
	bcc.n	.L_08090f14
	b.n	.L_0809115e
.L_08090f3e:
	movs	r3, #128
	lsls	r3, r3, #14
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_08090fde
	movs	r3, #31
	str	r0, [sp, #32]
	adds	r1, r0, #0
	lsrs	r2, r0, #5
	lsrs	r0, r0, #10
	ands	r1, r3
	mov	fp, r0
	ands	r2, r3
	str	r1, [sp, #32]
	mov	r1, fp
	ands	r1, r3
	str	r2, [sp, #28]
	ldr	r3, [sp, #36]
	movs	r2, #0
	mov	r9, r2
	mov	fp, r1
	cmp	r9, r3
	bcc.n	.L_08090f6e
	b.n	.L_0809115e
.L_08090f6e:
	mov	r1, sl
	ldrh	r4, [r1, #0]
	movs	r3, #248
	lsls	r0, r4, #11
	lsls	r3, r3, #8
	movs	r2, #2
	ands	r0, r3
	movs	r3, #248
	lsls	r3, r3, #9
	add	sl, r2
	lsls	r2, r4, #7
	ands	r2, r3
	movs	r3, #248
	lsls	r3, r3, #7
	ands	r3, r4
	adds	r0, r0, r2
	adds	r0, r0, r3
	movs	r1, #96
	ldr	r3, [pc, #328]
	bl	sub_080072f0
	ldr	r1, [sp, #32]
	adds	r4, r0, #0
	adds	r7, r1, #0
	muls	r7, r4
	ldr	r2, [sp, #28]
	adds	r0, r7, #0
	adds	r6, r2, #0
	muls	r6, r4
	mov	r5, fp
	muls	r5, r4
	bl	sub_080912a8
	adds	r7, r0, #0
	adds	r0, r6, #0
	bl	sub_080912a8
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl	sub_080912a8
	mov	r3, r8
	mov	r1, r8
	mov	r2, r8
	adds	r5, r0, #0
	strh	r5, [r3, #0]
	strh	r6, [r1, #2]
	strh	r7, [r2, #4]
	movs	r1, #1
	ldr	r2, [sp, #36]
	movs	r3, #6
	add	r9, r1
	add	r8, r3
	cmp	r9, r2
	bcc.n	.L_08090f6e
	b.n	.L_0809115e
.L_08090fde:
	movs	r3, #128
	lsls	r3, r3, #15
	ands	r3, r0
	cmp	r3, #0
	bne.n	.L_08090fea
	b.n	.L_080910e8
.L_08090fea:
	movs	r3, #31
	str	r0, [sp, #24]
	adds	r1, r0, #0
	lsrs	r2, r0, #5
	lsrs	r0, r0, #10
	ands	r1, r3
	mov	fp, r0
	ands	r2, r3
	str	r1, [sp, #24]
	mov	r1, fp
	ands	r1, r3
	str	r2, [sp, #20]
	ldr	r3, [sp, #36]
	movs	r2, #0
	mov	r9, r2
	mov	fp, r1
	cmp	r9, r3
	bcc.n	.L_08091010
	b.n	.L_0809115e
.L_08091010:
	ldr	r2, [sp, #20]
	ldr	r1, [sp, #24]
	ldr	r3, [sp, #24]
	adds	r1, r1, r2
	str	r1, [sp, #16]
	lsls	r1, r2, #16
	mov	r2, fp
	lsls	r3, r3, #16
	lsls	r2, r2, #16
	str	r3, [sp, #12]
	str	r1, [sp, #8]
	str	r2, [sp, #4]
.L_08091028:
	mov	r3, sl
	ldrh	r4, [r3, #0]
	movs	r2, #31
	adds	r6, r4, #0
	lsrs	r0, r4, #5
	ands	r6, r2
	ands	r0, r2
	lsrs	r3, r4, #10
	movs	r1, #2
	ands	r3, r2
	add	sl, r1
	adds	r0, r6, r0
	ldr	r1, [sp, #16]
	adds	r0, r0, r3
	add	r1, fp
	ldr	r3, [pc, #148]
	lsls	r0, r0, #4
	bl	sub_080072f0
	ldr	r3, [sp, #24]
	adds	r4, r0, #0
	adds	r0, r3, #0
	muls	r0, r4
	ldr	r2, [sp, #12]
	lsrs	r0, r0, #4
	lsls	r0, r0, #16
	asrs	r1, r2, #4
	ldr	r3, [pc, #128]
	mov	ip, pc
	bx	r3
	ldr	r1, [sp, #20]
	adds	r7, r0, #0
	adds	r0, r1, #0
	muls	r0, r4
	ldr	r2, [sp, #8]
	lsrs	r0, r0, #4
	lsls	r0, r0, #16
	asrs	r1, r2, #4
	mov	ip, pc
	bx	r3
	adds	r6, r0, #0
	mov	r0, fp
	muls	r0, r4
	ldr	r3, [sp, #4]
	lsrs	r0, r0, #4
	asrs	r1, r3, #4
	lsls	r0, r0, #16
	ldr	r3, [pc, #88]
	mov	ip, pc
	bx	r3
	adds	r5, r0, #0
	lsrs	r0, r7, #16
	bl	sub_08091294
	adds	r7, r0, #0
	lsrs	r0, r6, #16
	bl	sub_08091294
	adds	r6, r0, #0
	lsrs	r0, r5, #16
	bl	sub_08091294
	ldr	r1, [pc, #60]
	adds	r5, r0, #0
	lsls	r3, r5, #1
	ldrh	r3, [r1, r3]
	mov	r2, r8
	strh	r3, [r2, #0]
	movs	r3, #2
	add	r8, r3
	lsls	r3, r6, #1
	ldrh	r3, [r1, r3]
	mov	r1, r8
	strh	r3, [r1, #0]
	ldr	r1, [pc, #36]
	movs	r2, #2
	lsls	r3, r7, #1
	ldrh	r3, [r1, r3]
	add	r8, r2
	mov	r2, r8
	strh	r3, [r2, #0]
	movs	r1, #1
	ldr	r2, [sp, #36]
	movs	r3, #2
	add	r9, r1
	add	r8, r3
	cmp	r9, r2
	bcc.n	.L_08091028
	b.n	.L_0809115e
	movs	r0, r0
	.4byte 0x03000380
	.4byte 0x03000118
	.2byte 0xe92e
	.2byte 0x0809
.L_080910e8:
	movs	r3, #128
	lsls	r3, r3, #16
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_0809113e
	movs	r3, #0
	ldr	r1, [sp, #36]
	mov	r9, r3
	cmp	r9, r1
	bcs.n	.L_0809115e
	ldr	r5, [pc, #8]
	ldr	r0, [pc, #12]
	ldr	r2, [pc, #12]
	mov	r1, r8
	b.n	.L_08091114
	movs	r0, r0
	.4byte 0x00007c00
	.4byte 0x000003e0
	.2byte 0x001f
	.2byte 0x0000
.L_08091114:
	mov	r3, sl
	ldrh	r4, [r3, #0]
	movs	r3, #2
	add	sl, r3
	adds	r3, r4, #0
	ands	r3, r5
	strh	r3, [r1, #0]
	adds	r3, r4, #0
	ands	r3, r0
	lsls	r3, r3, #5
	ands	r4, r2
	strh	r3, [r1, #2]
	lsls	r3, r4, #10
	strh	r3, [r1, #4]
	movs	r3, #1
	add	r9, r3
	ldr	r3, [sp, #36]
	adds	r1, #6
	cmp	r9, r3
	bcc.n	.L_08091114
	b.n	.L_0809115e
.L_0809113e:
	cmp	r2, #2
	bne.n	.L_08091148
	movs	r1, #168
	lsls	r1, r1, #3
	adds	r0, r0, r1
.L_08091148:
	ldr	r3, [sp, #36]
	lsls	r2, r3, #1
	adds	r2, r2, r3
	movs	r4, #132
	lsls	r4, r4, #24
	lsrs	r2, r2, #1
	ldr	r3, [pc, #24]
.L_08091156:
	mov	r1, r8
	orrs	r2, r4
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L_0809115e:
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x040000d4
