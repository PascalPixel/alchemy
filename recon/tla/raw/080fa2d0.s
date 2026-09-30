.syntax unified
	.thumb
	.global Func_080fa2d0
	.thumb_func
Func_080fa2d0:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r5, [r3]
	movs r2, #180
	lsls r2, r2, #1
	adds r3, r5, r2
	ldrh r0, [r3]
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #22
	adds r2, #175
	adds r7, r5, r3
	adds r2, r2, r5
	ldrb r1, [r7]
	mov r8, r2
	ldrb r2, [r2]
	bl Func_08100738
	movs r3, #1
	adds r6, r0, #0
	negs r3, r3
	cmp r6, r3
	bne .L_080fa33a
	movs r0, #114
	bl Audio_PlayCue
	ldr r0, [r5, #48]
	bl RenderOutput_ClearListFar
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #94
	adds r3, r5, r2
	movs r2, #0
	ldrsh r0, [r3, r2]
	ldr r3, .L_080fa364
	adds r2, r6, #0
	adds r0, r0, r3
	adds r1, r6, #0
	bl Func_080f8ce8
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #30
	adds r2, r5, r3
	movs r3, #1
	strh r3, [r2]
	adds r0, r6, #0
	b .L_080fa35e
.L_080fa33a:
	movs r2, #182
	lsls r2, r2, #1
	adds r3, r5, r2
	ldrh r3, [r3]
	movs r0, #128
	lsls r0, r0, #1
	adds r0, #255
	ands r0, r3
	bl Func_08100d40
	ldrb r0, [r7]
	bl BattleUnit_Recalculate
	mov r3, r8
	ldrb r0, [r3]
	bl BattleUnit_Recalculate
	movs r0, #1
.L_080fa35e:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_080fa364:
	.4byte 0x00001120
