.syntax unified
	.thumb
	.global Func_080e807c
	.thumb_func
Func_080e807c:
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
	ldr r3, [r3, #108]
	movs r2, #133
	mov r11, r3
	ldr r3, .L_080e8130
	lsls r2, r2, #2
	adds r3, r3, r2
	mov r10, r0
	ldr r0, [r3]
	sub sp, #16
	bl Object_GetById
	ldr r3, [r0, #8]
	add r7, sp, #4
	str r3, [r7]
	mov r9, r0
	ldr r3, [r0, #12]
	movs r0, #128
	lsls r0, r0, #13
	adds r3, r3, r0
	str r3, [r7, #4]
	mov r2, r9
	ldr r3, [r2, #16]
	str r3, [r7, #8]
	movs r3, #164
	lsls r3, r3, #6
	adds r3, #133
	add r3, r10
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_080e80ea
	movs r3, #192
	lsls r3, r3, #4
	adds r3, #174
	add r3, r11
	movs r2, #0
	ldrsh r1, [r3, r2]
	movs r0, #128
	lsls r1, r1, #11
	lsls r0, r0, #10
	negs r1, r1
	adds r2, r7, #0
	bl Vector_AddPolarOffset
.L_080e80ea:
	adds r0, r7, #0
	bl Camera_WorldToScreen
	movs r3, #2
	ldrsh r0, [r7, r3]
	ldr r3, .L_080e812c
	ldr r4, .L_080e8134
	subs r2, r3, r0
	strh r2, [r4]
	movs r2, #10
	ldrsh r1, [r7, r2]
	subs r0, #64
	subs r3, r3, r1
	strh r3, [r4, #2]
	subs r1, #64
	bl Func_080e7fec
	movs r3, #164
	lsls r3, r3, #6
	adds r3, #134
	add r3, r10
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_080e8198
	movs r6, #164
	lsls r6, r6, #6
	movs r3, #0
	adds r6, #136
	mov r8, r3
	add r6, r10
	b .L_080e8138
.L_080e812c:
	.4byte 0x00000040
.L_080e8130:
	.4byte gPartyState
.L_080e8134:
	.4byte Data_03001120
.L_080e8138:
	ldrh r0, [r6]
	lsrs r0, r0, #1
	add r0, r8
	lsls r0, r0, #11
	bl Trig_Sin
	lsls r5, r0, #3
	ldrh r0, [r6]
	movs r2, #144
	lsrs r0, r0, #2
	add r0, r8
	lsls r2, r2, #7
	lsls r0, r0, #11
	adds r0, r0, r2
	bl Trig_Sin
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #1
	asrs r5, r5, #16
	asrs r3, r3, #16
	adds r5, #23
	adds r3, #12
	lsls r3, r3, #10
	lsls r5, r5, #5
	orrs r3, r5
	movs r0, #28
	orrs r3, r0
	ldr r0, .L_080e8250
	mov r2, r8
	lsls r1, r2, #1
	adds r2, r1, r0
	strh r3, [r2]
	ldr r2, .L_080e8254
	adds r1, r1, r2
	strh r3, [r1]
	movs r3, #1
	add r8, r3
	mov r0, r8
	cmp r0, #15
	ble .L_080e8138
	movs r2, #164
	lsls r2, r2, #6
	adds r2, #136
	add r2, r10
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_080e8198:
	movs r3, #164
	lsls r3, r3, #6
	adds r3, #132
	add r3, r10
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_080e8276
	movs r2, #164
	movs r6, #152
	movs r4, #132
	lsls r2, r2, #6
	lsls r6, r6, #6
	lsls r4, r4, #6
	adds r2, #130
	movs r3, #15
	add r6, r10
	add r4, r10
	mov r8, r3
	add r10, r2
.L_080e81c2:
	ldr r5, [r6, #24]
	cmp r5, #0
	bne .L_080e8200
	str r4, [sp, #0]
	bl Random16
	ldr r4, [sp, #0]
	adds r2, r7, #0
	strh r0, [r4, #28]
	str r5, [r7, #8]
	str r5, [r7, #4]
	str r5, [r7]
	movs r0, #128
	ldrh r1, [r4, #28]
	lsls r0, r0, #6
	adds r1, r1, r0
	movs r0, #216
	lsls r0, r0, #14
	bl Vector_AddPolarOffset
	ldr r3, [r7]
	movs r2, #128
	str r3, [r6]
	ldr r3, [r7, #4]
	lsls r2, r2, #13
	adds r3, r3, r2
	str r3, [r6, #4]
	ldr r3, [r7, #8]
	ldr r5, [r6, #24]
	str r3, [r6, #8]
	ldr r4, [sp, #0]
.L_080e8200:
	cmp r5, #0
	blt .L_080e825a
	asrs r2, r5, #2
	movs r3, #3
	ands r2, r3
	mov r3, r10
	ldrh r1, [r3]
	ldr r3, .L_080e8248
	lsls r2, r2, #2
	adds r1, r1, r2
	ands r1, r3
	ldr r2, .L_080e824c
	ldrh r3, [r4, #8]
	mov r0, r9
	ands r3, r2
	orrs r3, r1
	strh r3, [r4, #8]
	adds r1, r7, #0
	ldr r3, [r0, #8]
	ldr r2, [r6]
	str r4, [sp, #0]
	adds r3, r3, r2
	str r3, [r7]
	ldr r3, [r0, #12]
	ldr r2, [r6, #4]
	adds r3, r3, r2
	str r3, [r7, #4]
	ldr r3, [r0, #16]
	ldr r2, [r6, #8]
	adds r0, r4, #0
	adds r3, r3, r2
	str r3, [r7, #8]
	bl Func_080eb298
	ldr r5, [r6, #24]
	b .L_080e8258
.L_080e8248:
	.4byte 0x000003ff
.L_080e824c:
	.4byte 0xfffffc00
.L_080e8250:
	.4byte 0x050001e0
.L_080e8254:
	.4byte 0x050003e0
.L_080e8258:
	ldr r4, [sp, #0]
.L_080e825a:
	adds r3, r5, #1
	str r3, [r6, #24]
	cmp r3, #16
	bne .L_080e8266
	movs r3, #0
	str r3, [r6, #24]
.L_080e8266:
	movs r2, #1
	negs r2, r2
	add r8, r2
	mov r3, r8
	adds r4, #40
	adds r6, #28
	cmp r3, #0
	bge .L_080e81c2
.L_080e8276:
	movs r3, #192
	lsls r3, r3, #4
	adds r3, #173
	add r3, r11
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_080e829e
	movs r2, #192
	lsls r2, r2, #4
	adds r2, #174
	add r2, r11
	movs r0, #0
	ldrsh r3, [r2, r0]
	ldrh r1, [r2]
	cmp r3, #0
	beq .L_080e82ae
	subs r3, r1, #1
	strh r3, [r2]
.L_080e829e:
	movs r3, #192
	lsls r3, r3, #4
	adds r3, #174
	add r3, r11
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_080e82bc
.L_080e82ae:
	movs r2, #179
	movs r3, #128
	lsls r2, r2, #1
	lsls r3, r3, #6
	add r2, r11
	adds r3, #155
	strh r3, [r2]
.L_080e82bc:
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
