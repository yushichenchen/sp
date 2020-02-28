# 執行方法

```
user@DESKTOP-96FRN6B MSYS /d/ccc/course/sp/code/c/04-toolchain/hack/v1
$ make
gcc -std=c99 -O0 -Wall  as/asm.c lib/util.c lib/map.c lib/strTable.c -o bin/as
gcc -std=c99 -O0 -Wall  vm/vm.c lib/util.c lib/map.c lib/strTable.c -o bin/vm

user@DESKTOP-96FRN6B MSYS /d/ccc/course/sp/code/c/04-toolchain/hack/v1
$ make sRun file=add
bin/as test/s/add
bin/vm test/s/add.ox -d
0002EC100003E0900000E308
PC=0000 I=0002 A=0002 D=0000=00000 m[A]=0003
PC=0001 I=EC10 A=0002 D=0002=00002 m[A]=0003 a=0 c=30 d=2 j=0
PC=0002 I=0003 A=0003 D=0002=00002 m[A]=E090
PC=0003 I=E090 A=0003 D=0005=00005 m[A]=E090 a=0 c=02 d=2 j=0
PC=0004 I=0000 A=0000 D=0005=00005 m[A]=0002
PC=0005 I=E308 A=0000 D=0005=00005 m[A]=0005 a=0 c=0C d=1 j=0

```