#include "shared.h"

#include <Protocol/DevicePathToText.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/LoadedImage.h>
#include <Guid/FileInfo.h>
#include <Library/DevicePathLib.h>

EFI_STATUS PrintNode(EFI_DEVICE_PATH_PROTOCOL * Node)
{
	Print(L"(%d %d)/", Node->Type, Node->SubType);
	return 0;
}

EFI_DEVICE_PATH_PROTOCOL* WalkthroughDevicePath(EFI_DEVICE_PATH_PROTOCOL* DevPath, EFI_STATUS (*Callbk)(EFI_DEVICE_PATH_PROTOCOL*))
{
	EFI_DEVICE_PATH_PROTOCOL* pDevPath = DevPath;
	while(!IsDevicePathEnd (pDevPath))
	{
		if(Callbk)
		{
			EFI_STATUS Status = Callbk(pDevPath);
			if(Status != 0)
			{
				if(Status < 0) pDevPath = NULL;
				break;
			}
		}
		pDevPath = NextDevicePathNode (pDevPath);
	}
	return pDevPath;
}

EFI_STATUS EFIAPI LoaderInit(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
	Print(L"Debug Started 0000\n");

	EFI_LOADED_IMAGE        *LoadedImage;
	EFI_DEVICE_PATH         *DevicePath;
	EFI_FILE_IO_INTERFACE   *Vol;
	EFI_FILE_HANDLE         RootFs;
	EFI_FILE_HANDLE         FileHandle;

	EFI_DEVICE_PATH_TO_TEXT_PROTOCOL* Device2TextProtocol = 0;

	gBS->LocateProtocol(&gEfiDevicePathToTextProtocolGuid, NULL, (VOID **)&Device2TextProtocol);
	gBS->HandleProtocol(ImageHandle, &gEfiLoadedImageProtocolGuid, (VOID *)&LoadedImage);
	gBS->HandleProtocol(LoadedImage->DeviceHandle, &gEfiDevicePathProtocolGuid, (VOID *)&DevicePath);

	CHAR16* TextDevicePath = Device2TextProtocol->ConvertDevicePathToText(DevicePath, FALSE, TRUE);
	Print(L"%s\n", TextDevicePath);
	if(TextDevicePath)
		gBS->FreePool(TextDevicePath);
	WalkthroughDevicePath(DevicePath, PrintNode);
	Print(L"\n");

	gBS->HandleProtocol(LoadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (VOID *)&Vol);
	Vol->OpenVolume(Vol, &RootFs);
//	RootFs->Open(RootFs, &FileHandle, (CHAR16 *)L"kernel.bin", EFI_FILE_MODE_READ, 0);
	RootFs->Open(RootFs, &FileHandle, (CHAR16 *)L"EFI/BOOT/bootx64.efi", EFI_FILE_MODE_READ, 0);

	EFI_FILE_INFO *FileInfo;
	UINTN BufferSize = 0;
	EFI_PHYSICAL_ADDRESS pages = 0x100000;

	BufferSize = sizeof(EFI_FILE_INFO) + sizeof(CHAR16) * 100;
	gBS->AllocatePool(EfiRuntimeServicesData, BufferSize, (VOID **)&FileInfo);
	FileHandle->GetInfo(FileHandle, &gEfiFileInfoGuid, &BufferSize, FileInfo);
	Print(L"FileName: %s Size: %d FileSize: %d Physical Size: %d\n", FileInfo->FileName, FileInfo->Size, FileInfo->FileSize, FileInfo->PhysicalSize);

	gBS->AllocatePages(AllocateAddress, EfiLoaderData, (FileInfo->FileSize + 0x1000 - 1) / 0x1000, &pages);
	BufferSize = FileInfo->FileSize;
	FileHandle->Read(FileHandle, &BufferSize, (VOID *)pages);
	gBS->FreePool(FileInfo);
	FileHandle->Close(FileHandle);
	RootFs->Close(RootFs);

	gBS->CloseProtocol(LoadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, ImageHandle, NULL);
	gBS->CloseProtocol(LoadedImage->DeviceHandle, &gEfiDevicePathProtocolGuid, ImageHandle, NULL);
	gBS->CloseProtocol(ImageHandle, &gEfiLoadedImageProtocolGuid, ImageHandle, NULL);
	gBS->CloseProtocol(Device2TextProtocol, &gEfiDevicePathToTextProtocolGuid, ImageHandle, NULL);
	Print(L"Loader Module Finished\n\n");
	return EFI_SUCCESS;
}