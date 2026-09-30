.syntax unified
	.thumb
	.global Func_0811c2b4
	.thumb_func
Func_0811c2b4:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	sub sp, #12
	mov r8, r1
	mov r10, r0
	bl GetBattleObjectSlot
	ldr r5, [r0]
	movs r1, #0
	adds r0, r5, #0
	bl GetMotionRecord
	adds r5, #8
	adds r6, r0, #0
	bl Func_0811bd10
	mov r1, r8
	adds r0, r5, #0
	bl Render_ProjectPoint
	ldr r1, [r6, #12]
	ldr r6, .L_0811c310
	mov lr, r6
	.2byte 0xf800
	adds r5, r0, #0
	mov r0, r10
	bl Func_0811c37c
	adds r1, r0, #0
	asrs r1, r1, #16
	adds r0, r5, #0
	mov lr, r6
	.2byte 0xf800
	mov r2, r8
	ldr r3, [r2, #4]
	add sp, #12
	subs r3, r3, r0
	str r3, [r2, #4]
	movs r0, #0
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0811c310:
	.4byte IwramMulQ16
