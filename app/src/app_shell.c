/*
 * Copyright (c) 2024  Callender-Consulting LLC
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/sys/printk.h>
#include <zephyr/shell/shell.h>
#include <zephyr/drivers/uart.h>

#include "app_shell.h"

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(app_shell, LOG_LEVEL_INF);

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/

//static const struct device * const device =
//                  DEVICE_DT_GET_OR_NULL(DT_NODELABEL(uart0));

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
static int cmd_shell_info(const struct shell *sh, size_t argc, char *argv[])
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    shell_print(sh, "** Welcome to PICO-W App");
    shell_print(sh, "** Built on %s at %s", __DATE__, __TIME__);
    shell_print(sh, "** Board '%s'", CONFIG_BOARD);

    return 0;
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
static int cmd_shell_test(const struct shell *sh, size_t argc, char *argv[])
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    char * string = (char*) argv[1];

    shell_print(sh, "test: %s\n", string);

    return 0;
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/

SHELL_STATIC_SUBCMD_SET_CREATE(app_cmds,
    SHELL_CMD_ARG(test, NULL, "App test <string>", cmd_shell_test, 2, 0),
    SHELL_CMD(info,     NULL, "App info", cmd_shell_info),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(app, &app_cmds, "App commands", NULL);
