#include "types.h"

typedef void *(*WordCopy)(void *destination, const void *source, u32 size);
typedef void *(*CopiedTransform)(
    void *destination,
    const void *source,
    void *workspace,
    volatile void *dma
);

typedef struct {
    const void *source;
    void *destination;
    u32 control;
} DmaTransfer;

void *Func_08004938(u32 size);
void *Func_08004970(u32 size);
void Func_08002df0(const void *address);
void Func_0800a37c(void);
void Func_0800fac8(void);

extern u8 Data_02010000[];
extern u8 Value_0000009c;

void Func_0800fac8(void)
{
    u32 workspace_size = 0x8000;
    void *workspace;
    u32 module_size;
    void *module;
    u32 control;
    volatile DmaTransfer *dma;

    workspace = Func_08004970(workspace_size);
    {
        WordCopy copy = (WordCopy)0x03001388;
        const void *copy_source = (const void *)0x02010000;

        copy(workspace, copy_source, workspace_size);
    }

    module_size = (u32)&Value_0000009c;
    module = Func_08004938(module_size);

    control = 0x84000000;
    control |= module_size >> 2;
    dma = (volatile DmaTransfer *)0x040000d4;
    dma->source = (const void *)Func_0800a37c;
    dma->destination = module;
    dma->control = control;

    {
        const void *transform_source = Data_02010000;

        ((CopiedTransform)module)(
            (void *)0x02018000,
            transform_source,
            workspace,
            dma
        );
    }

    Func_08002df0(module);
    Func_08002df0(workspace);
}
