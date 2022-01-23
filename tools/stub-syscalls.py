#!/usr/bin/env python3

import argparse
import sys

from rich import print

def main(orig_hdr_in, stub_hdr_out):
    lines = list(map(lambda s: s.strip().split(), open(orig_hdr_in).readlines()))
    sysdefs = {}
    for l in lines:
        if len(l) == 3 and l[0] == '#define' and l[1].startswith('__NR_'):
            syscall_name = l[1].removeprefix('__NR_')
            sysdefs[syscall_name] = int(l[2])
    print(sysdefs)

    with open(stub_hdr_out, 'w') as f:
        for syscall in sysdefs.keys():
            print(f"#define SYS_{syscall} ((long)SYS_IMP_{syscall})", file=f)
        print("\n\n\n", file=f)
        for syscall in sysdefs.keys():
            print(f"void SYS_IMP_{syscall}();", file=f)
        print("", file=f)
    return 0


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Translate a regular syscall.h.in from an arch to a baremetal stub header",
    )

    parser.add_argument(
        "orig_hdr_in",
        type=str,
        help="An original header like arch/aarch64/bits/syscall.h.in",
        metavar="HDR",
    )

    parser.add_argument(
        "stub_hdr_out",
        type=str,
        help="The output stub header like arch/aarch64-baremetal/bits/syscall.h.in",
        metavar="HDR",
    )

    args = parser.parse_args()
    sys.exit(main(args.orig_hdr_in, args.stub_hdr_out))
