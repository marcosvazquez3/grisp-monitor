-module(test).

-export([on_load/0]).
-export([hello/0]).
-export([start/2]).
-export([stop/1]).


-on_load(on_load/0).

on_load() ->
    ok  = erlang:load_nif(atom_to_list(?MODULE), 0).

% @private
start(_Type, _Args) -> 
    Sup = test_sup:start_link(),
    grisp_led:flash(1, magenta, 500),
    Sup.

% @private
stop(_State) -> ok.

hello() -> hello_nif().

hello_nif() ->
    erlang:nif_error("NIF library not loaded").
