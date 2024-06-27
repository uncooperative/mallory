// SPDX-License-Identifier: BSD-2-Clause-Patent
/*
 * dxe.c - support for DXE services we may use
 * Copyright Peter Jones <pjones@redhat.com>
 */

#include "shim.h"

void
init_dxe(EFI_SYSTEM_TABLE *systab)
{
	EFI_GUID dxe_services_guid = EFI_DXE_SERVICES_TABLE_GUID;
	unsigned int i;

	for (i = 0; i < systab->NumberOfTableEntries; i++) {
		EFI_CONFIGURATION_TABLE *ct = &systab->ConfigurationTable[i];
		if (CompareMem(&ct->VendorGuid, &dxe_services_guid, sizeof(EFI_GUID)) == 0) {
			EFI_DXE_SERVICES_TABLE *dst = (EFI_DXE_SERVICES_TABLE *)ct->VendorTable;
			if (dst->Hdr.Signature == EFI_DXE_SERVICES_TABLE_SIGNATURE) {
				dxe_services_table = dst;
				return;
			}
		}
	}
}

EFI_STATUS
dxe_update_mem_attrs(EFI_PHYSICAL_ADDRESS physaddr, size_t size,
		     UINT64 set_attrs, UINT64 clear_attrs)
{
	EFI_STATUS status;
	EFI_GCD_MEMORY_SPACE_DESCRIPTOR desc;
	EFI_PHYSICAL_ADDRESS start, end, next;

	start = ALIGN_DOWN(physaddr, EFI_PAGE_SIZE);
	end = ALIGN_UP(physaddr + size, EFI_PAGE_SIZE);

	if (!dxe_services_table)
		return EFI_UNSUPPORTED;

	if (!dxe_services_table->GetMemorySpaceDescriptor ||
	    !dxe_services_table->SetMemorySpaceAttributes)
		return EFI_UNSUPPORTED;

	for (; start < end; start = next) {
		EFI_PHYSICAL_ADDRESS mod_start;
		UINT64 mod_size;

		status = dxe_services_table->GetMemorySpaceDescriptor(start, &desc);
		if (EFI_ERROR(status))
			return status;

		next = desc.BaseAddress + desc.Length;

		if (desc.GcdMemoryType != EFI_GCD_MEMORY_TYPE_SYSTEM_MEMORY)
			continue;

		mod_start = MAX(start, desc.BaseAddress);
		mod_size = MIN(end, next) - mod_start;

		desc.Attributes |= set_attrs;
		desc.Attributes &= ~clear_attrs;

		status = dxe_services_table->SetMemorySpaceAttributes(mod_start, mod_size, desc.Attributes);
		if (EFI_ERROR(status))
			return status;

	}

	return EFI_SUCCESS;
}

// vim:fenc=utf-8:tw=75:noet
