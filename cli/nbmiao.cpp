#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>

// 常量定义
const std::string RED = "\033[0;31m";
const std::string GREEN = "\033[0;32m";
const std::string NC = "\033[0m";
const std::string NCURL = "https://ghproxy.net/https://github.com/liyihao1110/ncatbot/archive/refs/heads/main.zip";
const std::string ZIP_FILE = "main.zip";
const std::string MAIN_DIR = "./NcatBotPluginsEdition";
const std::string DEST_DIR = "./NcatBotPluginsEdition/NcatBot";
const std::string PLUGIN_DIR = "./NcatBotPluginsEdition/plugins";

// 打印帮助信息
void print_help() {
    std::cout << "Usage: nbmiao <command> [arguments]\n";
    std::cout << "Commands:\n";
    std::cout << "  get       install/update NcatBot Plugins Edition\n";
    std::cout << "  init      init enviroment\n";
    std::cout << "  install   install/update a plugin\n";
    std::cout << "  run       run the main program\n";
    std::cout << "  uninstall uninstall a plugin\n";
}

// 执行系统命令并检查错误
void execute_command(const std::string& cmd) {
    if (system(cmd.c_str()) != 0) {
        std::cerr << "Command failed: " << cmd << "\n";
        exit(EXIT_FAILURE);
    }
}

// 打印带颜色的提示信息
void print_colored_message(const std::string& message, const std::string& color) {
    std::cout << color << message << NC << "\n";
}

// 安装 NcatBot 插件版
void install_ncatbot() {
    // 打印欢迎信息
    print_colored_message("欢迎使用 NcatBot 插件版安装程序", RED);
    
    // 安装主程序代码
    print_colored_message("正在下载主程序代码...", RED);

    //下载main.py和config.yaml
    execute_command("mkdir -p " + MAIN_DIR);
    std::string main_url = "https://plugins.ncatbot.xyz/downloads/main.py";
    std::string config_url = "https://plugins.ncatbot.xyz/downloads/config.yaml";
    execute_command("curl -L -o " + MAIN_DIR + "/main.py " + main_url);
    execute_command("curl -L -o " + MAIN_DIR + "/config.yaml " + config_url);

    // 安装完成信息
    print_colored_message("主程序代码下载完成。", RED);
}

void install_plugin(const std::string& plugin_name) {
    // 下载插件列表
    print_colored_message("正在获取插件列表...", RED);
    execute_command("curl -L -o plugins_list.json https://plugins.ncatbot.xyz/plugins/list.json");

    // 读取插件列表
    std::ifstream plugins_list_file("plugins_list.json");
    if (!plugins_list_file) {
        std::cerr << "无法读取插件列表文件\n";
        exit(EXIT_FAILURE);
    }

    // 查找插件
    std::string line;
    bool plugin_found = false;
    while (std::getline(plugins_list_file, line)) {
        if (line.find("\"name\": \"" + plugin_name + "\"") != std::string::npos) {
            plugin_found = true;
            break;
        }
    }
    plugins_list_file.close();
    execute_command("rm plugins_list.json");

    if (!plugin_found) {
        print_colored_message("插件 " + plugin_name + " 不存在。", RED);
        exit(EXIT_FAILURE);
    }

    // 下载插件
    print_colored_message("正在下载插件 " + plugin_name + "...", RED);
    std::string plugin_url = "https://plugins.ncatbot.xyz/downloads/plugins/" + plugin_name + ".zip";
    execute_command("curl -L -o " + plugin_name + ".zip " + plugin_url);

    // 检查本地是否有插件，存在则更新
    std::string plugin_file = PLUGIN_DIR + "/" + plugin_name + "/main.py";
    std::ifstream plugin_file_stream(plugin_file);
    if (plugin_file_stream) {
        print_colored_message("插件 " + plugin_name + " 已存在，正在更新...", RED);
    }

    // 解压插件
    execute_command("unzip -q " + plugin_name + ".zip -d " + PLUGIN_DIR);

    // 清理临时文件
    execute_command("rm " + plugin_name + ".zip");

    // 安装完成信息
    print_colored_message("插件 " + plugin_name + " 安装完成。", RED);
}

void uninstall_plugin(const std::string& plugin_name) {
    // 检查本地是否有插件，存在则删除
    std::string plugin_file = PLUGIN_DIR + "/" + plugin_name + "/main.py";
    std::ifstream plugin_file_stream(plugin_file);
    if (!plugin_file_stream) {
        print_colored_message("插件 " + plugin_name + " 不存在。", RED);
        exit(EXIT_FAILURE);
    }

    // 提示用户确认删除
    std::cout << "确定要删除插件 " << plugin_name << " 吗？(y/n): ";
    char confirm;
    std::cin >> confirm;
    if (confirm != 'y' && confirm != 'Y') {
        print_colored_message("取消卸载插件 " + plugin_name + "。", RED);
        return;
    }

    // 删除插件
    execute_command("rm -rf " + PLUGIN_DIR + "/" + plugin_name);

    // 卸载完成信息
    print_colored_message("插件 " + plugin_name + " 卸载完成。", RED);
}

void run_main() {
    // 执行主程序，并实时显示输出
    execute_command(MAIN_DIR + "/.venv/bin/python3 " + MAIN_DIR + "/main.py");
}

void init_enviroment() {
    execute_command("python3 -m venv " + MAIN_DIR + "/.venv");
    execute_command(MAIN_DIR + "/.venv/bin/python3 -m pip install git+https://ghproxy.net/https://github.com/liyihao1110/ncatbot.git");
}
// 主函数
int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_help();
        return 1;
    }

    std::string command = argv[1];
    if (command == "get") {
        install_ncatbot();
    } else if (command == "init") {
        init_enviroment();
    } else if (command == "install") {
        if (argc < 3) {
            std::cerr << "Usage: nbmiao install <plugin_name>\n";
            return 1;
        }
        std::string plugin_name = argv[2];
        install_plugin(plugin_name);
    } else if (command == "run") {
        run_main();
    } else if (command == "uninstall") {
        if (argc < 3) {
            std::cerr << "Usage: nbmiao uninstall <plugin_name>\n";
            return 1;
        }
        std::string plugin_name = argv[2];
        uninstall_plugin(plugin_name);
    } else {
        print_help();
        return 1;
    }

    return 0;
}