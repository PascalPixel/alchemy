.syntax unified
	.thumb
	.set Func_080048f4, 0x080048f4
	.set Func_080072f0, 0x080072f0
	.set Func_080041d8, 0x080041d8
	.global BattleFx_ScheduleRatioTransition
	.thumb_func
BattleFx_ScheduleRatioTransition:
	push {r5, r6, r7, lr}
	ldr r3, [pc, #88]
	adds r6, r0, #0
	adds r7, r1, #0
	movs r0, #27
	ldr r1, [pc, #84]
	ldr r5, [r3]
	bl Func_080048f4
	movs r1, #207
	lsls r1, r1, #1
	adds r3, r0, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #3
	bne .L_done
	movs r1, #128
	ldr r3, [pc, #64]
	lsls r1, r1, #9
	adds r0, r6, #0
	bl Func_080072f0
	movs r3, #212
	lsls r3, r3, #2
	adds r1, r5, r3
	adds r3, #4
	adds r2, r5, r3
	ldr r3, [r2]
	str r3, [r1]
	movs r1, #214
	lsls r1, r1, #2
	adds r3, r5, r1
	adds r1, #2
	str r0, [r2]
	strh r7, [r3]
	movs r2, #0
	adds r3, r5, r1
	strh r2, [r3]
	ldr r0, [pc, #24]
	ldr r1, [pc, #28]
	bl Func_080041d8
.L_done:
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001e70
	.4byte 0x00000ccc
	.4byte 0x0300013c
	.4byte 0x080935d5
	.4byte 0x00000c94
