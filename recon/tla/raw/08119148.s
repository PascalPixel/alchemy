.syntax unified
	.thumb
	.global Func_08119148
	.thumb_func
Func_08119148:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r5, #170
	lsls r5, r5, #1
	adds r0, r5, #0
	sub sp, #16
	bl Runtime_BumpAllocateAlternatePool
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #36]
	adds r6, r0, #0
	mov r9, r3
	movs r2, #255
	movs r5, #7
	adds r3, #79
.L_08119172:
	subs r5, #1
	strb r2, [r3]
	subs r3, #1
	cmp r5, #0
	bge .L_08119172
	mov r7, sp
	adds r0, r7, #0
	bl BattleParty_PrepareActiveOwners
	movs r5, #0
	mov r8, r0
	cmp r5, r8
	bge .L_081191e6
	movs r1, #149
	lsls r1, r1, #1
	adds r1, r1, r6
	mov r10, r7
	mov r11, r1
	movs r7, #0
.L_08119198:
	mov r2, r10
	ldrh r0, [r7, r2]
	bl Owner_GetState
	movs r2, #170
	adds r1, r0, #0
	lsls r2, r2, #1
	ldr r3, .L_0811929c
	adds r0, r6, #0
	mov lr, r3
	.2byte 0xf800
	movs r3, #2
	mov r4, r11
	mov r1, r10
	strb r3, [r4]
	ldrh r3, [r7, r1]
	adds r2, r5, #0
	adds r3, #72
	subs r2, #128
	mov r4, r9
	movs r1, #170
	lsls r1, r1, #1
	strb r2, [r4, r3]
	adds r0, r6, #0
	bl Func_0801680c
	movs r1, #1
	negs r1, r1
	cmp r0, r1
	beq .L_081191e6
	bl Func_080168a0
	adds r5, #1
	movs r0, #2
	bl WaitFrames
	adds r7, #2
	cmp r5, r8
	blt .L_08119198
.L_081191e6:
	movs r2, #149
	lsls r2, r2, #1
	movs r3, #0
	adds r7, r6, r2
	mov r8, r3
	b .L_081191fe
.L_081191f2:
	bl Func_080168a0
	movs r0, #2
	bl WaitFrames
	adds r5, #1
.L_081191fe:
	cmp r5, #2
	bgt .L_08119218
	mov r4, r8
	movs r1, #170
	lsls r1, r1, #1
	strb r4, [r7]
	adds r0, r6, #0
	bl Func_0801680c
	movs r1, #1
	negs r1, r1
	cmp r0, r1
	bne .L_081191f2
.L_08119218:
	movs r5, #170
	adds r0, r6, #0
	lsls r5, r5, #1
	bl Sys_Free
	adds r0, r5, #0
	bl Runtime_BumpAllocateAlternatePool
	adds r6, r0, #0
	movs r0, #0
	bl Resource_FarCall005
	ldr r3, .L_0811929c
	adds r1, r0, #0
	adds r2, r5, #0
	adds r0, r6, #0
	mov lr, r3
	.2byte 0xf800
	adds r4, r6, #0
	movs r3, #148
	lsls r3, r3, #1
	adds r2, r6, r3
	ldr r3, [r2]
	movs r1, #0
	adds r4, #8
	cmp r1, r3
	bge .L_08119266
	adds r0, r2, #0
	adds r2, r4, #0
.L_08119252:
	ldrb r3, [r2, #2]
	mov r4, r9
	adds r3, #72
	ldrb r3, [r4, r3]
	adds r1, #1
	strb r3, [r2, #2]
	adds r2, #4
	ldr r3, [r0]
	cmp r1, r3
	blt .L_08119252
.L_08119266:
	movs r1, #170
	lsls r1, r1, #1
	adds r0, r6, #0
	bl Func_0801680c
	movs r1, #1
	negs r1, r1
	cmp r0, r1
	beq .L_08119288
	bl Func_080168a0
	movs r0, #1
	bl WaitFrames
	movs r0, #2
	bl WaitFrames
.L_08119288:
	adds r0, r6, #0
	bl Sys_Free
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0811929c:
	.4byte IwramCopyWords
