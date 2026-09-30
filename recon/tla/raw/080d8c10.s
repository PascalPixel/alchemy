.syntax unified
	.thumb
	.global Func_080d8c10
	.thumb_func
Func_080d8c10:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #108]
	movs r0, #130
	adds r3, #156
	lsls r0, r0, #1
	mov r8, r1
	ldr r6, [r3]
	movs r7, #0
	bl GameFlag_Test
	cmp r0, #0
	bne .L_080d8cd2
	movs r3, #194
	lsls r3, r3, #1
	add r3, r8
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #12
	ble .L_080d8cd2
	ldr r2, .L_080d8d38
	movs r1, #128
	lsls r1, r1, #2
	adds r1, #18
	adds r3, r2, r1
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_080d8cbc
	adds r1, #2
	adds r3, r2, r1
	ldr r0, [r3]
	bl Func_080cd91c
	movs r2, #1
	adds r5, r6, #0
	negs r2, r2
	adds r5, #12
	cmp r0, r2
	beq .L_080d8cd6
	movs r1, #12
	ldrsh r3, [r6, r1]
	cmp r3, r0
	beq .L_080d8c90
	movs r2, #2
	ldrsh r3, [r5, r2]
	cmp r3, r0
	beq .L_080d8c94
	adds r2, r5, #0
.L_080d8c76:
	adds r7, #1
	adds r2, #32
	cmp r7, #7
	bgt .L_080d8cd6
	movs r1, #0
	ldrsh r3, [r2, r1]
	cmp r3, r0
	beq .L_080d8c94
	movs r1, #2
	ldrsh r3, [r2, r1]
	cmp r3, r0
	bne .L_080d8c76
	b .L_080d8c94
.L_080d8c90:
	adds r5, r6, #0
	adds r5, #12
.L_080d8c94:
	cmp r7, #7
	bgt .L_080d8cd6
	lsls r3, r7, #5
	adds r3, #12
	ldrsh r0, [r6, r3]
	adds r3, r6, r3
	movs r2, #2
	ldrsh r1, [r3, r2]
	bl Func_080d99b8
	cmp r0, #0
	beq .L_080d8cd6
	movs r2, #181
	movs r3, #252
	lsls r2, r2, #1
	lsls r3, r3, #8
	add r2, r8
	adds r3, #133
	strh r3, [r2]
	b .L_080d8cd6
.L_080d8cbc:
	cmp r3, #3
	bne .L_080d8cd2
	movs r2, #181
	movs r3, #252
	lsls r2, r2, #1
	lsls r3, r3, #8
	add r2, r8
	adds r3, #134
	adds r5, r6, #0
	strh r3, [r2]
	b .L_080d8cd4
.L_080d8cd2:
	adds r5, r6, #0
.L_080d8cd4:
	adds r5, #12
.L_080d8cd6:
	ldr r3, .L_080d8d3c
	movs r7, #0
	mov r8, r3
.L_080d8cdc:
	ldr r3, [r5, #24]
	cmp r3, #0
	beq .L_080d8d28
	ldr r0, [r5, #20]
	bl Trig_Sin
	ldr r1, [r5, #12]
	mov lr, r8
	.2byte 0xf800
	ldr r3, [r5, #20]
	movs r1, #128
	lsls r1, r1, #5
	adds r3, r3, r1
	movs r1, #243
	lsls r1, r1, #8
	adds r6, r0, #0
	str r3, [r5, #20]
	ldr r0, [r5, #12]
	adds r1, #51
	mov lr, r8
	.2byte 0xf800
	movs r2, #200
	lsls r2, r2, #5
	adds r2, #152
	str r0, [r5, #12]
	cmp r0, r2
	bgt .L_080d8d18
	movs r3, #0
	str r3, [r5, #12]
	str r3, [r5, #24]
.L_080d8d18:
	movs r3, #0
	ldrsh r1, [r5, r3]
	movs r3, #2
	ldrsh r2, [r5, r3]
	adds r0, r7, #0
	adds r3, r6, #0
	bl Func_080d8fa8
.L_080d8d28:
	adds r7, #1
	adds r5, #32
	cmp r7, #7
	ble .L_080d8cdc
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080d8d38:
	.4byte gPartyState
.L_080d8d3c:
	.4byte IwramMulQ16
