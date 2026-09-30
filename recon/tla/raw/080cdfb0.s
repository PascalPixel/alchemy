.syntax unified
	.thumb
	.global Event_FindFacingTrigger
	.thumb_func
Event_FindFacingTrigger:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	sub sp, #4
	ldr r6, [r3, #16]
	mov r8, r0
	bl Func_080cdf5c
	bl ObjectTable_Get
	ldrh r0, [r0, #6]
	mov r11, r0
	bl Func_080cdf5c
	bl Func_080cd91c
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	mov r2, r8
	ands r2, r3
	mov r9, r0
	mov r8, r2
	bl Func_080cb09c
	ldr r1, [r6]
	movs r3, #1
	negs r3, r3
	mov r10, r0
	cmp r1, r3
	beq .L_080ce09a
.L_080cdffc:
	movs r3, #4
	ldrsh r5, [r6, r3]
	movs r3, #240
	lsls r3, r3, #8
	ldrh r2, [r6, #4]
	ands r5, r3
	ldr r3, .L_080ce034
	movs r4, #255
	ands r3, r2
	lsls r3, r3, #16
	asrs r7, r3, #16
	movs r3, #15
	ands r3, r1
	ands r4, r2
	cmp r3, #4
	bne .L_080ce08c
	movs r2, #6
	ldrsh r0, [r6, r2]
	str r4, [sp, #0]
	bl GameFlag_IsConditionActive
	ldr r4, [sp, #0]
	cmp r0, #0
	beq .L_080ce08c
	cmp r7, #0
	beq .L_080ce052
	b .L_080ce038
	.2byte 0x0000
.L_080ce034:
	.4byte 0x00000800
.L_080ce038:
	mov r2, r11
	subs r3, r5, r2
	movs r2, #184
	lsls r2, r2, #5
	adds r2, #255
	adds r3, r3, r2
	movs r2, #188
	lsls r3, r3, #16
	lsls r2, r2, #6
	lsrs r3, r3, #16
	adds r2, #254
	cmp r3, r2
	bhi .L_080ce08c
.L_080ce052:
	ldr r1, [r6]
	adds r3, r1, #0
	cmp r1, #0
	bge .L_080ce05c
	adds r3, #255
.L_080ce05c:
	lsls r3, r3, #8
	lsrs r0, r3, #16
	mov r3, r8
	cmp r3, #0
	beq .L_080ce076
	mov r1, r8
	str r4, [sp, #0]
	bl Func_080cdf80
	ldr r4, [sp, #0]
	cmp r0, #0
	beq .L_080ce08c
	ldr r1, [r6]
.L_080ce076:
	movs r3, #16
	ands r3, r1
	cmp r3, #0
	beq .L_080ce086
	cmp r4, r9
	bne .L_080ce08c
	adds r0, r6, #0
	b .L_080ce09c
.L_080ce086:
	adds r0, r6, #0
	cmp r4, r10
	beq .L_080ce09c
.L_080ce08c:
	adds r6, #12
	ldr r3, [r6]
	movs r2, #1
	negs r2, r2
	adds r1, r3, #0
	cmp r3, r2
	bne .L_080cdffc
.L_080ce09a:
	movs r0, #0
.L_080ce09c:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
