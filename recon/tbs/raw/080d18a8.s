.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_080022fc, 0x080022fc
	.set sub_08002304, 0x08002304
	.set sub_0800231c, 0x0800231c
	.set sub_08002322, 0x08002322
	.set sub_08002dd8, 0x08002dd8
	.set sub_08002f40, 0x08002f40
	.set sub_080030f8, 0x080030f8
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_080049ac, 0x080049ac
	.set sub_08004c1c, 0x08004c1c
	.set sub_08004c6c, 0x08004c6c
	.set sub_08004cb4, 0x08004cb4
	.set sub_080051d8, 0x080051d8
	.set sub_08005340, 0x08005340
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.set sub_080072f8, 0x080072f8
	.set sub_08007300, 0x08007300
	.set sub_08009008, 0x08009008
	.set sub_08009038, 0x08009038
	.set sub_080b5038, 0x080b5038
	.set sub_080b5040, 0x080b5040
	.set sub_080b5098, 0x080b5098
	.set sub_080b50e8, 0x080b50e8
	.set sub_080cd52c, 0x080cd52c
	.set sub_080cdbc0, 0x080cdbc0
	.set sub_080d6750, 0x080d6750
	.set sub_080d67dc, 0x080d67dc
	.set sub_080d6888, 0x080d6888
	.set sub_080dbb24, 0x080dbb24
	.set sub_080e3944, 0x080e3944
	.set sub_080f9010, 0x080f9010
	.global Unnamed_080d18a8
	.global Func_080d18a8
	.thumb_func
Unnamed_080d18a8:
Func_080d18a8:
.L_080d18a8:
	ldr	r3, [pc, #312]
	ldr	r3, [r3, #0]
	movs	r2, #3
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080d18ce
	ldr	r7, [sp, #80]
	cmp	r7, #159
	bgt.n	.L_080d18c0
	movs	r0, #160
	str	r0, [sp, #80]
	b.n	.L_080d18ce
.L_080d18c0:
	movs	r2, #197
	ldr	r1, [sp, #80]
	lsls	r2, r2, #1
	cmp	r1, r2
	bgt.n	.L_080d18ce
	ldr	r3, [pc, #284]
	str	r3, [sp, #80]
.L_080d18ce:
	bl	sub_080049ac
	ldr	r0, [sp, #96]
	ldr	r1, [sp, #36]
	bl	sub_080051d8
	ldr	r4, [sp, #80]
	cmp	r4, #16
	bne.n	.L_080d18e6
	movs	r0, #141
	bl	sub_080f9010
.L_080d18e6:
	movs	r7, #128
	ldr	r5, [sp, #80]
	lsls	r7, r7, #1
	cmp	r5, r7
	bne.n	.L_080d18f6
	movs	r0, #140
	bl	sub_080f9010
.L_080d18f6:
	movs	r1, #167
	ldr	r0, [sp, #80]
	lsls	r1, r1, #1
	cmp	r0, r1
	bne.n	.L_080d1906
	movs	r0, #212
	bl	sub_080f9010
.L_080d1906:
	ldr	r2, [sp, #80]
	ldr	r3, [pc, #224]
	cmp	r2, r3
	bne.n	.L_080d1914
	movs	r0, #212
	bl	sub_080f9010
.L_080d1914:
	ldr	r4, [sp, #80]
	ldr	r5, [pc, #216]
	cmp	r4, r5
	bne.n	.L_080d1922
	movs	r0, #212
	bl	sub_080f9010
.L_080d1922:
	movs	r0, #186
	ldr	r7, [sp, #80]
	lsls	r0, r0, #1
	cmp	r7, r0
	bne.n	.L_080d1932
	movs	r0, #212
	bl	sub_080f9010
.L_080d1932:
	ldr	r2, [pc, #148]
	ldr	r4, [sp, #92]
	ldr	r3, [r4, r2]
	ldr	r3, [r3, #20]
	movs	r1, #0
	mov	r9, r1
	cmp	r3, #0
	bne.n	.L_080d1944
	b.n	.L_080d1aac
.L_080d1944:
	movs	r4, #0
.L_080d1946:
	mov	r5, r9
	ldr	r0, [sp, #80]
	lsls	r7, r5, #4
	cmp	r0, r7
	bgt.n	.L_080d1952
	b.n	.L_080d1a98
.L_080d1952:
	ldr	r1, [sp, #92]
	lsls	r5, r5, #1
	ldr	r2, [r1, r2]
	adds	r3, r5, #0
	str	r5, [sp, #56]
	adds	r3, #36
	ldrsh	r0, [r2, r3]
	str	r4, [sp, #8]
	bl	sub_080b5098
	ldr	r4, [sp, #8]
	add	r6, sp, #324
	ldr	r5, [r0, #0]
	ldr	r0, [r6, r4]
	mov	sl, r4
	bl	sub_08002322
	movs	r1, #146
	ldr	r4, [sp, #8]
	lsls	r1, r1, #1
	add	r1, sp
	ldr	r3, [r1, r4]
	muls	r3, r0
	asrs	r3, r3, #1
	str	r3, [r5, #8]
	ldr	r0, [r6, r4]
	mov	r8, r1
	bl	sub_0800231c
	ldr	r4, [sp, #8]
	mov	r2, r8
	ldr	r3, [r2, r4]
	muls	r3, r0
	asrs	r3, r3, #1
	str	r3, [r5, #16]
	ldr	r3, [sp, #80]
	cmp	r3, #159
	bgt.n	.L_080d1a0a
	adds	r3, r7, #0
	ldr	r7, [sp, #80]
	adds	r3, #16
	cmp	r7, r3
	ble.n	.L_080d19b0
	add	r7, sp, #260
	ldr	r3, [r7, r4]
	adds	r3, #48
	str	r3, [r7, r4]
.L_080d19b0:
	mov	r0, r8
	ldr	r3, [r0, r4]
	cmp	r3, #31
	bgt.n	.L_080d19f4
	ldr	r3, [r5, #12]
	movs	r1, #192
	lsls	r1, r1, #11
	adds	r3, r3, r1
	b.n	.L_080d19fc
	movs	r0, r0
	subs	r0, r6, #3
	lsls	r0, r0, #12
	.2byte 0x7828
	.2byte 0x0000
	lsls	r2, r0, #2
	movs	r0, r0
	asrs	r0, r1, #14
	lsls	r0, r0, #12
	lsls	r3, r6, #1
	movs	r0, r0
	strb	r4, [r0, #30]
	movs	r0, r0
	bcs.n	.L_080d1aa2
	lsrs	r4, r1, #32
	lsls	r0, r3, #7
	lsls	r0, r0, #12
	.4byte 0x03001b04
	.4byte 0x0000018b
	.4byte 0x0000015b
	.2byte 0x0167
	.2byte 0x0000
.L_080d19f4:
	ldr	r3, [r5, #12]
	movs	r2, #128
	lsls	r2, r2, #8
	adds	r3, r3, r2
.L_080d19fc:
	str	r3, [r5, #12]
	movs	r2, #248
	ldr	r3, [r5, #12]
	lsls	r2, r2, #15
	cmp	r3, r2
	ble.n	.L_080d1a0a
	str	r2, [r5, #12]
.L_080d1a0a:
	movs	r7, #245
	ldr	r3, [sp, #80]
	lsls	r7, r7, #1
	cmp	r3, r7
	bgt.n	.L_080d1a40
	add	r7, sp, #260
	ldr	r2, [r6, r4]
	ldr	r3, [r7, r4]
	movs	r0, #128
	adds	r2, r2, r3
	lsls	r0, r0, #9
	str	r2, [r6, r4]
	cmp	r2, r0
	ble.n	.L_080d1a2e
	ldr	r1, [pc, #576]
	adds	r3, r2, r1
	mov	r2, sl
	str	r3, [r6, r2]
.L_080d1a2e:
	mov	r3, sl
	ldr	r2, [r7, r3]
	cmp	r2, #0
	bge.n	.L_080d1a38
	adds	r2, #3
.L_080d1a38:
	ldrh	r3, [r5, #6]
	asrs	r2, r2, #2
	adds	r3, r3, r2
	strh	r3, [r5, #6]
.L_080d1a40:
	ldr	r7, [sp, #80]
	ldr	r0, [pc, #552]
	cmp	r7, r0
	bne.n	.L_080d1a4c
	ldr	r3, [pc, #548]
	str	r3, [r5, #72]
.L_080d1a4c:
	movs	r1, #0
	ldr	r6, [pc, #548]
	mov	fp, r1
.L_080d1a52:
	ldrh	r3, [r6, #0]
	ldr	r2, [sp, #80]
	adds	r6, #2
	cmp	r2, r3
	bne.n	.L_080d1a80
	movs	r3, #0
	str	r3, [r5, #40]
	ldr	r7, [sp, #92]
	ldr	r0, [pc, #532]
	adds	r3, r7, r0
	ldr	r2, [r3, #0]
	ldr	r3, [sp, #56]
	adds	r3, #36
	ldrsh	r0, [r2, r3]
	movs	r3, #8
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	mov	r3, r9
	str	r4, [sp, #8]
	bl	sub_080d6888
	ldr	r4, [sp, #8]
.L_080d1a80:
	movs	r2, #1
	add	fp, r2
	mov	r3, fp
	cmp	r3, #4
	bne.n	.L_080d1a52
	add	r5, sp, #292
	ldr	r3, [r5, r4]
	cmp	r3, #16
	ble.n	.L_080d1a98
	subs	r3, #2
	mov	r7, sl
	str	r3, [r5, r7]
.L_080d1a98:
	ldr	r2, [pc, #476]
	ldr	r1, [sp, #92]
	ldr	r3, [r1, r2]
	movs	r0, #1
	ldr	r3, [r3, #20]
.L_080d1aa2:
	add	r9, r0
	adds	r4, #4
	cmp	r9, r3
	beq.n	.L_080d1aac
	b.n	.L_080d1946
.L_080d1aac:
	ldr	r3, [sp, #80]
	subs	r3, #16
	cmp	r3, #143
	bhi.n	.L_080d1afc
	ldr	r2, [sp, #80]
	lsls	r3, r2, #1
	adds	r5, r3, #0
	subs	r5, #32
	cmp	r5, #48
	ble.n	.L_080d1ac2
	movs	r5, #48
.L_080d1ac2:
	ldr	r0, [sp, #80]
	cmp	r0, #0
	bge.n	.L_080d1aca
	adds	r0, #3
.L_080d1aca:
	movs	r1, #3
	asrs	r0, r0, #2
	bl	sub_080022fc
	movs	r3, #48
	subs	r2, r3, r5
	lsls	r1, r0, #1
	lsls	r3, r2, #1
	adds	r1, r1, r0
	adds	r3, r3, r2
	lsls	r3, r3, #4
	ldr	r4, [sp, #92]
	lsls	r1, r1, #10
	adds	r1, r1, r3
	movs	r7, #48
	movs	r3, #112
	adds	r1, r4, r1
	subs	r3, r3, r5
	str	r7, [sp, #0]
	str	r5, [sp, #4]
	ldr	r0, [sp, #100]
	movs	r2, #32
	ldr	r4, [sp, #84]
	bl	sub_080072f4
.L_080d1afc:
	ldr	r3, [sp, #80]
	subs	r3, #48
	cmp	r3, #111
	bhi.n	.L_080d1b48
	ldr	r5, [sp, #80]
	lsls	r3, r5, #1
	adds	r5, r3, #0
	subs	r5, #96
	cmp	r5, #64
	ble.n	.L_080d1b12
	movs	r5, #64
.L_080d1b12:
	ldr	r0, [sp, #80]
	cmp	r0, #0
	bge.n	.L_080d1b1a
	adds	r0, #3
.L_080d1b1a:
	movs	r1, #3
	asrs	r0, r0, #2
	bl	sub_080022fc
	movs	r3, #64
	subs	r3, r3, r5
	lsls	r1, r0, #1
	lsls	r2, r3, #1
	adds	r1, r1, r0
	adds	r2, r2, r3
	lsls	r2, r2, #4
	lsls	r1, r1, #10
	ldr	r7, [sp, #92]
	adds	r1, r1, r2
	movs	r0, #48
	str	r0, [sp, #0]
	adds	r1, r7, r1
	str	r5, [sp, #4]
	ldr	r0, [sp, #100]
	movs	r2, #32
	ldr	r4, [sp, #84]
	bl	sub_080072f4
.L_080d1b48:
	ldr	r5, [sp, #80]
	subs	r5, #160
	mov	sl, r5
	cmp	r5, #239
	bhi.n	.L_080d1b98
	ldr	r0, [sp, #80]
	cmp	r0, #0
	bge.n	.L_080d1b5a
	adds	r0, #3
.L_080d1b5a:
	movs	r1, #3
	asrs	r0, r0, #2
	bl	sub_080022fc
	lsls	r5, r0, #1
	adds	r5, r5, r0
	ldr	r7, [sp, #92]
	ldr	r4, [sp, #84]
	lsls	r5, r5, #10
	adds	r5, r7, r5
	movs	r0, #48
	movs	r6, #64
	str	r0, [sp, #0]
	adds	r1, r5, #0
	movs	r2, #8
	movs	r3, #0
	str	r4, [sp, #8]
	str	r6, [sp, #4]
	ldr	r0, [sp, #100]
	bl	sub_080072f4
	movs	r1, #48
	str	r1, [sp, #0]
	str	r6, [sp, #4]
	ldr	r0, [sp, #100]
	adds	r1, r5, #0
	movs	r2, #8
	movs	r3, #64
	ldr	r4, [sp, #8]
	bl	sub_080072f4
.L_080d1b98:
	ldr	r2, [sp, #80]
	cmp	r2, #159
	bgt.n	.L_080d1c90
	movs	r4, #152
	ldr	r5, [sp, #92]
	movs	r7, #225
	movs	r3, #0
	add	r4, sp
	lsls	r7, r7, #7
	mov	r9, r3
	mov	r8, r4
	adds	r6, r5, r7
.L_080d1bb0:
	ldr	r4, [r6, #24]
	cmp	r4, #0
	bne.n	.L_080d1c80
	ldr	r0, [r6, #16]
	str	r4, [sp, #8]
	bl	sub_08002322
	ldr	r3, [r6, #8]
	muls	r3, r0
	mov	r5, r8
	str	r3, [r5, #0]
	ldr	r3, [r6, #4]
	str	r3, [r5, #4]
	ldr	r0, [r6, #16]
	bl	sub_0800231c
	ldr	r3, [r6, #8]
	muls	r3, r0
	add	r7, sp, #140
	str	r3, [r5, #8]
	adds	r0, r5, #0
	adds	r1, r7, #0
	bl	sub_080e3944
	ldr	r3, [r7, #0]
	asrs	r2, r3, #1
	str	r2, [r7, #0]
	ldr	r0, [pc, #148]
	ldr	r3, [r6, #4]
	ldr	r4, [sp, #8]
	cmp	r3, r0
	bgt.n	.L_080d1c0e
	ldr	r3, [r7, #4]
	ldr	r5, [sp, #92]
	movs	r7, #144
	movs	r1, #16
	lsls	r7, r7, #6
	str	r1, [sp, #0]
	str	r1, [sp, #4]
	subs	r2, #12
	adds	r1, r5, r7
	subs	r3, #12
	ldr	r0, [sp, #100]
	ldr	r5, [sp, #84]
	bl	sub_080072f8
	ldr	r4, [sp, #8]
.L_080d1c0e:
	ldr	r3, [r6, #8]
	cmp	r3, #24
	ble.n	.L_080d1c18
	subs	r3, #4
	str	r3, [r6, #8]
.L_080d1c18:
	ldr	r3, [r6, #12]
	ldr	r2, [r6, #16]
	lsls	r3, r3, #1
	movs	r7, #128
	adds	r2, r2, r3
	lsls	r7, r7, #9
	str	r2, [r6, #16]
	cmp	r2, r7
	ble.n	.L_080d1c30
	ldr	r0, [pc, #60]
	adds	r3, r2, r0
	str	r3, [r6, #16]
.L_080d1c30:
	ldr	r3, [r6, #12]
	movs	r2, #128
	adds	r3, #50
	lsls	r2, r2, #5
	str	r3, [r6, #12]
	cmp	r3, r2
	ble.n	.L_080d1c40
	str	r2, [r6, #12]
.L_080d1c40:
	ldr	r3, [r6, #16]
	movs	r1, #128
	lsls	r1, r1, #4
	adds	r3, r3, r1
	ldr	r2, [r6, #4]
	str	r3, [r6, #16]
	lsls	r3, r3, #1
	adds	r2, r2, r3
	movs	r3, #192
	lsls	r3, r3, #14
	str	r2, [r6, #4]
	cmp	r2, r3
	ble.n	.L_080d1c84
	movs	r3, #100
	str	r4, [r6, #4]
	str	r3, [r6, #8]
	str	r4, [r6, #16]
	str	r4, [r6, #12]
	b.n	.L_080d1c84
	movs	r0, r0
	.4byte 0xffff0000
	.4byte 0x0000018b
	.4byte 0x0000ab85
	.4byte 0x080ee16c
	.4byte 0x00007828
	.2byte 0xffff
	.2byte 0x003f
.L_080d1c80:
	subs	r3, r4, #1
	str	r3, [r6, #24]
.L_080d1c84:
	movs	r4, #1
	add	r9, r4
	mov	r5, r9
	adds	r6, #28
	cmp	r5, #24
	bne.n	.L_080d1bb0
.L_080d1c90:
	ldr	r7, [sp, #80]
	cmp	r7, #160
	beq.n	.L_080d1c98
	b.n	.L_080d1dd2
.L_080d1c98:
	ldr	r5, [pc, #772]
	movs	r0, #1
	adds	r1, r5, #0
	movs	r2, #0
	bl	sub_080b5040
	adds	r1, r5, #0
	movs	r2, #8
	movs	r0, #1
	bl	sub_080b5038
	ldr	r1, [pc, #756]
	ldr	r0, [sp, #92]
	adds	r3, r0, r1
	ldr	r0, [r3, #0]
	bl	sub_080d6750
	movs	r1, #188
	lsls	r1, r1, #1
	movs	r2, #2
	movs	r0, #9
	bl	sub_080dbb24
	ldr	r0, [pc, #736]
	bl	sub_08002f40
	adds	r5, r0, #0
	movs	r0, #160
	adds	r1, r5, #0
	ldr	r3, [pc, #728]
	movs	r2, #128
	lsls	r0, r0, #19
	bl	sub_080072f0
	movs	r3, #216
	ldr	r2, [sp, #92]
	lsls	r3, r3, #6
	adds	r5, #128
	adds	r1, r2, r3
	adds	r0, r5, #0
	bl	sub_08005340
	movs	r4, #0
	movs	r2, #128
	ldr	r3, [pc, #700]
	mov	r9, r4
	movs	r1, #0
	lsls	r2, r2, #2
.L_080d1cf8:
	movs	r5, #1
	add	r9, r5
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r9, r2
	bne.n	.L_080d1cf8
	ldr	r3, [pc, #668]
	ldr	r0, [sp, #92]
	ldr	r3, [r0, r3]
	ldr	r3, [r3, #20]
	movs	r7, #0
	mov	r9, r7
	cmp	r3, #0
	beq.n	.L_080d1d40
	ldr	r1, [pc, #652]
	movs	r7, #36
	adds	r6, r0, r1
.L_080d1d1a:
	ldr	r3, [r6, #0]
	ldrsh	r0, [r3, r7]
	bl	sub_080b5098
	ldr	r5, [r0, #0]
	bl	sub_08004458
	movs	r3, #15
	ands	r3, r0
	adds	r3, #8
	lsls	r3, r3, #16
	str	r3, [r5, #12]
	movs	r3, #1
	add	r9, r3
	ldr	r3, [r6, #0]
	ldr	r3, [r3, #20]
	adds	r7, #2
	cmp	r9, r3
	bne.n	.L_080d1d1a
.L_080d1d40:
	ldr	r2, [pc, #624]
	movs	r3, #72
	str	r3, [r2, #12]
	ldr	r5, [sp, #92]
	movs	r0, #225
	movs	r4, #0
	lsls	r0, r0, #7
	mov	r9, r4
	adds	r7, r5, r0
.L_080d1d52:
	bl	sub_08004458
	movs	r3, #127
	adds	r6, r0, #0
	ands	r6, r3
	bl	sub_08004458
	ldr	r5, [pc, #596]
	ldr	r1, [pc, #600]
	ands	r5, r0
	adds	r5, r5, r1
	adds	r0, r5, #0
	bl	sub_08002322
	adds	r3, r6, #0
	muls	r3, r0
	adds	r0, r5, #0
	str	r3, [r7, #0]
	bl	sub_0800231c
	adds	r3, r6, #0
	muls	r3, r0
	str	r3, [r7, #4]
	bl	sub_08004458
	movs	r1, #200
	bl	sub_08002304
	movs	r2, #1
	movs	r3, #0
	subs	r0, #100
	add	r9, r2
	str	r3, [r7, #24]
	lsls	r0, r0, #16
	mov	r3, r9
	str	r0, [r7, #8]
	adds	r7, #28
	cmp	r3, #32
	bne.n	.L_080d1d52
	ldr	r3, [pc, #512]
	ldr	r5, [sp, #92]
	ldr	r3, [r5, r3]
	ldr	r3, [r3, #20]
	movs	r4, #0
	mov	r9, r4
	cmp	r3, #0
	beq.n	.L_080d1dd2
	ldr	r1, [pc, #496]
	ldr	r7, [sp, #92]
	adds	r3, r7, r1
	ldr	r5, [pc, #520]
	ldr	r1, [r3, #0]
	add	r4, sp, #292
	movs	r6, #16
	add	r0, sp, #260
	movs	r2, #0
.L_080d1dc2:
	str	r6, [r2, r4]
	str	r5, [r2, r0]
	movs	r3, #1
	add	r9, r3
	ldr	r3, [r1, #20]
	adds	r2, #4
	cmp	r9, r3
	bne.n	.L_080d1dc2
.L_080d1dd2:
	ldr	r4, [sp, #80]
	cmp	r4, #159
	bgt.n	.L_080d1dda
	b.n	.L_080d2330
.L_080d1dda:
	ldr	r3, [pc, #488]
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	mov	r7, sl
	str	r3, [sp, #104]
	str	r4, [sp, #108]
	movs	r5, #16
	cmp	r7, #64
	bgt.n	.L_080d1dee
	movs	r5, #32
.L_080d1dee:
	mov	r1, sl
	lsls	r0, r1, #7
	bl	sub_08002322
	ldr	r2, [sp, #68]
	lsls	r0, r0, #5
	asrs	r0, r0, #6
	subs	r0, r2, r0
	mov	r3, sl
	str	r0, [sp, #68]
	lsls	r0, r3, #9
	bl	sub_08002322
	adds	r3, r5, #0
	muls	r3, r0
	ldr	r4, [sp, #72]
	ldr	r2, [sp, #60]
	asrs	r3, r3, #6
	adds	r3, r4, r3
	str	r3, [sp, #72]
	adds	r1, r3, #0
	lsrs	r3, r2, #31
	ldr	r4, [sp, #64]
	adds	r3, r3, r2
	asrs	r3, r3, #1
	ldr	r5, [sp, #60]
	str	r3, [sp, #60]
	lsrs	r3, r4, #31
	adds	r3, r3, r4
	asrs	r3, r3, #1
	ldr	r0, [sp, #64]
	str	r3, [sp, #64]
	movs	r3, #128
	lsls	r3, r3, #9
	add	r4, sp, #104
	ldr	r7, [sp, #68]
	str	r3, [sp, #104]
	add	r2, sp, #112
	str	r3, [r4, #4]
	movs	r3, #0
	adds	r0, r0, r1
	str	r3, [r2, #12]
	adds	r5, r5, r7
	str	r0, [sp, #72]
	ldr	r7, [sp, #92]
	ldr	r0, [pc, #380]
	str	r5, [sp, #68]
	adds	r5, r7, r0
	movs	r7, #255
	mov	r9, r3
	adds	r6, r2, #0
	lsls	r7, r7, #16
.L_080d1e56:
	ldr	r3, [pc, #372]
	mov	r1, r9
	ldrb	r3, [r3, r1]
	ldr	r2, [sp, #68]
	lsls	r3, r3, #16
	movs	r0, #160
	adds	r3, r3, r2
	lsls	r0, r0, #14
	adds	r3, r3, r0
	str	r3, [r6, #0]
	ldr	r3, [pc, #356]
	ldrb	r3, [r3, r1]
	lsls	r2, r1, #6
	adds	r3, r3, r2
	ldr	r1, [sp, #72]
	lsls	r3, r3, #16
	adds	r3, r3, r1
	adds	r2, r4, #0
	str	r7, [r6, #4]
	str	r3, [r6, #8]
	ldmia	r5!, {r0}
	movs	r3, #0
	adds	r1, r6, #0
	str	r4, [sp, #8]
	bl	sub_08009008
	movs	r3, #1
	movs	r2, #128
	add	r9, r3
	lsls	r2, r2, #15
	mov	r0, r9
	adds	r7, r7, r2
	ldr	r4, [sp, #8]
	cmp	r0, #9
	bne.n	.L_080d1e56
	ldr	r1, [sp, #80]
	cmp	r1, #255
	bgt.n	.L_080d1ea4
	b.n	.L_080d2330
.L_080d1ea4:
	movs	r3, #128
	add	r5, sp, #128
	movs	r2, #0
	lsls	r3, r3, #17
	str	r3, [r5, #8]
	str	r2, [r5, #0]
	str	r2, [r5, #4]
	mov	sl, r2
	bl	sub_080049ac
	adds	r0, r5, #0
	bl	sub_08004cb4
	ldr	r4, [pc, #276]
	ldr	r3, [sp, #80]
	cmp	r3, r4
	ble.n	.L_080d1ec8
	b.n	.L_080d2030
.L_080d1ec8:
	ldr	r7, [sp, #92]
	movs	r0, #225
	movs	r5, #0
	lsls	r0, r0, #7
	mov	r9, r5
	adds	r6, r7, r0
.L_080d1ed4:
	ldr	r3, [r6, #0]
	asrs	r3, r3, #8
	adds	r0, r3, #0
	muls	r0, r3
	ldr	r3, [r6, #4]
	asrs	r3, r3, #8
	adds	r2, r3, #0
	muls	r2, r3
	ldr	r3, [r6, #8]
	asrs	r3, r3, #8
	adds	r1, r3, #0
	muls	r1, r3
	adds	r0, r0, r2
	adds	r3, r1, #0
	adds	r0, r0, r3
	ldr	r3, [pc, #228]
	bl	sub_080072f0
	asrs	r0, r0, #9
	mov	r8, r0
	cmp	r0, #0
	beq.n	.L_080d1fe0
	add	r7, sp, #140
	adds	r1, r7, #0
	adds	r0, r6, #0
	bl	sub_080e3944
	ldr	r3, [r7, #0]
	ldr	r4, [sp, #68]
	asrs	r3, r3, #17
	asrs	r2, r4, #17
	adds	r3, r3, r2
	adds	r3, #32
	ldr	r0, [sp, #72]
	str	r3, [r7, #0]
	movs	r5, #6
	ldrsh	r3, [r7, r5]
	asrs	r2, r0, #16
	adds	r3, r3, r2
	subs	r3, #4
	str	r3, [r7, #4]
	movs	r1, #10
	ldrsh	r3, [r7, r1]
	str	r3, [r7, #8]
	cmp	r3, #169
	bgt.n	.L_080d1f34
	movs	r3, #170
	str	r3, [r7, #8]
.L_080d1f34:
	movs	r3, #175
	ldr	r0, [r7, #8]
	lsls	r3, r3, #1
	cmp	r0, r3
	ble.n	.L_080d1f42
	str	r3, [r7, #8]
	adds	r0, r3, #0
.L_080d1f42:
	movs	r1, #36
	subs	r0, #170
	bl	sub_080022ec
	movs	r3, #6
	subs	r4, r3, r0
	lsls	r0, r4, #1
	ldr	r2, [pc, #136]
	subs	r3, r0, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #76]
	lsrs	r3, r4, #31
	adds	r1, r2, r1
	adds	r3, r4, r3
	ldr	r2, [r7, #0]
	asrs	r3, r3, #1
	subs	r2, r2, r3
	ldr	r3, [r7, #4]
	str	r4, [sp, #0]
	subs	r3, r3, r4
	str	r0, [sp, #4]
	ldr	r4, [sp, #88]
	ldr	r0, [sp, #100]
	bl	sub_080072f4
	ldr	r5, [r6, #0]
	mov	r1, r8
	adds	r0, r5, #0
	bl	sub_080022ec
	subs	r5, r5, r0
	str	r5, [r6, #0]
	ldr	r5, [r6, #4]
	mov	r1, r8
	adds	r0, r5, #0
	bl	sub_080022ec
	subs	r5, r5, r0
	str	r5, [r6, #4]
	ldr	r5, [r6, #8]
	mov	r1, r8
	adds	r0, r5, #0
	bl	sub_080022ec
	subs	r5, r5, r0
	str	r5, [r6, #8]
	b.n	.L_080d1fe4
	.4byte 0x0000003b
	.4byte 0x00007828
	.4byte 0x00000088
	.4byte 0x03001388
	.4byte 0x02010018
	.4byte 0x03001ce0
	.4byte 0x00007fff
	.4byte 0x00009fff
	.4byte 0x00001770
	.4byte 0x080eda78
	.4byte 0x000077d8
	.4byte 0x080ee15a
	.4byte 0x080ee163
	.4byte 0x00000149
	.4byte 0x030001d8
	.2byte 0xde48
	.2byte 0x080e
.L_080d1fe0:
	movs	r5, #1
	add	sl, r5
.L_080d1fe4:
	movs	r7, #1
	add	r9, r7
	mov	r0, r9
	adds	r6, #28
	cmp	r0, #32
	beq.n	.L_080d1ff2
	b.n	.L_080d1ed4
.L_080d1ff2:
	mov	r1, sl
	cmp	r1, #0
	ble.n	.L_080d2030
	movs	r1, #10
	mov	r0, sl
	bl	sub_080022ec
	adds	r4, r0, #1
	lsls	r0, r4, #1
	ldr	r2, [pc, #928]
	subs	r3, r0, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #76]
	ldr	r3, [sp, #68]
	adds	r1, r2, r1
	asrs	r2, r3, #17
	lsrs	r3, r4, #31
	adds	r3, r4, r3
	ldr	r5, [sp, #72]
	asrs	r3, r3, #1
	subs	r2, r2, r3
	asrs	r3, r5, #16
	subs	r3, r3, r4
	str	r0, [sp, #4]
	adds	r2, #32
	subs	r3, #4
	str	r4, [sp, #0]
	ldr	r0, [sp, #100]
	ldr	r7, [sp, #88]
	bl	sub_08007300
.L_080d2030:
	movs	r0, #0
	mov	fp, r0
	mov	sl, r0
.L_080d2036:
	ldr	r2, [pc, #884]
	mov	r1, fp
	lsls	r3, r1, #1
	ldrh	r3, [r2, r3]
	ldr	r4, [sp, #80]
	cmp	r4, r3
	bne.n	.L_080d20b0
	ldr	r7, [pc, #872]
	movs	r5, #0
	mov	r9, r5
	add	r7, sl
	mov	r8, r5
.L_080d204e:
	bl	sub_08004458
	movs	r6, #127
	ands	r6, r0
	bl	sub_08004458
	ldr	r3, [pc, #856]
	adds	r5, r0, #0
	ands	r5, r3
	adds	r0, r5, #0
	bl	sub_08002322
	adds	r6, #16
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #12]
	adds	r0, r5, #0
	bl	sub_0800231c
	adds	r3, r6, #0
	muls	r3, r0
	negs	r3, r3
	asrs	r3, r3, #6
	str	r3, [r7, #16]
	bl	sub_08004458
	movs	r3, #255
	ands	r3, r0
	subs	r3, #128
	mov	r0, r8
	lsls	r3, r3, #10
	str	r3, [r7, #16]
	str	r0, [r7, #0]
	str	r0, [r7, #4]
	str	r0, [r7, #8]
	bl	sub_08004458
	movs	r3, #15
	movs	r1, #1
	ands	r3, r0
	add	r9, r1
	adds	r3, #64
	mov	r2, r9
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	r2, #96
	bne.n	.L_080d204e
	ldr	r2, [pc, #764]
.L_080d20b0:
	ldrh	r3, [r2, #0]
	ldr	r4, [sp, #80]
	cmp	r4, r3
	blt.n	.L_080d2156
	movs	r5, #0
	mov	r9, r5
	ldr	r5, [pc, #752]
.L_080d20be:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	ble.n	.L_080d2148
	add	r7, sp, #140
	adds	r1, r7, #0
	adds	r0, r5, #0
	bl	sub_080e3944
	ldr	r3, [r7, #0]
	asrs	r3, r3, #17
	adds	r3, #32
	str	r3, [r7, #0]
	movs	r0, #6
	ldrsh	r3, [r7, r0]
	adds	r3, #56
	str	r3, [r7, #4]
	movs	r1, #10
	ldrsh	r3, [r7, r1]
	str	r3, [r7, #8]
	cmp	r3, #169
	bgt.n	.L_080d20ec
	movs	r3, #170
	str	r3, [r7, #8]
.L_080d20ec:
	movs	r3, #175
	ldr	r0, [r7, #8]
	lsls	r3, r3, #1
	cmp	r0, r3
	ble.n	.L_080d20fa
	str	r3, [r7, #8]
	adds	r0, r3, #0
.L_080d20fa:
	movs	r1, #90
	subs	r0, #170
	bl	sub_080022ec
	movs	r3, #3
	subs	r4, r3, r0
	lsls	r0, r4, #1
	ldr	r2, [pc, #668]
	subs	r3, r0, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #76]
	lsrs	r3, r4, #31
	adds	r1, r2, r1
	adds	r3, r4, r3
	ldr	r2, [r7, #0]
	asrs	r3, r3, #1
	subs	r2, r2, r3
	ldr	r3, [r7, #4]
	str	r4, [sp, #0]
	subs	r3, r3, r4
	str	r0, [sp, #4]
	ldr	r4, [sp, #84]
	ldr	r0, [sp, #100]
	bl	sub_080072f4
	ldr	r2, [r5, #0]
	ldr	r3, [r5, #12]
	adds	r2, r2, r3
	str	r2, [r5, #0]
	ldr	r3, [r5, #4]
	ldr	r2, [r5, #16]
	adds	r3, r3, r2
	str	r3, [r5, #4]
	ldr	r3, [r5, #8]
	adds	r3, r3, r2
	str	r3, [r5, #8]
	ldr	r3, [r5, #24]
	subs	r3, #1
	str	r3, [r5, #24]
.L_080d2148:
	movs	r7, #1
	movs	r0, #128
	add	r9, r7
	lsls	r0, r0, #2
	adds	r5, #28
	cmp	r9, r0
	bne.n	.L_080d20be
.L_080d2156:
	movs	r2, #1
	movs	r1, #224
	add	fp, r2
	lsls	r1, r1, #4
	mov	r3, fp
	add	sl, r1
	cmp	r3, #4
	beq.n	.L_080d2168
	b.n	.L_080d2036
.L_080d2168:
	movs	r4, #0
	ldr	r3, [pc, #588]
	mov	r9, r4
	str	r4, [sp, #24]
	ldr	r2, [sp, #92]
	ldr	r4, [sp, #68]
	ldr	r0, [pc, #580]
	ldr	r7, [sp, #80]
	ldr	r5, [pc, #580]
	movs	r1, #165
	adds	r6, r2, r3
	asrs	r3, r4, #17
	adds	r0, r7, r0
	lsls	r1, r1, #1
	adds	r3, #32
	str	r5, [sp, #28]
	str	r0, [sp, #20]
	str	r1, [sp, #16]
	str	r3, [sp, #52]
.L_080d218e:
	ldr	r5, [sp, #80]
	ldr	r7, [sp, #16]
	cmp	r5, r7
	bne.n	.L_080d21d4
	ldr	r1, [sp, #72]
	ldr	r0, [sp, #52]
	asrs	r3, r1, #16
	subs	r3, #4
	str	r3, [r6, #4]
	str	r3, [r6, #16]
	str	r0, [r6, #0]
	str	r0, [r6, #12]
	ldr	r4, [sp, #92]
	ldr	r5, [pc, #536]
	movs	r2, #0
	mov	fp, r2
	adds	r3, r4, r5
	movs	r2, #4
.L_080d21b2:
	movs	r7, #1
	add	fp, r7
	mov	r0, fp
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r0, #28
	bne.n	.L_080d21b2
	ldr	r3, [sp, #92]
	ldr	r2, [pc, #516]
	ldr	r4, [pc, #516]
	movs	r1, #128
	lsls	r1, r1, #12
	str	r2, [sp, #64]
	adds	r2, r3, r4
	movs	r3, #8
	str	r1, [sp, #60]
	str	r3, [r2, #0]
.L_080d21d4:
	ldr	r5, [sp, #80]
	ldr	r7, [sp, #16]
	cmp	r5, r7
	bge.n	.L_080d21de
	b.n	.L_080d230a
.L_080d21de:
	ldr	r0, [sp, #20]
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r4, r3, #1
	cmp	r4, #2
	ble.n	.L_080d21ec
	movs	r4, #2
.L_080d21ec:
	ldr	r2, [pc, #480]
	lsls	r3, r4, #1
	ldrh	r1, [r2, r3]
	ldr	r3, [pc, #480]
	ldrb	r0, [r3, r4]
	ldr	r3, [r6, #4]
	subs	r3, r3, r0
	ldr	r0, [pc, #476]
	ldr	r2, [sp, #92]
	ldrb	r0, [r0, r4]
	adds	r1, r2, r1
	ldr	r2, [r6, #0]
	str	r0, [sp, #0]
	ldr	r0, [pc, #468]
	ldrb	r0, [r0, r4]
	ldr	r4, [sp, #88]
	str	r0, [sp, #4]
	ldr	r0, [sp, #100]
	bl	sub_080072f4
	ldr	r3, [r6, #0]
	subs	r3, #8
	str	r3, [r6, #0]
	ldr	r3, [r6, #4]
	adds	r3, #2
	str	r3, [r6, #4]
	ldr	r3, [sp, #16]
	ldr	r5, [sp, #80]
	adds	r3, #8
	cmp	r5, r3
	bge.n	.L_080d230a
	ldr	r0, [pc, #436]
	bl	sub_08004c6c
	ldr	r0, [pc, #432]
	bl	sub_08004c1c
	add	r0, sp, #152
	ldr	r1, [sp, #24]
	ldr	r2, [sp, #28]
	movs	r7, #0
	mov	r8, r0
	movs	r3, #0
	ldr	r4, [sp, #92]
	movs	r0, #232
	mov	fp, r7
	lsls	r0, r0, #7
	add	r7, sp, #140
	str	r1, [sp, #48]
	str	r2, [sp, #44]
	str	r3, [sp, #32]
	mov	sl, r7
	adds	r5, r4, r0
.L_080d2256:
	mov	r1, r8
	movs	r3, #0
	str	r3, [r1, #0]
	ldr	r0, [sp, #32]
	bl	sub_0800231c
	ldr	r3, [r5, #24]
	muls	r3, r0
	mov	r2, r8
	str	r3, [r2, #4]
	ldr	r0, [sp, #32]
	bl	sub_08002322
	ldr	r3, [r5, #24]
	muls	r3, r0
	mov	r4, r8
	str	r3, [r4, #8]
	ldr	r3, [r5, #24]
	adds	r3, #2
	str	r3, [r5, #24]
	mov	r1, sl
	mov	r0, r8
	bl	sub_080e3944
	mov	r0, sl
	ldr	r1, [sp, #48]
	ldr	r4, [pc, #348]
	ldr	r2, [r0, #0]
	ldr	r0, [sp, #92]
	adds	r3, r1, r4
	ldr	r3, [r0, r3]
	asrs	r2, r2, #17
	mov	r1, sl
	adds	r2, r2, r3
	str	r2, [r1, #0]
	ldr	r4, [sp, #44]
	movs	r3, #6
	ldrsh	r2, [r1, r3]
	ldr	r3, [r0, r4]
	adds	r2, r2, r3
	movs	r0, #10
	ldrsh	r3, [r1, r0]
	str	r2, [r1, #4]
	str	r3, [r1, #8]
	cmp	r3, #169
	bgt.n	.L_080d22b6
	movs	r3, #170
	str	r3, [r7, #8]
.L_080d22b6:
	movs	r3, #175
	ldr	r0, [r7, #8]
	lsls	r3, r3, #1
	cmp	r0, r3
	ble.n	.L_080d22c4
	str	r3, [r7, #8]
	adds	r0, r3, #0
.L_080d22c4:
	movs	r1, #90
	subs	r0, #170
	bl	sub_080022ec
	movs	r3, #3
	subs	r4, r3, r0
	lsls	r0, r4, #1
	ldr	r2, [pc, #212]
	subs	r3, r0, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #76]
	lsrs	r3, r4, #31
	adds	r1, r2, r1
	adds	r3, r4, r3
	ldr	r2, [r7, #0]
	asrs	r3, r3, #1
	subs	r2, r2, r3
	ldr	r3, [r7, #4]
	str	r4, [sp, #0]
	subs	r3, r3, r4
	str	r0, [sp, #4]
	ldr	r4, [sp, #84]
	ldr	r0, [sp, #100]
	bl	sub_080072f4
	ldr	r1, [pc, #244]
	ldr	r0, [sp, #32]
	movs	r2, #1
	add	fp, r2
	adds	r0, r0, r1
	mov	r3, fp
	str	r0, [sp, #32]
	adds	r5, #28
	cmp	r3, #28
	bne.n	.L_080d2256
.L_080d230a:
	ldr	r4, [sp, #28]
	ldr	r5, [sp, #24]
	ldr	r7, [sp, #20]
	ldr	r0, [sp, #16]
	movs	r1, #1
	add	r9, r1
	adds	r4, #28
	adds	r5, #28
	subs	r7, #12
	adds	r0, #12
	mov	r2, r9
	str	r4, [sp, #28]
	str	r5, [sp, #24]
	str	r7, [sp, #20]
	str	r0, [sp, #16]
	adds	r6, #28
	cmp	r2, #4
	beq.n	.L_080d2330
	b.n	.L_080d218e
.L_080d2330:
	bl	sub_080cd52c
	ldr	r4, [pc, #184]
	ldr	r3, [sp, #92]
	adds	r2, r3, r4
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r5, [sp, #80]
	movs	r7, #200
	adds	r5, #1
	lsls	r7, r7, #1
	str	r5, [sp, #80]
	cmp	r5, r7
	beq.n	.L_080d2356
	bl	.L_080d18a8
.L_080d2356:
	movs	r0, #134
	bl	sub_080b50e8
	ldr	r3, [pc, #148]
	ldr	r1, [sp, #92]
	adds	r2, r1, r3
	ldr	r3, [r2, #0]
	ldr	r3, [r3, #20]
	movs	r0, #0
	mov	r9, r0
	cmp	r3, #0
	beq.n	.L_080d23a0
	adds	r6, r2, #0
	add	r7, sp, #164
	add	r5, sp, #196
	movs	r1, #36
.L_080d2376:
	ldr	r3, [r6, #0]
	ldrsh	r0, [r3, r1]
	str	r1, [sp, #12]
	bl	sub_080b5098
	ldr	r3, [r5, #0]
	ldr	r2, [r0, #0]
	str	r3, [r2, #8]
	ldr	r3, [r5, #4]
	str	r3, [r2, #16]
	ldmia	r7!, {r3}
	strh	r3, [r2, #6]
	ldr	r3, [r6, #0]
	ldr	r1, [sp, #12]
	movs	r0, #1
	ldr	r3, [r3, #20]
	add	r9, r0
	adds	r5, #8
	adds	r1, #2
	cmp	r9, r3
	bne.n	.L_080d2376
.L_080d23a0:
	ldr	r2, [pc, #84]
	movs	r3, #120
	str	r3, [r2, #12]
	b.n	.L_080d23fc
	.4byte 0x080ede48
	.4byte 0x080ee16c
	.4byte 0x02010000
	.4byte 0x0000ffff
	.4byte 0x00007710
	.4byte 0xfffffeb6
	.4byte 0x00007720
	.4byte 0x00007418
	.4byte 0xfffe0000
	.4byte 0x000077a8
	.4byte 0x080ee17e
	.4byte 0x080ee17a
	.4byte 0x080ee174
	.4byte 0x080ee177
	.4byte 0xfffff800
	.4byte 0xfffff000
	.4byte 0x0000771c
	.4byte 0x00000924
	.4byte 0x00007824
	.4byte 0x00007828
	.2byte 0x1ce0
	.2byte 0x0300
.L_080d23fc:
	bl	sub_080d67dc
	ldr	r3, [pc, #64]
	ldr	r2, [sp, #92]
	movs	r1, #0
	mov	r9, r1
	adds	r5, r2, r3
.L_080d240a:
	ldmia	r5!, {r0}
	bl	sub_08009038
	movs	r4, #1
	add	r9, r4
	mov	r7, r9
	cmp	r7, #9
	bne.n	.L_080d240a
	ldr	r0, [pc, #44]
	bl	sub_08004278
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	bl	sub_080cdbc0
	add	sp, #356
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x000077d8
	.4byte 0x080cd261
