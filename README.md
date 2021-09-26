# clex

一个C语言编写的C语言词法分析器

## 构建

1. 从官网[获取xmake](https://xmake.io/#/zh-cn/guide/installation)
2. 执行
```shell
xmake build
```
3. 可执行文件会生成于``build/<your OS name>/<your architecture>/release``

要切换到Debug mode，执行：
```shell
xmake f -m debug
```

## 使用

```
Usage:
  clex [files]
```

可以传入任意多个文件，程序会依次进行分析并输出结果。
