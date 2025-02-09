from plugin import Plugin
from ncatbot.message import GroupMessage, PrivateMessage
import logging

class EchoPlugin(Plugin):
    def __init__(self):
        super().__init__("echo")
        
    def init_plugin(self, bot):
        self.bot = bot
        
        @self.group_event()
        async def on_group_message(msg: GroupMessage):
            await self.bot.api.post_group_msg(msg.group_id, text=msg.raw_message, reply=msg.message_id)
            
        @self.private_event()  
        async def on_private_message(msg: PrivateMessage):
            await self.bot.api.post_private_msg(msg.user_id, text=msg.raw_message, reply=msg.message_id)


plugin = EchoPlugin()