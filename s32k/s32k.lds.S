/* SPDX-License-Identifier: MIT */
/*
 * Author: Andreas Werner <kernel@andy89.org>
 * Date: 2016
 */
#include <linker.h>
#include <driver.h>

ENTRY(reset_handler)

MEM_START
#ifdef CONFIG_S32K116
#error Mem Region not implemented by now
	MEM_ADD(flash, 0x0, 128K)
	MEM_ADD(sram, 0x1FFFFC00, 17K-2K)
#endif
#ifdef CONFIG_S32K118
#error Mem Region not implemented by now
	MEM_ADD(flash, 0x0, 265K)
	MEM_ADD(sram, 0x1FFF8000, 25K-2K)
#endif
#ifdef CONFIG_S32K142
	MEM_ADD(flash, 0x0, 256K)
	MEM_ADD(sram, 0x1FFFC000, 32K-4K)
#endif
#ifdef CONFIG_S32K144
	MEM_ADD(flash, 0x0, 512K)
	MEM_ADD(sram, 0x1FFF8000, 64K-4K)
#endif
#ifdef CONFIG_S32K146
	MEM_ADD(flash, 0x0, 1M)
	MEM_ADD(sram, 0x1FFF0000, 128K-4K)
#endif
#ifdef CONFIG_S32K148
	MEM_ADD(flash, 0x0, 2M)
	MEM_ADD(sram, 0x1FFE0000, 256K-4K)
#endif
MEM_STOP

SECTIONS_START
	. = ORIGIN(flash);
	VECTOR_START
		VECTOR_DEFAULT
		. = 0x400;
		*(.flashconfig)
		. = ALIGN(0x4);
	VECTOR_STOP(flash)

	TEXT_START
		DATA_TABLE_START
			DATA_TABLE(.data)
		DATA_TABLE_STOP
		. = ALIGN(0x4);
		TEXT_DEFAULT
	TEXT_STOP(flash)

	DEV_DEFAULT(flash)

	RODATA_START
		RODATA_DEFAULT
	RODATA_STOP(flash)

	DATA_START
		DATA_DEFAULT
	DATA_STOP(sram, flash)

	BSS_START
		BSS_DEFAULT
	BSS_STOP(sram)

	HEAP(sram, CONFIG_MACH_INIT_STACK_SIZE)

	STACK(sram)
SECTIONS_STOP
