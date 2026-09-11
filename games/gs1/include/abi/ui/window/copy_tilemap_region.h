#ifndef ALCHEMY_ABI_UI_WINDOW_COPY_TILEMAP_REGION
#define ALCHEMY_ABI_UI_WINDOW_COPY_TILEMAP_REGION

/* Binding layer — not production source. Address / far-call ABI only. */

u32 Func_08005340(const void *source, void *destination);
void Func_08002df0(void *buffer);

#define Ui_Apply Func_08005340
#define Ui_Do Func_08002df0
#define gIw Data_03001e8c

#endif
