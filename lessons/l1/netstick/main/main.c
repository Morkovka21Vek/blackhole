#include "softap_sta.h"
#include "server.h"

void app_main(void)
{
  enable_wifi();
  start_server();
}
