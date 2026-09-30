.syntax unified
	.thumb
	.global Func_08118f6c
	.thumb_func
Func_08118f6c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #32
	mov r1, sp
	adds r1, #8
	adds r0, r1, #0
	str r1, [sp, #4]
	bl Func_0811a038
	mov r11, r0
	movs r0, #0
	bl Resource_FarCall005
	ldr r0, [r0]
	ldr r3, .L_0811904c
	mov r9, r0
	mov r2, r9
	movs r1, #28
	ands r2, r3
	add r1, sp
	movs r3, #0
	mov r9, r2
	mov r8, r3
	mov r10, r1
.L_08118fa6:
	mov r2, r10
	movs r3, #0
	mov r1, r8
	strb r3, [r2, r1]
	mov r2, r11
	cmp r2, #0
	ble .L_08118fde
	movs r1, #140
	ldr r6, [sp, #4]
	lsls r1, r1, #1
	mov r7, r10
	add r1, r8
	mov r5, r11
.L_08118fc0:
	ldrh r0, [r6]
	str r1, [sp, #0]
	bl Owner_GetState
	ldr r1, [sp, #0]
	mov r2, r8
	ldrb r3, [r7, r2]
	ldrb r2, [r0, r1]
	subs r5, #1
	adds r3, r3, r2
	mov r2, r8
	adds r6, #2
	strb r3, [r7, r2]
	cmp r5, #0
	bne .L_08118fc0
.L_08118fde:
	movs r3, #1
	add r8, r3
	mov r1, r8
	cmp r1, #3
	ble .L_08118fa6
	movs r2, #0
	mov r8, r2
.L_08118fec:
	mov r0, r8
	bl Trade_AddOfferFar + 0x18
	cmp r0, #0
	beq .L_08119028
	mov r3, r10
	adds r0, #4
	ldrb r2, [r3]
	ldrb r3, [r0]
	movs r4, #0
	cmp r2, r3
	bcc .L_08119018
	mov r1, r10
.L_08119006:
	adds r4, #1
	cmp r4, #3
	bgt .L_08119018
	adds r1, #1
	adds r0, #1
	ldrb r2, [r1]
	ldrb r3, [r0]
	cmp r2, r3
	bcs .L_08119006
.L_08119018:
	cmp r4, #4
	bne .L_08119028
	movs r3, #1
	mov r1, r8
	mov r2, r9
	lsls r3, r1
	orrs r2, r3
	mov r9, r2
.L_08119028:
	movs r3, #1
	add r8, r3
	mov r1, r8
	cmp r1, #15
	ble .L_08118fec
	movs r0, #0
	bl Resource_FarCall005
	mov r2, r9
	str r2, [r0]
	add sp, #32
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0811904c:
	.4byte 0xffff0000
	.4byte 0x00004770
