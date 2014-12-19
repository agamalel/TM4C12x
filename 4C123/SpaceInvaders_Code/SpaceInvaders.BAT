SET PATH=D:\Keil\ARM\ARMCC\bin\;C:/CooCox/CoIDE/jre/bin/client;C:/CooCox/CoIDE/jre/bin;C:/CooCox/CoIDE/jre/lib/i386;c:\Program Files (x86)\Intel\iCLS Client\;c:\Program Files\Intel\iCLS Client\;C:\Program Files (x86)\AMD APP\bin\x86_64;C:\Program Files (x86)\AMD APP\bin\x86;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Program Files (x86)\ATI Technologies\ATI.ACE\Core-Static;C:\Program Files\Intel\Intel(R) Management Engine Components\DAL;C:\Program Files\Intel\Intel(R) Management Engine Components\IPT;C:\Program Files (x86)\Intel\Intel(R) Management Engine Components\DAL;C:\Program Files (x86)\Intel\Intel(R) Management Engine Components\IPT;C:\Program Files\Microsoft SQL Server\110\Tools\Binn\;C:\Program Files (x86)\Microchip\xc8\v1.31\bin;C:\Program Files (x86)\yagarto\bin;C:\CooCox\CoIDE\gcc\bin;C:\CooCox\CoIDE;
SET CPU_TYPE=LM4F120H5QR
SET CPU_VENDOR=Texas Instruments
SET UV2_TARGET=SpaceInvaders
SET CPU_CLOCK=0x00F42400
"D:\Keil\ARM\ARMCC\bin\ArmAsm" --Via ".\startup._ia"
"D:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\nokia5110.__i"
"D:\Keil\ARM\ARMCC\bin\ArmAsm" --Via ".\random._ia"
"D:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\spaceinvaders.__i"
"D:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\texas.__i"
"D:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\dac.__i"
"D:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\adc.__i"
"D:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\sound.__i"
"D:\Keil\ARM\ARMCC\bin\ArmLink" --Via ".\SpaceInvaders.lnp"
D:\Keil\ARM\ARMCC\bin\fromelf.exe ".\SpaceInvaders.axf" --i32combined --output ".\SpaceInvaders.hex"
