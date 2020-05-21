// SPDX-License-Identifier: BSD-3-Clause
/* Copyright 2020, Intel Corporation */

/*
 * mocks_pmem2.c -- mocked pmem2 functions used
 *                  indirectly in pmem2_badblock_mocks.c
 */

#include <ndctl/libndctl.h>
#include "unittest.h"
#include "out.h"
#include "extent.h"
#include "pmem2_utils.h"
#include "pmem2_badblock_mocks.h"

/*
 * pmem2_region_namespace - mock pmem2_region_namespace
 */
FUNC_MOCK(pmem2_region_namespace, int,
		struct ndctl_ctx *ctx,
		enum pmem2_file_type ftype,
		dev_t st_rdev,
		struct ndctl_region **pregion,
		struct ndctl_namespace **pndns)
FUNC_MOCK_RUN_DEFAULT {
	UT_ASSERTne(pregion, NULL);
	*pregion = NULL;

	if (pndns)
		*pndns = NULL;

	if (ftype == PMEM2_FTYPE_DIR) {
		ERR("cannot check region or namespace of a directory");
		return PMEM2_E_INVALID_FILE_TYPE;
	}

	UT_ASSERT(ftype == PMEM2_FTYPE_REG || ftype == PMEM2_FTYPE_DEVDAX);

	*pregion = (void *)st_rdev;
	if (pndns == NULL)
		return 0;

	if (IS_MODE_NO_DEVICE(st_rdev)) { /* no matching device */
		/* did not found any matching device */
		*pndns = NULL;
		return 0;
	}

	*pndns = (void *)st_rdev;

	return 0;
}
FUNC_MOCK_END

/*
 * pmem2_extents_create_get -- allocate extents structure and get extents
 *                             of the given file
 */
FUNC_MOCK(pmem2_extents_create_get, int,
		int fd, struct extents **exts)
FUNC_MOCK_RUN_DEFAULT {
	return get_extents(fd, exts);
}
FUNC_MOCK_END
