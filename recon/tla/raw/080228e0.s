.syntax unified
	.thumb
	.global InitializeAnimationObjects
	.thumb_func
InitializeAnimationObjects:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	ldrb r3, [r7, #27]
	movs r1, #0
	sub sp, #8
	cmp r1, r3
	bge .L_08022962
	adds r2, r7, #0
	adds r2, #40
	str r2, [sp, #4]
	mov r8, r1
.L_080228fa:
	ldr r2, [sp, #4]
	str r1, [sp, #0]
	ldmia r2!, {r6}
	adds r3, r2, #0
	str r3, [sp, #4]
	movs r3, #0
	ldrsh r0, [r6, r3]
	bl Resource_GetMetadataRecordFar
	adds r5, r0, #0
	ldrb r2, [r5]
	ldr r1, [sp, #0]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_0802295a
	cmp r1, #0
	bne .L_08022930
	ldrb r3, [r5, #1]
	strb r2, [r7, #20]
	strb r3, [r7, #21]
	ldrh r3, [r5, #2]
	lsls r3, r3, #8
	str r3, [r7, #12]
	ldrb r3, [r5, #7]
	strb r3, [r7, #23]
	ldrb r3, [r5, #6]
	strb r3, [r7, #22]
.L_08022930:
	ldr r0, [r5, #12]
	cmp r0, #0
	bne .L_08022942
	movs r2, #0
	ldrsh r0, [r6, r2]
	str r1, [sp, #0]
	bl Animation_LookupValueByKey
	ldr r1, [sp, #0]
.L_08022942:
	ldrb r3, [r5, #4]
	strb r3, [r6, #4]
	ldr r3, [r5, #16]
	str r0, [r6, #8]
	str r3, [r6, #12]
	ldrb r3, [r5, #10]
	strb r3, [r6, #7]
	movs r3, #255
	strb r3, [r6, #22]
	mov r3, r8
	str r3, [r6, #16]
	strb r3, [r6, #20]
.L_0802295a:
	ldrb r3, [r7, #27]
	adds r1, #1
	cmp r1, r3
	blt .L_080228fa
.L_08022962:
	movs r0, #0
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
