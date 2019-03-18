typedef struct message_t Message;

struct message_t {
  int payloadSize;
  int responseSize;
  int retries;
  char* payload;
};
