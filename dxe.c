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

// vim:fenc=utf-8:tw=75:noet
