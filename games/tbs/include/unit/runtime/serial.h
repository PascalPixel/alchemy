#ifndef ALCHEMY_UNIT_RUNTIME_SERIAL_H
#define ALCHEMY_UNIT_RUNTIME_SERIAL_H

#include "types.h"
#include "callback_scheduler.h"
#include "irq.h"

void SerialRuntime_Initialize(void);
void SerialRuntime_EnableTransferTimer(void);
void SerialRuntime_DisableTransfer(void);
s32 SerialRuntime_PollAndTransfer(void *send, void *receive);
s32 SerialRuntime_PollStatus(void);
s32 SerialRuntime_ExchangePayloads(void *send, void *receive);
void SerialRuntime_PrepareSendPacket(void *payload);
u8 SerialRuntime_CollectReceivedPayloads(void *payload);
void SerialRuntime_HandleTransferInterrupt(void);
void SerialRuntime_RemoveIrqHandlers(void);
u32 SerialRuntime_WaitForStatusMask(s32 mask);
s32 SerialRuntime_BeginTransferA(void *source, s32 transfer_value);
s32 SerialRuntime_BeginTransferB(void *destination);
void SerialRuntime_WaitForTransfers(void);
s32 SerialRuntime_GetActiveTransfers(void);
void BattleLink_ResetTransferState(void);
void SerialRuntime_StepBlockTransfer(void);
void RuntimeWait_BusyLoopTick(void);
void SerialTest_Run(void);

#endif
