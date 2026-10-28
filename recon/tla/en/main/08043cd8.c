// Draft, differ 10 of 749 in tla-en under -mthumb-slow-loads (2026-10-26). A
// device search (each statement followed by an empty asm or a one-pass block,
// wrapped in one, or swapped with the next, kept when it helps) reaches 8 with
// an empty asm after `summaryDirty = 0;`; the volatile reads made plain, and
// finds nothing closer.
// Near miss, differ 14 of 749: the save file menu, The Broken Seal's
// SaveMenu_SelectSlot with this game's modes: 1 skips files whose send flag
// is set, 4 files with no flag count, 5 files whose send flag is clear and 6
// files with the flag at 51 set; 2 and 3 skip empty files and 0 takes any.
// The file list is written as in The Broken Seal's Japanese layout (i * 16,
// save->summary[i] beside the name pointer) and y is reused for the four
// Djinn sprites' x, which keeps it a separate register. Left, instruction
// order only: the native finishes the name pointer and stores it before it
// builds the level pointer after the file list window is made, stores the
// cleared redraw flag before it builds the file's offset, and loads
// gAdventure before it builds 0x225 at both glow restores.
#include "types.h"
#include "ram_buffer.h"
#include "save.h"
#include "adventure.h"
#include "uiwindow.h"
#include "input.h"
#include "dma.h"
#include "io_reg.h"

void *HeapSlot_Alloc(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
u32 BuildStamp_GetTimeFar(void);
void PaletteGlow_Start(void);
void PaletteGlow_Stop(void);
struct Window *Window_Create(s32 x, s32 y, s32 width, s32 height,
                                 s32 style);
void Text_DrawMessage(s32 resource, struct Window *window, s32 x, s32 y);
void Text_DrawString(u8 *text, struct Window *window, s32 x, s32 y);
void *RenderResource_CreatePair(s32 mode, struct Window *window, s32 x,
                                s32 y);
void Func_080f8070(void);
void Func_080f8078(void);
void Func_080f8080(s32 shown);
void Func_080f8090(s32 index, s32 x, s32 y);
void wait(s32 frames);
void Func_0803f9c0(s32 glow, s32 sub);
void StatusMenu_DrawCharacterSummary(struct Window *window,
                                     struct SaveSummary *summary);
void Menu_ClearFirstObjectRowAndScheduleUpdate(void);
void Func_08043a64(struct Window *window, s32 x, s32 y,
                   struct SaveSummary *summary);
void FileList_DrawDjinnCounts(struct Window *window,
                                 struct SaveSummary *summary);
void Window_Close(void *window, s32 mode);
void WindowSprite_RedrawSavedRect(struct Window *window);
void Window_DrawDividerLine(struct Window *window, s32 x0, s32 y0, s32 x1,
                              s32 y1);
void Window_FillTilemapRect(struct Window *window, s32 x, s32 y, s32 width,
                              s32 height);
void Func_08045018(void *pair);
void Sound_PlayFar(s32 cue);

// Messages 0 to 4 and 3672, the first area name.
extern u8 MsgSaveNoData[];
extern u8 MsgSaveContinue[];
extern u8 MsgSaveNotCleared[];
extern u8 MsgSaveCorrupted[];
extern u8 MsgSavePostReunion[];
extern u8 MsgTitleScreen[];

#define SAVE_FILE_COUNT 3
#define SAVE_MENU_NO_FILE -2

s32 Func_08043cd8(s32 selectedFile, s32 menuMode) {
  struct Window *fileList;
  struct SaveWorkspace *save;
  struct Window *summaryWindow;
  struct Window *partyWindow;
  struct Window *djinnWindow;
  struct SaveSummary *summary;
  void *modePicture;
  s32 summaryDirty;
  u32 buildStamp;
  void *spriteWork;
  s32 result;
  s32 i;
  s32 y;

  spriteWork = HeapSlot_Alloc(220, 0xcec);
  save = Ram_HeapSlots->serialWork;
  summaryWindow = NULL;
  partyWindow = NULL;
  djinnWindow = NULL;
  summaryDirty = 1;
  buildStamp = BuildStamp_GetTimeFar();
  if (selectedFile < 0)
    selectedFile = 0;
  if (menuMode == 1) {
    for (i = 0;
         i < SAVE_FILE_COUNT && (save->summary[selectedFile].level == 0 ||
                                 save->summary[selectedFile].sendFlag != 0);
         i++) {
      selectedFile++;
      if (selectedFile > 2)
        selectedFile = 0;
    }
    if (i == SAVE_FILE_COUNT) {
      result = SAVE_MENU_NO_FILE;
      goto done;
    }
  } else if (menuMode == 4) {
    for (i = 0;
         i < SAVE_FILE_COUNT && (save->summary[selectedFile].level == 0 ||
                                 save->summary[selectedFile].flagCount == 0);
         i++) {
      selectedFile++;
      if (selectedFile == SAVE_FILE_COUNT)
        selectedFile = 0;
    }
    if (i == SAVE_FILE_COUNT) {
      result = SAVE_MENU_NO_FILE;
      goto done;
    }
  } else if (menuMode == 5) {
    for (i = 0;
         i < SAVE_FILE_COUNT && (save->summary[selectedFile].level == 0 ||
                                 save->summary[selectedFile].sendFlag == 0);
         i++) {
      selectedFile++;
      if (selectedFile > 2)
        selectedFile = 0;
    }
    if (i == SAVE_FILE_COUNT) {
      result = SAVE_MENU_NO_FILE;
      goto done;
    }
  } else if (menuMode == 6) {
    for (i = 0;
         i < SAVE_FILE_COUNT && (save->summary[selectedFile].level == 0 ||
                                 save->summary[selectedFile].hasFlag34 != 0);
         i++) {
      selectedFile++;
      if (selectedFile > 2)
        selectedFile = 0;
    }
    if (i == SAVE_FILE_COUNT) {
      result = SAVE_MENU_NO_FILE;
      goto done;
    }
  } else if (menuMode != 0) {
    for (i = 0; i < SAVE_FILE_COUNT && save->summary[selectedFile].level == 0;
         i++) {
      selectedFile++;
      if (selectedFile > 2)
        selectedFile = 0;
    }
    if (i == SAVE_FILE_COUNT) {
      result = SAVE_MENU_NO_FILE;
      goto done;
    }
  }

  Dma_Fill32(0, spriteWork, 0xcec);
  PaletteGlow_Start();
  fileList = Window_Create(1, 2, 28, 7, 2);
  for (i = 0, summary = save->summary; i < SAVE_FILE_COUNT; i++, summary++) {
    if (save->summary[i].level == 0) {
      Text_DrawMessage((s32)MsgSaveNoData, fileList, 10, i * 16);
    } else if (save->summary[i].frames < buildStamp) {
      Text_DrawMessage((s32)MsgSaveContinue, fileList, 10, i * 16);
    } else if (save->summary[i].playTime != *(u32 *)save->summary[i].unk38) {
      Text_DrawMessage((s32)MsgSaveCorrupted, fileList, 10, i * 16);
    } else if (menuMode == 5 && save->summary[i].sendFlag == 0) {
      Text_DrawMessage((s32)MsgSaveNotCleared, fileList, 10, i * 16);
    } else if (menuMode == 6 && save->summary[i].hasFlag34 != 0) {
      Text_DrawMessage((s32)MsgSavePostReunion, fileList, 10, i * 16);
    } else {
      y = i * 16;
      Text_DrawString(summary->name, fileList, 12, y);
      Text_DrawMessage(save->summary[i].area + (s32)MsgTitleScreen, fileList,
                          72, y);
    }
  }
  modePicture = RenderResource_CreatePair(menuMode, fileList, 72, -24);
  Func_080f8070();
  for (i = 0, y = 144; i < 4; i++, y += 24)
    Func_080f8090(i, y, 128);
  Func_080f8080(0);
  wait(1);

  while (1) {
    if (summaryDirty != 0) {
      summaryDirty = 0;
      if (save->summary[selectedFile].level != 0) {
        Func_0803f9c0(save->summary[selectedFile].paletteGlow,
                      save->summary[selectedFile].paletteGlowSub);
        if (summaryWindow == NULL)
          summaryWindow = Window_Create(1, 10, 14, 9, 2);
        StatusMenu_DrawCharacterSummary(summaryWindow,
                                        &save->summary[selectedFile]);
        wait(1);
        if (partyWindow == NULL)
          partyWindow = Window_Create(16, 10, 13, 3, 2);
        Menu_ClearFirstObjectRowAndScheduleUpdate();
        Func_08043a64(partyWindow, 0, 0, &save->summary[selectedFile]);
        wait(1);
        if ((s8)save->summary[selectedFile].djinn[0] +
                (s8)save->summary[selectedFile].djinn[1] +
                (s8)save->summary[selectedFile].djinn[2] +
                (s8)save->summary[selectedFile].djinn[3] !=
            0) {
          if (djinnWindow == NULL)
            djinnWindow = Window_Create(16, 14, 13, 5, 2);
          FileList_DrawDjinnCounts(djinnWindow,
                                      &save->summary[selectedFile]);
          Func_080f8080(1);
        } else {
          Func_080f8080(0);
          Window_Close(djinnWindow, 2);
          djinnWindow = NULL;
        }
      } else {
        Func_0803f9c0(gAdventure.unk220[5], gAdventure.unk220[6]);
        Func_080f8080(0);
        Menu_ClearFirstObjectRowAndScheduleUpdate();
        Window_Close(djinnWindow, 2);
        Window_Close(partyWindow, 2);
        Window_Close(summaryWindow, 2);
        djinnWindow = NULL;
        partyWindow = NULL;
        summaryWindow = NULL;
      }
      WindowSprite_RedrawSavedRect(fileList);
      Window_DrawDividerLine(fileList, 0, 2, 27, 2);
      Window_DrawDividerLine(fileList, 0, 4, 27, 4);
      Window_FillTilemapRect(fileList, 0, selectedFile * 2, 26, 1);
    }
    Func_08045018(modePicture);
    wait(1);
    if (gInput.repeat & 0x40) {
      Sound_PlayFar(111);
      summaryDirty = 1;
      do {
        selectedFile = (selectedFile + SAVE_FILE_COUNT - 1) % SAVE_FILE_COUNT;
        if (menuMode == 0)
          break;
      } while (save->summary[selectedFile].level == 0 ||
               (menuMode == 1 && save->summary[selectedFile].sendFlag != 0) ||
               (menuMode == 4 && save->summary[selectedFile].flagCount == 0) ||
               (menuMode == 5 && save->summary[selectedFile].sendFlag == 0) ||
               (menuMode == 6 && save->summary[selectedFile].hasFlag34 != 0));
    } else if (gInput.repeat & 0x80) {
      Sound_PlayFar(111);
      summaryDirty = 1;
      do {
        selectedFile = (selectedFile + SAVE_FILE_COUNT + 1) % SAVE_FILE_COUNT;
        if (menuMode == 0)
          break;
      } while (save->summary[selectedFile].level == 0 ||
               (menuMode == 1 && save->summary[selectedFile].sendFlag != 0) ||
               (menuMode == 4 && save->summary[selectedFile].flagCount == 0) ||
               (menuMode == 5 && save->summary[selectedFile].sendFlag == 0) ||
               (menuMode == 6 && save->summary[selectedFile].hasFlag34 != 0));
      /* FAKEMATCH: the native reads the pressed keys again for each test;
       * read plainly the second test reuses the first load (differ 26). */
    } else if (((volatile struct InputState *)&gInput)->pressed & 2) {
      Sound_PlayFar(113);
      result = -1;
      break;
    } else if (((volatile struct InputState *)&gInput)->pressed & 1) {
      Sound_PlayFar(112);
      result = selectedFile;
      break;
    }
  }
  Func_080f8078();
  Menu_ClearFirstObjectRowAndScheduleUpdate();
  Window_Close(djinnWindow, 2);
  Window_Close(partyWindow, 2);
  Window_Close(summaryWindow, 2);
  Window_Close(fileList, 2);
  PaletteGlow_Stop();
  HeapSlot_Release(220);
  Func_0803f9c0(gAdventure.unk220[5], gAdventure.unk220[6]);
  wait(1);
done:
  return result;
}
