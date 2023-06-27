#include <pspkernel.h>
#include <pspdebug.h>
#include <string>
#include <json/json.h>

PSP_MODULE_INFO("Hello World", 0, 1,0);

int main(void)
{
  Json::Value data;
  Json::Reader reader;
  std::string text = "{\"message\":\"hello json\"}";
  
  reader.parse(text, data);
  
  pspDebugScreenInit();

  while(1)
    {
      pspDebugScreenPrintf("%s\n",data.get("message", "Could not load json message!").asString().c_str());
      sceKernelDelayThread(1000000);
    }
  return 0;
}
