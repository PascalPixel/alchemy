.syntax unified
	.thumb
	.global Func_08040628
	.thumb_func
Func_08040628:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #208
	ldr r3, [r3]
	movs r0, #191
	movs r2, #3
	lsls r0, r0, #1
	sub sp, #12
	mov r11, r3
	mov r10, r2
	bl GameFlag_Test
	movs r3, #0
	mov r9, r0
	str r3, [sp, #8]
	cmp r0, #0
	beq .L_08040660
	movs r3, #2
	str r3, [sp, #8]
	movs r2, #1
	mov r10, r2
.L_08040660:
	ldr r3, .L_08040784
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0804066c
	movs r2, #3
	add r10, r2
.L_0804066c:
	movs r3, #8
	mov r2, r10
	subs r1, r3, r2
	lsls r3, r2, #1
	add r3, r10
	adds r4, r3, #1
	adds r3, r1, r4
	cmp r3, #19
	ble .L_08040682
	movs r1, #1
	movs r4, #19
.L_08040682:
	movs r3, #2
	str r3, [sp, #0]
	movs r0, #5
	adds r3, r4, #0
	movs r2, #20
	bl UiWindow_Create
	mov r3, r10
	mov r8, r0
	cmp r3, #1
	ble .L_080406b4
	mov r5, r10
	movs r6, #3
	subs r5, #1
.L_0804069e:
	adds r2, r6, #0
	mov r0, r8
	movs r1, #0
	movs r3, #19
	subs r5, #1
	str r6, [sp, #0]
	bl UiWindow_DrawDividerLine
	adds r6, #3
	cmp r5, #0
	bne .L_0804069e
.L_080406b4:
	mov r2, r9
	movs r7, #4
	cmp r2, #0
	bne .L_080406da
	ldr r5, .L_08040788
	mov r1, r8
	adds r0, r5, #0
	movs r2, #48
	movs r3, #4
	adds r5, #1
	bl UiText_DrawResource
	adds r0, r5, #0
	mov r1, r8
	movs r2, #48
	movs r3, #28
	bl UiText_DrawResource
	movs r7, #52
.L_080406da:
	adds r3, r7, #0
	ldr r0, .L_0804078c
	mov r1, r8
	movs r2, #48
	bl UiText_DrawResource
	ldr r3, .L_08040784
	adds r7, #24
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0804071c
	ldr r5, .L_08040790
	adds r3, r7, #0
	adds r0, r5, #0
	mov r1, r8
	movs r2, #48
	adds r7, #24
	bl UiText_DrawResource
	adds r0, r5, #1
	adds r3, r7, #0
	mov r1, r8
	movs r2, #48
	adds r7, #24
	adds r5, #2
	bl UiText_DrawResource
	adds r0, r5, #0
	mov r1, r8
	movs r2, #48
	adds r3, r7, #0
	bl UiText_DrawResource
.L_0804071c:
	bl Func_08044460
	movs r1, #128
	movs r3, #0
	lsls r1, r1, #23
	mov r2, r8
	str r3, [sp, #0]
	bl RenderOutput_Create
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #164
	add r3, r11
	str r0, [r3]
	movs r7, #4
	mov r3, r10
	negs r7, r7
	cmp r3, #0
	ble .L_08040772
	ldr r3, .L_08040794
	ldr r2, [sp, #8]
	movs r4, #194
	lsls r4, r4, #3
	add r4, r11
	mov r5, r10
	adds r6, r2, r3
.L_08040750:
	ldrb r0, [r6]
	movs r1, #0
	lsls r0, r0, #24
	asrs r0, r0, #24
	mov r2, r8
	movs r3, #12
	str r7, [sp, #0]
	str r4, [sp, #4]
	bl Func_080450ac
	ldr r4, [sp, #4]
	subs r5, #1
	adds r6, #1
	stmia r4!, {r0}
	adds r7, #24
	cmp r5, #0
	bne .L_08040750
.L_08040772:
	mov r0, r8
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08040784:
	.4byte Data_03001238
.L_08040788:
	.4byte 0x00001154
.L_0804078c:
	.4byte 0x00001156
.L_08040790:
	.4byte 0x00001158
.L_08040794:
	.4byte Data_0805ea8f
