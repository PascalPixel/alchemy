.syntax unified
	.thumb
	.global UiText_RenderWideStringInWindow
	.thumb_func
UiText_RenderWideStringInWindow:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	adds r4, r3, #0
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #60]
	adds r7, r2, #0
	mov r8, r3
	lsls r3, r7, #16
	asrs r3, r3, #16
	adds r6, r0, #0
	sub sp, #8
	mov r9, r1
	mov r10, r3
	cmp r6, #0
	bne .L_0803ad1e
	movs r1, #152
	lsls r1, r1, #5
	adds r1, #66
	add r1, r8
	ldrh r3, [r1]
	movs r2, #244
	lsls r2, r2, #4
	mov r0, r8
	lsls r3, r3, #1
	adds r3, r3, r2
	adds r6, r0, r2
	ldr r2, .L_0803ad38
	strh r2, [r0, r3]
	ldr r2, .L_0803ad3c
	ldrh r3, [r1]
	adds r3, #1
	ands r3, r2
	strh r3, [r1]
.L_0803ad1e:
	movs r2, #0
	ldrsh r3, [r6, r2]
	adds r6, #2
	lsls r1, r3, #16
	cmp r1, #0
	beq .L_0803adfe
.L_0803ad2a:
	lsrs r5, r1, #16
	cmp r5, #30
	bls .L_0803ad40
	cmp r5, #176
	bne .L_0803adc8
	b .L_0803ad40
	.2byte 0x0000
.L_0803ad38:
	.4byte 0x00000000
.L_0803ad3c:
	.4byte 0x000001ff
.L_0803ad40:
	subs r1, r5, #3
	cmp r1, #26
	bhi .L_0803adf0
	ldr r2, .L_0803ae10
	lsls r3, r1, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_0803ad50:
	.4byte .L_0803adbc
	.4byte .L_0803adf0
	.4byte .L_0803adf0
	.4byte .L_0803adf0
	.4byte .L_0803adc4
	.4byte .L_0803adc4
	.4byte .L_0803adc4
	.4byte .L_0803adc4
	.4byte .L_0803adc4
	.4byte .L_0803adc4
	.4byte .L_0803adf0
	.4byte .L_0803adc2
	.4byte .L_0803adc2
	.4byte .L_0803adf0
	.4byte .L_0803adc4
	.4byte .L_0803adf0
	.4byte .L_0803adf0
	.4byte .L_0803adf0
	.4byte .L_0803adf0
	.4byte .L_0803adf0
	.4byte .L_0803adf0
	.4byte .L_0803adf0
	.4byte .L_0803adf0
	.4byte .L_0803adf0
	.4byte .L_0803adf0
	.4byte .L_0803adc2
	.4byte .L_0803adc4
.L_0803adbc:
	mov r7, r10
	adds r4, #1
	b .L_0803adf0
.L_0803adc2:
	adds r6, #2
.L_0803adc4:
	adds r6, #2
	b .L_0803adf0
.L_0803adc8:
	movs r3, #0
	str r3, [sp, #0]
	adds r2, r7, #0
	adds r3, r4, #0
	mov r0, r9
	adds r1, r5, #0
	str r4, [sp, #4]
	bl Func_0803c274
	movs r0, #255
	lsls r0, r0, #8
	adds r0, #34
	adds r3, r5, r0
	movs r2, #128
	lsls r3, r3, #16
	lsls r2, r2, #9
	ldr r4, [sp, #4]
	cmp r3, r2
	bls .L_0803adf0
	adds r7, #1
.L_0803adf0:
	movs r0, #0
	ldrsh r3, [r6, r0]
	adds r6, #2
	lsls r3, r3, #16
	adds r1, r3, #0
	cmp r3, #0
	bne .L_0803ad2a
.L_0803adfe:
	movs r3, #1
	mov r2, r8
	strb r3, [r2, #3]
	add sp, #8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_0803ae10:
	.4byte .L_0803ad50
