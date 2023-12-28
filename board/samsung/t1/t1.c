// SPDX-License-Identifier: GPL-2.0+

void nooop(void) {}
void set_muxconf_regs(void) {}
struct omap_sysinfo {
	char *board_string;
};
const struct omap_sysinfo sysinfo = {
	"Board: Samsung Galaxy S II (GT-I9100G)\n"
};
int board_init(void) { return 0; }
int misc_init_r(void) { return 0; }
