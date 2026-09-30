.syntax unified
	.thumb
	.global Func_08119054
	.thumb_func
Func_08119054:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r0, #170
	lsls r0, r0, #1
	sub sp, #32
	bl Runtime_BumpAllocateAlternatePool
	movs r2, #0
	mov r8, r0
	mov r10, r2
	movs r7, #0
	b .L_081190ea
.L_08119070:
	bl Func_080168cc
	movs r2, #149
	lsls r2, r2, #1
	adds r3, r6, r2
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_08119084
	movs r3, #1
	add r10, r3
.L_08119084:
	movs r0, #2
	bl WaitFrames
	mov r5, sp
	ldr r0, .L_08119144
	adds r1, r5, #0
	bl Ui_AdjustValueWithoutLimitFar
	movs r0, #0
	ldrh r3, [r5, r0]
	cmp r3, #0
	beq .L_081190ac
	adds r2, r5, #0
.L_0811909e:
	adds r0, #1
	cmp r0, #4
	bgt .L_081190ac
	adds r2, #2
	ldrh r3, [r2]
	cmp r3, #0
	bne .L_0811909e
.L_081190ac:
	adds r4, r0, #0
	movs r0, #14
	cmp r0, r4
	blt .L_081190cc
	subs r3, r6, r4
	adds r1, r6, #0
	adds r2, r3, #0
	adds r1, #14
	adds r2, #14
.L_081190be:
	ldrb r3, [r2]
	subs r0, #1
	strb r3, [r1]
	subs r2, #1
	subs r1, #1
	cmp r0, r4
	bge .L_081190be
.L_081190cc:
	cmp r4, #0
	ble .L_081190e4
	adds r2, r6, #0
	adds r1, r5, #0
	adds r0, r4, #0
.L_081190d6:
	ldrh r3, [r1]
	subs r0, #1
	strb r3, [r2]
	adds r1, #2
	adds r2, #1
	cmp r0, #0
	bne .L_081190d6
.L_081190e4:
	movs r3, #0
	strb r3, [r6, #14]
	adds r7, #1
.L_081190ea:
	cmp r7, #2
	bgt .L_08119104
	adds r0, r7, #0
	adds r0, #128
	bl Owner_GetState
	adds r6, r0, #0
	bl Func_08016854
	movs r2, #1
	negs r2, r2
	cmp r0, r2
	bne .L_08119070
.L_08119104:
	mov r0, r8
	bl Sys_Free
	movs r0, #170
	lsls r0, r0, #1
	bl Runtime_BumpAllocateAlternatePool
	mov r8, r0
	movs r0, #1
	bl Resource_FarCall005
	bl Func_08016854
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	beq .L_08119130
	bl Func_080168cc
	movs r0, #2
	bl WaitFrames
.L_08119130:
	mov r0, r8
	bl Sys_Free
	mov r0, r10
	add sp, #32
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08119144:
	.4byte 0x00000c58
