#include <iostream>
#include <cstdlib>
#include <string>

// 打印帮助信息的函数
void print_help() {
    std::cout << "Usage: nbpe <command> [arguments]\n";
    std::cout << "Commands:\n";
    std::cout << "  get       install/update NcatBot Plugins Edition\n";
}

// 执行系统命令并检查错误的函数
void execute_command(const std::string& cmd) {
    if (system(cmd.c_str()) != 0) {
        std::cerr << "Command failed: " << cmd << "\n";
        exit(EXIT_FAILURE);
    }
}

// 安装 NcatBot 插件版的函数
void install_ncatbot() {
    const std::string URL = "https://ghproxy.net/https://github.com/liyihao1110/ncatbot/archive/refs/heads/main.zip";
    const std::string ZIP_FILE = "main.zip";
    const std::string DEST_DIR = "./NcatBotPluginsEdition/NcatBot";
    const std::string RED = "\033[0;31m";
    const std::string GREEN = "\033[0;32m";
    const std::string NC = "\033[0m";

    // 打印欢迎信息
    std::cout << RED << "欢迎使用 NcatBot 插件版安装程序\n";
    std::cout << "正在安装 NcatBot 插件版...\n";
    std::cout << GREEN << "使用代理: ghproxy.net" << NC << "\n";

    // 下载 zip 文件
    std::cout << RED << "正在下载..." << NC << "\n";
    execute_command("curl -L -o " + ZIP_FILE + " " + URL);

    // 解压下载的文件
    std::cout << RED << "正在解压..." << NC << "\n";
    execute_command("unzip -q " + ZIP_FILE + " -d .");

    // 将解压后的文件移动到目标目录
    execute_command("mkdir -p " + DEST_DIR);
    execute_command("mv ./ncatbot-main/* " + DEST_DIR);
    execute_command("rm -rf ./ncatbot-main");
    execute_command("rm " + ZIP_FILE);

    // 打印安装完成信息
    std::cout << RED << "安装完成。" << NC << "\n";
}

int main(int argc, char* argv[]) {
    // 检查是否提供了命令行参数
    if (argc < 2) {
        std::cout << "Usage: nbpe <command> [arguments]\n";
        std::cout << "Use 'nbpe help' for usage.\n";
        return 1;
    }

    std::string command = argv[1];

    // 处理不同的命令
    if (command == "help") {
        print_help();
    } else if (command == "get") {
        install_ncatbot();
    } else {
        std::cout << "Unknown command: " << command << "\n";
        std::cout << "Use 'nbpe help' for usage.\n";
        return 1;
    }

    return 0;
}
