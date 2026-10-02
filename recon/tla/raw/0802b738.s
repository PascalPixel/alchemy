.syntax unified
	.thumb
	.global Func_0802b738
	.thumb_func
Func_0802b738:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	sub sp, #12
	ldr r3, .L_0802b7d8
	add r7, sp, #8
	mov r8, r0
	adds r0, r7, #0
	str r3, [sp, #8]
	mov r10, r1
	bl Func_0802b6e8
	ldr r3, .L_0802b7dc
	lsls r0, r0, #16
	lsrs r0, r0, #16
	adds r0, r0, r3
	str r0, [sp, #8]
.L_0802b75c:
	adds r0, r7, #0
	bl Func_0802b6e8
	movs r3, #255
	lsls r0, r0, #16
	lsrs r0, r0, #16
	lsls r3, r3, #8
	adds r2, r0, #1
	adds r3, #255
	ands r2, r3
	cmp r2, #0
	beq .L_0802b7ce
	movs r3, #240
	lsls r3, r3, #4
	adds r3, #255
	ands r3, r0
	cmp r3, r8
	bne .L_0802b7c6
	ldr r3, [sp, #8]
	ldrb r0, [r3]
	adds r3, #1
	str r3, [sp, #8]
	ldrb r1, [r3]
	adds r3, #1
	str r3, [sp, #8]
	ldrb r6, [r3]
	adds r3, #1
	str r3, [sp, #8]
	ldrb r5, [r3]
	adds r3, #1
	str r3, [sp, #8]
	ldrb r2, [r3]
	adds r3, #1
	str r3, [sp, #8]
	ldrb r4, [r3]
	adds r3, #1
	str r3, [sp, #8]
	mov r3, r10
	cmp r3, #0
	beq .L_0802b7b8
	adds r3, r4, #0
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Map_CopyCellAttributeRect
	b .L_0802b75c
.L_0802b7b8:
	str r2, [sp, #0]
	adds r3, r5, #0
	adds r2, r6, #0
	str r4, [sp, #4]
	bl Map_CopyMetatileIndicesRect
	b .L_0802b75c
.L_0802b7c6:
	ldr r3, [sp, #8]
	adds r3, #6
	str r3, [sp, #8]
	b .L_0802b75c
.L_0802b7ce:
	add sp, #12
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0802b7d8:
	.4byte Data_0202e002
.L_0802b7dc:
	.4byte Data_0202e000
