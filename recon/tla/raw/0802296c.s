.syntax unified
	.thumb
	.global Animation_InitWorkFromMetadata
	.thumb_func
Animation_InitWorkFromMetadata:
	push {r5, r6, lr}
	adds r5, r0, #0
	cmp r5, #0
	beq .L_080229aa
	movs r3, #0
	ldrsh r0, [r5, r3]
	bl Resource_GetMetadataRecordFar
	adds r6, r0, #0
	ldrb r3, [r6]
	cmp r3, #0
	beq .L_080229aa
	ldr r0, [r6, #12]
	cmp r0, #0
	bne .L_08022992
	movs r3, #0
	ldrsh r0, [r5, r3]
	bl Func_080228bc
.L_08022992:
	ldrb r3, [r6, #4]
	movs r2, #0
	strb r3, [r5, #4]
	str r2, [r5, #16]
	ldr r3, [r6, #16]
	str r0, [r5, #8]
	str r3, [r5, #12]
	strb r2, [r5, #20]
	ldrb r3, [r6, #10]
	strb r3, [r5, #7]
	movs r3, #255
	strb r3, [r5, #22]
.L_080229aa:
	pop {r5, r6, pc}
