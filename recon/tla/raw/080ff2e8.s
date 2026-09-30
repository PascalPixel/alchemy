.syntax unified
	.thumb
	.global Func_080ff2e8
	.thumb_func
Func_080ff2e8:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	mov r8, r0
	adds r6, r1, #0
	ldr r3, [r3]
	cmp r2, #0
	bne .L_080ff304
	cmp r6, #3
	ble .L_080ff304
	adds r6, #1
.L_080ff304:
	cmp r6, #1
	bne .L_080ff33c
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #22
	adds r7, r3, r2
	ldrb r0, [r7]
	bl Owner_GetState
	adds r5, r0, #0
	ldrb r3, [r5, #15]
	cmp r3, #99
	bne .L_080ff322
	movs r6, #8
	b .L_080ff33c
.L_080ff322:
	ldrb r1, [r5, #15]
	ldrb r0, [r7]
	adds r1, #1
	bl Func_080ad200
	movs r2, #146
	lsls r2, r2, #1
	adds r3, r5, r2
	ldr r3, [r3]
	movs r1, #5
	subs r0, r0, r3
	bl UiText_DrawQuantity
.L_080ff33c:
	movs r0, #128
	bl Runtime_BumpAllocate
	adds r5, r0, #0
	ldr r0, .L_080ff36c
	adds r1, r5, #0
	adds r0, r6, r0
	movs r2, #64
	bl UiText_CopyMessageStringFar
	movs r3, #1
	adds r0, r5, #0
	negs r3, r3
	mov r1, r8
	movs r2, #0
	bl Func_08038250
	adds r0, r5, #0
	bl Sys_Free
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080ff36c:
	.4byte 0x00001117
