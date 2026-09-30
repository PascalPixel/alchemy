.syntax unified
	.thumb
	.global Func_081098c0
	.thumb_func
Func_081098c0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r8, r1
	adds r7, r0, #0
	mov r0, r8
	adds r5, r2, #0
	bl Item_Get
	adds r6, r0, #0
	movs r3, #0
	ldrb r1, [r6, #2]
	adds r0, r7, #0
	mov r10, r3
	bl Djinn_IsActiveFar + 0x20
	mov r9, r0
	movs r0, #101
	bl Audio_PlayCue
	cmp r10, r5
	bge .L_08109912
.L_081098f0:
	mov r1, r8
	adds r0, r7, #0
	bl Inventory_AddItemFar
	mov r10, r0
	ldrh r0, [r6]
	subs r5, #1
	negs r0, r0
	bl Func_080ad1d8
	ldrh r0, [r6]
	bl Djinn_AddToLeastLoadedOwnerFar + 0x10
	bl Func_08109188
	cmp r5, #0
	bne .L_081098f0
.L_08109912:
	ldr r0, .L_08109938
	bl Func_0810857c
	adds r0, r7, #0
	mov r1, r10
	bl Func_0810993c
	cmp r0, #0
	beq .L_0810992c
	adds r0, r7, #0
	mov r1, r9
	bl Func_08109a3c
.L_0810992c:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08109938:
	.4byte 0x00001252
