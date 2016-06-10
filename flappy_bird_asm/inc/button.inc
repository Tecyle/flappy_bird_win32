fnFuncOnClick		typedef	proto stdcall
FuncOnClick			typedef ptr fnFuncOnClick

Button		struct
	left	SDWORD		?
	right	SDWORD		?
	top		SDWORD		?
	bottom	SDWORD		?
	enabled	BOOLEAN		?
	onClick	FuncOnClick	?
Button		ends
ButtonPtr	typedef ptr Button

Button_construct	proto stdcall	o : ButtonPtr, left : SDWORD, right : SDWORD, \
					top : SDWORD, bottom : SDWORD, onClickFunction : FuncOnClick

Button_isHit		proto stdcall	o : ButtonPtr, x : SDWORD, y : SDWORD