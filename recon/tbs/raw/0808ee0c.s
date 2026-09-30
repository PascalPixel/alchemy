.syntax unified
	.thumb
	.global BattleFx_EmitRandomParticle
	.thumb_func
BattleFx_EmitRandomParticle:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_0808eed0
	movs r2, #250
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #4
	bl ObjectTable_Get
	ldr r3, .L_0808eed4
	movs r6, #142
	ldr r3, [r3]
	lsls r6, r6, #1
	adds r4, r3, r6
	ldrb r3, [r4, #4]
	adds r5, r0, #0
	movs r7, #0
	cmp r3, #0
	beq .L_0808eebc
	ldr r2, [r5, #8]
	ldr r3, [r5, #16]
	mov r9, r2
	mov r10, r3
	ldr r6, .L_0808eed8
	ldr r2, .L_0808eedc
	movs r3, #128
	lsls r3, r3, #12
	mov lr, r6
	mov r12, r2
	mov r11, r3
.L_0808ee54:
	ldrb r3, [r4, #6]
	mov r6, r9
	lsls r0, r3, #20
	subs r2, r6, r0
	adds r3, r2, #0
	add r3, lr
	mov r8, r3
	ldrb r3, [r4, #7]
	mov r6, r10
	lsls r1, r3, #20
	subs r3, r6, r1
	mov r6, lr
	adds r6, r3, r6
	str r6, [sp, #0]
	ldr r6, .L_0808eee0
	adds r2, r2, r6
	cmp r2, r12
	bhi .L_0808eeae
	adds r3, r3, r6
	cmp r3, r12
	bhi .L_0808eeae
	mov r2, r11
	adds r3, r0, r2
	str r3, [r5, #8]
	adds r3, r1, r2
	str r3, [r5, #16]
	ldr r0, [sp, #0]
	mov r1, r8
	bl ArcTan2
	adds r1, r0, #0
	lsls r1, r1, #16
	movs r0, #160
	adds r2, r5, #0
	lsrs r1, r1, #16
	lsls r0, r0, #13
	adds r2, #8
	bl Vector_AddPolarOffset
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r5, #56]
	str r3, [r5, #60]
	str r3, [r5, #64]
	b .L_0808eebc
.L_0808eeae:
	adds r7, #1
	adds r4, #8
	cmp r7, #9
	bgt .L_0808eebc
	ldrb r3, [r4, #4]
	cmp r3, #0
	bne .L_0808ee54
.L_0808eebc:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
.L_0808eed0:
	.4byte gCell
.L_0808eed4:
	.4byte gEventWork
.L_0808eed8:
	.4byte 0xfff80000
.L_0808eedc:
	.4byte 0x001ffffe
.L_0808eee0:
	.4byte 0x0007ffff
