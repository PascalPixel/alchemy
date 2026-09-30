.syntax unified
	.thumb
	.global Func_080ae358
	.thumb_func
Func_080ae358:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #12
	bl Party_CountActiveOwners
	mov r11, r0
.L_080ae36e:
	movs r2, #0
	movs r4, #186
	lsls r4, r4, #2
	mov r9, r2
	mov r8, r2
	adds r4, #255
	mov r10, r2
	cmp r9, r11
	bge .L_080ae3c4
	ldr r3, .L_080ae3f8
	movs r2, #134
	lsls r2, r2, #2
	adds r7, r3, r2
	mov r6, r11
.L_080ae38a:
	ldrb r5, [r7]
	str r4, [sp, #0]
	adds r0, r5, #0
	bl Owner_GetState
	movs r3, #140
	ldr r4, [sp, #0]
	lsls r3, r3, #1
	adds r7, #1
	movs r2, #0
	adds r0, r0, r3
	movs r1, #3
.L_080ae3a2:
	ldrb r3, [r0]
	subs r1, #1
	adds r0, #1
	adds r2, r2, r3
	cmp r1, #0
	bge .L_080ae3a2
	cmp r2, r8
	ble .L_080ae3b6
	mov r8, r2
	mov r10, r5
.L_080ae3b6:
	cmp r2, r4
	bge .L_080ae3be
	adds r4, r2, #0
	mov r9, r5
.L_080ae3be:
	subs r6, #1
	cmp r6, #0
	bne .L_080ae38a
.L_080ae3c4:
	mov r2, r8
	subs r3, r2, r4
	cmp r3, #1
	ble .L_080ae3ea
	add r2, sp, #4
	mov r0, r10
	add r1, sp, #8
	bl Func_080ae2fc
	adds r2, r0, #0
	cmp r2, #0
	bne .L_080ae3ea
	ldr r1, [sp, #8]
	ldr r2, [sp, #4]
	mov r0, r10
	mov r3, r9
	bl Djinn_Transfer
	b .L_080ae36e
.L_080ae3ea:
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080ae3f8:
	.4byte gPartyState
