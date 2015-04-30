"""LCM type definitions
This file automatically generated by lcm.
DO NOT MODIFY BY HAND!!!!
"""

try:
    import cStringIO.StringIO as BytesIO
except ImportError:
    from io import BytesIO
import struct

class waypoint_t(object):
    __slots__ = ["gps"]

    def __init__(self):
        self.gps = [ 0.0 for dim0 in range(3) ]

    def encode(self):
        buf = BytesIO()
        buf.write(waypoint_t._get_packed_fingerprint())
        self._encode_one(buf)
        return buf.getvalue()

    def _encode_one(self, buf):
        buf.write(struct.pack('>3d', *self.gps[:3]))

    def decode(data):
        if hasattr(data, 'read'):
            buf = data
        else:
            buf = BytesIO(data)
        if buf.read(8) != waypoint_t._get_packed_fingerprint():
            raise ValueError("Decode error")
        return waypoint_t._decode_one(buf)
    decode = staticmethod(decode)

    def _decode_one(buf):
        self = waypoint_t()
        self.gps = struct.unpack('>3d', buf.read(24))
        return self
    _decode_one = staticmethod(_decode_one)

    _hash = None
    def _get_hash_recursive(parents):
        if waypoint_t in parents: return 0
        tmphash = (0x65693b45e75564fa) & 0xffffffffffffffff
        tmphash  = (((tmphash<<1)&0xffffffffffffffff)  + (tmphash>>63)) & 0xffffffffffffffff
        return tmphash
    _get_hash_recursive = staticmethod(_get_hash_recursive)
    _packed_fingerprint = None

    def _get_packed_fingerprint():
        if waypoint_t._packed_fingerprint is None:
            waypoint_t._packed_fingerprint = struct.pack(">Q", waypoint_t._get_hash_recursive([]))
        return waypoint_t._packed_fingerprint
    _get_packed_fingerprint = staticmethod(_get_packed_fingerprint)
