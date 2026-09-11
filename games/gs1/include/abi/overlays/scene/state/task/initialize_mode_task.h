#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_STATE_TASK_INITIALIZE_MODE_TASK
#define ALCHEMY_ABI_OVERLAYS_SCENE_STATE_TASK_INITIALIZE_MODE_TASK

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_02006700(s32 taskAddress, s32 frameBudget);

#define State_Apply Func_02006700
#define State_Run Func_02002940
#define gOv Data_0200cc30
#define gOv10 Data_0200ca4a
#define gOv11 Data_0200c0f6
#define gOv12 Data_0200cac8
#define gOv2 Data_0200cc04
#define gOv3 Data_0200cc3c
#define gOv4 Data_0200cc40
#define gOv5 Data_0200cc98
#define gOv6 Data_0200cc0c
#define gOv7 Data_0200cc10
#define gOv8 Data_0200ca1e
#define gOv9 Data_0200c0ce

#endif
