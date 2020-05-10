#include <pspkernel.h>
#include <pspdebug.h>
#include <stdlib.h>
#include<fstream>
#include<json/json.h>

/*Sets attributes for program
 *Parameter 1: program's name
 *Parameter 2: thread attributes (such as user/kernel mode)
 *Parameter 3 and 4: programs major and minor version numbers (so this is v1.0)
 */
PSP_MODULE_INFO("Hello World", 0, 1,0);

/*Called when the home button is pushed and exit is selected
 *I could not find any documentation on the meaning of
 *these parameters
 */
int exit_callback(int arg1, int arg2, void *common)
{
  //Kill the program
  sceKernelExitGame();
  return 0;
}

/*The function that is the entry point for the exit thread
 *Parameter 1: the number of bytes of data passed to the function
 *Parameter 2: the data passed to the function
 */
int CallbackThreadEntry(SceSize n, void* argp)
{
  /*Create a callback
   *Parameter 1: the new callback's name
   *Parameter 2: the new callback's enty function
   *Parameter 3: unknown, use NULL
   */
  int callback_id = sceKernelCreateCallback("Exit Callback", exit_callback,NULL);

  //If callback was successfully created
  if(callback_id >= 0)
    //Tell the kernel to use this callback when the user wishes to exit
    sceKernelRegisterExitCallback(callback_id);
  //Puts the thread to sleep, but allows it to be woken by a callback
  sceKernelSleepThreadCB();
  return 0;
}

//Sets up a callback to exit the program when the user exits using the home button
void setup_exit_callback()
{
  /*Create a new thread
   *Parameter 1: the new thread's name
   *Parameter 2: the new thread's entry function
   *Parameter 3: the new thread's priority (lower indicates higher priority)
   *Parameter 4: the new thread's stack size (important to consider
   *             when doing memory-intensive work). 0xFA0 equates to 4000 bytes
   *Parameter 5: parameters for the thread such as the thread's mode and some other options
   *Parameter 6: additional options for the thread (seems like only memory location)
   */
  int thread_id = sceKernelCreateThread("Exit Thread", CallbackThreadEntry, 0x18, 0xFA0,0,0);

  //If the thread was successfully created
  if(thread_id >= 0)
    {
      /*Start the thread
       *Parameter 1: the thread id returned by sceKernelCreateThread
       *Parameter 2: the size of data to be passed as an argument to the thread (0 in this case)
       *Parameter 3: pointer to the start of the data to be passed to the thread (null in this case)
       */
      sceKernelStartThread(thread_id,0,NULL);
    }
}

void LoadJson(std::string filename, Json::Value *data) {
    std::ifstream configFile(filename);
    Json::CharReaderBuilder builder;
    JSONCPP_STRING errs;
    parseFromStream(builder, configFile, data, &errs);
    configFile.close();
}

int main(void)
{
  //Sets up callback to exit the program
  setup_exit_callback();
  //Initialize the PSP's debugging console
  pspDebugScreenInit();

  while(1)
    {
      //Behaves the same as printf, but prints to the PSP's debug console
      Json::Value data;
	  LoadJson("message.json", &data);
      pspDebugScreenPrintf("%s\n",data.get("message", "Could not load json message!").asString().c_str());
      //Sleeps the thread for 1000000us or 1s
      sceKernelDelayThread(1000000);
    }
  return 0;
}
