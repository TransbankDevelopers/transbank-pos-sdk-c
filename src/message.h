#ifndef MESSAGEHEADER_FILE
#define MESSAGEHEADER_FILE

typedef struct
{
  int payloadSize;
  int responseSize;
  int retries;
  char *payload;
} Message;

#endif
