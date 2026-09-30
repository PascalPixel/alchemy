.syntax unified
	.thumb
	.global Func_080d9d40
	.thumb_func
Func_080d9d40:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #108]
	movs r0, #130
	adds r3, #160
	lsls r0, r0, #1
	mov r8, r1
	ldr r6, [r3]
	movs r7, #0
	bl GameFlag_Test
	cmp r0, #0
	bne .L_080d9e06
	movs r3, #194
	lsls r3, r3, #1
	add r3, r8
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #12
	ble .L_080d9e06
	ldr r2, .L_080d9e6c
	movs r1, #128
	lsls r1, r1, #2
	adds r1, #18
	adds r3, r2, r1
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_080d9df0
	adds r1, #2
	adds r3, r2, r1
	ldr r0, [r3]
	bl Func_080cd91c
	movs r2, #1
	adds r5, r6, #0
	negs r2, r2
	adds r5, #12
	cmp r0, r2
	beq .L_080d9e0a
	movs r1, #12
	ldrsh r3, [r6, r1]
	cmp r3, r0
	beq .L_080d9dc0
	movs r2, #2
	ldrsh r3, [r5, r2]
	cmp r3, r0
	beq .L_080d9dc4
	adds r2, r5, #0
.L_080d9da6:
	adds r7, #1
	adds r2, #28
	cmp r7, #7
	bgt .L_080d9e0a
	movs r1, #0
	ldrsh r3, [r2, r1]
	cmp r3, r0
	beq .L_080d9dc4
	movs r1, #2
	ldrsh r3, [r2, r1]
	cmp r3, r0
	bne .L_080d9da6
	b .L_080d9dc4
.L_080d9dc0:
	adds r5, r6, #0
	adds r5, #12
.L_080d9dc4:
	cmp r7, #7
	bgt .L_080d9e0a
	lsls r3, r7, #3
	subs r3, r3, r7
	lsls r3, r3, #2
	adds r3, #12
	ldrsh r0, [r6, r3]
	adds r3, r6, r3
	movs r2, #2
	ldrsh r1, [r3, r2]
	bl Func_080d99b8
	cmp r0, #0
	beq .L_080d9e0a
	movs r2, #181
	movs r3, #252
	lsls r2, r2, #1
	lsls r3, r3, #8
	add r2, r8
	adds r3, #131
	strh r3, [r2]
	b .L_080d9e0a
.L_080d9df0:
	cmp r3, #4
	bne .L_080d9e06
	movs r2, #181
	movs r3, #252
	lsls r2, r2, #1
	lsls r3, r3, #8
	add r2, r8
	adds r3, #132
	adds r5, r6, #0
	strh r3, [r2]
	b .L_080d9e08
.L_080d9e06:
	adds r5, r6, #0
.L_080d9e08:
	adds r5, #12
.L_080d9e0a:
	ldr r3, .L_080d9e70
	movs r7, #0
	mov r8, r3
.L_080d9e10:
	ldr r3, [r5, #20]
	cmp r3, #0
	beq .L_080d9e5c
	ldr r0, [r5, #16]
	bl Trig_Sin
	ldr r1, [r5, #8]
	mov lr, r8
	.2byte 0xf800
	ldr r3, [r5, #16]
	movs r1, #128
	lsls r1, r1, #5
	adds r3, r3, r1
	movs r1, #243
	lsls r1, r1, #8
	adds r6, r0, #0
	str r3, [r5, #16]
	ldr r0, [r5, #8]
	adds r1, #51
	mov lr, r8
	.2byte 0xf800
	movs r2, #200
	lsls r2, r2, #5
	adds r2, #152
	str r0, [r5, #8]
	cmp r0, r2
	bgt .L_080d9e4c
	movs r3, #0
	str r3, [r5, #8]
	str r3, [r5, #20]
.L_080d9e4c:
	movs r3, #0
	ldrsh r1, [r5, r3]
	movs r3, #2
	ldrsh r2, [r5, r3]
	adds r0, r7, #0
	adds r3, r6, #0
	bl Func_080da060
.L_080d9e5c:
	adds r7, #1
	adds r5, #28
	cmp r7, #7
	ble .L_080d9e10
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080d9e6c:
	.4byte gPartyState
.L_080d9e70:
	.4byte IwramMulQ16
