import sys

option_names = ["ROP",
                "UBC",
                "NUBC",
                "AFR",
                "NAFR",
                "MISC1",
                "MISC2",
                "CLK1",
                "CLK2",
                "HSECNT",
                "NHSECNT",
                "reserved",
                "reserved",
                "reserved",
                "reserved",
                ]

with open(sys.argv[1], "rb") as f:
    for i, e in enumerate(f.read()):
        print "%08s %02x" % (option_names[i], ord(e))
