// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2018, Bin Meng <bmeng.cn@gmail.com>
 */

#include <common.h>
#include <splash.h>
#include <init.h>
#include <smbios.h>
#include <asm/cb_sysinfo.h>
#include <asm/global_data.h>

DECLARE_GLOBAL_DATA_PTR;

int board_early_init_r(void)
{
	/*
	 * Make sure PCI bus is enumerated so that peripherals on the PCI bus
	 * can be discovered by their drivers
	 */
	pci_init();

	return 0;
}

#ifdef CONFIG_SMBIOS_PARSER
int show_board_info(void)
{
	const struct smbios_entry *smbios = smbios_entry(lib_sysinfo.smbios_start, lib_sysinfo.smbios_size);

	if (!smbios)
		goto fallback;

	const struct smbios_header *bios = smbios_header(smbios, SMBIOS_BIOS_INFORMATION);
	const struct smbios_header *system = smbios_header(smbios, SMBIOS_SYSTEM_INFORMATION);
	const struct smbios_type0 *t0 = (struct smbios_type0 *)bios;
	const struct smbios_type1 *t1 = (struct smbios_type1 *)system;

	if (!t0 || !t1)
		goto fallback;

	const char *bios_ver = smbios_string(bios, t0->bios_ver);
	const char *bios_date = smbios_string(bios, t0->bios_release_date);
	const char *model = smbios_string(system, t1->product_name);
	const char *manufacturer = smbios_string(system, t1->manufacturer);

	if (!model || !manufacturer || !bios_ver)
		goto fallback;

	printf("Vendor: %s\n", manufacturer);
	printf("Model: %s\n", model);
	printf("BIOS Version: %s\n", bios_ver);
	if (bios_date)
		printf("BIOS date: %s\n", bios_date);

	return 0;

fallback:
	if (IS_ENABLED(CONFIG_OF_CONTROL)) {
		model = fdt_getprop(gd->fdt_blob, 0, "model", NULL);

		if (model)
			printf("Model: %s\n", model);
	}

	return checkboard();
}
#endif

static struct splash_location coreboot_splash_locations[] = {
	{
		.name = "virtio_fs",
		.storage = SPLASH_STORAGE_VIRTIO,
		.flags = SPLASH_STORAGE_RAW,
		.devpart = "0",
	},
};

int splash_screen_prepare(void)
{
	return splash_source_load(coreboot_splash_locations,
				  ARRAY_SIZE(coreboot_splash_locations));
}
