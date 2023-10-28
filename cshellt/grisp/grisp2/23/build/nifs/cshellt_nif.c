/*IMPORTANTES EN TODOS OS ARCHIVOS*/
#define STATIC_ERLANG_NIF 1

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <inttypes.h>

/*********************************/

#include <rtems/cpuuse.h>
#include <rtems/printer.h>
#include <rtems/score/threadimpl.h>
#include <rtems/score/todimpl.h>

#include "/home/mvcampos/TFG/grisp2-rtems-toolchain/external/rtems/cpukit/libmisc/cpuuse/cpuuseimpl.h"


#include "/home/mvcampos/TFG/grisp2-rtems-toolchain/external/rtems/cpukit/libmisc/cpuuse/cpuusagereport.c"

/**********************************************************************************************************/

#include <erl_nif.h>

typedef struct {
  Timestamp_Control    total;
  Timestamp_Control    uptime_at_last_reset;
} cpu_usage_context_mod;


typedef struct {
  uint32_t           id;
  char               name[ 38 ];
  uint32_t           ival;
  uint32_t           fval;
  Timestamp_Control  uptime;
  Timestamp_Control  used;
  uint32_t           seconds;
  uint32_t           nanoseconds;

} info_cpu;

//info_cpu total[100];

int counter = 0;
char out_log [100000] = "";


static bool cpu_usage_visitor_mod( Thread_Control *the_thread, void *arg )
{
  cpu_usage_context_mod *ctx;
  char               name[ 38 ];
  uint32_t           ival;
  uint32_t           fval;
  Timestamp_Control  uptime;
  Timestamp_Control  used;
  uint32_t           seconds;
  uint32_t           nanoseconds;
  char str_buffer[1000];


  /*uint32_t           seconds;
  uint32_t           nanoseconds;*/

  ctx = arg;

  /*total[counter].id = the_thread->Object.id;
  _Thread_Get_name( the_thread, total[counter].name, sizeof( total[counter].name ) );

  _Thread_Get_CPU_time_used( the_thread, &total[counter].used );
  _TOD_Get_uptime( &total[counter].uptime );
  _Timestamp_Subtract( &ctx->uptime_at_last_reset, &total[counter].uptime, &ctx->total );
  _Timestamp_Divide( &total[counter].used, &ctx->total, &total[counter].ival, &total[counter].fval );
  total[counter].seconds = _Timestamp_Get_seconds( &total[counter].used );
  total[counter].nanoseconds = _Timestamp_Get_nanoseconds( &total[counter].used ) /
    TOD_NANOSECONDS_PER_MICROSECOND;*/

  _Thread_Get_name( the_thread, name, sizeof( name ) );

  _Thread_Get_CPU_time_used( the_thread, &used );
  _TOD_Get_uptime( &uptime );
  _Timestamp_Subtract( &ctx->uptime_at_last_reset, &uptime, &ctx->total );
  _Timestamp_Divide( &used, &ctx->total, &ival, &fval );
  seconds = _Timestamp_Get_seconds( &used );
  nanoseconds = _Timestamp_Get_nanoseconds( &used ) /
    TOD_NANOSECONDS_PER_MICROSECOND;

  /*rtems_printf(
    ctx->printer,
    " 0x%08" PRIx32 " | %-38s |"
      "%7" PRIu32 ".%06" PRIu32 " |%4" PRIu32 ".%03" PRIu32 "\n",
    the_thread->Object.id,
    name,
    seconds, nanoseconds,
    ival, fval
  );*/

  sprintf(str_buffer,
    " 0x%08" PRIx32 " | %-38s |"
      "%7" PRIu32 ".%06" PRIu32 " |%4" PRIu32 ".%03" PRIu32 "\n",
    the_thread->Object.id,
    name,
    seconds, nanoseconds,
    ival, fval);
  
  strcat(out_log, str_buffer);


  counter = counter + 1;

  return false;
}

/*
 *  rtems_cpu_usage_report
 */
void rtems_cpu_usage_report_with_plugin_mod()
{
  cpu_usage_context_mod  ctx;
  /*uint32_t           seconds;
  uint32_t           nanoseconds;*/


  /*
   *  When not using nanosecond CPU usage resolution, we have to count
   *  the number of "ticks" we gave credit for to give the user a rough
   *  guideline as to what each number means proportionally.
   */
  _Timestamp_Set_to_zero( &ctx.total );
  ctx.uptime_at_last_reset = CPU_usage_Uptime_at_last_reset;

  /*rtems_printf(
     printer,
     "-------------------------------------------------------------------------------\n"
     "                              CPU USAGE BY THREAD\n"
     "------------+----------------------------------------+---------------+---------\n"
     " ID         | NAME                                   | SECONDS       | PERCENT\n"
     "------------+----------------------------------------+---------------+---------\n"
  );*/

  rtems_task_iterate( cpu_usage_visitor_mod, &ctx );

  /*seconds = _Timestamp_Get_seconds( &ctx.total );
  nanoseconds = _Timestamp_Get_nanoseconds( &ctx.total ) /
    TOD_NANOSECONDS_PER_MICROSECOND;*/
  /*rtems_printf(
     printer,
     "------------+----------------------------------------+---------------+---------\n"
     " TIME SINCE LAST CPU USAGE RESET IN SECONDS:                    %7" PRIu32 ".%06" PRIu32 "\n"
     "-------------------------------------------------------------------------------\n",
     seconds, nanoseconds
  );*/

  /*for (i = 0; i < counter; i++) {
    rtems_printf(printer1,
      " 0x%08" PRIx32 " | %-38s |"
      "%7" PRIu32 ".%06" PRIu32 " |%4" PRIu32 ".%03" PRIu32 "\n",
    total[i].id,
    total[i].name,
    total[i].seconds, total[i].nanoseconds,
    total[i].ival, total[i].fval
    );
  }*/

  //rtems_printf(printer1, "BUFFER  = %s\n", out_log);


}

static ERL_NIF_TERM commands_nif (ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
  /*rtems_printer printer1;


  rtems_print_printer_printk( &printer1 );
  rtems_cpu_usage_report_with_plugin_mod( &printer1 );*/

  rtems_cpu_usage_report_with_plugin_mod();

  return enif_make_string(env, out_log, ERL_NIF_LATIN1);
  
}

/**********************************************************************************************************/


static ErlNifFunc nif_funcs[] =
{
    {"commands_nif", 0, commands_nif}
};

ERL_NIF_INIT(cshellt, nif_funcs, NULL, NULL, NULL, NULL)
