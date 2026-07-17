@ Reconstructed thumb disassembly of a call-graph-reached code region
@ (code interleaved with its data), verified byte-identical by build_asm.
.syntax unified
	.thumb
	.set sub_080030f8, 0x080030f8
	.set sub_08009080, 0x08009080
	.set sub_08009088, 0x08009088
	.set sub_08015120, 0x08015120
	.set sub_080151c8, 0x080151c8
	.set sub_08077000, 0x08077000
	.set sub_08077008, 0x08077008
	.set sub_08077010, 0x08077010
	.set sub_08077080, 0x08077080
	.set sub_080770c0, 0x080770c0
	.set sub_08077170, 0x08077170
	.set sub_080771a0, 0x080771a0
	.set sub_080771b0, 0x080771b0
	.set sub_080771c0, 0x080771c0
	.set sub_080771c8, 0x080771c8
	.set sub_080771e0, 0x080771e0
	.set sub_080771e8, 0x080771e8
	.set sub_08077208, 0x08077208
	.set sub_08077210, 0x08077210
	.set sub_080b7dd0, 0x080b7dd0
	.set sub_080bbabc, 0x080bbabc
	.set sub_080bd808, 0x080bd808
	.set sub_080bdfec, 0x080bdfec
	.set sub_080be02c, 0x080be02c
	.set sub_080be070, 0x080be070
	.set sub_080be0b4, 0x080be0b4
	.set sub_080be18c, 0x080be18c
	.set sub_080bef88, 0x080bef88
	.set sub_080befac, 0x080befac
	.set sub_080befb4, 0x080befb4
	.set sub_080bf1a8, 0x080bf1a8
	.set sub_080bf1d6, 0x080bf1d6
	.set sub_080c10e8, 0x080c10e8
	.set sub_080c1798, 0x080c1798
	.set sub_080c23e8, 0x080c23e8
	.set sub_080f9010, 0x080f9010
	.global Region_080beb08
Region_080beb08:
	mov r2, r10
	ldr r3, [r2, #0]
	ldrh r3, [r3, #8]
	ldr r6, [pc, #52]
	lsls r0, r3, #16
	movs r5, #255
	asrs r0, r0, #24
	adds r1, r5, #0
	ands r1, r3
	ands r0, r6
	bl sub_080771e8
	mov r4, r10
	ldr r3, [r4, #0]
	mov r11, r0
	movs r1, #0
	ldrsh r0, [r3, r1]
	ldrh r3, [r3, #8]
	lsls r1, r3, #16
	asrs r1, r1, #24
	adds r2, r5, #0
	ands r1, r6
	ands r2, r3
	bl sub_08077208
	cmp r0, #0
	beq .L_080beb40
	b .L_080bec90
.L_080beb40:
	b .L_080beb48
	.2byte 0x0000
	.4byte 0x0000000f
.L_080beb48:
	mov r2, r10
	ldr r3, [r2, #0]
	movs r4, #0
	ldrsh r0, [r3, r4]
	ldrh r3, [r3, #8]
	lsls r1, r3, #16
	asrs r1, r1, #24
	adds r2, r5, #0
	ands r1, r6
	ands r2, r3
	bl sub_08077210
	cmp r0, #0
	bne .L_080beb66
	b .L_080bec62
.L_080beb66:
	mov r0, r11
	bl sub_08077080
	movs r1, #0
	movs r0, #0
	bl sub_080c10e8
	mov r0, r10
	ldr r3, [r0, #0]
	movs r1, #0
	ldrsh r0, [r3, r1]
	ldrh r3, [r3, #8]
	lsls r1, r3, #16
	asrs r1, r1, #24
	adds r2, r5, #0
	ands r2, r3
	ands r1, r6
	bl sub_080771b0
	mov r2, r10
	ldr r3, [r2, #0]
	movs r4, #0
	ldrsh r0, [r3, r4]
	ldrh r3, [r3, #8]
	lsls r1, r3, #16
	asrs r1, r1, #24
	adds r2, r5, #0
	ands r1, r6
	ands r2, r3
	bl sub_080771c0
	mov r0, r10
	ldr r3, [r0, #0]
	movs r1, #0
	ldrsh r0, [r3, r1]
	bl sub_08077010
	bl sub_080bdfec
	movs r0, #30
	bl sub_080bd808
	mov r2, r10
	ldr r3, [r2, #0]
	movs r0, #0
	movs r4, #0
	ldrsh r1, [r3, r4]
	bl sub_080bbabc
	mov r0, r10
	ldr r3, [r0, #0]
	ldrh r2, [r3, #8]
	lsls r3, r2, #16
	asrs r3, r3, #24
	ands r3, r6
	lsls r1, r3, #2
	adds r1, r1, r3
	adds r3, r5, #0
	ands r3, r2
	lsls r1, r1, #2
	movs r2, #150
	lsls r2, r2, #1
	adds r1, r1, r3
	adds r1, r1, r2
	movs r0, #3
	bl sub_080bbabc
	movs r1, #175
	movs r0, #14
	bl sub_080bbabc
	movs r1, #0
	movs r0, #10
	bl sub_080bbabc
	ldr r1, [pc, #856]
	movs r0, #4
	bl sub_080bbabc
	mov r4, r10
	ldr r3, [r4, #0]
	movs r0, #0
	ldrsh r1, [r3, r0]
	movs r0, #11
	bl sub_080bbabc
	movs r0, #212
	bl sub_080f9010
	mov r1, r10
	ldr r3, [r1, #0]
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl sub_080b7dd0
	movs r1, #3
	ldr r0, [r0, #0]
	bl sub_08009080
	mov r4, r10
	ldr r3, [r4, #0]
	movs r1, #0
	ldrsh r0, [r3, r1]
	bl sub_080b7dd0
	movs r1, #32
	ldr r0, [r0, #0]
	bl sub_08009088
	mov r2, r10
	ldr r3, [r2, #0]
	ldrh r1, [r3, #8]
	lsls r1, r1, #16
	asrs r1, r1, #24
	movs r4, #0
	ldrsh r0, [r3, r4]
	ands r1, r6
	movs r2, #3
	movs r3, #0
	bl sub_080c1798
	bl sub_080be02c
	movs r0, #2
	negs r0, r0
	b sub_080bf1d6
.L_080bec62:
	mov r0, r10
	ldr r3, [r0, #0]
	movs r1, #0
	ldrsh r0, [r3, r1]
	movs r1, #1
	bl sub_08015120
	movs r1, #4
	mov r0, r11
	bl sub_08015120
	movs r0, #114
	bl sub_080f9010
	ldr r0, [pc, #732]
	bl sub_080151c8
	movs r0, #60
	bl sub_080030f8
	movs r0, #1
	negs r0, r0
	b sub_080bf1d6
.L_080bec90:
	add r2, sp, #48
	mov r9, r2
	mov r0, r11
	bl sub_080be18c
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	bne .L_080beca4
	b sub_080bf1d6
.L_080beca4:
	mov r4, r10
	ldr r3, [r4, #0]
	movs r1, #0
	ldrsh r0, [r3, r1]
	ldrh r3, [r3, #8]
	lsls r1, r3, #16
	adds r2, r5, #0
	asrs r1, r1, #24
	ands r2, r3
	ands r1, r6
	bl sub_080771c8
	mov r0, r11
	bl sub_08077080
	mov r2, r10
	ldr r3, [r2, #0]
	adds r5, r0, #0
	movs r1, #1
	movs r4, #0
	ldrsh r0, [r3, r4]
	bl sub_08015120
	mov r0, r11
	movs r1, #4
	bl sub_08015120
	ldr r0, [pc, #644]
	bl sub_080151c8
	ldr r0, [sp, #8]
	ldrb r3, [r5, #2]
	ldr r2, [r0, #0]
	str r3, [r2, #80]
	b .L_080bee00
	mov r1, r10
	ldr r3, [r1, #0]
	movs r2, #8
	ldrsh r0, [r3, r2]
	bl sub_080771e0
	mov r4, r10
	movs r2, #24
	ldr r3, [r4, #0]
	add r2, sp
	mov r8, r2
	mov r9, r0
	movs r1, #0
	ldrsh r0, [r3, r1]
	mov r1, r8
	bl sub_080be0b4
	mov r4, r10
	ldr r3, [r4, #0]
	ldrh r3, [r3, #0]
	movs r0, #0
	cmp r3, #7
	bls .L_080bed1a
	movs r0, #1
.L_080bed1a:
	bl sub_08077000
	adds r0, #8
	str r0, [sp, #4]
	mov r1, r9
	adds r1, #4
	mov r0, r8
	ldrb r2, [r0, #0]
	ldrb r3, [r1, #0]
	movs r7, #0
	cmp r2, r3
	bcc .L_080bed56
	movs r5, #4
	mov r6, r8
	movs r4, #4
.L_080bed38:
	mov r2, r9
	ldrb r3, [r2, r5]
	adds r7, #1
	strb r3, [r0, #0]
	adds r4, #1
	adds r0, #1
	cmp r7, #3
	bgt .L_080bed56
	adds r6, #1
	adds r1, #1
	ldrb r2, [r6, #0]
	ldrb r3, [r1, #0]
	adds r5, r4, #0
	cmp r2, r3
	bcs .L_080bed38
.L_080bed56:
	mov r3, r9
	ldrh r3, [r3, #0]
	add r4, sp, #48
	mov r11, r3
	mov r9, r4
	mov r0, r11
	bl sub_080be18c
	movs r5, #1
	negs r5, r5
	cmp r0, r5
	bne .L_080bed70
	b sub_080bf1d6
.L_080bed70:
	cmp r7, #4
	beq .L_080bed94
	mov r0, r10
	ldr r3, [r0, #0]
	movs r1, #0
	ldrsh r0, [r3, r1]
	movs r1, #1
	bl sub_08015120
	mov r0, r11
	movs r1, #4
	bl sub_08015120
	ldr r0, [pc, #472]
	bl sub_080151c8
	adds r0, r5, #0
	b sub_080bf1d6
.L_080bed94:
	mov r2, r10
	ldr r3, [r2, #0]
	movs r1, #1
	movs r4, #0
	ldrsh r0, [r3, r4]
	bl sub_08015120
	movs r1, #4
	mov r0, r11
	bl sub_08015120
	ldr r0, [pc, #444]
	bl sub_080151c8
	movs r1, #128
	ldr r0, [sp, #4]
	lsls r1, r1, #1
	adds r3, r0, r1
	ldr r3, [r3, #0]
	movs r7, #0
	cmp r3, #0
	beq .L_080bee00
	mov r9, r5
	adds r5, r0, #0
.L_080bedc4:
	movs r3, #3
	ldrsb r3, [r5, r3]
	cmp r3, r9
	bne .L_080bedee
	ldrb r0, [r5, #2]
	bl sub_080be070
	cmp r0, #0
	beq .L_080bedee
	ldrb r1, [r5, #0]
	mov r3, r8
	ldrb r2, [r3, r1]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_080bedee
	movs r3, #254
	strb r3, [r5, #3]
	adds r3, r2, #0
	adds r3, #255
	mov r4, r8
	strb r3, [r4, r1]
.L_080bedee:
	ldr r0, [sp, #4]
	movs r1, #128
	lsls r1, r1, #1
	adds r3, r0, r1
	ldr r3, [r3, #0]
	adds r7, #1
	adds r5, #4
	cmp r7, r3
	bne .L_080bedc4
.L_080bee00:
	mov r2, r11
	cmp r2, #1
	beq .L_080bee08
	b sub_080befb4
.L_080bee08:
	ldr r4, [sp, #8]
	ldr r3, [r4, #0]
	ldrb r0, [r3, #2]
	bl sub_08077008
	adds r6, r0, #0
	ldr r0, [sp, #8]
	ldr r2, [r0, #0]
	mov r1, r10
	movs r3, #1
	str r3, [r2, #76]
	ldr r3, [r1, #0]
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl sub_08077170
	ldr r3, [sp, #8]
	ldr r1, [r3, #0]
	movs r3, #2
	str r0, [r1, #80]
	str r3, [r1, #84]
	ldr r4, [sp, #12]
	ldr r0, [pc, #308]
	ldr r2, [r4, #0]
	adds r3, r2, r0
	ldrb r3, [r3, #0]
	cmp r3, #0
	bne .L_080bee58
	movs r1, #148
	lsls r1, r1, #1
	adds r3, r2, r1
	ldrb r0, [r3, #0]
	bl sub_080c23e8
	ldr r3, [sp, #8]
	ldr r2, [r3, #0]
	movs r3, #128
	lsls r3, r3, #7
	orrs r3, r0
	b .L_080beea6
.L_080bee58:
	movs r3, #0
	movs r4, #148
	str r3, [r1, #88]
	lsls r4, r4, #1
	adds r3, r2, r4
	ldrb r3, [r3, #0]
	cmp r3, #5
	bhi .L_080beea8
	ldr r2, [pc, #260]
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.4byte 0x080beea0
	.4byte 0x080bee88
	.4byte 0x080bee90
	.4byte 0x080bee98
	.4byte 0x080beea8
	.4byte 0x080beea0
	ldr r1, [sp, #8]
	ldr r3, [pc, #232]
	ldr r2, [r1, #0]
	b .L_080beea6
	ldr r3, [sp, #8]
	ldr r2, [r3, #0]
	ldr r3, [pc, #224]
	b .L_080beea6
	ldr r4, [sp, #8]
	ldr r3, [pc, #220]
	ldr r2, [r4, #0]
	b .L_080beea6
	ldr r0, [sp, #8]
	ldr r3, [pc, #208]
	ldr r2, [r0, #0]
.L_080beea6:
	str r3, [r2, #88]
.L_080beea8:
	mov r1, r10
	ldr r3, [r1, #0]
	movs r1, #1
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl sub_08015120
	ldr r0, [pc, #196]
	bl sub_080151c8
	b .L_080beef4
.L_080beebe:
	ldr r4, [sp, #12]
	movs r0, #156
	ldr r3, [r4, #0]
	lsls r0, r0, #1
	adds r3, r3, r0
	ldrb r3, [r3, #0]
	cmp r3, #0
	beq .L_080beee0
	bl sub_080771a0
	movs r3, #255
	ands r0, r3
	cmp r0, #152
	bgt .L_080beee0
	ldr r1, [sp, #8]
	ldr r3, [r1, #0]
	strb r5, [r3, #30]
.L_080beee0:
	bl sub_080771a0
	movs r3, #31
	ands r0, r3
	cmp r0, #0
	bne .L_080bef28
	ldr r2, [sp, #8]
	ldr r3, [r2, #0]
	strb r0, [r3, #30]
	b .L_080bef28
.L_080beef4:
	movs r4, #56
	ldrsh r3, [r6, r4]
	cmp r3, #0
	beq .L_080bef28
	movs r0, #158
	lsls r0, r0, #1
	adds r3, r6, r0
	ldrb r3, [r3, #0]
	cmp r3, #0
	bne .L_080bef28
	ldr r1, [pc, #116]
	adds r3, r6, r1
	ldrb r3, [r3, #0]
	cmp r3, #0
	bne .L_080bef28
	ldr r2, [pc, #112]
	adds r3, r6, r2
	ldrb r3, [r3, #0]
	cmp r3, #0
	bne .L_080bef28
	movs r4, #157
	lsls r4, r4, #1
	adds r3, r6, r4
	ldrb r5, [r3, #0]
	cmp r5, #0
	beq .L_080beebe
.L_080bef28:
	movs r0, #183
	lsls r0, r0, #1
	bl sub_080770c0
	cmp r0, #0
	beq .L_080bef3c
	ldr r0, [sp, #8]
	ldr r2, [r0, #0]
	movs r3, #0
	strb r3, [r2, #30]
.L_080bef3c:
	movs r1, #56
	ldrsh r3, [r6, r1]
	cmp r3, #0
	bne .L_080bef46
	b sub_080bf1a8
.L_080bef46:
	bl sub_080771a0
	movs r3, #31
	ands r0, r3
	cmp r0, #0
	bne sub_080bef88
	ldr r2, [sp, #8]
	ldr r3, [r2, #0]
	b sub_080befac
