.syntax unified
	.thumb
	.global UiText_DecodeMessage
	.thumb_func
UiText_DecodeMessage:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	adds r7, r2, #0
	movs r2, #192
	lsls r2, r2, #18
	adds r2, #200
	ldr r3, [r2]
	sub sp, #12
	mov r9, r0
	adds r6, r1, #0
	mov r8, r2
	mov r10, r3
	cmp r3, #0
	bne .L_0803ca66
	ldr r5, .L_0803cb04
	movs r0, #200
	adds r1, r5, #0
	bl Runtime_AllocateHeapBlock
	movs r2, #132
	movs r3, #128
	lsrs r5, r5, #2
	lsls r2, r2, #24
	lsls r3, r3, #19
	adds r1, r0, #0
	adds r3, #212
	ldr r0, .L_0803cb08
	orrs r2, r5
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	mov r2, r8
	ldr r3, [r2]
.L_0803ca66:
	mov r5, sp
	mov r1, r9
	adds r0, r5, #0
	mov r8, r3
	bl Func_0803d178
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	mov r9, r3
	b .L_0803cad8
.L_0803ca7c:
	cmp r0, #14
	beq .L_0803ca94
	cmp r0, #14
	bhi .L_0803ca8e
	cmp r0, #12
	bhi .L_0803cace
	cmp r0, #8
	bcc .L_0803cace
	b .L_0803cab4
.L_0803ca8e:
	cmp r0, #15
	beq .L_0803cab4
	b .L_0803cace
.L_0803ca94:
	subs r7, #3
	cmp r7, #0
	ble .L_0803cae2
	strh r0, [r6]
	adds r0, r5, #0
	mov lr, r8
	.2byte 0xf800
	adds r6, #2
	add r0, r9
	strh r0, [r6]
	adds r0, r5, #0
	mov lr, r8
	.2byte 0xf800
	adds r6, #2
	add r0, r9
	b .L_0803cad4
.L_0803cab4:
	subs r7, #1
	cmp r7, #0
	ble .L_0803cae2
	strh r0, [r6]
	adds r0, r5, #0
	mov lr, r8
	.2byte 0xf800
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	adds r6, #2
	adds r0, r0, r2
	b .L_0803cad4
.L_0803cace:
	subs r7, #1
	cmp r7, #0
	ble .L_0803cae2
.L_0803cad4:
	strh r0, [r6]
	adds r6, #2
.L_0803cad8:
	adds r0, r5, #0
	mov lr, r8
	.2byte 0xf800
	cmp r0, #0
	bne .L_0803ca7c
.L_0803cae2:
	mov r3, r10
	cmp r3, #0
	bne .L_0803caee
	movs r0, #200
	bl Runtime_ReleaseHeapBlock
.L_0803caee:
	ldr r3, .L_0803cb00
	add sp, #12
	strh r3, [r6]
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0803cb00:
	.4byte 0x00000000
.L_0803cb04:
	.4byte 0x00000144
.L_0803cb08:
	.4byte Text_DecodeSymbolCode
