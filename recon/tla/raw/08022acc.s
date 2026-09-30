.syntax unified
	.thumb
	.global Func_08022acc
	.thumb_func
Func_08022acc:
	push {r5, r6, r7, lr}
	adds r5, r0, #0
	ldr r3, [r5, #12]
	adds r6, r1, #0
	movs r7, #128
	ands r7, r6
	cmp r3, #0
	beq .L_08022b02
	movs r3, #0
	ldrsh r0, [r5, r3]
	bl Resource_GetMetadataRecordFar
	ldrb r3, [r0, #5]
	cmp r6, r3
	bge .L_08022b02
	ldr r2, [r5, #12]
	lsls r3, r6, #2
	ldr r2, [r3, r2]
	ldrb r3, [r0, #4]
	str r2, [r5, #16]
	strb r3, [r5, #4]
	movs r3, #16
	strb r3, [r5, #21]
	cmp r7, #0
	bne .L_08022b02
	strb r7, [r5, #20]
	strh r7, [r5, #2]
.L_08022b02:
	pop {r5, r6, r7, pc}
