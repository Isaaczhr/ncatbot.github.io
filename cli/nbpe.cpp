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
const std::string DEST_DIR = "./NcatBotPluginsEdition/NcatBot";
const std::string PLUGIN_DIR = "./NcatBotPluginsEdition/NcatBot/plugins";

// 打印帮助信息
void print_help() {
    std::cout << "Usage: nbpe <command> [arguments]\n";
    std::cout << "Commands:\n";
    std::cout << "  get       install/update NcatBot Plugins Edition\n";
    std::cout << "  install   install/update a plugin\n";
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
    std::cout << "正在下载 NcatBot 核心代码...\n";
    print_colored_message("使用代理: ghproxy.net", GREEN);

    // 下载 zip 文件
    print_colored_message("正在下载...", RED);
    execute_command("curl -L -o " + ZIP_FILE + " " + NCURL);

    // 解压下载的文件
    print_colored_message("正在解压...", RED);
    execute_command("unzip -q " + ZIP_FILE + " -d .");

    // 移动文件到目标目录并清理临时文件
    execute_command("mkdir -p " + DEST_DIR);
    execute_command("mv ./ncatbot-main/* " + DEST_DIR);
    execute_command("rm -rf ./ncatbot-main");
    execute_command("rm " + ZIP_FILE);

    // 安装完成信息
    print_colored_message("下载完成。", RED);

    // 安装主程序代码
    print_colored_message("正在下载主程序代码...", RED);

    //下载main.py
    std::string main_url = "https://plugins.ncatbot.xyz/downloads/main.py";
    execute_command("curl -L -o " + DEST_DIR + "/main.py " + main_url);

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

// 主函数
int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_help();
        return 1;
    }

    std::string command = argv[1];
    if (command == "get") {
        install_ncatbot();
    } else if (command == "install") {
        if (argc < 3) {
            std::cerr << "Usage: nbpe install <plugin_name>\n";
            return 1;
        }
        std::string plugin_name = argv[2];
        install_plugin(plugin_name);
    } else {
        print_help();
        return 1;
    }

    return 0;
}