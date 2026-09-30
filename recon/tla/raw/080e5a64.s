.syntax unified
	.thumb
	.global Func_080e5a64
	.thumb_func
Func_080e5a64:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #92]
	adds r3, #240
	ldr r3, [r3]
	movs r2, #192
	lsls r2, r2, #5
	mov r8, r0
	adds r2, #177
	mov r10, r3
	add r2, r8
	movs r3, #0
	strb r3, [r2]
	movs r3, #192
	lsls r3, r3, #5
	adds r3, #172
	add r3, r8
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #1
	bne .L_080e5af8
	movs r2, #144
	movs r3, #248
	mov r6, r10
	lsls r2, r2, #12
	lsls r3, r3, #7
	movs r7, #0
	adds r6, #160
	mov r9, r2
	mov r11, r3
.L_080e5aae:
	ldr r0, [r6]
	lsrs r0, r0, #1
	adds r0, r0, r7
	lsls r0, r0, #16
	add r0, r9
	lsrs r0, r0, #5
	bl Trig_Sin
	lsls r5, r0, #1
	adds r5, r5, r0
	ldr r0, [r6]
	lsls r5, r5, #2
	lsrs r0, r0, #1
	adds r0, r0, r7
	lsls r0, r0, #16
	add r0, r9
	lsrs r0, r0, #5
	bl Trig_Sin
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #2
	asrs r5, r5, #16
	asrs r3, r3, #16
	adds r5, #16
	adds r3, #16
	lsls r3, r3, #5
	ldr r1, .L_080e5ce4
	mov r0, r11
	orrs r3, r0
	lsls r2, r7, #1
	orrs r3, r5
	adds r2, r2, r1
	strh r3, [r2]
	adds r7, #1
	cmp r7, #31
	ble .L_080e5aae
.L_080e5af8:
	movs r3, #192
	lsls r3, r3, #5
	adds r3, #176
	add r3, r8
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_080e5b46
	movs r5, #128
	movs r6, #156
	lsls r5, r5, #5
	lsls r6, r6, #5
	add r5, r8
	add r6, r8
	movs r7, #11
.L_080e5b18:
	ldr r3, [r5, #24]
	cmp r3, #15
	bhi .L_080e5b38
	ldr r3, [r5]
	adds r0, r6, #0
	str r3, [r6, #12]
	ldr r3, [r5, #4]
	str r3, [r6, #16]
	bl Func_080eb01c
	adds r0, r5, #0
	movs r1, #64
	movs r2, #0
	bl BattleFx_IntegrateVector2
	ldr r3, [r5, #24]
.L_080e5b38:
	adds r3, #1
	subs r7, #1
	str r3, [r5, #24]
	adds r6, #40
	adds r5, #28
	cmp r7, #0
	bge .L_080e5b18
.L_080e5b46:
	movs r3, #192
	lsls r3, r3, #5
	adds r3, #172
	add r3, r8
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #4
	bls .L_080e5b58
	b .L_080e5cc8
.L_080e5b58:
	ldr r2, .L_080e5ce8
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_080e5b60:
	.4byte .L_080e5b74
	.4byte .L_080e5be0
	.4byte .L_080e5c24
	.4byte .L_080e5c54
	.4byte .L_080e5c94
.L_080e5b74:
	movs r3, #192
	lsls r3, r3, #5
	adds r3, #168
	add r3, r8
	ldr r2, [r3]
	ldr r0, .L_080e5cec
	ldr r3, [r2, #12]
	movs r5, #192
	adds r3, r3, r0
	str r3, [r2, #12]
	lsls r5, r5, #5
	adds r5, #174
	add r5, r8
	movs r1, #0
	ldrsh r3, [r5, r1]
	cmp r3, #8
	beq .L_080e5b98
	b .L_080e5cc8
.L_080e5b98:
	mov r3, r10
	movs r2, #128
	lsls r2, r2, #6
	adds r3, #176
	movs r1, #166
	str r2, [r3]
	lsls r1, r1, #9
	adds r3, #4
	str r2, [r3]
	adds r1, #204
	movs r2, #128
	lsls r2, r2, #9
	adds r0, r1, #0
	bl Func_08020228
	movs r0, #156
	lsls r0, r0, #1
	adds r0, #255
	bl Audio_PlayCue
	movs r3, #192
	lsls r3, r3, #5
	adds r3, #176
	movs r2, #1
	add r3, r8
	strb r2, [r3]
	movs r3, #192
	lsls r3, r3, #5
	adds r3, #177
	add r3, r8
	strb r2, [r3]
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #8
	bne .L_080e5cc8
	b .L_080e5c7c
.L_080e5be0:
	mov r2, r10
	adds r2, #176
	ldr r3, [r2]
	movs r0, #128
	lsls r0, r0, #5
	mov r1, r10
	adds r3, r3, r0
	adds r1, #180
	str r3, [r2]
	str r3, [r1]
	movs r0, #204
	ldr r3, [r2]
	lsls r0, r0, #8
	adds r0, #203
	cmp r3, r0
	ble .L_080e5cc8
	movs r3, #204
	lsls r3, r3, #8
	adds r3, #204
	str r3, [r2]
	movs r2, #192
	str r3, [r1]
	lsls r2, r2, #5
	adds r2, #172
	add r2, r8
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r2, #192
	lsls r2, r2, #5
	movs r3, #255
	adds r2, #174
	lsls r3, r3, #8
	b .L_080e5cc2
.L_080e5c24:
	movs r1, #192
	lsls r1, r1, #5
	mov r2, r10
	adds r1, #174
	adds r2, #193
	movs r3, #3
	add r1, r8
	strb r3, [r2]
	movs r2, #0
	ldrsh r3, [r1, r2]
	cmp r3, #32
	bne .L_080e5cc8
	movs r3, #192
	lsls r3, r3, #5
	adds r3, #172
	add r3, r8
	ldrh r2, [r3]
	adds r2, #1
	strh r2, [r3]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r1]
	b .L_080e5cc8
.L_080e5c54:
	movs r5, #192
	lsls r5, r5, #5
	adds r5, #174
	add r5, r8
	movs r0, #0
	ldrsh r3, [r5, r0]
	ldrh r2, [r5]
	cmp r3, #0
	bne .L_080e5c72
	movs r2, #1
	movs r0, #1
	movs r1, #1
	bl Func_08020228
	ldrh r2, [r5]
.L_080e5c72:
	movs r1, #160
	lsls r3, r2, #16
	lsls r1, r1, #12
	cmp r3, r1
	bne .L_080e5cc8
.L_080e5c7c:
	movs r3, #192
	lsls r3, r3, #5
	adds r3, #172
	add r3, r8
	ldrh r2, [r3]
	adds r2, #1
	strh r2, [r3]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r5]
	b .L_080e5cc8
.L_080e5c94:
	movs r3, #192
	lsls r3, r3, #5
	adds r3, #168
	add r3, r8
	ldr r2, [r3]
	movs r0, #192
	ldr r3, [r2, #12]
	lsls r0, r0, #9
	adds r3, r3, r0
	str r3, [r2, #12]
	movs r3, #192
	lsls r3, r3, #5
	adds r3, #174
	add r3, r8
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #13
	bne .L_080e5cc8
	movs r2, #192
	lsls r2, r2, #5
	movs r3, #186
	adds r2, #172
	lsls r3, r3, #2
.L_080e5cc2:
	add r2, r8
	adds r3, #255
	strh r3, [r2]
.L_080e5cc8:
	movs r2, #192
	lsls r2, r2, #5
	adds r2, #174
	add r2, r8
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080e5ce4:
	.4byte 0x050003c0
.L_080e5ce8:
	.4byte .L_080e5b60
.L_080e5cec:
	.4byte 0xfffc8000
