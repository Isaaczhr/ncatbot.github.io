#!/bin/bash

# 定义变量
URL="https://gh-proxy.com/github.com/liyihao1110/ncatbot/archive/refs/heads/main.zip"
ZIP_FILE="main.zip"
DEST_DIR="./NcatBotPluginsEdition/NcatBot"
RED='\033[0;31m'
NC='\033[0m' # 无颜色

# 打印欢迎信息
echo -e "${RED}"
echo "欢迎使用 NcatBot 插件版安装程序"
echo "正在安装 NcatBot 插件版..."
echo "使用代理: gh-proxy.com"

# 使用正常颜色
echo -e "${NC}"

# 下载 zip 文件
echo -e "${RED}正在下载...${NC}"
wget --progress=bar:force -O $ZIP_FILE $URL

# 如果目标目录不存在则创建
mkdir -p $DEST_DIR

# 解压文件到目标目录
echo -e "${RED}正在解压...${NC}"
unzip -q $ZIP_FILE -d $DEST_DIR

# 移动解压后的文件
mv $DEST_DIR/ncatbot-main/* $DEST_DIR

# 删除解压后的目录
rm -rf $DEST_DIR/ncatbot-main

# 删除原始 zip 文件
rm $ZIP_FILE

# 打印完成信息
echo -e "${RED}安装完成。${NC}"