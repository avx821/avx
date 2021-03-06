"""LCM type definitions
This file automatically generated by lcm.
DO NOT MODIFY BY HAND!!!!
"""

try:
    import cStringIO.StringIO as BytesIO
except ImportError:
    from io import BytesIO
import struct

class vision_detection_t(object):
    __slots__ = ["timestamp", "frame_timestamp", "detected", "x", "y", "area", "number_of_objects", "type", "color"]

    def __init__(self):
        self.timestamp = 0
        self.frame_timestamp = 0
        self.detected = False
        self.x = 0.0
        self.y = 0.0
        self.area = 0.0
        self.number_of_objects = 0
        self.type = 0
        self.color = 0

    def encode(self):
        buf = BytesIO()
        buf.write(vision_detection_t._get_packed_fingerprint())
        self._encode_one(buf)
        return buf.getvalue()

    def _encode_one(self, buf):
        buf.write(struct.pack(">qqbdddiii", self.timestamp, self.frame_timestamp, self.detected, self.x, self.y, self.area, self.number_of_objects, self.type, self.color))

    def decode(data):
        if hasattr(data, 'read'):
            buf = data
        else:
            buf = BytesIO(data)
        if buf.read(8) != vision_detection_t._get_packed_fingerprint():
            raise ValueError("Decode error")
        return vision_detection_t._decode_one(buf)
    decode = staticmethod(decode)

    def _decode_one(buf):
        self = vision_detection_t()
        self.timestamp, self.frame_timestamp = struct.unpack(">qq", buf.read(16))
        self.detected = bool(struct.unpack('b', buf.read(1))[0])
        self.x, self.y, self.area, self.number_of_objects, self.type, self.color = struct.unpack(">dddiii", buf.read(36))
        return self
    _decode_one = staticmethod(_decode_one)

    _hash = None
    def _get_hash_recursive(parents):
        if vision_detection_t in parents: return 0
        tmphash = (0xee8ce710fcd43b04) & 0xffffffffffffffff
        tmphash  = (((tmphash<<1)&0xffffffffffffffff)  + (tmphash>>63)) & 0xffffffffffffffff
        return tmphash
    _get_hash_recursive = staticmethod(_get_hash_recursive)
    _packed_fingerprint = None

    def _get_packed_fingerprint():
        if vision_detection_t._packed_fingerprint is None:
            vision_detection_t._packed_fingerprint = struct.pack(">Q", vision_detection_t._get_hash_recursive([]))
        return vision_detection_t._packed_fingerprint
    _get_packed_fingerprint = staticmethod(_get_packed_fingerprint)

