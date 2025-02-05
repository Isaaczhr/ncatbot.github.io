from NcatBot.ncatbot.message import GroupMessage, PrivateMessage
import logging

_log = logging.getLogger(__name__)

def init_plugin(bot):
    """初始化插件并注册事件"""
    @bot.group_event()
    async def on_group_message(msg: GroupMessage):
        _log.info(f"Group message: {msg}")
    
    @bot.private_event()
    async def on_private_message(msg: PrivateMessage):
        _log.info(f"Private message: {msg}")
        if msg.raw_message == "test":
            await bot.api.post_private_msg(
                msg.user_id, text="你好,", face=1, reply=msg.message_id
            )
        elif msg.raw_message == "ping":
            await bot.api.add_at(msg.user_id).add_face(1).add_text("你好").add_face(
                2
            ).send_private_msg(msg.user_id, reply=msg.message_id)
        elif msg.raw_message == "md":
            await bot.api.post_private_file(user_id=msg.user_id, markdown="README.md")
