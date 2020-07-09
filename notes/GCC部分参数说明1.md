# GCC  部分参数说明 1

1. 测试实例

```C
// hello.c
#include <stdio.h>

int main(void)
{
        printf("Hello World\n");

        return 0;
}
```

2. **参数 -M**

生成文件关联的信息，包含目标文件所依赖的所有源代码

```shell
$ gcc -M hello.c
hello.o: hello.c /usr/include/stdc-predef.h /usr/include/stdio.h \
 /usr/include/features.h /usr/include/x86_64-linux-gnu/sys/cdefs.h \
 /usr/include/x86_64-linux-gnu/bits/wordsize.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
 /usr/lib/gcc/x86_64-linux-gnu/5/include/stddef.h \
 /usr/include/x86_64-linux-gnu/bits/types.h \
 /usr/include/x86_64-linux-gnu/bits/typesizes.h /usr/include/libio.h \
 /usr/include/_G_config.h /usr/include/wchar.h \
 /usr/lib/gcc/x86_64-linux-gnu/5/include/stdarg.h \
 /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
 /usr/include/x86_64-linux-gnu/bits/sys_errlist.h
```

3. **参数 -MM**

生成文件关联的信息，包含目标文件所依赖的所有源代码

与 `-M` 类似，但是忽略由 `#include` 包含的依赖关系

```shell
$ gcc -MM hello.c
hello.o: hello.c
```

4. **参数 -MMD**

生成文件关联信息，包含目标文件所依赖的所有源代码

与 `-MM` 类似，但是输出将导入到 `*.d` 文件里面。`*.d` 中的内容，与 `-MM` 输出内容一样，只是输出到文件中

```shell
$ gcc -MMD hello.c -o hello
$ ll
total 76
drwxrwxr-x  2 kyzoon kyzoon 4096 Jul  8 12:47 ./
drwxr-xr-x 44 kyzoon kyzoon 4096 Jul  8 12:37 ../
-rwxrwxr-x  1 kyzoon kyzoon 8600 Jul  8 12:47 hello*
-rw-rw-r--  1 kyzoon kyzoon   78 Jul  8 12:37 hello.c
-rw-rw-r--  1 kyzoon kyzoon   15 Jul  8 12:47 hello.d
$ cat hello.d
hello: hello.c
```

5. **参数 -MD**

生成文件关联信息，包含目标文件所依赖的所有源代码
与 `-MMD` 类似，输出内容到 `*.d` 文件里面。输出内容与 `-M` 相同，包含由 `#include` 所引入的依赖关系

```shell
kyzoon@ubuntu:~/test$ gcc -MD hello.c -o hello
kyzoon@ubuntu:~/test$ ll
total 76
drwxrwxr-x  2 kyzoon kyzoon 4096 Jul  8 12:54 ./
drwxr-xr-x 44 kyzoon kyzoon 4096 Jul  8 12:37 ../
-rwxrwxr-x  1 kyzoon kyzoon 8600 Jul  8 12:54 hello*
-rw-rw-r--  1 kyzoon kyzoon   78 Jul  8 12:37 hello.c
-rw-rw-r--  1 kyzoon kyzoon  646 Jul  8 12:54 hello.d
kyzoon@ubuntu:~/test$ cat hello.d
hello: hello.c /usr/include/stdc-predef.h /usr/include/stdio.h \
 /usr/include/features.h /usr/include/x86_64-linux-gnu/sys/cdefs.h \
 /usr/include/x86_64-linux-gnu/bits/wordsize.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
 /usr/lib/gcc/x86_64-linux-gnu/5/include/stddef.h \
 /usr/include/x86_64-linux-gnu/bits/types.h \
 /usr/include/x86_64-linux-gnu/bits/typesizes.h /usr/include/libio.h \
 /usr/include/_G_config.h /usr/include/wchar.h \
 /usr/lib/gcc/x86_64-linux-gnu/5/include/stdarg.h \
 /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
 /usr/include/x86_64-linux-gnu/bits/sys_errlist.h
```

6. 对比 4 参数

| 参数 | 功能 | 差异 |
| :--: | :-- | :-- |
| `-M` | 生成文件关联信息，<br>包含目标文件所依赖的所有源代码 | 包含由 `#include` 所引入的依赖关系<br>内容输出到命令行 `gcc` 返回信息<br>默认打开 `-E` 选项，使得编译器在预处理结束时就停止编译，即不会生成目标文件，如果使用 `-o` 指定输出，生成的目标文件没有可执行权限 |
| `-MM` | 生成文件关联信息，<br/>包含目标文件所依赖的所有源代码 | 不包含由 `#include` 所引入的依赖关系<br>内容输出到命令行 `gcc` 返回信息<br/>默认打开 `-E` 选项，使得编译器在预处理结束时就停止编译，即不会生成目标文件，如果使用 `-o` 指定输出，生成的目标文件没有可执行权限 |
| `-MD` | 生成文件关联信息，<br/>包含目标文件所依赖的所有源代码 | 包含由 `#include` 所引入的依赖关系<br>内容输出到 `*.d` 文件<br>默认关闭 `-E` 选项，即会继续编译，生成目标文件 |
| `-MMD` | 生成文件关联信息，<br/>包含目标文件所依赖的所有源代码 | 不包含由 `#include` 所引入的依赖关系<br>内容输出到 `*.d` 文件<br/>默认关闭 `-E` 选项，即会继续编译，生成目标文件 |

>1 个 M 输出所有依赖关系，包含 `#include` 关系，2 个 M 输出部分依赖关系，即不包含 `#include` 关系
>
>不带 D 输出依赖信息到命令行，带 D 输出依赖信息到 `*.d` 文件

7. **参数 -MF <u>file</u>**

指定一个文件用于存放，生成文件的关联信息。这此信息与 `-M` 和 `-MM` 一样

参数 `-MF` 要与 `-M` 或 `-MM` 一起使用，否则会报错

`-MF` 参数后，要紧跟指定的存放依赖关系信息的文件名，如：`-MF "hello.d"`，可任意命名

```shell
$ gcc -MF "hello.d" hello.c -o hello
cc1: error: to generate dependencies you must specify either -M or -MM
```

```shell
kyzoon@ubuntu:~/test$ gcc -M -MF "hello.d" hello.c -o hello
kyzoon@ubuntu:~/test$ ll
total 64
drwxrwxr-x  2 kyzoon kyzoon 4096 Jul  8 12:54 ./
drwxr-xr-x 44 kyzoon kyzoon 4096 Jul  8 12:37 ../
-rwxrwxr-x  1 kyzoon kyzoon    0 Jul  8 13:10 hello*
-rw-rw-r--  1 kyzoon kyzoon   78 Jul  8 12:37 hello.c
-rw-rw-r--  1 kyzoon kyzoon  648 Jul  8 13:10 hello.d
kyzoon@ubuntu:~/test$ cat hello.d
hello.o: hello.c /usr/include/stdc-predef.h /usr/include/stdio.h \
 /usr/include/features.h /usr/include/x86_64-linux-gnu/sys/cdefs.h \
 /usr/include/x86_64-linux-gnu/bits/wordsize.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
 /usr/lib/gcc/x86_64-linux-gnu/5/include/stddef.h \
 /usr/include/x86_64-linux-gnu/bits/types.h \
 /usr/include/x86_64-linux-gnu/bits/typesizes.h /usr/include/libio.h \
 /usr/include/_G_config.h /usr/include/wchar.h \
 /usr/lib/gcc/x86_64-linux-gnu/5/include/stdarg.h \
 /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
 /usr/include/x86_64-linux-gnu/bits/sys_errlist.h
 
 $ gcc -MM -MF "hello.lst" hello.c -o hello
kyzoon@ubuntu:~/test$ ll
total 68
drwxrwxr-x  2 kyzoon kyzoon 4096 Jul  8 13:11 ./
drwxr-xr-x 44 kyzoon kyzoon 4096 Jul  8 12:37 ../
-rwxrwxr-x  1 kyzoon kyzoon    0 Jul  8 13:12 hello*
-rw-rw-r--  1 kyzoon kyzoon   78 Jul  8 12:37 hello.c
-rw-rw-r--  1 kyzoon kyzoon  648 Jul  8 13:10 hello.d
-rw-rw-r--  1 kyzoon kyzoon   17 Jul  8 13:12 hello.lst
kyzoon@ubuntu:~/test$ cat hello.lst
hello.o: hello.c
```

> **注意：**
>
> `-MF` 与 `-MD` 或 `-MMD` 混用时，会覆盖 `-MD` 或 `-MMD` 的特性。即会覆盖默认的 `*.d` 文件，而使用 `-MF` 指定的文件，`*.d` 文件将不会生成

8. **参数 -MT <u>target</u>**

在生成的依赖文件中，指定依赖规则中的目标

```shell
$ gcc -MM hello.c
hello.o: hello.c defs.h
$ ll
total 52
drwxrwxr-x  2 kyzoon kyzoon 4096 Jul  8 13:47 ./
drwxr-xr-x 44 kyzoon kyzoon 4096 Jul  8 13:47 ../
-rw-rw-r--  1 kyzoon kyzoon   94 Jul  8 13:42 defs.h
-rw-rw-r--  1 kyzoon kyzoon  117 Jul  8 13:43 hello.c

$ gcc -MM -MT "hello.d" hello.c
hello.d: hello.c defs.h
$ ll
total 52
drwxrwxr-x  2 kyzoon kyzoon 4096 Jul  8 13:47 ./
drwxr-xr-x 44 kyzoon kyzoon 4096 Jul  8 13:47 ../
-rw-rw-r--  1 kyzoon kyzoon   94 Jul  8 13:42 defs.h
-rw-rw-r--  1 kyzoon kyzoon  117 Jul  8 13:43 hello.c
```

可以看出，依赖规则中的目标，由 hello.o 改为了 hello.d

9. **参数 -MP**

生成的依赖文件里面，依赖规则中的所有 `*.h` 依赖项都会在该文件中生成一个伪目标，此伪目标不依赖任何其它依赖项。

该伪规则将避免删除了对应的头文件而没有更新 `Makefile`，而去匹配新的依赖关系而导致 `make` 出错的问题

为了演示说明，添加了一个依赖头文件，并修改源文件

```C
// defs.h
#ifndef _DEFS_H__
#define _DEFS_H__

typedef U8      unsigned char;

#endif /* End of _DEFS_H__ */

// hello.c
#include <stdio.h>
#include "defs.h"

int main(void)
{
        u8 i = 10;

        printf("Hello World [%d]\n", i);

        return 0;
}
```

```shell
$ gcc -MM hello.c
hello.o: hello.c defs.h
$ gcc -MM -MP hello.c
hello.o: hello.c defs.h

defs.h:	# 此为生成的伪目标，无任何依赖项
```

10. **参数 -MG**


要求把缺失的头文件按存在对待，并且假定它们和源文件在同一目录下。必须和 `-M` 或 `-MM` 一起使用

```shell
# 正常依赖情况
$ gcc -MM -MG hello.c
hello.o: hello.c defs.h
# 将 defs.h 重命名，破坏关系
$ mv defs.h def.h
# -MG 把缺失的头文件按存在对待
$ gcc -MM -MG hello.c
hello.o: hello.c defs.h
# 实际头文件缺失，会编译错误
$ gcc -MM hello.c
hello.c:2:18: fatal error: defs.h: No such file or directory
compilation terminated.
```

