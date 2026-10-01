.syntax unified
	.thumb
	.global Func_080bd850
	.thumb_func
Func_080bd850:
	push {lr}
	mov r12, r3
	mov r3, r9
	push {r3}
	mov r3, r12
	sub sp, #4
	mov r3, r9
	str r3, [sp, #0]
	ldrb r3, [r0, #28]
	ldr r2, .L_080bd88c
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r2, [r3, #2]
	ldr r3, .L_080bd890
	adds r2, r2, r3
	adds r3, r0, #0
	adds r3, #32
	adds r0, #33
	ldrb r1, [r3]
	ldrb r3, [r0]
	adds r0, r2, #0
	muls r1, r3
	ldr r3, .L_080bd894
	bl _call_via_r3
	add sp, #4
	pop {r3}
	mov r9, r3
	pop {r0}
	bx r0
.L_080bd88c:
	.4byte ResourceTableEntries
.L_080bd890:
	.4byte 0x06010000
.L_080bd894:
	.4byte IwramClearWords
