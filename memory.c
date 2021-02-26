#include "shared.h"

EFI_STATUS EFIAPI MemoryInit(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
    UINTN MemoryMapSize = 0, MapKey = 0, DescriptorSize = 0;
    UINT32 DesVersion = 0;
    EFI_MEMORY_DESCRIPTOR *MemoryMap = 0;
    int i = 0;

    gBS->GetMemoryMap(&MemoryMapSize, MemoryMap, &MapKey, &DescriptorSize, &DesVersion);
    gBS->AllocatePool(EfiRuntimeServicesData, MemoryMapSize, (VOID **)&MapKey);
    gBS->GetMemoryMap(&MemoryMapSize, MemoryMap, &MapKey, &DescriptorSize, &DesVersion);

    for (i = 0; i < MemoryMapSize / DescriptorSize; i++)
    {
        EFI_MEMORY_DESCRIPTOR *MemMap = (EFI_MEMORY_DESCRIPTOR *) (((CHAR8 *)MemoryMapSize + i * DescriptorSize));
        Print(L"MemoryMap %4d %10d (%10lx~%10lx) %016lx\n", MemMap->Type, MemMap->NumberOfPages, \
        MemMap->PhysicalStart, MemMap->PhysicalStart + (MemMap->NumberOfPages << 12), MemMap->Attribute);
    }

    gBS->FreePool(MemoryMap);

    Print(L"Memory Module Finished\n\n");
    return EFI_SUCCESS;
}