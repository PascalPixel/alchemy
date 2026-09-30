.syntax unified
	.thumb
	.global Func_0802de8c
	.thumb_func
Func_0802de8c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #60
	movs r0, #144
	movs r1, #96
	movs r2, #1
	movs r3, #0
	str r0, [sp, #20]
	str r1, [sp, #16]
	movs r0, #36
	movs r1, #160
	mov r11, r2
	mov r8, r3
	str r2, [sp, #12]
	str r3, [sp, #8]
	bl Runtime_AllocateBlock
	ldr r2, .L_0802df3c
	movs r3, #3
	str r0, [sp, #4]
	strb r3, [r2]
	ldr r0, [sp, #8]
	add r4, sp, #24
	movs r3, #128
	movs r2, #133
	str r0, [r4]
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r4, #0
	ldr r1, [sp, #4]
	adds r2, #1
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r1, [sp, #8]
	movs r2, #133
	lsls r2, r2, #24
	str r1, [r4]
	adds r0, r4, #0
	add r1, sp, #28
	adds r2, #8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #1
	movs r1, #1
	negs r0, r0
	bl Func_0802e444
	add r2, sp, #28
	ldr r1, .L_0802df30
	mov r10, r2
	movs r6, #0
	movs r2, #1
	mov r3, r10
.L_0802df02:
	adds r6, #1
	strh r2, [r3, #2]
	strb r1, [r3, #5]
	strh r0, [r3]
	adds r3, #8
	cmp r6, #3
	bls .L_0802df02
	ldr r2, .L_0802df40
	movs r3, #1
	mov r0, r10
	strb r3, [r0, #4]
	movs r3, #2
	strb r3, [r2]
	ldr r3, .L_0802df34
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	ldr r3, .L_0802df38
	movs r2, #160
	lsls r2, r2, #19
	b .L_0802df44
	.2byte 0x0000
.L_0802df30:
	.4byte 0x00000001
.L_0802df34:
	.4byte 0x00003f42
.L_0802df38:
	.4byte 0x00007c00
.L_0802df3c:
	.4byte gDecodeFillByte
.L_0802df40:
	.4byte Data_03001238
.L_0802df44:
	strh r3, [r2]
	ldr r3, .L_0802df84
	movs r2, #128
	lsls r2, r2, #19
	strh r3, [r2]
	movs r0, #1
	bl Func_08013eb4
.L_0802df54:
	bl Func_08014c6c
	bl Func_080144c0
	movs r1, #160
	movs r0, #36
	bl Runtime_AllocateBlock
	str r0, [sp, #4]
	bl Func_08014368
	movs r0, #2
	bl Func_080230e0
	mov r0, r10
	movs r3, #0
	ldrsh r2, [r0, r3]
	ldr r1, .L_0802df88
	movs r3, #0
	movs r0, #0
	bl Func_080227e0
	mov r2, r10
	b .L_0802df8c
.L_0802df84:
	.4byte 0x00001140
.L_0802df88:
	.4byte gMapCellBuffer
.L_0802df8c:
	movs r1, #0
	ldrsh r0, [r2, r1]
	bl Resource_GetMetadataRecordFar
	ldrb r3, [r0, #4]
	cmp r3, #20
	bne .L_0802dfac
	mov r0, r10
	movs r3, #0
	ldrsh r2, [r0, r3]
	ldr r1, .L_0802e280
	adds r2, #1
	movs r0, #1
	movs r3, #0
	bl Func_080227e0
.L_0802dfac:
	movs r6, #0
	mov r7, r10
.L_0802dfb0:
	movs r1, #0
	ldrsh r0, [r7, r1]
	bl Resource_GetMetadataRecordFar
	ldrb r3, [r0, #4]
	movs r5, #0
	cmp r3, #20
	bne .L_0802dfca
	movs r3, #1
	ands r3, r6
	cmp r3, #0
	beq .L_0802dfca
	movs r5, #1
.L_0802dfca:
	movs r2, #0
	ldrsh r0, [r7, r2]
	lsls r3, r5, #12
	adds r0, r0, r5
	adds r0, r0, r3
	bl Func_08022d40
	movs r3, #8
	ldrsh r1, [r7, r3]
	adds r5, r0, #0
	bl ResourceMetadata_Register
	movs r0, #16
	ldrsh r1, [r7, r0]
	adds r0, r5, #0
	bl ResourceMetadata_Register
	movs r2, #24
	ldrsh r1, [r7, r2]
	adds r0, r5, #0
	bl ResourceMetadata_Register
	add r3, sp, #12
	ldrb r3, [r3]
	adds r6, #1
	strb r3, [r5, #26]
	cmp r6, #9
	bls .L_0802dfb0
	mov r7, r10
	movs r0, #4
	movs r6, #0
	mov r5, r10
	adds r7, #4
	mov r9, r0
.L_0802e00e:
	mov r1, r9
	mov r2, r10
	ldrb r3, [r1, r2]
	cmp r3, #0
	beq .L_0802e024
	movs r1, #1
	ldrsb r1, [r7, r1]
	adds r0, r6, #0
	bl Func_0802e6d8
	b .L_0802e02c
.L_0802e024:
	adds r0, r6, #0
	movs r1, #8
	bl Func_0802e6d8
.L_0802e02c:
	movs r1, #6
	ldrsb r1, [r5, r1]
	adds r0, r6, #0
	bl Func_0802e6b4
	movs r3, #2
	ldrsh r1, [r5, r3]
	adds r0, r6, #0
	bl Func_0802e6fc
	adds r6, #1
	movs r0, #8
	adds r5, #8
	adds r7, #8
	add r9, r0
	cmp r6, #3
	bls .L_0802e00e
	ldr r0, [sp, #20]
	ldr r1, [sp, #16]
	ldr r2, [sp, #4]
	bl Func_0802e4c8
	movs r1, #144
	ldr r0, .L_0802e284
	lsls r1, r1, #3
	bl Func_080145a8
.L_0802e062:
	movs r0, #1
	bl WaitFrames
	ldr r4, .L_0802e288
	mov r9, r10
.L_0802e06c:
	adds r6, r4, #0
	ldr r2, [r6]
	movs r3, #8
	ands r2, r3
	cmp r2, #0
	beq .L_0802e0ca
	ldr r2, [r6]
	movs r3, #32
	ands r2, r3
	cmp r2, #0
	beq .L_0802e088
	ldr r1, [sp, #20]
	subs r1, #1
	str r1, [sp, #20]
.L_0802e088:
	ldr r2, [r6]
	movs r3, #16
	ands r2, r3
	cmp r2, #0
	beq .L_0802e098
	ldr r2, [sp, #20]
	adds r2, #1
	str r2, [sp, #20]
.L_0802e098:
	ldr r2, [r6]
	movs r3, #64
	ands r2, r3
	cmp r2, #0
	beq .L_0802e0a8
	ldr r3, [sp, #16]
	subs r3, #1
	str r3, [sp, #16]
.L_0802e0a8:
	ldr r2, [r6]
	movs r3, #128
	ands r2, r3
	cmp r2, #0
	beq .L_0802e0b8
	ldr r0, [sp, #16]
	adds r0, #1
	str r0, [sp, #16]
.L_0802e0b8:
	ldr r0, [sp, #20]
	ldr r1, [sp, #16]
	ldr r2, [sp, #4]
	str r4, [sp, #0]
	bl Func_0802e4c8
	ldr r6, .L_0802e288
	ldr r4, [sp, #0]
	b .L_0802e126
.L_0802e0ca:
	ldr r3, [r6, #12]
	ldr r2, [r6, #12]
	movs r3, #64
	ands r2, r3
	cmp r2, #0
	beq .L_0802e0e4
	movs r1, #1
	negs r1, r1
	add r8, r1
	mov r2, r8
	movs r3, #3
	ands r2, r3
	mov r8, r2
.L_0802e0e4:
	ldr r2, [r6, #12]
	movs r3, #128
	ands r2, r3
	cmp r2, #0
	beq .L_0802e0fa
	movs r0, #1
	add r8, r0
	mov r1, r8
	movs r2, #3
	ands r1, r2
	mov r8, r1
.L_0802e0fa:
	ldr r2, [r6, #12]
	movs r3, #32
	ands r2, r3
	cmp r2, #0
	beq .L_0802e110
	subs r3, #33
	add r11, r3
	mov r0, r11
	movs r1, #3
	ands r0, r1
	mov r11, r0
.L_0802e110:
	ldr r2, [r6, #12]
	movs r3, #16
	ands r2, r3
	cmp r2, #0
	beq .L_0802e126
	movs r2, #1
	add r11, r2
	mov r3, r11
	movs r0, #3
	ands r3, r0
	mov r11, r3
.L_0802e126:
	ldr r2, [r4, #4]
	movs r3, #8
	ands r2, r3
	cmp r2, #0
	beq .L_0802e150
	ldr r1, [sp, #12]
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #16]
	movs r3, #1
	eors r1, r3
	str r1, [sp, #12]
	movs r6, #0
.L_0802e140:
	add r3, sp, #12
	ldrb r3, [r3]
	adds r6, #1
	strb r3, [r2, #26]
	adds r2, #56
	cmp r6, #9
	bls .L_0802e140
	ldr r6, .L_0802e288
.L_0802e150:
	mov r0, r11
	cmp r0, #1
	beq .L_0802e1e0
	cmp r0, #1
	bcc .L_0802e168
	cmp r0, #2
	bne .L_0802e160
	b .L_0802e28c
.L_0802e160:
	cmp r0, #3
	bne .L_0802e166
	b .L_0802e2fe
.L_0802e166:
	b .L_0802e378
.L_0802e168:
	mov r1, r8
	cmp r1, #1
	bne .L_0802e170
	b .L_0802e378
.L_0802e170:
	ldr r2, [r4, #12]
	movs r3, #128
	lsls r3, r3, #2
	ands r2, r3
	cmp r2, #0
	beq .L_0802e1a4
	lsls r3, r1, #3
	mov r0, r10
	adds r2, r0, r3
	ldrh r3, [r2, #2]
	movs r1, #0
	subs r3, #1
	strh r3, [r2, #2]
	lsls r3, r3, #16
	cmp r3, #0
	bge .L_0802e192
	strh r1, [r2, #2]
.L_0802e192:
	mov r1, r8
	cmp r1, #0
	beq .L_0802e19a
	b .L_0802e062
.L_0802e19a:
	mov r2, r10
	ldrh r3, [r2, #2]
	mov r0, r10
	strh r3, [r0, #10]
	b .L_0802e062
.L_0802e1a4:
	ldr r2, [r6, #12]
	movs r3, #128
	lsls r3, r3, #1
	ands r2, r3
	cmp r2, #0
	bne .L_0802e1b2
	b .L_0802e378
.L_0802e1b2:
	mov r1, r8
	lsls r3, r1, #3
	mov r0, r10
	adds r2, r0, r3
	ldrh r3, [r2, #2]
	movs r1, #198
	adds r3, #1
	strh r3, [r2, #2]
	lsls r1, r1, #15
	lsls r3, r3, #16
	cmp r3, r1
	ble .L_0802e1ce
	movs r3, #99
	strh r3, [r2, #2]
.L_0802e1ce:
	mov r2, r8
	cmp r2, #0
	beq .L_0802e1d6
	b .L_0802e062
.L_0802e1d6:
	mov r0, r10
	ldrh r3, [r0, #2]
	mov r1, r10
	strh r3, [r1, #10]
	b .L_0802e062
.L_0802e1e0:
	ldr r2, [r4]
	movs r3, #8
	ands r2, r3
	movs r1, #0
	movs r7, #1
	cmp r2, #0
	beq .L_0802e1f0
	movs r7, #10
.L_0802e1f0:
	ldr r2, [r6, #12]
	movs r3, #128
	lsls r3, r3, #2
	ands r2, r3
	cmp r2, #0
	beq .L_0802e214
	mov r2, r8
	lsls r5, r2, #3
	mov r3, r9
	ldrsh r0, [r3, r5]
	negs r1, r7
	str r4, [sp, #0]
	bl Func_0802e444
	mov r2, r9
	strh r0, [r2, r5]
	ldr r4, [sp, #0]
	movs r1, #1
.L_0802e214:
	ldr r2, [r6, #12]
	movs r3, #128
	lsls r3, r3, #1
	ands r2, r3
	cmp r2, #0
	beq .L_0802e238
	mov r3, r8
	lsls r5, r3, #3
	mov r1, r9
	ldrsh r0, [r1, r5]
	adds r1, r7, #0
	str r4, [sp, #0]
	bl Func_0802e444
	mov r3, r9
	strh r0, [r3, r5]
	ldr r4, [sp, #0]
	movs r1, #1
.L_0802e238:
	cmp r1, #0
	bne .L_0802e23e
	b .L_0802e376
.L_0802e23e:
	mov r0, r8
	cmp r0, #0
	bne .L_0802e246
	b .L_0802df54
.L_0802e246:
	lsls r6, r0, #3
	adds r5, r6, #4
	mov r1, r10
	ldrb r3, [r1, r5]
	cmp r3, #0
	bne .L_0802e254
	b .L_0802e062
.L_0802e254:
	ldrsh r1, [r1, r6]
	add r5, r10
	bl Func_0802e76c
	movs r1, #1
	ldrsb r1, [r5, r1]
	mov r0, r8
	bl Func_0802e6d8
	movs r1, #2
	ldrsb r1, [r5, r1]
	mov r0, r8
	bl Func_0802e6b4
	mov r0, r10
	adds r3, r0, r6
	movs r2, #2
	ldrsh r1, [r3, r2]
	mov r0, r8
	bl Func_0802e6fc
	b .L_0802e062
.L_0802e280:
	.4byte Data_02018000
.L_0802e284:
	.4byte Func_0802e7a8
.L_0802e288:
	.4byte gInput
.L_0802e28c:
	ldr r2, [r4, #12]
	movs r3, #128
	lsls r3, r3, #2
	ands r2, r3
	movs r1, #0
	cmp r2, #0
	beq .L_0802e2b6
	mov r0, r8
	lsls r3, r0, #3
	adds r3, #4
	mov r1, r10
	adds r2, r1, r3
	ldrb r3, [r2, #1]
	subs r3, #1
	strb r3, [r2, #1]
	lsls r3, r3, #24
	cmp r3, #0
	bge .L_0802e2b4
	movs r3, #3
	strb r3, [r2, #1]
.L_0802e2b4:
	movs r1, #1
.L_0802e2b6:
	ldr r2, [r4, #12]
	movs r3, #128
	lsls r3, r3, #1
	ands r2, r3
	cmp r2, #0
	beq .L_0802e2e2
	mov r0, r8
	lsls r3, r0, #3
	adds r3, #4
	mov r1, r10
	adds r2, r1, r3
	ldrb r3, [r2, #1]
	movs r0, #192
	adds r3, #1
	strb r3, [r2, #1]
	lsls r0, r0, #18
	lsls r3, r3, #24
	movs r1, #0
	cmp r3, r0
	ble .L_0802e2e0
	strb r1, [r2, #1]
.L_0802e2e0:
	movs r1, #1
.L_0802e2e2:
	cmp r1, #0
	beq .L_0802e376
	mov r1, r8
	lsls r3, r1, #3
	adds r2, r3, #4
	mov r0, r10
	ldrb r3, [r0, r2]
	cmp r3, #0
	bne .L_0802e2f6
	b .L_0802e062
.L_0802e2f6:
	adds r3, r0, r2
	movs r1, #1
	ldrsb r1, [r3, r1]
	b .L_0802e3e2
.L_0802e2fe:
	ldr r2, [r4, #12]
	movs r3, #128
	lsls r3, r3, #2
	ands r2, r3
	movs r1, #0
	cmp r2, #0
	beq .L_0802e328
	mov r1, r8
	lsls r3, r1, #3
	adds r3, #4
	mov r0, r10
	adds r2, r0, r3
	ldrb r3, [r2, #2]
	subs r3, #1
	strb r3, [r2, #2]
	lsls r3, r3, #24
	cmp r3, #0
	bge .L_0802e326
	movs r3, #15
	strb r3, [r2, #2]
.L_0802e326:
	movs r1, #1
.L_0802e328:
	ldr r2, [r4, #12]
	movs r3, #128
	lsls r3, r3, #1
	ands r2, r3
	cmp r2, #0
	beq .L_0802e354
	mov r1, r8
	lsls r3, r1, #3
	adds r3, #4
	mov r0, r10
	adds r2, r0, r3
	ldrb r3, [r2, #2]
	movs r0, #240
	adds r3, #1
	strb r3, [r2, #2]
	lsls r0, r0, #20
	lsls r3, r3, #24
	movs r1, #0
	cmp r3, r0
	ble .L_0802e352
	strb r1, [r2, #2]
.L_0802e352:
	movs r1, #1
.L_0802e354:
	cmp r1, #0
	beq .L_0802e376
	mov r1, r8
	lsls r3, r1, #3
	adds r2, r3, #4
	mov r0, r10
	ldrb r3, [r0, r2]
	cmp r3, #0
	bne .L_0802e368
	b .L_0802e062
.L_0802e368:
	adds r3, r0, r2
	movs r1, #2
	ldrsb r1, [r3, r1]
	mov r0, r8
	bl Func_0802e6b4
	b .L_0802e062
.L_0802e376:
	ldr r6, .L_0802e434
.L_0802e378:
	ldr r2, [r4, #12]
	movs r3, #1
	ands r2, r3
	cmp r2, #0
	beq .L_0802e3ea
	mov r1, r11
	cmp r1, #0
	bne .L_0802e3be
	mov r2, r8
	cmp r2, #1
	beq .L_0802e3ea
	lsls r2, r2, #3
	adds r3, r2, #4
	mov r0, r10
	ldrb r3, [r0, r3]
	cmp r3, #0
	bne .L_0802e39c
	b .L_0802e062
.L_0802e39c:
	adds r3, r0, r2
	movs r2, #2
	ldrsh r1, [r3, r2]
	mov r0, r8
	bl Func_0802e6fc
	mov r3, r8
	cmp r3, #0
	beq .L_0802e3b0
	b .L_0802e062
.L_0802e3b0:
	mov r2, r10
	movs r0, #10
	ldrsh r1, [r2, r0]
	movs r0, #1
	bl Func_0802e6fc
	b .L_0802e062
.L_0802e3be:
	mov r3, r8
	cmp r3, #0
	beq .L_0802e3ea
	lsls r3, r3, #3
	adds r1, r3, #4
	mov r0, r9
	ldrb r2, [r0, r1]
	movs r3, #1
	eors r2, r3
	strb r2, [r0, r1]
	cmp r2, #0
	beq .L_0802e3e0
	mov r2, r10
	adds r3, r2, r1
	movs r1, #1
	ldrsb r1, [r3, r1]
	b .L_0802e3e2
.L_0802e3e0:
	movs r1, #8
.L_0802e3e2:
	mov r0, r8
	bl Func_0802e6d8
	b .L_0802e062
.L_0802e3ea:
	ldr r2, [r4, #12]
	movs r3, #4
	ands r2, r3
	cmp r2, #0
	beq .L_0802e402
	bl Func_080144c0
	ldr r0, .L_0802e438
	ldr r1, .L_0802e43c
	bl Runtime_ConstantZeroResult
	b .L_0802df54
.L_0802e402:
	ldr r2, [r6, #4]
	movs r3, #2
	ands r2, r3
	cmp r2, #0
	beq .L_0802e426
	ldr r3, [sp, #8]
	ldr r2, .L_0802e440
	adds r3, #1
	str r3, [sp, #8]
	ldr r0, [sp, #8]
	movs r3, #7
	ands r0, r3
	str r0, [sp, #8]
	lsls r3, r0, #1
	ldrh r3, [r2, r3]
	movs r1, #160
	lsls r1, r1, #19
	strh r3, [r1]
.L_0802e426:
	movs r0, #1
	str r4, [sp, #0]
	bl WaitFrames
	ldr r4, [sp, #0]
	b .L_0802e06c
	.2byte 0x0000
.L_0802e434:
	.4byte gInput
.L_0802e438:
	.4byte 0x00000012
.L_0802e43c:
	.4byte Resource_Data012
.L_0802e440:
	.4byte Data_0802f054
