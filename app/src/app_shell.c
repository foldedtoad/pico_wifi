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
#include <zephyr/net/sntp.h>
#include <zephyr/posix/time.h>

#include <stdio.h>
#include <time.h>

#ifdef CONFIG_POSIX_API
#include <arpa/inet.h>
#endif

#include "app_shell.h"

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(app_shell, LOG_LEVEL_INF);

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/

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
static const char * format_time(time_t time, long nsec)
{
    static char buf[48];
    char * bp = buf;
    char * const bpe = bp + sizeof(buf);
    struct tm tv;
    struct tm * tp = gmtime_r(&time, &tv);

    bp += strftime(bp, bpe - bp, "%Y-%m-%d %H:%M:%S", tp);
    bp += strftime(bp, bpe - bp, " %a %j", tp);

    return buf;
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
static int cmd_shell_time(const struct shell *sh, size_t argc, char *argv[])
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    #define SNTP_PORT   123
    #define SERVER_ADDR "132.163.96.4"  // NIST, Boulder, Colorado
    //                   129.6.15.26    // NIST, Gaithersburg, Maryland

    struct sntp_ctx ctx;
    struct sockaddr_in addr;
    struct sntp_time sntp_time;
    int rv;

    /* ipv4 */
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SNTP_PORT);
    zsock_inet_pton(AF_INET, SERVER_ADDR, &addr.sin_addr);

    rv = sntp_init(&ctx, (struct sockaddr *) &addr,
               sizeof(struct sockaddr_in));
    if (rv < 0) {
        LOG_ERR("Failed to init SNTP IPv4 ctx: %d", rv);
        goto end;
    }

    LOG_DBG("Sending SNTP IPv4 request...");
    rv = sntp_query(&ctx, 4 * MSEC_PER_SEC, &sntp_time);
    if (rv < 0) {
        LOG_ERR("SNTP IPv4 request failed: %d", rv);
        goto end;
    }

    LOG_DBG("Acquired time from NTP server: %u",
                (uint32_t)sntp_time.seconds);

    shell_print(sh, "\t%s", format_time(sntp_time.seconds, sntp_time.fraction));

end:
    sntp_close(&ctx);
    return 0;
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/

SHELL_STATIC_SUBCMD_SET_CREATE(app_cmds,
    SHELL_CMD(info,     NULL, "App info", cmd_shell_info),
    SHELL_CMD(time,     NULL, "App time", cmd_shell_time),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(app, &app_cmds, "App commands", NULL);
