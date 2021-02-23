#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>

EFI_STATUS EFIAPI VideoInit(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable, UINTN *PosX, UINTN *PosY);
EFI_STATUS EFIAPI MemoryInit(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable, UINTN *PosX, UINTN *PosY);

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif