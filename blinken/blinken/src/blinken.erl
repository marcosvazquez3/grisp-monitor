% @doc blinken public API.
-module(blinken).

-behavior(application).

% Callbacks
-export([start/2]).
-export([stop/1]).

%--- Callbacks -----------------------------------------------------------------

% @private
start(_Type, _Args) -> 
	Sup = blinken_sup:start_link(),
	grisp_led:flash(1, magenta, 500),
	Sup.

% @private
stop(_State) -> ok.
