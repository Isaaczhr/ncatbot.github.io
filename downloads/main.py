import os
import subprocess
import sys
import importlib.util
from NcatBot.ncatbot.client import BotClient

bot = BotClient()

def install_requirements(requirements_file):
    """安装 requirements.txt 中列出的依赖"""
    if not os.path.exists(requirements_file):
        return
    try:
        print("检查依赖中...")
        subprocess.check_call([sys.executable, "-m", "pip", "install", "-r", requirements_file], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    except subprocess.CalledProcessError as e:
        print(f"Error installing requirements from {requirements_file}: {e}")
        sys.exit(1)

def load_plugins(plugin_folder, bot_instance):
    """加载插件并为每个插件安装依赖"""
    # 获取插件
    plugin_dirs = [d for d in os.listdir(plugin_folder) if os.path.isdir(os.path.join(plugin_folder, d))]

    for plugin_dir in plugin_dirs:
        plugin_requirements_file = os.path.join(plugin_folder, plugin_dir, 'requirements.txt')

        # 检查并安装插件依赖
        install_requirements(plugin_requirements_file)

        plugin_file = os.path.join(plugin_folder, plugin_dir, f"{plugin_dir}.py")
        if os.path.exists(plugin_file):
            print(f"加载 {plugin_dir} 插件")
            module_name = plugin_dir
            spec = importlib.util.spec_from_file_location(module_name, plugin_file)
            plugin = importlib.util.module_from_spec(spec)
            sys.modules[module_name] = plugin
            spec.loader.exec_module(plugin)

            # 初始化
            if hasattr(plugin, 'init_plugin'):
                plugin.init_plugin(bot_instance)
        else:
            print(f"Plugin file {plugin_file} does not exist!")

if __name__ == "__main__":
    # 安装主程序依赖
    install_requirements('NcatBot/requirements.txt')
    
    # 加载插件并传递 bot 实例
    load_plugins('./plugins', bot)
    
    # 运行 bot
    bot.run(reload=True)
