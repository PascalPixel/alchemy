.syntax unified
	.thumb
	.global UiText_DrawPrefixedNumberAtOffset
	.thumb_func
UiText_DrawPrefixedNumberAtOffset:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	mov r8, r3
	movs r3, #192
	sub sp, #32
	lsls r3, r3, #18
	ldr r5, [sp, #56]
	ldr r3, [r3, #60]
	adds r4, r0, #0
	adds r6, r1, #0
	adds r7, r2, #0
	add r0, sp, #16
	adds r1, r4, #0
	movs r2, #4
	mov r10, r3
	bl Func_0803ae14
	cmp r5, #0
	bne .L_080422a8
	movs r3, #240
	lsls r3, r3, #8
	mov r4, sp
	adds r3, #29
	b .L_080422b0
.L_080422a8:
	movs r3, #240
	lsls r3, r3, #8
	mov r4, sp
	adds r3, #31
.L_080422b0:
	strh r3, [r4]
	ldr r3, .L_0804230c
	strh r3, [r4, #2]
	adds r2, r4, #4
	movs r1, #4
.L_080422ba:
	ldrb r3, [r0]
	subs r1, #1
	strh r3, [r2]
	adds r0, #1
	adds r2, #2
	cmp r1, #0
	bge .L_080422ba
	movs r3, #0
	strh r3, [r4, #12]
	movs r1, #14
	ldrsh r3, [r6, r1]
	mov r1, r8
	lsrs r2, r1, #3
	adds r3, r3, r2
	movs r1, #12
	ldrsh r2, [r6, r1]
	adds r3, #1
	lsrs r1, r7, #3
	adds r2, r2, r1
	lsls r3, r3, #5
	adds r3, r3, r2
	movs r2, #160
	adds r1, r3, #1
	lsls r2, r2, #2
	cmp r1, r2
	bcs .L_08042302
	ldr r3, .L_08042310
	lsls r1, r1, #1
	adds r2, r1, r3
	add r1, r10
	movs r3, #7
	adds r1, #8
	ands r3, r7
	adds r0, r4, #0
	bl Func_080416cc
.L_08042302:
	add sp, #32
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0804230c:
	.4byte 0x0000f01e
.L_08042310:
	.4byte 0x06002000
