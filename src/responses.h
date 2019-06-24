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
} TotalsResponse;

typedef struct
{
  int function;
  int responseCode;
  long long commerceCode;
  char terminalId[9];
  char authorizationCode[7];
  int operationID;
  int initilized;
} RefundResponse;

typedef struct
{
  int function;
  int responseCode;
  long long commerceCode;
  char terminalId[9];
  char ticketID[7];
  char authorizationCode[7];
  int amount;
  int lastDigits;
  int operationID;
  char cardType[3];
  char accountingDate[9];
  char numberAccount[20];
  char cardAbbr[3];
  char transactionDate[9];
  char transactionTime[7];
  int employeID;
  int tip;
  int feeAmount;
  int feeNumber;
  int initilized;
} SalesDetailResponse;

#endif
