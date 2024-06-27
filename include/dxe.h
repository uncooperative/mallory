// SPDX-License-Identifier: BSD-2-Clause-Patent
/*
 * dxe.h - support for DXE services we may use
 * Copyright Peter Jones <pjones@redhat.com>
 */

#ifndef SHIM_DXE_H_
#define SHIM_DXE_H_

extern void init_dxe(EFI_SYSTEM_TABLE *systab);
extern EFI_STATUS dxe_update_mem_attrs(EFI_PHYSICAL_ADDRESS physaddr,
                                       size_t size, UINT64 set_attrs,
                                       UINT64 clear_attrs);

#endif /* !SHIM_DXE_H_ */
// vim:fenc=utf-8:tw=75:noet
