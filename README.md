# LongboardSTM32FW
Firmware for the additional longboard-functionalities controlled by the STM32F103C8T.

**Import Project into STM32 Workbench**  
Open STM32 Workbench -> open Workspace
File->Import->General->Existing Projects into Workspace -> "Select Path to Repo Directory" -> uncheck "Copy projects into workspace" -> Finsh

**Install Openocd Debugger in Eclipse**  
Help -> Install New Software -> "Work with: http://gnu-mcu-eclipse.netlify.com/v4-neon-updates" -> Select Cross Development Tools -> Next-> Next

**Configure Openocd Debugger**  
Run -> Debug Configurations -> Openocd Debug -> Debugger -> "Set Path to Repo"  

Config options: -f interface/stlink.cfg -f board/stm32f103c8_blue_pill.cfg -c "reset_config trst_only combined"
GDB Path: ${openstm32_compiler_path}\arm-none-eabi-gdb

**Git-Bash**  
https://git-scm.com/downloads

**Git Commands/Explenations**  
https://rogerdudler.github.io/git-guide/

**Coding Guidlines**  
minimize coupling  
maximize cohesion  
Have fun!  

