from plugin import Plugin
from ncatbot.message import GroupMessage, PrivateMessage
from ncatbot.logger import get_log

_log = get_log()

class ExamplePlugin(Plugin):
    def __init__(self):
        super().__init__("example")
        
    def init_plugin(self, bot):
        self.bot = bot
        
        @self.group_event()
        async def on_group_message(msg: GroupMessage):
            _log.info(msg)
            
        @self.private_event()
        async def on_private_message(msg: PrivateMessage):
            _log.info(msg)
        
        @self.request_event()
        async def on_request(msg):
            _log.info(msg)

        @self.notice_event()
        async def on_notice(msg):
            _log.info(msg)

plugin = ExamplePlugin()
