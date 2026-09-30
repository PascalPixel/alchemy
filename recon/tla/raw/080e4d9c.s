.syntax unified
	.thumb
	.global Func_080e4d9c
	.thumb_func
Func_080e4d9c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r2, r3, #0
	ldr r7, [r3, #92]
	adds r2, #224
	adds r3, #240
	ldr r2, [r2]
	ldr r3, [r3]
	movs r0, #168
	lsls r0, r0, #5
	adds r0, #4
	mov r10, r2
	mov r11, r3
	adds r2, r7, r0
	movs r3, #0
	movs r1, #168
	strh r3, [r2]
	lsls r1, r1, #5
	adds r1, #12
	adds r3, r7, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	sub sp, #16
	cmp r3, #1
	bne .L_080e4e28
	movs r3, #248
	ldr r5, .L_080e50c8
	mov r6, r11
	movs r2, #31
	lsls r3, r3, #7
	movs r4, #0
	adds r6, #160
	mov r9, r2
	mov r8, r3
.L_080e4df0:
	ldr r0, [r6]
	movs r1, #144
	lsrs r0, r0, #1
	adds r0, r0, r4
	lsls r1, r1, #12
	lsls r0, r0, #16
	adds r0, r0, r1
	lsrs r0, r0, #5
	str r4, [sp, #0]
	bl Trig_Sin
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #2
	asrs r3, r3, #16
	adds r3, #16
	lsls r3, r3, #5
	mov r2, r8
	orrs r3, r2
	mov r0, r9
	orrs r3, r0
	strh r3, [r5]
	adds r5, #2
	ldr r4, [sp, #0]
	adds r4, #1
	cmp r4, #31
	ble .L_080e4df0
	b .L_080e4e50
.L_080e4e28:
	movs r1, #168
	lsls r1, r1, #5
	adds r1, #12
	adds r4, r7, r1
	movs r3, #0
	ldrsb r3, [r4, r3]
	cmp r3, #2
	bne .L_080e4e50
	movs r3, #128
	movs r2, #128
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_080e50cc
	ldr r1, .L_080e50d0
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r3, #0
	strb r3, [r4]
.L_080e4e50:
	mov r2, r10
	ldr r3, [r2, #4]
	add r5, sp, #4
	str r3, [r5]
	adds r0, r5, #0
	ldr r3, [r2, #8]
	str r3, [r5, #4]
	ldr r3, [r2, #12]
	str r3, [r5, #8]
	bl Camera_WorldToScreen
	movs r3, #168
	add r3, r11
	mov r9, r3
	movs r0, #2
	ldrsh r3, [r5, r0]
	mov r1, r9
	str r3, [r1]
	movs r2, #172
	movs r0, #10
	ldrsh r3, [r5, r0]
	add r2, r11
	str r3, [r2]
	mov r3, r10
	adds r3, #32
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	mov r8, r2
	cmp r3, #0
	beq .L_080e4e90
	b .L_080e5388
.L_080e4e90:
	movs r1, #128
	movs r2, #240
	lsls r1, r1, #3
	lsls r2, r2, #3
	adds r6, r7, r1
	adds r5, r7, r2
	movs r4, #31
.L_080e4e9e:
	ldr r3, [r6, #24]
	cmp r3, #29
	bhi .L_080e4ed2
	mov r3, r9
	ldr r2, [r3]
	ldr r3, [r6]
	lsls r2, r2, #16
	adds r0, r5, #0
	adds r3, r3, r2
	str r3, [r0, #12]
	mov r1, r8
	ldr r2, [r1]
	ldr r3, [r6, #4]
	lsls r2, r2, #16
	adds r3, r3, r2
	str r3, [r0, #16]
	str r4, [sp, #0]
	bl Func_080eb01c
	adds r0, r6, #0
	movs r1, #60
	movs r2, #0
	bl BattleFx_IntegrateVector2
	ldr r3, [r6, #24]
	ldr r4, [sp, #0]
.L_080e4ed2:
	adds r3, #1
	subs r4, #1
	str r3, [r6, #24]
	adds r5, #40
	adds r6, #28
	cmp r4, #0
	bge .L_080e4e9e
	movs r2, #168
	lsls r2, r2, #5
	adds r2, #10
	adds r3, r7, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	movs r4, #0
	cmp r4, r3
	bge .L_080e4f76
	movs r1, #168
	lsls r1, r1, #5
	adds r1, #8
	adds r1, r1, r7
	mov r8, r1
.L_080e4efc:
	mov r0, r8
	movs r3, #0
	ldrsh r2, [r0, r3]
	movs r1, #140
	lsls r3, r2, #3
	subs r3, r3, r2
	lsls r3, r3, #2
	lsls r1, r1, #5
	adds r3, r7, r3
	adds r6, r3, r1
	movs r3, #0
	str r3, [r6, #24]
	mov r2, r10
	ldr r3, [r2, #4]
	str r4, [sp, #0]
	str r3, [r6]
	ldr r3, [r2, #8]
	str r3, [r6, #4]
	ldr r3, [r2, #12]
	str r3, [r6, #8]
	bl Random16
	lsls r5, r0, #1
	adds r5, r5, r0
	movs r3, #128
	lsls r3, r3, #11
	lsls r5, r5, #2
	adds r5, r5, r3
	bl Random16
	adds r2, r6, #0
	adds r1, r0, #0
	adds r0, r5, #0
	bl Vector_AddPolarOffset
	mov r0, r8
	ldrh r3, [r0]
	mov r1, r8
	adds r3, #1
	strh r3, [r1]
	lsls r3, r3, #16
	asrs r2, r3, #16
	adds r3, r2, #0
	ldr r4, [sp, #0]
	cmp r2, #0
	bge .L_080e4f5a
	adds r3, #31
.L_080e4f5a:
	asrs r3, r3, #5
	lsls r3, r3, #5
	subs r3, r2, r3
	movs r0, #168
	mov r2, r8
	strh r3, [r2]
	lsls r0, r0, #5
	adds r0, #10
	adds r3, r7, r0
	movs r1, #0
	ldrsh r3, [r3, r1]
	adds r4, #1
	cmp r4, r3
	blt .L_080e4efc
.L_080e4f76:
	movs r3, #168
	lsls r3, r3, #5
	adds r3, #10
	adds r2, r7, r3
	movs r3, #0
	movs r0, #140
	strh r3, [r2]
	lsls r0, r0, #5
	movs r4, #0
	adds r6, r7, r0
.L_080e4f8a:
	ldr r2, [r6, #24]
	cmp r2, #71
	bls .L_080e4f92
	b .L_080e5140
.L_080e4f92:
	lsls r3, r4, #2
	adds r3, r3, r4
	lsls r3, r3, #3
	movs r1, #200
	adds r3, r7, r3
	lsls r1, r1, #4
	adds r0, r3, r1
	subs r3, r2, #6
	cmp r3, #56
	bls .L_080e4fa8
	b .L_080e5126
.L_080e4fa8:
	ldr r2, .L_080e50d4
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_080e4fb0:
	.4byte .L_080e5094
	.4byte .L_080e5126
	.4byte .L_080e5126
	.4byte .L_080e5126
	.4byte .L_080e5126
	.4byte .L_080e5126
	.4byte .L_080e50a4
	.4byte .L_080e5126
	.4byte .L_080e5126
	.4byte .L_080e5126
	.4byte .L_080e5126
	.4byte .L_080e5126
	.4byte .L_080e5126
	.4byte .L_080e5126
	.4byte .L_080e5126
	.4byte .L_080e5126
	.4byte .L_080e50b4
	.4byte .L_080e5126
	.4byte .L_080e5126
	.4byte .L_080e5126
	.4byte .L_080e5126
	.4byte .L_080e5126
	.4byte .L_080e5126
	.4byte .L_080e5126
	.4byte .L_080e5126
	.4byte .L_080e5126
	.4byte .L_080e50d8
	.4byte .L_080e5126
	.4byte .L_080e5126
	.4byte .L_080e5126
	.4byte .L_080e5126
	.4byte .L_080e5126
	.4byte .L_080e5126
	.4byte .L_080e5126
	.4byte .L_080e5126
	.4byte .L_080e5126
	.4byte .L_080e50e8
	.4byte .L_080e5126
	.4byte .L_080e5126
	.4byte .L_080e5126
	.4byte .L_080e5126
	.4byte .L_080e5126
	.4byte .L_080e5126
	.4byte .L_080e5126
	.4byte .L_080e5126
	.4byte .L_080e5126
	.4byte .L_080e50f8
	.4byte .L_080e5126
	.4byte .L_080e5126
	.4byte .L_080e5126
	.4byte .L_080e5126
	.4byte .L_080e5126
	.4byte .L_080e5126
	.4byte .L_080e5126
	.4byte .L_080e5126
	.4byte .L_080e5126
	.4byte .L_080e510c
.L_080e5094:
	movs r2, #168
	lsls r2, r2, #5
	adds r2, #6
	adds r3, r7, r2
	ldrh r2, [r3]
	ldr r3, .L_080e50c4
	adds r2, #4
	b .L_080e511a
.L_080e50a4:
	movs r1, #168
	lsls r1, r1, #5
	adds r1, #6
	adds r3, r7, r1
	ldrh r2, [r3]
	ldr r3, .L_080e50c4
	adds r2, #8
	b .L_080e511a
.L_080e50b4:
	movs r2, #168
	lsls r2, r2, #5
	adds r2, #6
	adds r3, r7, r2
	ldrh r2, [r3]
	ldr r3, .L_080e50c4
	adds r2, #12
	b .L_080e511a
.L_080e50c4:
	.4byte 0x000003ff
.L_080e50c8:
	.4byte 0x050003c0
.L_080e50cc:
	.4byte Data_080f0fa0
.L_080e50d0:
	.4byte 0x050003e0
.L_080e50d4:
	.4byte .L_080e4fb0
.L_080e50d8:
	movs r1, #168
	lsls r1, r1, #5
	adds r1, #6
	adds r3, r7, r1
	ldrh r2, [r3]
	ldr r3, .L_080e5108
	adds r2, #16
	b .L_080e511a
.L_080e50e8:
	movs r2, #168
	lsls r2, r2, #5
	adds r2, #6
	adds r3, r7, r2
	ldrh r2, [r3]
	ldr r3, .L_080e5108
	adds r2, #20
	b .L_080e511a
.L_080e50f8:
	movs r1, #168
	lsls r1, r1, #5
	adds r1, #6
	adds r3, r7, r1
	ldrh r2, [r3]
	ldr r3, .L_080e5108
	adds r2, #24
	b .L_080e511a
.L_080e5108:
	.4byte 0x000003ff
.L_080e510c:
	movs r2, #168
	lsls r2, r2, #5
	adds r2, #6
	adds r3, r7, r2
	ldrh r2, [r3]
	ldr r3, .L_080e5138
	adds r2, #28
.L_080e511a:
	ands r2, r3
	ldrh r1, [r0, #8]
	ldr r3, .L_080e513c
	ands r3, r1
	orrs r3, r2
	strh r3, [r0, #8]
.L_080e5126:
	adds r1, r6, #0
	str r4, [sp, #0]
	bl Func_080eb298
	ldr r3, [r6, #24]
	ldr r4, [sp, #0]
	adds r3, #1
	str r3, [r6, #24]
	b .L_080e5140
.L_080e5138:
	.4byte 0x000003ff
.L_080e513c:
	.4byte 0xfffffc00
.L_080e5140:
	adds r4, #1
	adds r6, #28
	cmp r4, #31
	bgt .L_080e514a
	b .L_080e4f8a
.L_080e514a:
	movs r0, #168
	lsls r0, r0, #5
	adds r3, r7, r0
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #6
	bls .L_080e515a
	b .L_080e5702
.L_080e515a:
	ldr r2, .L_080e5384
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_080e5164:
	.4byte .L_080e5180
	.4byte .L_080e51b2
	.4byte .L_080e51d4
	.4byte .L_080e51e8
	.4byte .L_080e526a
	.4byte .L_080e52ce
	.4byte .L_080e52e2
.L_080e5180:
	movs r2, #168
	lsls r2, r2, #5
	adds r2, #2
	adds r5, r7, r2
	movs r0, #0
	ldrsh r3, [r5, r0]
	ldrh r2, [r5]
	cmp r3, #0
	bne .L_080e519a
	movs r0, #140
	bl Audio_PlayCue
	ldrh r2, [r5]
.L_080e519a:
	movs r1, #160
	lsls r3, r2, #16
	lsls r1, r1, #12
	cmp r3, r1
	beq .L_080e51a6
	b .L_080e5702
.L_080e51a6:
	movs r2, #168
	lsls r2, r2, #5
	adds r3, r7, r2
	ldrh r2, [r3]
	adds r2, #1
	b .L_080e56f8
.L_080e51b2:
	mov r1, r11
	adds r1, #176
	ldr r3, [r1]
	movs r0, #192
	lsls r0, r0, #2
	mov r2, r11
	adds r3, r3, r0
	adds r2, #180
	str r3, [r1]
	str r3, [r2]
	ldr r3, [r1]
	movs r1, #128
	lsls r1, r1, #8
	cmp r3, r1
	bge .L_080e51d2
	b .L_080e5702
.L_080e51d2:
	b .L_080e5638
.L_080e51d4:
	movs r2, #168
	lsls r2, r2, #5
	adds r2, #2
	adds r1, r7, r2
	movs r0, #0
	ldrsh r3, [r1, r0]
	cmp r3, #40
	beq .L_080e51e6
	b .L_080e5702
.L_080e51e6:
	b .L_080e56b0
.L_080e51e8:
	movs r3, #168
	lsls r3, r3, #5
	adds r3, #2
	adds r1, r7, r3
	movs r0, #0
	ldrsh r3, [r1, r0]
	cmp r3, #0
	bne .L_080e5212
	movs r2, #168
	lsls r2, r2, #5
	adds r2, #12
	adds r3, r7, r2
	movs r2, #2
	strb r2, [r3]
	movs r0, #0
	ldrsh r3, [r1, r0]
	cmp r3, #0
	bne .L_080e5212
	movs r0, #144
	bl Audio_PlayCue
.L_080e5212:
	movs r1, #168
	lsls r1, r1, #5
	adds r1, #2
	adds r5, r7, r1
	ldrh r2, [r5]
	movs r3, #7
	ands r3, r2
	cmp r3, #0
	bne .L_080e524c
	movs r1, #192
	movs r2, #230
	lsls r1, r1, #10
	lsls r2, r2, #8
	adds r2, #102
	adds r0, r1, #0
	bl Func_08020228
	movs r3, #168
	lsls r3, r3, #5
	movs r0, #168
	adds r3, #10
	lsls r0, r0, #5
	adds r2, r7, r3
	adds r0, #4
	movs r3, #1
	strh r3, [r2]
	adds r2, r7, r0
	movs r3, #2
	strh r3, [r2]
.L_080e524c:
	mov r2, r11
	movs r3, #5
	adds r2, #193
	strb r3, [r2]
	movs r1, #0
	ldrsh r3, [r5, r1]
	cmp r3, #16
	beq .L_080e525e
	b .L_080e5702
.L_080e525e:
	movs r2, #168
	lsls r2, r2, #5
	adds r3, r7, r2
	ldrh r2, [r3]
	adds r2, #1
	b .L_080e56f8
.L_080e526a:
	movs r3, #168
	lsls r3, r3, #5
	adds r3, #2
	adds r5, r7, r3
	ldrh r2, [r5]
	movs r3, #15
	ands r3, r2
	cmp r3, #0
	bne .L_080e5284
	movs r0, #144
	bl Audio_PlayCue
	ldrh r2, [r5]
.L_080e5284:
	movs r3, #7
	ands r3, r2
	cmp r3, #0
	bne .L_080e52b6
	movs r1, #192
	movs r2, #230
	lsls r1, r1, #10
	lsls r2, r2, #8
	adds r0, r1, #0
	adds r2, #102
	bl Func_08020228
	movs r0, #168
	lsls r0, r0, #5
	movs r1, #168
	adds r0, #10
	lsls r1, r1, #5
	adds r2, r7, r0
	movs r3, #1
	adds r1, #4
	strh r3, [r2]
	adds r2, r7, r1
	movs r3, #2
	strh r3, [r2]
	ldrh r2, [r5]
.L_080e52b6:
	lsls r3, r2, #16
	movs r2, #128
	lsls r2, r2, #16
	cmp r3, r2
	beq .L_080e52c2
	b .L_080e5702
.L_080e52c2:
	movs r0, #168
	lsls r0, r0, #5
	adds r3, r7, r0
	ldrh r2, [r3]
	adds r2, #1
	b .L_080e56f8
.L_080e52ce:
	movs r2, #168
	lsls r2, r2, #5
	adds r2, #2
	adds r1, r7, r2
	movs r0, #0
	ldrsh r3, [r1, r0]
	cmp r3, #35
	beq .L_080e52e0
	b .L_080e5702
.L_080e52e0:
	b .L_080e56b0
.L_080e52e2:
	movs r3, #168
	lsls r3, r3, #5
	adds r3, #2
	adds r5, r7, r3
	movs r0, #0
	ldrsh r3, [r5, r0]
	ldrh r2, [r5]
	cmp r3, #0
	bne .L_080e5302
	movs r1, #168
	lsls r1, r1, #5
	adds r1, #10
	adds r2, r7, r1
	movs r3, #10
	strh r3, [r2]
	ldrh r2, [r5]
.L_080e5302:
	movs r0, #128
	lsls r3, r2, #16
	lsls r0, r0, #11
	cmp r3, r0
	bne .L_080e531a
	movs r1, #168
	lsls r1, r1, #5
	adds r1, #10
	adds r2, r7, r1
	movs r3, #10
	strh r3, [r2]
	ldrh r2, [r5]
.L_080e531a:
	lsls r3, r2, #16
	cmp r3, #0
	bne .L_080e534a
	movs r1, #192
	movs r2, #230
	lsls r1, r1, #12
	lsls r2, r2, #8
	adds r2, #102
	adds r0, r1, #0
	bl Func_08020228
	movs r0, #145
	bl Audio_PlayCue
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #0
	bne .L_080e534a
	movs r3, #168
	lsls r3, r3, #5
	adds r3, #4
	adds r2, r7, r3
	movs r3, #3
	strh r3, [r2]
.L_080e534a:
	movs r0, #168
	lsls r0, r0, #5
	adds r0, #2
	adds r1, r7, r0
	movs r0, #0
	ldrsh r3, [r1, r0]
	ldrh r2, [r1]
	cmp r3, #20
	bne .L_080e536a
	movs r3, #168
	lsls r3, r3, #5
	adds r3, #4
	adds r2, r7, r3
	movs r3, #1
	strh r3, [r2]
	ldrh r2, [r1]
.L_080e536a:
	movs r0, #144
	lsls r3, r2, #16
	lsls r0, r0, #15
	cmp r3, r0
	beq .L_080e5376
	b .L_080e5702
.L_080e5376:
	movs r2, #168
	lsls r2, r2, #5
	adds r3, r7, r2
	movs r2, #186
	lsls r2, r2, #2
	adds r2, #255
	b .L_080e56b8
.L_080e5384:
	.4byte .L_080e5164
.L_080e5388:
	movs r3, #128
	movs r0, #240
	lsls r3, r3, #3
	lsls r0, r0, #3
	adds r6, r7, r3
	adds r5, r7, r0
	movs r4, #11
.L_080e5396:
	ldr r3, [r6, #24]
	cmp r3, #29
	bhi .L_080e53ca
	mov r1, r9
	ldr r2, [r1]
	ldr r3, [r6]
	lsls r2, r2, #16
	adds r0, r5, #0
	adds r3, r3, r2
	str r3, [r0, #12]
	mov r3, r8
	ldr r2, [r3]
	ldr r3, [r6, #4]
	lsls r2, r2, #16
	adds r3, r3, r2
	str r3, [r0, #16]
	str r4, [sp, #0]
	bl Func_080eb01c
	adds r0, r6, #0
	movs r1, #60
	movs r2, #0
	bl BattleFx_IntegrateVector2
	ldr r3, [r6, #24]
	ldr r4, [sp, #0]
.L_080e53ca:
	adds r3, #1
	subs r4, #1
	str r3, [r6, #24]
	adds r5, #40
	adds r6, #28
	cmp r4, #0
	bge .L_080e5396
	movs r0, #168
	lsls r0, r0, #5
	adds r0, #10
	adds r2, r7, r0
	movs r1, #0
	ldrsh r3, [r2, r1]
	movs r4, #0
	cmp r4, r3
	bge .L_080e543c
	movs r3, #168
	lsls r3, r3, #5
	adds r3, #8
	adds r1, r7, r3
	adds r0, r2, #0
.L_080e53f4:
	movs r3, #0
	ldrsh r2, [r1, r3]
	lsls r3, r2, #3
	subs r3, r3, r2
	lsls r3, r3, #2
	movs r2, #140
	adds r3, r7, r3
	lsls r2, r2, #5
	adds r6, r3, r2
	movs r3, #0
	str r3, [r6, #24]
	mov r2, r10
	ldr r3, [r2, #4]
	str r3, [r6]
	ldr r3, [r2, #8]
	str r3, [r6, #4]
	ldr r3, [r2, #12]
	str r3, [r6, #8]
	ldrh r3, [r1]
	adds r3, #1
	strh r3, [r1]
	lsls r3, r3, #16
	asrs r2, r3, #16
	adds r3, r2, #0
	cmp r2, #0
	bge .L_080e542a
	adds r3, #31
.L_080e542a:
	asrs r3, r3, #5
	lsls r3, r3, #5
	subs r3, r2, r3
	strh r3, [r1]
	adds r4, #1
	movs r2, #0
	ldrsh r3, [r0, r2]
	cmp r4, r3
	blt .L_080e53f4
.L_080e543c:
	movs r3, #168
	lsls r3, r3, #5
	adds r3, #10
	adds r2, r7, r3
	movs r3, #0
	movs r0, #140
	strh r3, [r2]
	lsls r0, r0, #5
	movs r4, #0
	adds r6, r7, r0
.L_080e5450:
	ldr r2, [r6, #24]
	cmp r2, #47
	bls .L_080e5458
	b .L_080e55b0
.L_080e5458:
	lsls r3, r4, #2
	adds r3, r3, r4
	lsls r3, r3, #3
	movs r1, #200
	adds r3, r7, r3
	lsls r1, r1, #4
	adds r0, r3, r1
	subs r3, r2, #4
	cmp r3, #37
	bls .L_080e546e
	b .L_080e5596
.L_080e546e:
	ldr r2, .L_080e5544
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_080e5478:
	.4byte .L_080e5510
	.4byte .L_080e5596
	.4byte .L_080e5596
	.4byte .L_080e5596
	.4byte .L_080e5520
	.4byte .L_080e5596
	.4byte .L_080e5596
	.4byte .L_080e5596
	.4byte .L_080e5596
	.4byte .L_080e5596
	.4byte .L_080e5530
	.4byte .L_080e5596
	.4byte .L_080e5596
	.4byte .L_080e5596
	.4byte .L_080e5596
	.4byte .L_080e5596
	.4byte .L_080e5596
	.4byte .L_080e5596
	.4byte .L_080e5548
	.4byte .L_080e5596
	.4byte .L_080e5596
	.4byte .L_080e5596
	.4byte .L_080e5596
	.4byte .L_080e5596
	.4byte .L_080e5558
	.4byte .L_080e5596
	.4byte .L_080e5596
	.4byte .L_080e5596
	.4byte .L_080e5596
	.4byte .L_080e5596
	.4byte .L_080e5596
	.4byte .L_080e5568
	.4byte .L_080e5596
	.4byte .L_080e5596
	.4byte .L_080e5596
	.4byte .L_080e5596
	.4byte .L_080e5596
	.4byte .L_080e557c
.L_080e5510:
	movs r2, #168
	lsls r2, r2, #5
	adds r2, #6
	adds r3, r7, r2
	ldrh r2, [r3]
	ldr r3, .L_080e5540
	adds r2, #4
	b .L_080e558a
.L_080e5520:
	movs r1, #168
	lsls r1, r1, #5
	adds r1, #6
	adds r3, r7, r1
	ldrh r2, [r3]
	ldr r3, .L_080e5540
	adds r2, #8
	b .L_080e558a
.L_080e5530:
	movs r2, #168
	lsls r2, r2, #5
	adds r2, #6
	adds r3, r7, r2
	ldrh r2, [r3]
	ldr r3, .L_080e5540
	adds r2, #12
	b .L_080e558a
.L_080e5540:
	.4byte 0x000003ff
.L_080e5544:
	.4byte .L_080e5478
.L_080e5548:
	movs r1, #168
	lsls r1, r1, #5
	adds r1, #6
	adds r3, r7, r1
	ldrh r2, [r3]
	ldr r3, .L_080e5578
	adds r2, #16
	b .L_080e558a
.L_080e5558:
	movs r2, #168
	lsls r2, r2, #5
	adds r2, #6
	adds r3, r7, r2
	ldrh r2, [r3]
	ldr r3, .L_080e5578
	adds r2, #20
	b .L_080e558a
.L_080e5568:
	movs r1, #168
	lsls r1, r1, #5
	adds r1, #6
	adds r3, r7, r1
	ldrh r2, [r3]
	ldr r3, .L_080e5578
	adds r2, #24
	b .L_080e558a
.L_080e5578:
	.4byte 0x000003ff
.L_080e557c:
	movs r2, #168
	lsls r2, r2, #5
	adds r2, #6
	adds r3, r7, r2
	ldrh r2, [r3]
	ldr r3, .L_080e55a8
	adds r2, #28
.L_080e558a:
	ands r2, r3
	ldrh r1, [r0, #8]
	ldr r3, .L_080e55ac
	ands r3, r1
	orrs r3, r2
	strh r3, [r0, #8]
.L_080e5596:
	adds r1, r6, #0
	str r4, [sp, #0]
	bl Func_080eb298
	ldr r3, [r6, #24]
	ldr r4, [sp, #0]
	adds r3, #1
	str r3, [r6, #24]
	b .L_080e55b0
.L_080e55a8:
	.4byte 0x000003ff
.L_080e55ac:
	.4byte 0xfffffc00
.L_080e55b0:
	adds r4, #1
	adds r6, #28
	cmp r4, #31
	bgt .L_080e55ba
	b .L_080e5450
.L_080e55ba:
	movs r0, #168
	lsls r0, r0, #5
	adds r3, r7, r0
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #4
	bls .L_080e55ca
	b .L_080e5702
.L_080e55ca:
	ldr r2, .L_080e5720
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_080e55d4:
	.4byte .L_080e55e8
	.4byte .L_080e5618
	.4byte .L_080e56a0
	.4byte .L_080e5652
	.4byte .L_080e56c4
.L_080e55e8:
	movs r2, #168
	lsls r2, r2, #5
	adds r2, #2
	adds r5, r7, r2
	movs r0, #0
	ldrsh r3, [r5, r0]
	ldrh r2, [r5]
	cmp r3, #0
	bne .L_080e5602
	movs r0, #140
	bl Audio_PlayCue
	ldrh r2, [r5]
.L_080e5602:
	movs r1, #160
	lsls r3, r2, #16
	lsls r1, r1, #12
	cmp r3, r1
	bne .L_080e5702
	movs r2, #168
	lsls r2, r2, #5
	adds r3, r7, r2
	ldrh r2, [r3]
	adds r2, #1
	b .L_080e56f8
.L_080e5618:
	mov r1, r11
	adds r1, #176
	ldr r3, [r1]
	movs r0, #160
	lsls r0, r0, #3
	mov r2, r11
	adds r3, r3, r0
	adds r2, #180
	str r3, [r1]
	str r3, [r2]
	ldr r3, [r1]
	movs r1, #152
	lsls r1, r1, #7
	adds r1, #203
	cmp r3, r1
	ble .L_080e5702
.L_080e5638:
	movs r3, #168
	lsls r3, r3, #5
	adds r2, r7, r3
	ldrh r3, [r2]
	movs r0, #168
	lsls r0, r0, #5
	adds r3, #1
	adds r0, #2
	strh r3, [r2]
	movs r1, #0
	adds r3, r7, r0
	strh r1, [r3]
	b .L_080e5702
.L_080e5652:
	movs r3, #168
	lsls r3, r3, #5
	adds r3, #2
	adds r1, r7, r3
	movs r0, #0
	ldrsh r3, [r1, r0]
	cmp r3, #0
	bne .L_080e5698
	movs r2, #168
	lsls r2, r2, #5
	adds r2, #12
	adds r3, r7, r2
	movs r2, #2
	strb r2, [r3]
	movs r0, #0
	ldrsh r3, [r1, r0]
	cmp r3, #0
	bne .L_080e5698
	movs r1, #192
	movs r2, #230
	lsls r1, r1, #10
	lsls r2, r2, #8
	adds r0, r1, #0
	adds r2, #102
	bl Func_08020228
	movs r1, #168
	lsls r1, r1, #5
	adds r1, #10
	adds r2, r7, r1
	movs r3, #1
	strh r3, [r2]
	movs r0, #144
	bl Audio_PlayCue
.L_080e5698:
	mov r2, r11
	adds r2, #193
	movs r3, #5
	strb r3, [r2]
.L_080e56a0:
	movs r2, #168
	lsls r2, r2, #5
	adds r2, #2
	adds r1, r7, r2
	movs r0, #0
	ldrsh r3, [r1, r0]
	cmp r3, #16
	bne .L_080e5702
.L_080e56b0:
	subs r2, #2
	adds r3, r7, r2
	ldrh r2, [r3]
	adds r2, #1
.L_080e56b8:
	strh r2, [r3]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r1]
	b .L_080e5702
.L_080e56c4:
	movs r3, #168
	lsls r3, r3, #5
	adds r3, #2
	adds r5, r7, r3
	movs r0, #0
	ldrsh r3, [r5, r0]
	ldrh r2, [r5]
	cmp r3, #5
	bne .L_080e56e2
	mov r1, r10
	ldr r0, [r1, #16]
	movs r1, #0
	bl Func_080e1420
	ldrh r2, [r5]
.L_080e56e2:
	lsls r3, r2, #16
	movs r2, #160
	lsls r2, r2, #13
	cmp r3, r2
	bne .L_080e5702
	movs r0, #168
	movs r2, #186
	lsls r0, r0, #5
	lsls r2, r2, #2
	adds r3, r7, r0
	adds r2, #255
.L_080e56f8:
	strh r2, [r3]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r5]
.L_080e5702:
	movs r1, #168
	lsls r1, r1, #5
	adds r1, #2
	adds r2, r7, r1
	ldrh r3, [r2]
	add sp, #16
	adds r3, #1
	strh r3, [r2]
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080e5720:
	.4byte .L_080e55d4
