# NcatBot Plugins Edition

**NcatBot 喵～**

## 简介

NcatBot 插件版是以 [NcatBot](https://github.com/liyihao1110/NcatBot) 为核心的、面向用户的 QQ Bot 启动器。

你需要的环境: Python 3.10 及以上、安装并开启的 NapCatQQ

## 安装

请按照以下步骤安装:

1. 下载命令行工具: nbmiao

   1. Debian/Ubuntu: 下载 [nbmiao](https://github.com/Isaaczhr/plugins.ncatbot.xyz/blob/main/cli/nbmiao) 。
   
   2. 其他系统暂无二进制文件发行，可参考 [源代码](https://github.com/Isaaczhr/plugins.ncatbot.xyz/blob/main/cli/nbmiao.cpp) 自行编译。
   
2. 下载核心代码:

   ```bash
   nbmiao get
   ```
   这将在当前目录创建 NcatBotPluginsEdition 文件夹并下载核心代码至其中。

3. 创建运行环境:

   程序默认使用 `python3 -m venv` 创建虚拟环境，其他方式请等待后续推出。
   
   您要做的就是保证已安装 Python3 并存在于系统路径。使用 `Python --version` 确保安装成功。

   ```bash
   nbmiao init
   ```

   这将自动创建虚拟环境于项目目录并安装所需依赖。

4. 安装插件:
   
   ```bash
   nbmiao install <plugin-name>
   ```

   例如，你可以使用 `nbmiao install example` 来安装示例插件。


## 启动

请按照以下步骤启动 Bot :

1. 确保 NapCatQQ 已启动并开放 HTTP AT Port 3000 ， Websocket AT Port 3001 。
2. 使用 `nbmiao run` 启动 Bot 服务，程序会自动使用 venv 执行。
3. 你会看到类似
   ```
   检查依赖中...
   [INFO] (main.py:40) load_plugins : 加载插件 example
   [INFO] (gateway.py:51) ws_connect : websocket连接已建立
   [INFO] (gateway.py:31) receive : 机器人 xxxxxxxxxx 成功启动
   ```
   的程序输出，这代表您已经成功启动了 Bot 并正在使用示例插件。

## 插件开发

请参考示例插件 [example.py](https://github.com/Isaaczhr/plugins.ncatbot.xyz/blob/main/plugins/example/example.py) 。

开发完的插件请提交到本项目的 plugins 文件夹中，更新 list.json ，并在 downloads/plugins 中提供压缩包以供 nbmiao 自动下载。（三个地方需要同步更改）

## 回家

主项目地址: [NcatBot](https://github.com/liyihao1110/NcatBot)

文档地址: [docs.ncatbot.xyz](https://github.com/Isaaczhr/docs.ncatbot.xyz)

插件版本地址（本项目）: [plugins.ncatbot.xyz](https://github.com/Isaaczhr/plugins.ncatbot.xyz)