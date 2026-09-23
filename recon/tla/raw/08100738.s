.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08014878, 0x08014878
	.set sub_08016ca4, 0x08016ca4
	.set sub_08038120, 0x08038120
	.set sub_080ad008, 0x080ad008
	.set sub_080ad010, 0x080ad010
	.set sub_080ad050, 0x080ad050
	.set sub_080ad078, 0x080ad078
	.set sub_080ad0d0, 0x080ad0d0
	.set sub_080ad138, 0x080ad138
	.set sub_080fad88, 0x080fad88
	.global Func_08100738
	.thumb_func
Func_08100738:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r7, r1, #0
	mov	fp, r0
	adds	r0, r7, #0
	sub	sp, #4
	mov	r9, r2
	bl	sub_08016ca4
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	mov	r1, fp
	str	r3, [sp, #0]
	movs	r2, #128
	lsls	r3, r1, #1
	adds	r5, r0, #0
	adds	r3, #216
	lsls	r2, r2, #1
	adds	r2, #255
	mov	r8, r3
	ldrh	r3, [r5, r3]
	mov	sl, r2
	mov	r1, sl
	ands	r1, r3
	mov	sl, r1
	mov	r0, sl
	bl	sub_080ad010
	adds	r6, r0, #0
	ldrh	r3, [r6, #40]
	movs	r0, #252
	lsls	r0, r0, #6
	adds	r0, #255
	ands	r0, r3
	mov	r2, r9
	adds	r1, r7, #0
	movs	r3, #1
	bl	.L_081007f8
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	beq.n	.L_081007ea
	mov	r3, r8
	ldrh	r0, [r5, r3]
	bl	sub_080ad010
	adds	r6, r0, #0
	ldrb	r2, [r6, #12]
	adds	r3, r2, #0
	cmp	r3, #1
	bne.n	.L_081007d2
	mov	r1, fp
	adds	r0, r7, #0
	bl	sub_080ad050
	ldr	r2, [sp, #0]
	movs	r3, #226
	lsls	r3, r3, #1
	adds	r1, r2, r3
	adds	r0, r5, #0
	movs	r2, #0
	bl	sub_080fad88
	ldr	r1, [sp, #0]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r1, r2
	strb	r0, [r3, #0]
	ldrb	r2, [r6, #12]
.L_081007d2:
	adds	r3, r2, #0
	cmp	r3, #4
	bne.n	.L_081007e8
	mov	r3, sl
	cmp	r3, #184
	bne.n	.L_081007e2
	movs	r1, #185
	mov	sl, r1
.L_081007e2:
	mov	r3, sl
	mov	r2, r8
	strh	r3, [r5, r2]
.L_081007e8:
	movs	r0, #0
.L_081007ea:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
.L_081007f8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #20
	str	r1, [sp, #12]
	str	r3, [sp, #8]
	str	r0, [sp, #16]
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	mov	sl, r2
	str	r3, [sp, #4]
	bl	sub_080ad078
	movs	r1, #0
	mov	fp, r0
	mov	r2, sl
	movs	r0, #0
	mov	r9, r0
	movs	r7, #0
	mov	r8, r1
	cmp	r2, #9
	bne.n	.L_0810083c
	ldr	r3, [pc, #856]
	movs	r4, #133
	lsls	r4, r4, #2
	adds	r3, r3, r4
	ldr	r0, [r3, #0]
	b.n	.L_0810083e
.L_0810083c:
	mov	r0, sl
.L_0810083e:
	bl	sub_08016ca4
	adds	r5, r0, #0
	mov	r0, fp
	ldrb	r2, [r0, #8]
	movs	r1, #0
	ldr	r4, [sp, #4]
	movs	r0, #139
	str	r1, [sp, #0]
	lsls	r0, r0, #1
	adds	r0, #255
	adds	r3, r4, r0
	ldrb	r3, [r3, #0]
	cmp	r1, r3
	bcc.n	.L_0810085e
	b.n	.L_08100cd0
.L_0810085e:
	adds	r3, r2, #0
	cmp	r3, #255
	bne.n	.L_0810087c
	ldr	r1, [sp, #0]
	ldr	r4, [sp, #4]
	movs	r2, #129
	lsls	r3, r1, #1
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldrh	r3, [r4, r3]
	mov	sl, r3
	mov	r0, sl
	bl	sub_08016ca4
	adds	r5, r0, #0
.L_0810087c:
	mov	r0, fp
	ldrb	r2, [r0, #1]
	movs	r3, #15
	ands	r3, r2
	subs	r3, #1
	ldrh	r6, [r0, #10]
	cmp	r3, #10
	bls.n	.L_0810088e
	b.n	.L_08100a74
.L_0810088e:
	ldr	r2, [pc, #768]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x081008c4
	.4byte 0x08100a74
	.4byte 0x08100a74
	.4byte 0x08100a74
	.4byte 0x08100a74
	.4byte 0x08100a74
	.4byte 0x08100a74
	.4byte 0x08100a74
	.4byte 0x0810095e
	.4byte 0x08100a74
	.2byte 0x0a1e
	.2byte 0x0810
	ldr	r1, [sp, #8]
	cmp	r1, #0
	bne.n	.L_081008f2
	mov	r2, fp
	ldrb	r3, [r2, #2]
	cmp	r3, #4
	beq.n	.L_081008e4
	ldr	r0, [sp, #12]
	bl	sub_08016ca4
	mov	r4, fp
	ldrb	r3, [r4, #2]
	lsls	r3, r3, #2
	adds	r3, #72
	ldrsh	r1, [r0, r3]
	b.n	.L_081008e6
.L_081008e4:
	movs	r1, #100
.L_081008e6:
	movs	r2, #128
	adds	r0, r6, #0
	lsls	r2, r2, #1
	bl	sub_080ad138
	adds	r6, r0, #0
.L_081008f2:
	movs	r4, #56
	ldrsh	r1, [r5, r4]
	ldrh	r3, [r5, #56]
	cmp	r1, #0
	bgt.n	.L_08100908
	mov	r0, r8
	cmp	r0, #0
	beq.n	.L_08100904
	b.n	.L_08100a74
.L_08100904:
	movs	r7, #2
	b.n	.L_08100a74
.L_08100908:
	movs	r4, #52
	ldrsh	r2, [r5, r4]
	ldrh	r0, [r5, #52]
	cmp	r1, r2
	bne.n	.L_0810091e
	mov	r0, r8
	cmp	r0, #0
	beq.n	.L_0810091a
	b.n	.L_08100a74
.L_0810091a:
	movs	r7, #4
	b.n	.L_08100a74
.L_0810091e:
	adds	r3, r3, r6
	strh	r3, [r5, #56]
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	cmp	r3, r2
	ble.n	.L_0810093a
	subs	r3, r3, r2
	mov	r1, r8
	subs	r6, r6, r3
	strh	r0, [r5, #56]
	cmp	r1, #0
	bne.n	.L_08100942
	movs	r7, #0
	b.n	.L_08100942
.L_0810093a:
	mov	r2, r8
	cmp	r2, #0
	bne.n	.L_08100942
	movs	r7, #1
.L_08100942:
	mov	r0, sl
	bl	sub_080ad0d0
	movs	r3, #1
	mov	r4, fp
	mov	r9, r3
	ldrb	r3, [r4, #8]
	cmp	r3, #255
	beq.n	.L_08100956
	b.n	.L_08100a74
.L_08100956:
	movs	r0, #1
	mov	r8, r0
	movs	r7, #3
	b.n	.L_08100a74
	bl	sub_08014878
	lsls	r0, r0, #2
	lsrs	r0, r0, #16
	cmp	r0, #0
	bne.n	.L_08100970
	movs	r0, #1
	negs	r0, r0
	b.n	.L_0810097e
.L_08100970:
	movs	r1, #1
	adds	r2, r1, #0
	eors	r2, r0
	negs	r3, r2
	orrs	r3, r2
	lsrs	r0, r3, #31
	subs	r0, r1, r0
.L_0810097e:
	movs	r3, #252
	ldr	r1, [sp, #16]
	lsls	r3, r3, #6
	ldr	r2, [pc, #524]
	adds	r3, #255
	ands	r3, r1
	adds	r3, r3, r2
	cmp	r3, #5
	bhi.n	.L_08100a74
	ldr	r2, [pc, #516]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x081009b0
	.4byte 0x081009c0
	.4byte 0x081009ee
	.4byte 0x08100a06
	.4byte 0x081009d0
	.2byte 0x09e0
	.2byte 0x0810
	ldrh	r3, [r5, #16]
	adds	r2, r6, r0
	adds	r3, r3, r2
	strh	r3, [r5, #16]
	movs	r3, #1
	movs	r7, #16
	mov	r9, r3
	b.n	.L_08100a74
	ldrh	r3, [r5, #18]
	adds	r2, r6, r0
	adds	r3, r3, r2
	movs	r4, #1
	strh	r3, [r5, #18]
	movs	r7, #17
	mov	r9, r4
	b.n	.L_08100a74
	ldrh	r3, [r5, #28]
	adds	r2, r6, r0
	adds	r3, r3, r2
	movs	r0, #1
	strh	r3, [r5, #28]
	movs	r7, #18
	mov	r9, r0
	b.n	.L_08100a74
	ldrb	r3, [r5, #30]
	movs	r1, #1
	adds	r3, r3, r6
	strb	r3, [r5, #30]
	movs	r7, #19
	mov	r9, r1
	b.n	.L_08100a74
	ldrh	r3, [r5, #24]
	adds	r2, r6, r0
	adds	r3, r3, r2
	strh	r3, [r5, #24]
	movs	r0, #3
	movs	r1, #5
	bl	sub_08038120
	movs	r2, #1
	movs	r7, #20
	mov	r9, r2
	b.n	.L_08100a74
	ldrh	r3, [r5, #26]
	adds	r2, r6, r0
	adds	r3, r3, r2
	strh	r3, [r5, #26]
	movs	r0, #4
	movs	r1, #5
	bl	sub_08038120
	movs	r3, #1
	movs	r7, #21
	mov	r9, r3
	b.n	.L_08100a74
	movs	r4, #58
	ldrsh	r3, [r5, r4]
	movs	r4, #54
	ldrsh	r2, [r5, r4]
	ldrh	r1, [r5, #58]
	ldrh	r0, [r5, #54]
	cmp	r3, r2
	bne.n	.L_08100a38
	mov	r0, r8
	cmp	r0, #0
	bne.n	.L_08100a74
	movs	r7, #7
	b.n	.L_08100a74
.L_08100a38:
	adds	r3, r1, r6
	strh	r3, [r5, #58]
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	cmp	r3, r2
	ble.n	.L_08100a54
	subs	r3, r3, r2
	mov	r1, r8
	subs	r6, r6, r3
	strh	r0, [r5, #58]
	cmp	r1, #0
	bne.n	.L_08100a5c
	movs	r7, #5
	b.n	.L_08100a5c
.L_08100a54:
	mov	r2, r8
	cmp	r2, #0
	bne.n	.L_08100a5c
	movs	r7, #6
.L_08100a5c:
	mov	r0, sl
	bl	sub_080ad0d0
	movs	r3, #1
	mov	r4, fp
	mov	r9, r3
	ldrb	r3, [r4, #8]
	cmp	r3, #255
	bne.n	.L_08100a74
	movs	r0, #1
	mov	r8, r0
	movs	r7, #8
.L_08100a74:
	mov	r1, fp
	ldrb	r3, [r1, #3]
	subs	r3, #1
	cmp	r3, #56
	bls.n	.L_08100a80
	b.n	.L_08100ca6
.L_08100a80:
	ldr	r2, [pc, #280]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x08100b6c
	.4byte 0x08100bc2
	.4byte 0x08100c7e
	.4byte 0x08100ca6
	.4byte 0x08100c08
	.4byte 0x08100ca6
	.4byte 0x08100ca6
	.4byte 0x08100ca6
	.4byte 0x08100ca6
	.4byte 0x08100ca6
	.4byte 0x08100ca6
	.4byte 0x08100ca6
	.4byte 0x08100ca6
	.4byte 0x08100ca6
	.4byte 0x08100ca6
	.4byte 0x08100ca6
	.4byte 0x08100ca6
	.4byte 0x08100ca6
	.4byte 0x08100ca6
	.4byte 0x08100ca6
	.4byte 0x08100ca6
	.4byte 0x08100ca6
	.4byte 0x08100ca6
	.4byte 0x08100ca6
	.4byte 0x08100ca6
	.4byte 0x08100ca6
	.4byte 0x08100ca6
	.4byte 0x08100ca6
	.4byte 0x08100ca6
	.4byte 0x08100ca6
	.4byte 0x08100ca6
	.4byte 0x08100ca6
	.4byte 0x08100ca6
	.4byte 0x08100ca6
	.4byte 0x08100ca6
	.4byte 0x08100ca6
	.4byte 0x08100ca6
	.4byte 0x08100ca6
	.4byte 0x08100ca6
	.4byte 0x08100ca6
	.4byte 0x08100ca6
	.4byte 0x08100ca6
	.4byte 0x08100ca6
	.4byte 0x08100ca6
	.4byte 0x08100ca6
	.4byte 0x08100ca6
	.4byte 0x08100ca6
	.4byte 0x08100ca6
	.4byte 0x08100ca6
	.4byte 0x08100ca6
	.4byte 0x08100ca6
	.4byte 0x08100ca6
	.4byte 0x08100ca6
	.4byte 0x08100ca6
	.4byte 0x08100ca6
	.4byte 0x08100c32
	.2byte 0x0c4c
	.2byte 0x0810
	movs	r4, #56
	ldrsh	r2, [r5, r4]
	ldrh	r3, [r5, #56]
	cmp	r2, #0
	ble.n	.L_08100b80
	movs	r4, #52
	ldrsh	r1, [r5, r4]
	ldrh	r0, [r5, #52]
	cmp	r2, r1
	bne.n	.L_08100ba0
.L_08100b80:
	mov	r0, r8
	cmp	r0, #0
	beq.n	.L_08100b88
	b.n	.L_08100ca6
.L_08100b88:
	movs	r7, #2
	b.n	.L_08100ca6
	.4byte 0x02000240
	.4byte 0x08100898
	.4byte 0xfffffefc
	.4byte 0x08100998
	.2byte 0x0a88
	.2byte 0x0810
.L_08100ba0:
	adds	r3, r3, r6
	strh	r3, [r5, #56]
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	cmp	r3, r1
	ble.n	.L_08100bb8
	mov	r1, r8
	strh	r0, [r5, #56]
	cmp	r1, #0
	bne.n	.L_08100bfc
	movs	r7, #0
	b.n	.L_08100bfc
.L_08100bb8:
	mov	r2, r8
	cmp	r2, #0
	bne.n	.L_08100bfc
	movs	r7, #1
	b.n	.L_08100bfc
	movs	r4, #58
	ldrsh	r3, [r5, r4]
	movs	r4, #54
	ldrsh	r2, [r5, r4]
	ldrh	r1, [r5, #58]
	ldrh	r0, [r5, #54]
	cmp	r3, r2
	bne.n	.L_08100bdc
	mov	r0, r8
	cmp	r0, #0
	bne.n	.L_08100ca6
	movs	r7, #7
	b.n	.L_08100ca6
.L_08100bdc:
	adds	r3, r1, r6
	strh	r3, [r5, #58]
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	cmp	r3, r2
	ble.n	.L_08100bf4
	mov	r1, r8
	strh	r0, [r5, #58]
	cmp	r1, #0
	bne.n	.L_08100bfc
	movs	r7, #5
	b.n	.L_08100bfc
.L_08100bf4:
	mov	r2, r8
	cmp	r2, #0
	bne.n	.L_08100bfc
	movs	r7, #6
.L_08100bfc:
	mov	r0, sl
	bl	sub_080ad0d0
	movs	r3, #1
	mov	r9, r3
	b.n	.L_08100ca6
	movs	r4, #56
	ldrsh	r3, [r5, r4]
	cmp	r3, #0
	bne.n	.L_08100c28
	ldrh	r3, [r5, #52]
	mov	r0, sl
	strh	r3, [r5, #56]
	bl	sub_080ad0d0
	movs	r0, #1
	mov	r1, r8
	mov	r9, r0
	cmp	r1, #0
	bne.n	.L_08100ca6
	movs	r7, #12
	b.n	.L_08100ca6
.L_08100c28:
	mov	r2, r8
	cmp	r2, #0
	bne.n	.L_08100ca6
	movs	r7, #13
	b.n	.L_08100ca6
	movs	r4, #56
	ldrsh	r3, [r5, r4]
	cmp	r3, #0
	bne.n	.L_08100c74
	ldrh	r3, [r5, #52]
	mov	r0, sl
	lsls	r3, r3, #16
	asrs	r2, r3, #16
	lsrs	r3, r3, #31
	adds	r2, r2, r3
	asrs	r2, r2, #1
	strh	r2, [r5, #56]
	b.n	.L_08100c66
	movs	r2, #56
	ldrsh	r3, [r5, r2]
	cmp	r3, #0
	bne.n	.L_08100c74
	movs	r4, #52
	ldrsh	r3, [r5, r4]
	movs	r1, #10
	lsls	r0, r3, #3
	subs	r0, r0, r3
	bl	sub_08002054
	strh	r0, [r5, #56]
	mov	r0, sl
.L_08100c66:
	bl	sub_080ad0d0
	mov	r0, r8
	cmp	r0, #0
	bne.n	.L_08100ca6
	movs	r7, #12
	b.n	.L_08100ca6
.L_08100c74:
	mov	r1, r8
	cmp	r1, #0
	bne.n	.L_08100ca6
	movs	r7, #13
	b.n	.L_08100ca6
	movs	r3, #50
	adds	r3, #255
	adds	r2, r5, r3
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	cmp	r3, #0
	beq.n	.L_08100c9e
	movs	r3, #0
	movs	r4, #1
	mov	r0, r8
	strb	r3, [r2, #0]
	mov	r9, r4
	cmp	r0, #0
	bne.n	.L_08100ca6
	movs	r7, #10
	b.n	.L_08100ca6
.L_08100c9e:
	mov	r1, r8
	cmp	r1, #0
	bne.n	.L_08100ca6
	movs	r7, #11
.L_08100ca6:
	mov	r2, fp
	ldrb	r3, [r2, #8]
	adds	r2, r3, #0
	adds	r3, r2, #0
	cmp	r3, #255
	bne.n	.L_08100cd0
	ldr	r3, [sp, #0]
	ldr	r4, [sp, #4]
	adds	r3, #1
	lsls	r3, r3, #24
	lsrs	r3, r3, #24
	movs	r0, #139
	str	r3, [sp, #0]
	lsls	r0, r0, #1
	adds	r0, #255
	adds	r3, r4, r0
	ldrb	r3, [r3, #0]
	ldr	r1, [sp, #0]
	cmp	r1, r3
	bcs.n	.L_08100cd0
	b.n	.L_0810085e
.L_08100cd0:
	mov	r2, r9
	cmp	r2, #0
	bne.n	.L_08100ce8
	ldr	r4, [sp, #4]
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #94
	adds	r3, r4, r0
	movs	r0, #1
	strh	r7, [r3, #0]
	negs	r0, r0
	b.n	.L_08100d30
.L_08100ce8:
	movs	r1, #0
	ldr	r2, [sp, #4]
	movs	r4, #139
	str	r1, [sp, #0]
	lsls	r4, r4, #1
	adds	r4, #255
	adds	r3, r2, r4
	ldrb	r3, [r3, #0]
	cmp	r1, r3
	bcs.n	.L_08100d22
	adds	r5, r2, r4
.L_08100cfe:
	ldr	r0, [sp, #0]
	ldr	r2, [sp, #4]
	movs	r1, #129
	lsls	r3, r0, #1
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldrh	r0, [r2, r3]
	bl	sub_080ad008
	ldr	r3, [sp, #0]
	adds	r3, #1
	lsls	r3, r3, #24
	lsrs	r3, r3, #24
	str	r3, [sp, #0]
	ldr	r4, [sp, #0]
	ldrb	r3, [r5, #0]
	cmp	r4, r3
	bcc.n	.L_08100cfe
.L_08100d22:
	ldr	r0, [sp, #4]
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r1, #94
	adds	r3, r0, r1
	strh	r7, [r3, #0]
	movs	r0, #0
.L_08100d30:
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.align 2, 0
