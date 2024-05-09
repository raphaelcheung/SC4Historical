# SC4Historical

模拟城市4 的DLL插件，用于一键设置历史建筑。

城市快速发展中往往需要通过设置历史建筑，来避免高收入建筑挤兑低收入建筑，从而控制收入人口以及岗位的健康比例。

手动设置这些费时费力，因此开发这款插件，希望能有帮助。



插件可以从发布页下载: 

## 系统要求

* SimCity 4 v641 以上
* Windows 10 以上

## 安装

1. 关闭 SimCity 4。

2. 将 `SC4Historical.dll` 文件复制到 SimCity 4 的插件目录中。

3. 运行 SimCity 4。

## 使用插件

游戏进入地图后按 Ctrl+X 打开命令行，输入：

`his <建筑类型> <最小容量> [宣布历史建筑]`

`<建筑类型>`：这里可以是 r、o、s、i，分别对应住宅、商业、服务业、工业建筑；

`<最小容量>`：整数值，只有高于或等于这个容量的建筑才会应用本命令；   

`[宣布历史建筑]`：填入 true、false。

### 例子
假如想把容量高于 300 的写字楼都设置为历史建筑，命令如下：

`his o 300 true`

把所有住宅建筑都取消历史建筑：

`his r 0 false`

# 许可证

本项目遵循 MIT 协议。

查看 [LICENSE.txt](LICENSE.txt) 以了解详情。

## 第三方代码

[gzcom-dll](https://github.com/nsgomez/gzcom-dll/tree/master) Located in the vendor folder, MIT License.    
[EABase](https://github.com/electronicarts/EABase) Located in the vendor folder, BSD 3-Clause License.    
[EASTL](https://github.com/electronicarts/EASTL) Located in the vendor folder, BSD 3-Clause License.    
[Windows Implementation Library](https://github.com/microsoft/wil) - MIT License    
[SC4Fix](https://github.com/nsgomez/sc4fix) - MIT License    
[Boost.Algorithm](https://www.boost.org/doc/libs/1_84_0/libs/algorithm/doc/html/index.html) - Boost Software License, Version 1.0.   
[sc4-growify](https://github.com/0xC0000054/sc4-growify) - MIT License   

# 源码

## 前提条件

* Visual Studio 2022
* [VCPkg](https://github.com/microsoft/vcpkg)，按照官方指引安装并初始化，记得执行`vcpkg integrate install`以关联 Visual Studio
