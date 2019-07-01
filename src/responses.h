#ifndef RESPONSESHEADER_FILE
#define RESPONSESHEADER_FILE

typedef struct
{
  int index;
  int length;
} ParamInfo;

typedef struct
{
  int function;
  int responseCode;
  long long commerceCode;
  char terminalId[9];
  int initilized;
} BaseResponse;

typedef struct
{
  int function;
  int responseCode;
  int txCount;
  int txTotal;
  int initilized;
} TotalsCResponse;

typedef struct
{
  int function;
  int responseCode;
  long long commerceCode;
  char terminalId[9];
  char authorizationCode[7];
  int operationID;
  int initilized;
} RefundCResponse;

#endif
