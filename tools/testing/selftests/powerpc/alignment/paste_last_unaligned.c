/*
 * Copyright 2016, Chris Smart, IBM Corporation.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 *
 * Calls to paste_last which are not 128-byte aligned should be
 * caught and sent a SIGBUS.
 *
 */

#include <string.h>
#include <unistd.h>
#include "utils.h"
#include "instructions.h"
#include "copy_paste_unaligned_common.h"

unsigned int expected_instruction = PPC_INST_PASTE_LAST;
unsigned int instruction_mask = 0xfc2007ff;

int test_paste_last_unaligned(void)
{
	/* Only run this test on a P9 or later */
	SKIP_IF(!have_hwcap2(PPC_FEATURE2_ARCH_3_00));

	/* Register our signal handler with SIGBUS */
	setup_signal_handler();

	copy(cacheline_buf);

	/* +1 makes buf unaligned */
	paste_last(cacheline_buf+1);

	/* We should not get here */
	return 1;
}

int main(int argc, char *argv[])
{
	return test_harness(test_paste_last_unaligned, "test_paste_last_unaligned");
}
