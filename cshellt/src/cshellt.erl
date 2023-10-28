% @doc cshellt public API.
-module(cshellt).

-export([on_load/0]).
-export([commands/0]).
-export([start/2]).
-export([stop/1]).

-on_load(on_load/0).

on_load() ->
    ok  = erlang:load_nif(atom_to_list(?MODULE), 0).

% @private
start(_Type, _Args) -> 
    Sup = cshellt_sup:start_link(),
    grisp_led:flash(1, magenta, 500),
    Sup.

% @private
stop(_State) -> ok.

commands() -> 
    %Non se poden gardar os archivos na grisp
    %é mellor envialos ao nodo que ten a pantalla de monitorización
    %e que este se encargue de faceloß
    filelib:ensure_dir("./LOG/CPU"),
    Path = filename:join(["./LOG", "CPUUSAGE.LOG"]),
    {ok, F} = file:open(Path, [write]),
    group_leader(F, self()),
    erlang:display("Anything this process outputs now gets redirected"),
    commands_nif().

commands_nif() ->
    erlang:nif_error("NIF library not loaded").


