#define STATIC_ERLANG_NIF 1
#define CONFIGURE_SHELL_COMMANDS_INIT
#define CONFIGURE_SHELL_COMMANDS_ALL
#define CONFIGURE_SHELL_MOUNT_MSDOS
#define CONFIGURE_SHELL_MOUNT_NFS
#define CONFIGURE_SHELL_COMMAND_LS

#include <rtems/shellconfig.h>
#include <rtems/shell.h>

#include <erl_nif.h>


void start_shell_nif(void)
{
    printf(" =========================\n");
    printf(" starting shell\n");
    printf(" =========================\n");
    rtems_shell_init(
        "SHLL",                       /* task name */
        RTEMS_MINIMUM_STACK_SIZE * 4, /* task stack size */
        100,                          /* task priority */
        "/dev/console",               /* device name */
        false,                        /* run forever */
        true,                         /* wait for shell to terminate */
        rtems_shell_login_check       /* login check function,
        use NULL to disable a login check */
    );
}

static ErlNifFunc nif_funcs[] =
{
    {"start_shell_nif", 0, start_shell_nif}
};

ERL_NIF_INIT(rtemsshell, nif_funcs, NULL, NULL, NULL, NULL)
