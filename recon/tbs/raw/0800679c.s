@ Uncredited SerialTest_Run disassembly from the own English ROM.
.syntax unified
	.thumb
	.balign 4
	.global SerialTest_Run
	.thumb_func
SerialTest_Run:
	push {r5, r6, lr}
	movs r0, #3
	sub sp, #4
	bl Audio_PlayCue
	bl SerialRuntime_Initialize
	ldr r2, [pc, #140]
	ldr r3, [pc, #140]
	movs r1, #19
.L0:
	subs r1, #1
	strh r3, [r2]
	subs r2, #2
	subs r3, #1
	cmp r1, #0
	bge .L0
	mov r0, sp
	movs r3, #0
	str r3, [r0]
	ldr r1, [pc, #124]
	ldr r2, [pc, #124]
	bl Bios_CpuSet
	movs r0, #3
	bl SerialRuntime_WaitForStatusMask
.L6:
	ldr r0, [pc, #108]
	bl SerialRuntime_BeginTransferB
	ldr r6, [pc, #112]
.L7:
	ldr r3, [r6]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L1
	movs r0, #128
	movs r1, #160
	lsls r0, r0, #20
	lsls r1, r1, #2
	bl SerialRuntime_BeginTransferA
.L1:
	ldr r3, [r6]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L2
	movs r1, #160
	ldr r0, [pc, #80]
	lsls r1, r1, #2
	bl SerialRuntime_BeginTransferA
.L2:
	ldr r3, [r6]
	movs r2, #8
	ands r3, r2
	cmp r3, #0
	beq .L3
	ldr r5, [pc, #64]
.L4:
	subs r5, #1
	bl RuntimeWait_BusyLoopTick
	cmp r5, #0
	bge .L4
.L3:
	ldr r3, [pc, #56]
	ldr r3, [r3]
	cmp r3, #0
	bne .L5
	ldr r3, [pc, #52]
	ldr r0, [pc, #28]
	ldr r1, [pc, #52]
	ldr r2, [pc, #56]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	b .L6
.L5:
	movs r0, #1
	bl WaitFrames
	b .L7
	.2byte 0x0000
	.4byte 0x06002426
	.4byte 0xfffff093
	.4byte gMapCellBuffer
	.4byte 0x05000100
	.4byte gKeysHeld
	.4byte 0x08001000
	.4byte 0x0000270f
	.4byte gSerialReceiveDest
	.4byte 0x040000d4
	.4byte 0x06001000
	.4byte 0x840000a0
	.size SerialTest_Run, . - SerialTest_Run
