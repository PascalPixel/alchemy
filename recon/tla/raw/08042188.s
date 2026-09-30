.syntax unified
	.thumb
	.global UiText_DrawStringInWindow
	.thumb_func
UiText_DrawStringInWindow:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r5, r0, #0
	movs r0, #128
	lsls r0, r0, #2
	adds r7, r2, #0
	mov r8, r3
	mov r10, r1
	bl Runtime_BumpAllocateAlternatePool
	ldrb r3, [r5]
	adds r6, r0, #0
	adds r2, r6, #0
	cmp r3, #0
	beq .L_080421b8
.L_080421aa:
	ldrb r3, [r5]
	adds r5, #1
	strh r3, [r2]
	adds r2, #2
	ldrb r3, [r5]
	cmp r3, #0
	bne .L_080421aa
.L_080421b8:
	ldr r3, .L_080421d8
	lsrs r7, r7, #3
	strh r3, [r2]
	mov r3, r8
	lsrs r3, r3, #3
	adds r0, r6, #0
	mov r1, r10
	adds r2, r7, #0
	mov r8, r3
	bl Func_0803acd4
	adds r0, r6, #0
	bl Sys_Free
	b .L_080421dc
	.2byte 0x0000
.L_080421d8:
	.4byte 0x00000000
.L_080421dc:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
