#!/bin/bash

# CLI工具主函数
main() {
    # 检查是否有参数传入
    if [ $# -eq 0 ]; then
        echo "Usage: $0 <command> [arguments]"
        echo "Use '$0 help' for usage."
        exit 1
    fi

    # 解析命令
    case "$1" in
        help)
            # 帮助命令
            echo "Usage: $0 <command> [arguments]"
            echo "Commands:"
            echo "  get       install/update NcatBot Plugins Edition"
            ;;
        get)
            # 安装命令
            URL="https://ghproxy.net/https://github.com/liyihao1110/ncatbot/archive/refs/heads/main.zip"
            ZIP_FILE="main.zip"
            DEST_DIR="./NcatBotPluginsEdition/NcatBot"
            RED='\033[0;31m'
            GREEN='\033[0;32m'
            NC='\033[0m'

            # 打印欢迎信息
            echo -e "${RED}"
            echo "欢迎使用 NcatBot 插件版安装程序"
            echo "正在安装 NcatBot 插件版..."
            echo -e "${GREEN}使用代理: ghproxy.net${NC}"

            # 下载 zip 文件
            echo -e "${RED}正在下载...${NC}"
            curl -L -o $ZIP_FILE $URL
            # 解压 zip 文件 并移动到目标目录
            echo -e "${RED}正在解压...${NC}"
            unzip -q $ZIP_FILE -d .
            mkdir -p $DEST_DIR
            mv ./ncatbot-main/* $DEST_DIR
            rm -rf ./ncatbot-main
            rm $ZIP_FILE

            # 打印完成信息
            echo -e "${RED}安装完成。${NC}"
            ;;
        *)
            # 未知命令
            echo "Unknown command: $1"
            echo "Use '$0 help' for usage."
            exit 1
            ;;
    esac
}

# 调用主函数并传递所有参数
main "$@"
