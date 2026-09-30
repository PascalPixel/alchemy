.syntax unified
	.thumb
	.global Graphics_AdvancePaletteCycle
	.thumb_func
Graphics_AdvancePaletteCycle:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r4, [r3, #36]
	cmp r4, #0
	bne .L_08126bda
	b .L_08126cfa
.L_08126bda:
	movs r2, #207
	lsls r2, r2, #3
	adds r3, r4, r2
	ldrh r3, [r3]
	cmp r3, #6
	bne .L_08126c20
	movs r3, #192
	lsls r3, r3, #3
	adds r3, #118
	adds r0, r4, r3
	ldrh r3, [r0]
	lsrs r1, r3, #1
	cmp r1, #16
	bls .L_08126bf8
	movs r1, #16
.L_08126bf8:
	ldr r3, .L_08126c18
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	ldr r3, .L_08126c1c
	adds r2, #2
	subs r3, r3, r1
	lsls r3, r3, #8
	orrs r3, r1
	strh r3, [r2]
	ldrh r3, [r0]
	adds r3, #1
	strh r3, [r0]
	b .L_08126cfa
	.2byte 0x0000
.L_08126c18:
	.4byte 0x00003f40
.L_08126c1c:
	.4byte 0x00000010
.L_08126c20:
	cmp r3, #4
	bne .L_08126c60
	movs r2, #192
	lsls r2, r2, #3
	adds r2, #118
	adds r4, r4, r2
	ldrh r3, [r4]
	lsrs r0, r3, #1
	cmp r0, #16
	bls .L_08126c36
	movs r0, #16
.L_08126c36:
	ldr r3, .L_08126c58
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	ldr r3, .L_08126c5c
	movs r1, #128
	subs r3, r3, r0
	lsls r1, r1, #19
	lsls r2, r0, #8
	orrs r2, r3
	adds r1, #82
	strh r2, [r1]
	ldrh r3, [r4]
	adds r3, #1
	strh r3, [r4]
	b .L_08126cfa
.L_08126c58:
	.4byte 0x00003f40
.L_08126c5c:
	.4byte 0x00000010
.L_08126c60:
	cmp r3, #2
	bne .L_08126cb0
	ldr r3, .L_08126c98
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	movs r3, #192
	lsls r3, r3, #3
	adds r3, #118
	adds r4, r4, r3
	ldrh r3, [r4]
	ldr r2, .L_08126ca0
	movs r0, #128
	ldrsb r1, [r2, r3]
	ldr r3, .L_08126c9c
	lsls r2, r1, #8
	subs r3, r3, r1
	lsls r0, r0, #19
	orrs r2, r3
	adds r0, #82
	strh r2, [r0]
	movs r3, #15
	ldrh r2, [r4]
	adds r1, r2, #1
	ands r1, r3
	b .L_08126ca4
	.2byte 0x0000
.L_08126c98:
	.4byte 0x00003f40
.L_08126c9c:
	.4byte 0x00000010
.L_08126ca0:
	.4byte Data_0812ce54
.L_08126ca4:
	cmp r2, #14
	bls .L_08126cac
	movs r3, #16
	orrs r1, r3
.L_08126cac:
	strh r1, [r4]
	b .L_08126cfa
.L_08126cb0:
	cmp r3, #0
	beq .L_08126cfa
	ldr r3, .L_08126ce4
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	ldr r3, .L_08126ce8
	adds r2, #2
	strh r3, [r2]
	movs r3, #192
	lsls r3, r3, #3
	adds r3, #118
	adds r0, r4, r3
	ldr r2, .L_08126cec
	ldrh r3, [r0]
	movs r1, #128
	ldrsb r3, [r2, r3]
	lsls r1, r1, #19
	adds r1, #84
	strh r3, [r1]
	movs r3, #15
	ldrh r2, [r0]
	adds r1, r2, #1
	ands r1, r3
	b .L_08126cf0
.L_08126ce4:
	.4byte 0x00003f90
.L_08126ce8:
	.4byte 0x00000010
.L_08126cec:
	.4byte Data_0812ce54
.L_08126cf0:
	cmp r2, #14
	bls .L_08126cf8
	movs r3, #16
	orrs r1, r3
.L_08126cf8:
	strh r1, [r0]
.L_08126cfa:
	pop {pc}
