"""LCM type definitions
This file automatically generated by lcm.
DO NOT MODIFY BY HAND!!!!
"""

try:
    import cStringIO.StringIO as BytesIO
except ImportError:
    from io import BytesIO
import struct

class state_machine_mode_t(object):
    __slots__ = ["Mode_ID"]

    def __init__(self):
        self.Mode_ID = 0

    def encode(self):
        buf = BytesIO()
        buf.write(state_machine_mode_t._get_packed_fingerprint())
        self._encode_one(buf)
        return buf.getvalue()

    def _encode_one(self, buf):
        buf.write(struct.pack(">q", self.Mode_ID))

    def decode(data):
        if hasattr(data, 'read'):
            buf = data
        else:
            buf = BytesIO(data)
        if buf.read(8) != state_machine_mode_t._get_packed_fingerprint():
            raise ValueError("Decode error")
        return state_machine_mode_t._decode_one(buf)
    decode = staticmethod(decode)

    def _decode_one(buf):
        self = state_machine_mode_t()
        self.Mode_ID = struct.unpack(">q", buf.read(8))[0]
        return self
    _decode_one = staticmethod(_decode_one)

    _hash = None
    def _get_hash_recursive(parents):
        if state_machine_mode_t in parents: return 0
        tmphash = (0x44d3db65b8e0b2c) & 0xffffffffffffffff
        tmphash  = (((tmphash<<1)&0xffffffffffffffff)  + (tmphash>>63)) & 0xffffffffffffffff
        return tmphash
    _get_hash_recursive = staticmethod(_get_hash_recursive)
    _packed_fingerprint = None

    def _get_packed_fingerprint():
        if state_machine_mode_t._packed_fingerprint is None:
            state_machine_mode_t._packed_fingerprint = struct.pack(">Q", state_machine_mode_t._get_hash_recursive([]))
        return state_machine_mode_t._packed_fingerprint
    _get_packed_fingerprint = staticmethod(_get_packed_fingerprint)

