.syntax unified
	.thumb
	.global Func_081c1014
	.thumb_func
Func_081c1014:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #12
	ldr r3, .L_081c1144
	mov r9, sp
	mov r2, r9
	ldmia r3!, {r0, r1, r4}
	stmia r2!, {r0, r1, r4}
	movs r0, #3
	movs r7, #0
	mov r11, r0
	ldr r3, .L_081c1148
	str r7, [r3]
	ldr r2, .L_081c114c
	movs r1, #20
	ldrb r3, [r2, #2]
	movs r3, #1
	strb r3, [r2, #2]
	movs r2, #0
	mov r10, r1
	mov r8, r2
	mov r6, r9
.L_081c104a:
	mov r3, r10
	cmp r3, #0
	beq .L_081c1056
	movs r4, #1
	negs r4, r4
	add r10, r4
.L_081c1056:
	ldr r2, .L_081c1150
	ldr r3, [r2]
	cmp r3, #0
	beq .L_081c1066
	movs r3, #0
	str r3, [r2]
	movs r0, #2
	mov r10, r0
.L_081c1066:
	ldr r5, .L_081c1154
	movs r2, #4
	ldr r3, [r5, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_081c1082
	mov r0, r11
	adds r0, #1
	movs r1, #5
	bl __modsi3
	mov r11, r0
	bl Sound_LoadPresetParameters
.L_081c1082:
	ldr r3, [r5, #12]
	movs r2, #128
	lsls r2, r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_081c1094
	ldr r3, [r6]
	adds r3, #10
	str r3, [r6]
.L_081c1094:
	ldr r3, [r5, #12]
	movs r2, #128
	lsls r2, r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_081c10a6
	ldr r3, [r6]
	subs r3, #10
	str r3, [r6]
.L_081c10a6:
	ldr r3, [r5, #12]
	movs r2, #16
	ands r3, r2
	cmp r3, #0
	beq .L_081c10b6
	ldr r3, [r6]
	adds r3, #1
	str r3, [r6]
.L_081c10b6:
	ldr r3, [r5, #12]
	movs r2, #32
	ands r3, r2
	cmp r3, #0
	beq .L_081c10c6
	ldr r3, [r6]
	subs r3, #1
	str r3, [r6]
.L_081c10c6:
	ldr r3, [r5, #12]
	movs r2, #64
	ands r3, r2
	cmp r3, #0
	beq .L_081c10de
	cmp r7, #0
	ble .L_081c10de
	movs r1, #4
	negs r1, r1
	subs r6, #4
	add r8, r1
	subs r7, #1
.L_081c10de:
	ldr r5, .L_081c1154
	movs r2, #128
	ldr r3, [r5, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_081c10f6
	cmp r7, #1
	bgt .L_081c10f6
	movs r2, #4
	adds r6, #4
	add r8, r2
	adds r7, #1
.L_081c10f6:
	ldr r3, [r5, #12]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_081c110a
	mov r3, r8
	mov r4, r9
	ldr r0, [r3, r4]
	bl Func_081c0cb0
.L_081c110a:
	ldr r3, [r5, #12]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_081c111a
	movs r0, #0
	bl Func_081c0cb0
.L_081c111a:
	ldr r3, [r5, #12]
	movs r2, #8
	ands r3, r2
	cmp r3, #0
	beq .L_081c112a
	movs r0, #78
	bl Func_081c0cb0
.L_081c112a:
	ldr r3, [r5, #12]
	movs r2, #4
	ands r3, r2
	cmp r3, #0
	beq .L_081c113c
	movs r0, #195
	lsls r0, r0, #1
	bl Func_081c0cb0
.L_081c113c:
	movs r0, #1
	bl WaitFrames
	b .L_081c104a
.L_081c1144:
	.4byte Data_081c3430
.L_081c1148:
	.4byte Data_03007804
.L_081c114c:
	.4byte Data_03001138
.L_081c1150:
	.4byte Data_03000ee4
.L_081c1154:
	.4byte gInput
