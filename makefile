
MAKEFLAGS += --no-print-directory

IMAGE = /dev/disk/by-uuid/3AEE-1803
IMAGEMOUNTPATH = /mnt
BUILDPATH = ~/edk2/Build/AppPkg/DEBUG_GCC5/X64/AppPkg/Applications/ukrnldr/ukrnldr/OUTPUT
DEMOPATH = ~/edk2/Build/AppPkg/DEBUG_GCC5/X64/AppPkg/Applications/Test/Test/OUTPUT

DD = dd
DDFLAGS = conv=notrunc 2>> build.log
SUDO = sudo

.PHONY: clean clean_backup image mount umount image_main
	
install:
	@$(MAKE) -C . image
	
image:
	@$(MAKE) -C . mount
	-@$(MAKE) -C . image_main
	@$(MAKE) -C . umount
	
mount:
	$(SUDO) mount $(IMAGE) $(IMAGEMOUNTPATH)
	
umount:
	$(SUDO) umount $(IMAGEMOUNTPATH)
	
image_main:
	#@$(MAKE) -C boot image_main
	cp $(BUILDPATH)/ukrnldr.efi ~/bootx64.efi
	cp $(DEMOPATH)/Test.efi ~/krnlx64.efi
	$(SUDO) mv ~/bootx64.efi /mnt/EFI/BOOT/bootx64.efi
	
clean:
	@$(MAKE) -C . clean_backup
	
clean_backup:
	rm -rf *~ *.swp *.swo *.swa *.log
	
push:
	./.git.sh
	
