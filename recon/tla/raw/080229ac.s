.syntax unified
	.thumb
	.global ResourceMetadata_Register
	.thumb_func
ResourceMetadata_Register:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	ldr r5, [r6, #40]
	mov r8, r1
	movs r7, #0
	cmp r5, #0
	beq .L_080229d0
	adds r3, r6, #0
	adds r3, #40
.L_080229c2:
	adds r7, #1
	cmp r7, #3
	bgt .L_080229d0
	adds r3, #4
	ldr r5, [r3]
	cmp r5, #0
	bne .L_080229c2
.L_080229d0:
	cmp r7, #4
	bne .L_080229da
	movs r0, #1
	negs r0, r0
	b .L_08022a1c
.L_080229da:
	mov r0, r8
	bl Func_08022c98
	adds r5, r0, #0
	movs r0, #0
	cmp r5, #0
	beq .L_08022a1c
	lsls r3, r7, #2
	adds r3, #40
	mov r0, r8
	str r5, [r6, r3]
	bl Resource_GetMetadataRecordFar
	ldrb r3, [r6, #27]
	cmp r3, #0
	bne .L_08022a10
	ldrb r3, [r0]
	strb r3, [r6, #20]
	ldrb r3, [r0, #1]
	strb r3, [r6, #21]
	ldrh r3, [r0, #2]
	lsls r3, r3, #8
	str r3, [r6, #12]
	ldrb r3, [r0, #7]
	strb r3, [r6, #23]
	ldrb r3, [r0, #6]
	strb r3, [r6, #22]
.L_08022a10:
	ldrb r3, [r6, #27]
	cmp r7, r3
	bne .L_08022a1a
	adds r3, r7, #1
	strb r3, [r6, #27]
.L_08022a1a:
	adds r0, r5, #0
.L_08022a1c:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
