.syntax unified
	.thumb
	.global Func_080eaeb4
	.thumb_func
Func_080eaeb4:
	push {r5, r6, r7, lr}
	ldr r3, .L_080eaf24
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #12
	bl Object_GetById
	adds r6, r0, #0
	ldrh r2, [r6, #6]
	movs r3, #12
	lsrs r2, r2, #12
	adds r2, #2
	ands r2, r3
	ldr r3, [r6, #8]
	mov r5, sp
	str r3, [r5]
	lsls r7, r2, #12
	ldr r3, [r6, #12]
	movs r0, #128
	str r3, [r5, #4]
	lsls r0, r0, #13
	ldr r3, [r6, #16]
	adds r1, r7, #0
	adds r2, r5, #0
	str r3, [r5, #8]
	bl Vector_AddPolarOffset
	adds r0, r5, #0
	movs r1, #1
	bl Func_080eaf28
	cmp r0, #0
	bne .L_080eaf20
	ldr r3, [r6, #8]
	movs r0, #128
	str r3, [r5]
	lsls r0, r0, #14
	ldr r3, [r6, #12]
	adds r1, r7, #0
	str r3, [r5, #4]
	adds r2, r5, #0
	ldr r3, [r6, #16]
	str r3, [r5, #8]
	bl Vector_AddPolarOffset
	adds r0, r5, #0
	movs r1, #1
	bl Func_080eaf28
	cmp r0, #0
	bne .L_080eaf20
	movs r0, #0
.L_080eaf20:
	add sp, #12
	pop {r5, r6, r7, pc}
.L_080eaf24:
	.4byte gPartyState
