
#include "shared.h"

EFI_STATUS EFIAPI UefiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
	UINTN PosX = 0, PosY = 0;

	VideoInit(ImageHandle, SystemTable, &PosX, &PosY);
	MemoryInit(ImageHandle, SystemTable, &PosX, &PosY);

	for (;;);

	return EFI_SUCCESS;
}
