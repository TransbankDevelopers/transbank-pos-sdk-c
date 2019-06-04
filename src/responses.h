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
  int terminalId;
  int initilized;
} BaseResponse;

typedef struct
{
  int function;
  int responseCode;
  int txCount;
  int txTotal;
  int initilized;
} TotalsResponse;

typedef struct
{
  int function;
  int responseCode;
  long long commerceCode;
  int terminalId;
  int authorizationCode;
  int operationID;
  int initilized;
} CancellationResponse;

#endif
