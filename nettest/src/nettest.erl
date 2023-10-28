% @doc nettest public API.
-module(nettest).

-behavior(application).

% Callbacks
-export([start/2]).
-export([stop/1]).

%--- Callbacks -----------------------------------------------------------------

% @private
start(_Type, _Args) -> 
    Sup = nettest_sup:start_link(),
    grisp_led:flash(1, magenta, 500),
    Sup.

% @private
stop(_State) -> ok.
