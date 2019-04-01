#ifndef RESPONSESHEADER_FILE
#define RESPONSESHEADER_FILE

typedef struct{
  int index;
  int length;
} ParamInfo;


typedef struct{
  int function;
  int responseCode;
  long long commerceCode;
  long terminalId;
  int initilized;
} LoadKeyCloseResponse;

#endif
