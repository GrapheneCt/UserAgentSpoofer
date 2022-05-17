# UserAgentSpoofer
User agent spoofer plugin for PS Vita WebKit
Intended to work only with applications that use WebKit through webcore or mini webcore BG apps.

Install by adding following to config.txt
```
*NPXS10083
ur0:tai/UserAgentSpoofer.suprx
*NPXS10084
ur0:tai/UserAgentSpoofer.suprx
```

User agent string is read from ```ux0:data/external/webcore/user_agent.txt```, if that file is not present default Vita user agent will be used.
