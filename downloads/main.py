import os
import sys
import subprocess
import importlib.util
from ncatbot.config import config
from ncatbot.logger import get_log
from plugin import PluginManager

_log = get_log()

config.set_bot_uin("123456")
config.set_ws_uri("ws://127.0.0.1:3001")

def install_requirements(requirements_file):
    if not os.path.exists(requirements_file):
        return
    try:
        _log.info("检查依赖中...")
        subprocess.check_call([sys.executable, "-m", "pip", "install", "-r", requirements_file], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    except subprocess.CalledProcessError as e:
        _log.error(f"Error installing requirements from {requirements_file}: {e}")
        sys.exit(1)

def load_plugins(plugin_folder, plugin_manager):
    if not os.path.exists(plugin_folder):
        _log.error(f"插件文件夹 {plugin_folder} 不存在!")
        return
        
    plugin_dirs = [d for d in os.listdir(plugin_folder) if os.path.isdir(os.path.join(plugin_folder, d))]

    for plugin_dir in plugin_dirs:
        plugin_requirements_file = os.path.join(plugin_folder, plugin_dir, 'requirements.txt')
        install_requirements(plugin_requirements_file)

        plugin_file = os.path.join(plugin_folder, plugin_dir, f"{plugin_dir}.py")
        if os.path.exists(plugin_file):
            try:
                module_name = plugin_dir
                spec = importlib.util.spec_from_file_location(module_name, plugin_file)
                module = importlib.util.module_from_spec(spec)
                sys.modules[module_name] = module
                spec.loader.exec_module(module)

                if hasattr(module, 'plugin'):
                    plugin_manager.load_plugin(module.plugin)
            except Exception as e:
                _log.error(f"加载插件 {plugin_dir} 失败: {e}")

if __name__ == "__main__":
    plugin_manager = PluginManager()
    load_plugins(os.path.join(os.path.dirname(__file__), 'plugins'), plugin_manager)
    
    plugin_manager.bot.run(reload=True)
