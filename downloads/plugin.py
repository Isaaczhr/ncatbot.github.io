import logging
from typing import Optional, List, Callable
from ncatbot.client import BotClient
from ncatbot.message import GroupMessage, PrivateMessage
from ncatbot.logger import get_log

_log = get_log()

class Plugin:
    def __init__(self, name: str):
        self.name = name
        self.log = logging.getLogger(f"plugin.{name}")
        self._group_handlers = []
        self._private_handlers = []
        self._notice_handlers = []
        self._request_handlers = []

    def group_event(self, types: Optional[List[str]] = None):
        def decorator(func: Callable):
            self._group_handlers.append((func, types))
            return func
        return decorator
        
    def private_event(self, types: Optional[List[str]] = None):
        def decorator(func: Callable):
            self._private_handlers.append((func, types))
            return func
        return decorator
        
    def notice_event(self):
        def decorator(func: Callable):
            self._notice_handlers.append(func)
            return func
        return decorator
        
    def request_event(self):
        def decorator(func: Callable):
            self._request_handlers.append(func)
            return func
        return decorator

    async def handle_group_message(self, msg: GroupMessage):
        for handler, types in self._group_handlers:
            try:
                if types is None or any(i["type"] in types for i in msg.message):
                    await handler(msg)
            except Exception as e:
                self.log.error(f"处理群消息失败: {e}")

    async def handle_private_message(self, msg: PrivateMessage):
        for handler, types in self._private_handlers:
            try:
                if types is None or any(i["type"] in types for i in msg.message):
                    await handler(msg)
            except Exception as e:
                self.log.error(f"处理私聊消息失败: {e}")

    async def handle_notice_message(self, msg: dict):
        for handler in self._notice_handlers:
            try:
                await handler(msg)
            except Exception as e:
                self.log.error(f"处理通知消息失败: {e}")

    async def handle_request_message(self, msg: dict):
        for handler in self._request_handlers:
            try:
                await handler(msg)
            except Exception as e:
                self.log.error(f"处理请求消息失败: {e}")
                
    def init_plugin(self, bot) -> None:
        """插件初始化方法"""
        raise NotImplementedError

class PluginManager:
    def __init__(self):
        self.bot = BotClient()
        self._plugins = []
        
        # 将事件处理器绑定到bot实例
        self.bot.handle_group_event = self.handle_group_event
        self.bot.handle_private_event = self.handle_private_event
        self.bot.handle_notice_event = self.handle_notice_event
        self.bot.handle_request_event = self.handle_request_event

    def load_plugin(self, plugin):
        try:
            plugin.init_plugin(self.bot)
            self._plugins.append(plugin)
            _log.info(f"插件 {plugin.name} 加载成功")
        except Exception as e:
            _log.error(f"加载插件 {plugin.name} 失败: {e}")

    async def handle_group_event(self, msg: dict):
        group_msg = GroupMessage(msg)
        for plugin in self._plugins:
            await plugin.handle_group_message(group_msg)

    async def handle_private_event(self, msg: dict):
        private_msg = PrivateMessage(msg)
        for plugin in self._plugins:
            await plugin.handle_private_message(private_msg)

    async def handle_notice_event(self, msg: dict):
        for plugin in self._plugins:
            await plugin.handle_notice_message(msg)

    async def handle_request_event(self, msg: dict):
        for plugin in self._plugins:
            await plugin.handle_request_message(msg)
