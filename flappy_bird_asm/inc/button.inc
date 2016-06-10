fnFuncOnClick		typedef	proto stdcall
FuncOnClick			typedef ptr fnFuncOnClick

Button_construct	proto stdcall	o : DWORD, left : DWORD, right : DWORD, \
					top : DWORD, bottom : DWORD, onClickFunction : FuncOnClick

Button_isHit		proto stdcall	o : DWORD, x : DWORD, y : DWORD