.syntax unified
	.thumb
	.global Func_080ebf94
	.thumb_func
Func_080ebf94:
	push {r5, r6, r7, lr}
	movs r0, #32
	sub sp, #20
	bl Runtime_BumpAllocateAlternatePool
	ldr r7, .L_080ec128
	adds r6, r0, #0
	adds r1, r7, #0
	adds r1, #32
	str r1, [sp, #4]
	bl Func_080cdf5c
	bl Object_GetById
	movs r5, #0
	str r5, [sp, #12]
	str r5, [sp, #8]
	bl Resource_FindFreeEntry
	movs r2, #192
	lsls r2, r2, #3
	adds r2, #68
	movs r4, #201
	adds r3, r7, r2
	lsls r4, r4, #3
	strh r0, [r7]
	str r5, [r3]
	adds r3, r7, r4
	str r5, [r3]
	movs r2, #133
	movs r3, #128
	add r0, sp, #16
	lsls r3, r3, #19
	lsls r2, r2, #24
	str r5, [r0]
	adds r3, #212
	adds r1, r6, #0
	adds r2, #32
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r3, #255
	str r3, [r6]
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	str r3, [r6, #4]
	movs r3, #68
	str r3, [r6, #32]
	movs r3, #162
	lsls r3, r3, #1
	str r3, [r6, #36]
	movs r3, #119
	str r3, [r6, #64]
	movs r3, #120
	adds r3, #255
	str r3, [r6, #68]
	movs r3, #255
	lsls r3, r3, #4
	str r3, [r6, #96]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	str r3, [r6, #100]
	ldr r3, .L_080ec12c
	movs r2, #136
	str r3, [r6, #104]
	ldr r3, .L_080ec130
	lsls r2, r2, #1
	str r3, [r6, #108]
	movs r3, #136
	lsls r3, r3, #5
	str r2, [r6, #8]
	str r2, [r6, #40]
	str r2, [r6, #72]
	str r3, [r6, #112]
	movs r1, #128
	adds r2, r6, #0
	ldrh r0, [r7]
	bl VramBlock_LoadCached
	movs r3, #128
	lsls r3, r3, #3
	orrs r0, r3
	ldr r3, [sp, #4]
	movs r2, #0
	movs r1, #0
.L_080ec040:
	str r1, [r3]
	str r1, [r3, #4]
	str r0, [r3, #8]
	ldr r4, [sp, #4]
	adds r2, #1
	adds r4, #12
	adds r3, #12
	str r4, [sp, #4]
	cmp r2, #127
	bls .L_080ec040
	adds r0, r6, #0
	bl Sys_Free
	movs r0, #142
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080ec072
	movs r3, #132
	lsls r3, r3, #17
	str r3, [r7, #4]
	movs r3, #128
	lsls r3, r3, #13
	b .L_080ec084
.L_080ec072:
	add r0, sp, #12
	add r1, sp, #8
	bl Func_080ec1d0
	ldr r3, [sp, #12]
	lsls r3, r3, #16
	str r3, [r7, #4]
	ldr r3, [sp, #8]
	lsls r3, r3, #16
.L_080ec084:
	str r3, [r7, #8]
	ldr r2, [r7, #4]
	ldr r1, .L_080ec134
	ldr r4, .L_080ec138
	adds r2, r2, r1
	str r2, [sp, #12]
	ldr r3, [r7, #8]
	movs r1, #136
	adds r3, r3, r4
	movs r0, #224
	str r3, [sp, #8]
	movs r6, #0
	lsls r1, r1, #17
	lsls r0, r0, #16
	cmp r2, #0
	bge .L_080ec0a8
	str r6, [sp, #12]
	movs r2, #0
.L_080ec0a8:
	cmp r2, r1
	ble .L_080ec0ae
	str r1, [sp, #12]
.L_080ec0ae:
	cmp r3, #0
	bge .L_080ec0b6
	str r6, [sp, #8]
	movs r3, #0
.L_080ec0b6:
	cmp r3, r0
	ble .L_080ec0bc
	str r0, [sp, #8]
.L_080ec0bc:
	ldr r3, [r7, #4]
	ldr r2, .L_080ec13c
	str r3, [r2]
	ldr r3, [r7, #8]
	adds r2, #4
	str r3, [r2]
	bl Func_08038390 + 0x8
	strh r0, [r7, #2]
	ldr r2, .L_080ec140
	ldrh r3, [r7, #2]
	movs r1, #0
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r5, [r3, #2]
	movs r3, #2
	str r3, [sp, #0]
	movs r2, #0
	movs r3, #0
	movs r0, #0
	bl UiWindow_CreateFar
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r7, #18]
	adds r3, #1
	str r3, [r7, #24]
	str r0, [r7, #28]
	ldr r2, [sp, #4]
	movs r3, #128
	stmia r2!, {r6}
	lsls r3, r3, #23
	adds r1, r2, #0
	str r1, [sp, #4]
	stmia r2!, {r3}
	movs r3, #128
	lsrs r5, r5, #5
	lsls r3, r3, #3
	adds r4, r2, #0
	orrs r3, r5
	str r4, [sp, #4]
	str r3, [r2]
	ldr r2, .L_080ec144
	ldr r3, .L_080ec148
	str r3, [r2]
	ldr r3, [sp, #12]
	adds r2, #4
	str r3, [r2]
	ldr r3, [sp, #8]
	adds r2, #4
	str r3, [r2]
	add sp, #20
	pop {r5, r6, r7, pc}
.L_080ec128:
	.4byte Data_0202a000
.L_080ec12c:
	.4byte 0x0001ffff
.L_080ec130:
	.4byte 0x00011ff0
.L_080ec134:
	.4byte 0xff880000
.L_080ec138:
	.4byte 0xffb00000
.L_080ec13c:
	.4byte Data_0202a64c
.L_080ec140:
	.4byte ResourceTableEntries
.L_080ec144:
	.4byte Data_0202a62c
.L_080ec148:
	.4byte Data_0202a004
