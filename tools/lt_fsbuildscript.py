
#import os
import subprocess
#import json
#from datetime import datetime
from os.path import basename, join, normpath
Import("env")
from platformio.platform.base import PlatformBase
from platformio.platform.board import PlatformBoardConfig
#from SCons.Script import DefaultEnvironment, Environment
#env: Environment = DefaultEnvironment()
platform: PlatformBase = env.PioPlatform()

def build_ltfs():
    print("Current Data dir", env.get("PROJECT_DATA_DIR"))
    pathmk = env.get("PROJECT_DIR")+ "/tools/mklittlefs.exe"
    pathmk = normpath(pathmk).replace("\\", "/")
    print("Current tools dir", pathmk)
    FS_SIZE ="0x25000"
    FS_PAGE = "0x100"
    FS_BLOCK = "0x1000" 
    subprocess.call([pathmk, "-c", env.get("PROJECT_DATA_DIR"), "-s", FS_SIZE, "-p", FS_PAGE, "-b", FS_BLOCK, "lt_littlefs.bin", ])
    print("------------!!!!!!!!!-------------")
    print("FS_SIZE", FS_SIZE)
    print("FS_PAGE", FS_PAGE)
    print("FS_BLOCK", FS_BLOCK)
    print("------------!!!!!!!!!-------------") 
    #subprocess.call(["c:/USER_BMW/IoTManager_4dev/IoTManager_C6_Tiny/tools/mklittlefs.exe", '-c', '${SOURCES}', '-s', '${FS_SIZE}', '-p', '${FS_PAGE}', '-b', '${FS_BLOCK}', 'lt_littlefs.bin']) 
    # ./mklittlefs.exe -c c:\USER_BMW\IoTManager_4dev\IoTManager_C6_Tiny\data_svelte -s 0x25000 -p 0x100 -b 0x1000 lt_littlefs.bin
def upload_ltfs():  
    print("Flash UserData offset", env.get("FLASH_USERDATA_OFFSET"))  
    print("Flash UserData length", env.get("FLASH_USERDATA_LENGTH"))  
    LT_TOOL = 'python.exe -m ltchiptool -r -i 1 -L ' + platform.get_dir() + 'flash write'
    print(LT_TOOL)
    #subprocess.call(LT_TOOL , shell=True) 
    #"${LTCHIPTOOL}"
def before_build(): # source, target, env
    print("Current Build targets", BUILD_TARGETS)
    # Это всё потому что не работает "buildprog". При сборке прошивки Targets пустой, на всякий случай исключим все остальные
    if (BUILD_TARGETS == ['upload'] or
        BUILD_TARGETS == ['buildfs'] or 
        BUILD_TARGETS == ['uploadfs'] or 
        BUILD_TARGETS == ['uploadfsota'] or 
        BUILD_TARGETS == ['size']):
            return

    #print("Clear BUILD_TIME, delete main.o !")
  #  config = configparser.ConfigParser()  # создаём объекта парсера INI
   # config.read("platformio.ini")
    #deviceName = config["platformio"]["default_envs"]
    build_ltfs()
    upload_ltfs()

before_build()

#на всякий случай
#cd data_svelte/
#curl.exe -F "file=@edit.htm.gz;filename=edit.htm.gz" http://192.168.2.110/edit