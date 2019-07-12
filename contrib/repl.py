#!/usr/bin/python3

# Execute ./repl.py to obtain python REPL
# You can then import AA and call aa.change_profile() etc
# If you launch python directly, you'll end up 
# defining a profile for the python executable that probably won't end well

import code; code.interact(local=dict(globals(), **locals()))
