.syntax unified
	.thumb
	.global Func_080227e0
	.thumb_func
Func_080227e0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r6, r0, #0
	sub sp, #4
	adds r7, r1, #0
	adds r4, r2, #0
	mov r8, r3
	cmp r6, #7
	bls .L_080227fa
.L_080227f6:
	movs r0, #0
	b .L_080228a8
.L_080227fa:
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #24]
	adds r0, r4, #0
	lsls r3, r6, #3
	adds r5, r5, r3
	str r4, [sp, #0]
	bl Resource_GetMetadataRecordFar
	ldr r4, [sp, #0]
	ldr r2, .L_080228b4
	lsls r3, r6, #12
	adds r5, #28
	orrs r3, r4
	str r3, [r5]
	movs r1, #2
	ldrsh r3, [r2, r1]
	mov r10, r0
	lsls r3, r3, #16
	lsrs r3, r3, #16
	str r7, [r5, #4]
	ldrh r0, [r2]
	adds r1, r2, #6
	movs r5, #0
.L_0802282a:
	adds r2, #4
	cmp r3, #0
	beq .L_080227f6
	cmp r3, r4
	beq .L_08022848
	adds r5, #1
	cmp r5, #255
	bhi .L_08022848
	movs r6, #0
	ldrsh r3, [r1, r6]
	ldrh r0, [r2]
	lsls r3, r3, #16
	lsrs r3, r3, #16
	adds r1, #4
	b .L_0802282a
.L_08022848:
	bl Resource_GetTableEntry
	adds r1, r7, #0
	bl Resource_DecodeType01
	ldr r3, [r7]
	adds r4, r7, #0
	movs r5, #0
	cmp r3, #0
	beq .L_08022870
	adds r2, r3, #0
.L_0802285e:
	adds r3, r2, r7
	adds r5, #1
	stmia r4!, {r3}
	cmp r5, #255
	bhi .L_08022870
	ldr r3, [r4]
	adds r2, r3, #0
	cmp r3, #0
	bne .L_0802285e
.L_08022870:
	mov r1, r8
	cmp r1, #0
	beq .L_0802289e
	mov r2, r8
	subs r2, #1
	adds r5, r4, #4
	adds r0, r7, r0
	cmp r2, #4
	bls .L_08022884
	movs r2, #0
.L_08022884:
	ldr r3, .L_080228b8
	lsls r2, r2, #8
	adds r2, r2, r3
	cmp r5, r0
	bcs .L_0802289e
.L_0802288e:
	ldrb r4, [r5]
	cmp r4, #223
	bhi .L_08022898
	ldrb r4, [r2, r4]
	strb r4, [r5]
.L_08022898:
	adds r5, #1
	cmp r5, r0
	bcc .L_0802288e
.L_0802289e:
	mov r2, r10
	ldrb r3, [r2]
	ldrb r2, [r2, #1]
	adds r0, r2, #0
	muls r0, r3
.L_080228a8:
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080228b4:
	.4byte Data_0802e91c
.L_080228b8:
	.4byte Data_080203a8
