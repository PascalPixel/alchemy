.syntax unified
	.thumb
	.global ItemMenu_DrawStat
	.thumb_func
ItemMenu_DrawStat:
	push {r5, r6, r7, lr}
	sub sp, #4
	adds r6, r3, #0
	ldr r3, [sp, #20]
	adds r5, r0, #0
	str r3, [sp, #0]
	movs r1, #3
	adds r3, r6, #0
	adds r7, r2, #0
	bl UiText_DrawNumberInWindowFar
	movs r2, #1
	adds r3, r5, #0
	cmp r5, #0
	bge .L_080fbd54
	negs r3, r5
.L_080fbd54:
	cmp r3, #9
	ble .L_080fbd5a
	movs r2, #2
.L_080fbd5a:
	adds r3, r5, #0
	cmp r5, #0
	bge .L_080fbd62
	negs r3, r5
.L_080fbd62:
	cmp r3, #99
	ble .L_080fbd68
	movs r2, #3
.L_080fbd68:
	cmp r5, #0
	ble .L_080fbd7e
	lsls r2, r2, #3
	subs r2, r6, r2
	ldr r0, .L_080fbd94
	adds r2, #16
	adds r1, r7, #0
	ldr r3, [sp, #20]
	bl UiText_DrawStringInWindowFar
	b .L_080fbd8e
.L_080fbd7e:
	lsls r2, r2, #3
	subs r2, r6, r2
	ldr r0, .L_080fbd98
	adds r2, #16
	adds r1, r7, #0
	ldr r3, [sp, #20]
	bl UiText_DrawStringInWindowFar
.L_080fbd8e:
	add sp, #4
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080fbd94:
	.4byte Menu_PlusSignString
.L_080fbd98:
	.4byte Menu_MinusSignString
