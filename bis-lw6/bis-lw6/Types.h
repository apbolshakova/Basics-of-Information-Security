#ifndef TYPE
#define TYPE

typedef enum Status_ { FAIL, SUCCESS } status_t;
typedef enum Bool_ { FALSE_, TRUE_ } bool_t;

typedef enum Config_ 
{
	ADD_DUMMIES,
	CHANGE_VAR_NAMES,
	DEL_COMMS,
	DEL_SPACES
} config_t;

typedef enum HandlingMode_
{
	CODE, //common code, copy it with or without spaces
	INLINE_COMM, //inline comment, copy it until \n or EOF
	MULTILINE_COMM, //multiline comment, copy it until */ or EOF
	CONST_STRING, //constant string, copy it until " but not \"
	PREPROCESSOR, //preprocessor commands, copy it until \n or EOF
	VAR //variable initialization, copy it until = or ;
} mode_t;

#endif