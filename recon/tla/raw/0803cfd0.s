.syntax unified
	.thumb
	.global Func_0803cfd0
	.thumb_func
Func_0803cfd0:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	movs r5, #255
	ands r5, r1
	ldr r1, .L_0803d01c
	subs r5, #32
	lsls r5, r5, #5
	adds r5, r5, r1
	ldrh r1, [r5]
	adds r6, r0, #0
	mov r8, r1
	lsls r1, r3, #1
	adds r1, r1, r3
	lsls r1, r1, #6
	movs r3, #161
	adds r2, r2, r1
	lsls r3, r3, #1
	adds r6, r6, r2
	adds r5, #2
	adds r3, #255
	adds r1, r6, r3
	adds r0, r5, #0
	movs r2, #1
	bl Func_0803cfa0
	movs r3, #192
	lsls r3, r3, #1
	adds r0, r5, #0
	adds r1, r6, r3
	movs r2, #15
	bl Func_0803cfa0
	mov r0, r8
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0803d01c:
	.4byte UiText_Glyphs
