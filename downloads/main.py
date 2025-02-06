import os
import subprocess
import sys
import importlib.util
import time
from ncatbot.client import BotClient
from ncatbot.logger import get_log

base_dir = os.path.dirname(os.path.abspath(__file__))
config_file = os.path.join(base_dir, 'config.yaml')

bot = BotClient(config_path=config_file)
_log = get_log()

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
    if not os.path.exists(plugin_folder):
        print(f"Plugin folder {plugin_folder} does not exist!")
        return
    plugin_dirs = [d for d in os.listdir(plugin_folder) if os.path.isdir(os.path.join(plugin_folder, d))]

    for plugin_dir in plugin_dirs:
        plugin_requirements_file = os.path.join(plugin_folder, plugin_dir, 'requirements.txt')

        # 检查并安装插件依赖
        install_requirements(plugin_requirements_file)

        plugin_file = os.path.join(plugin_folder, plugin_dir, f"{plugin_dir}.py")
        if os.path.exists(plugin_file):
            try:
                _log.info(f"加载插件 {plugin_dir}")
                module_name = plugin_dir
                spec = importlib.util.spec_from_file_location(module_name, plugin_file)
                plugin = importlib.util.module_from_spec(spec)
                sys.modules[module_name] = plugin
                spec.loader.exec_module(plugin)

                # 初始化
                if hasattr(plugin, 'init_plugin'):
                    plugin.init_plugin(bot_instance)
            except Exception as e:
                _log.error(f"加载插件 {plugin_dir} 失败: {e}")
        else:
            print(f"Plugin file {plugin_file} does not exist!")

if __name__ == "__main__":
    
    # 加载插件并传递 bot 实例
    load_plugins(os.path.join(os.path.dirname(__file__), 'plugins'), bot)
    
    # 尝试运行 bot，最多重试 10 次
    max_retries = 10
    for attempt in range(max_retries):
        try:
            bot.run(reload=True)
            break
        except Exception as e:
            print(f"连接失败: {e}")
            if attempt < max_retries - 1:
                print("等待 3 秒后重试...")
                time.sleep(3)
            else:
                print("多次尝试连接失败，程序退出")
                sys.exit(1)
