"""
IPv4 Header — 32 bits (DWORD).

Field layout (bits 31 → 0):
  31:28 Version  —> 4 bits  (always 4 for IPv4)
  27:24 IHL      —> 4 bits  (header length in 32-bit)
  23:18 DSCP     —> 6 bits  (differentiated services code point)
  17:16 ECN      —> 2 bits  (explicit congestion notification)
  15:0 Length   —>  16 bits (total packet length in bytes)

"""

# Masks

MASK_VERSION = 0xF
MASK_IHL = 0xF
MASK_DSCP = 0x3F
MASK_ECN = 0x3
MASK_LENGTH = 0xFFFF


def unpack_header(raw):
    return {
        "version": (raw >> 28) & MASK_VERSION,
        "ihl": (raw >> 24) & MASK_IHL,
        "dscp": (raw >> 18) & MASK_DSCP,
        "ecn": (raw >> 16) & MASK_ECN,
        "length": raw & MASK_LENGTH,
    }


def pack_header(version, ihl, dscp, ecn, length):
    return (
        (version & MASK_VERSION) << 28
        | (ihl & MASK_IHL) << 24
        | (dscp & MASK_DSCP) << 18
        | (ecn & MASK_ECN) << 16
        | (length & MASK_LENGTH)
    )


def set_ecn(raw, ecnValue):
    return (raw & 0xFFFCFFFF) | ((ecnValue & MASK_ECN) << 16)


def check_dont_fragment(raw):
    return (raw >> 17) & 1


def main():
    rawPacket = 0x45020028
    print(f"Raw packet:      0x{rawPacket:08x}")
    print()

    # unpack
    fields = unpack_header(rawPacket)
    print("Unpacked fields:")
    for fieldName, value in fields.items():
        print(f" {fieldName:<10} = {value}")
    print()

    # repack and verify round trip
    repacked = pack_header(
        fields["version"],
        fields["ihl"],
        fields["dscp"],
        fields["ecn"],
        fields["length"],
    )
    print(f"Repacked:        0x{repacked:08x}")
    print(f"Round-trip OK:   {repacked == rawPacket}")
    print()

    # modify ECN
    modified = set_ecn(rawPacket, ecnValue=3)
    print(f"ECN set to 3:    0x{modified:08x}")
    print(f"ECN verified:    {(modified >> 16) & MASK_ECN}")
    print()

    # check DF flag
    dfSet = check_dont_fragment(rawPacket)
    print(f"DF flag set:     {dfSet}")


main()
