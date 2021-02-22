
#include "shared.h"

EFI_STATUS EFIAPI UefiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{

	VideoInit(ImageHandle, SystemTable);
	MemoryInit(ImageHandle, SystemTable);

	for (;;);

	return EFI_SUCCESS;
}




